<?php
/*
| √рафики ѕлатежей
*/
require_once('class/as-diagrams.php');
?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='text/javascript' src='js/jquery.autocomplete.js'></script>
<script> 

var year_option = '';
var year_now = new Date();

for (var i = 2008; i <= 2020; i++) {
	if ((year_now.getUTCFullYear()) == i){
		year_option+='<option value="'+i+'" selected>'+i+'</option>';
	}else{
		year_option+='<option value="'+i+'">'+i+'</option>';
	}
	
}

$(document).ready(function() 
    { 
        $("#paymenttable").tablesorter(); 
			
			$('#type_yearmonth, #type_day').click(function(){
			if($(this).attr('id')=='type_yearmonth'){
                $('#period').html('Выберите год: <select name=period>'+year_option+'</select>');
            }
            else if($(this).attr('id')=='type_day'){
				 $('#period').html('<tr><td>Выберите год: </td><td><select name=period_year>'+year_option+'</select></td></tr>');
                 $('#period').append('<tr><td>Выберите месяц:</td><td> <select name=period_month><option value="01">январь</option><option value="02">февраль</option><option value="03">март</option><option value="04">апрель</option><option value="05">май</option><option value="06">июнь</option><option value="07">июль</option><option value="08">август</option><option value="09">сентябрь</option><option value="10">октябрь</option><option value="11">ноябрь</option><option value="12">декабрь</option></select></td></tr>');
			}
        });
	}	
);

//$('#logtable').columnHover(); 
</script>

<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css" />
<style type="text/css">
#menu { /* Ћева¤ колонка */
    float: left; /* ќбтекание справа */
	#border: 1px solid #333; /* ѕараметры рамки вокруг */
	padding: 0px; /* ѕол¤ вокруг текста */
}
#content { /* ѕрава¤ колонка */
	background-color: white;
    margin: 34px 0px 0px 220px; /* «начени¤ отступов */
    padding: 5px; /* ѕол¤ вокруг текста */
	#border: 1px solid #333; /* ѕараметры рамки */
	width: 100%;
	height:65%;
}

</style>
<p id="tabname">Отчет по платежам</p>

<table>
<tr>
	<td style="height:50px; vertical-align:top;">
		<!--√рафики-->
		<form action="?page=report_graph_pay&table=2" autocomplete="off" method=post id="form_graphs">
		<table class=forms>
			<tr><th>Тип отчета</th></tr>
			<tr><td>
				<input type="radio" name="type" id="type_yearmonth" value="type_yearmonth">по месяцам в году<br>
				<input type="radio" name="type" id="type_day" value="type_day">по дням в месяце<br>
			</td></tr>
			<tr><th>Период</th></tr>
			<tr><td id="period" align=center> </td></tr>
			<tr><th>Операторы</th></tr>
			<tr><td>
				<details open="open">
				<summary align=left>Внешние</summary>
				<?php
					foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
						if ($ArrTypeValuteVal[0] == '0'){
							echo '<input type="checkbox" name="operator[]" value="'.$ArrTypeValuteKey.'" CHECKED>'.$ArrTypeValuteVal[1].'<br>';
						}
					}
				?>
				</details>
				<details>
				<summary align=left>Внутренние</summary>
				<?php
					foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
						if ($ArrTypeValuteVal[0] == '1'){
							echo '<input type="checkbox" name="operator[]" value="'.$ArrTypeValuteKey.'" >'.$ArrTypeValuteVal[1].'<br>';
						}
					}
				?>
				</details>
			</td></tr>
			<tr><th>Населенный пункт</th></tr>
			<tr>
				<td>
				<input type="radio" name="nasp" id="nasp" value="ALL" CHECKED>Все<br>
				<?php
				if (is_array($tinsp->APIGetNasp(2, NULL))){
					foreach($tinsp->APIGetNasp(2, NULL) as $nasp){
						echo '<input type="radio" name="nasp" id="nasp" value="'.$nasp["nasp_id"].'">'.$nasp["nasp_name"].'<br>';
					}
				}
				?>
				</td>
			</tr>
			<tr>
				<td align=center><input type=submit value="Построить отчет" ></td>
			</tr>
		</table>
		</form>	
	</td>
	<td style="height:50px; vertical-align:top;">
	<?php
			/* ќ“„≈“ —ѕ»—ќ  */
		if(count($_POST)>1 && $_GET["table"]==2){
			#≈сли в фильтре указан населенный пункт
			if ($_POST["nasp"] !="ALL"){
				$ArrayAbon=$tinsp->APIGetBillProp(3, false);
				$ArrayAbonNasp=array();
				foreach (@$ArrayAbon as $ArrayAbonVal) {
					$ArrayAbonNasp[$ArrayAbonVal["user_id"]]=$ArrayAbonVal["user_nasp"];
					
				}
			}
			#–азбиваем список операторов дл¤ sql запроса
			foreach($_POST["operator"] as $oper_key => $oper_val){
				(isset($_POST["operator"][$oper_key+1]) ? $sql_oper.="sender = '".$oper_val."' OR " : $sql_oper.="sender = '".$oper_val."'");
			}
			
			#за мес¤ца
			if($_POST["type"]=="type_yearmonth"){
				$data_title = 'График: отчет по месяцам в году';
				$legend_y = $_POST["operator"];
				$legend_x = array('1','2','3','4','5','6','7','8','9','10','11','12');
				#выборка по каждому мес¤цу 
				foreach ($legend_x as $month){  
					//$array[$month][$operator]=$tinsp->Query("Select sum(sum) from billing_payment where MONTH(date) = '".$month."' and YEAR(date) = '".$_POST["period"]."' and sender = '".$operator."' ".($_POST["nasp"] !="ALL" ? "and billing_users.user_nasp = '".$_POST["nasp"]."' ": "")." ", false);
					//$array[$month][$operator]=$tinsp->Query("Select sum(sum) from billing_payment where date >= '".$_POST["period"]."-".$month."-01 00:00:00' and date < DATE_ADD('".$_POST["period"]."-".$month."-01 00:00:00', INTERVAL 1 MONTH) and sender = '".$operator."' ".($_POST["nasp"] !="ALL" ? "and billing_users.user_nasp = '".$_POST["nasp"]."' ": "")." ", false);
					$q=$tinsp->Query("Select sum, user_id, sender from billing_payment where date >= '".$_POST["period"]."-".$month."-01 00:00:00' and date < DATE_ADD('".$_POST["period"]."-".$month."-01 00:00:00', INTERVAL 1 MONTH) and (".$sql_oper.") ", false);
					if (is_array($q)){
						foreach ($_POST["operator"] as $operator) {
							$array[$month][$operator] = 0;
							foreach (@$q as $q_val){
								if ($q_val["sender"] == $operator){
									#≈сли передан параметр населенного пункта
									if ($_POST["nasp"] !="ALL"){
										if ($ArrayAbonNasp[$q_val["user_id"]] == $_POST["nasp"]){
											$array[$month][$operator]=$array[$month][$operator]+$q_val["sum"];	
										}
									}else{
										$array[$month][$operator]=$array[$month][$operator]+$q_val["sum"];
										
									}										
								}
							}
						}
					}
				}
				$i=1;
				#собираем данные дл¤ графика
				foreach($array as $mon => $op){
					$op_value_array=array();
					foreach(array_keys ($op) as $op_value){
							$op_value_array[]=$op[$op_value];
					}
					#график
					$data[] = $op_value_array;
					
					#считаем итоги по мес¤цам
					$ItogMonth[$i++]=array_sum($op_value_array);
				} 
				
				#новый вектор X && total summ
				foreach($legend_x as $legend_value){
					$new_legend_x[]=date("M", mktime(0, 0, 0, $legend_value))."<br>".number_format($ItogMonth[$legend_value],2)."";
					$Total=$ItogMonth[$legend_value]+$Total;	
				}
				$graph = new CAsBarDiagram;
				$graph->bwidth = 5; // set one bar width, pixels
				$graph->bt_total = 'Totals<br>'.number_format($Total,2).''; // 'totals' column title, if other than 'Totals'
				#$graph->showtotals = 0;  // uncomment it if You don't need 'totals' column
				$graph->precision = 0;  // decimal precision
				$graph->DiagramBar($new_legend_x, $legend_y, $data, $data_title);
				
			}elseif($_POST["type"]=="type_day"){
				/*
				Select `sum`,`date`,`sender`,`order` from billing_payment
LEFT JOIN billing_users ON billing_users.user_dogovor = billing_payment.order
where MONTH(date) = '5' and YEAR(date) = '2012' and sender = 'kassira.net' and billing_users.user_nasp = 'C8A6DD48-EE07-4CA1-845E-8D62803814A5'
			*/
			#за дни в мес¤це
				$data_title = 'График: отчет по дням в месяце';
				$legend_y = $_POST["operator"];
				$legend_x = array('1','2','3','4','5','6','7','8','9','10','11','12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31');
				

				#POST Ќј—≈Ћ≈ЌЌџ≈ ѕ”Ќ “џ
				if ($_POST["nasp"]!="ALL" && strlen($_POST["nasp"])>5) {
					$sql=$tinsp->Query("Select `sum`,`date`,`sender` from billing_payment LEFT JOIN billing_users ON billing_users.user_id = billing_payment.user_id where date >= '".$_POST["period_year"]."-".$_POST["period_month"]."-01 00:00:00' and date < DATE_ADD('".$_POST["period_year"]."-".$_POST["period_month"]."-01 00:00:00', INTERVAL 1 MONTH) and (".$sql_oper.") and billing_users.user_nasp = '".$_POST["nasp"]."' ", false);
				}else{
					
					$sql=$tinsp->Query("Select `sum`,`date`,`sender` from billing_payment where date >= '".$_POST["period_year"]."-".$_POST["period_month"]."-01 00:00:00' and date < DATE_ADD('".$_POST["period_year"]."-".$_POST["period_month"]."-01 00:00:00', INTERVAL 1 MONTH) and (".$sql_oper.")", false);
				}
				#заполним массив нул¤ми
				foreach ($_POST["operator"] as $operator) {
					foreach ($legend_x as $days){
						$array[(integer)$days][$operator]=$array[(integer)$days][$operator];	
					}
				}
				#заполним реальными значени¤ми
				foreach($sql as $val){
					$d=date("d", strtotime($val["date"]));
					if(in_array((integer)$d, $legend_x)){
						$array[(integer)$d][$val["sender"]]=$array[(integer)$d][$val["sender"]]+$val["sum"];
					}	
				}
				$i=1;
				#собираем данные дл¤ графика
				foreach($array as $day => $op){
					$op_value_array=array();
					foreach(array_keys ($op) as $op_value){
						$op_value_array[]=$op[$op_value];	
					}
					#график
					$data[] = $op_value_array;
					
					#считаем итоги по дн¤м
					$ItogDay[$i++]=array_sum($op_value_array);
				} 
					#новый вектор X && total summ
				foreach($legend_x as $legend_value){
					$new_legend_x[]=$legend_value."<br>".number_format($ItogDay[$legend_value],2)."";
					$Total=$ItogDay[$legend_value]+$Total;	
				}
				$graph = new CAsBarDiagram;
				$graph->bwidth = 5; // set one bar width, pixels
				$graph->bt_total = 'Totals<br>'.number_format($Total,2).''; // 'totals' column title, if other than 'Totals'
				#$graph->showtotals = 0;  // uncomment it if You don't need 'totals' column
				$graph->precision = 0;  // decimal precision
				$graph->DiagramBar($new_legend_x, $legend_y, $data, $data_title);
				
			}
		}
	?>
	</td>
</tr>
</table>