<?php
/*
| Поиск платежей
| Графики
*/
require_once('class/as-diagrams.php');
?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='text/javascript' src='js/jquery.autocomplete.js'></script>
<script> 
$(function() {
	$("#pay_dogovor_fio").autocomplete({
		url: 'query/json_query.php?FUNCTION=BillingGetAjaxUser',
		showResult: function(value, data) {			
				return '<span style="color:gray">' + value + '</span>';		
		},
		onItemSelect: function(item) {
 
		}
	});
});
$(document).ready(function() 
    { 
        $("#paymenttable").tablesorter(); 
			
			$('#form_graphs :radio').click(function(){
			if($('#form_graphs :radio:checked').attr('id')=='type_yearmonth'){
                $('#period').html('Выберите год: <select name=period><option value="2010">2010</option><option value="2011">2011</option><option value="2012">2012</option><option value="2013">2013</option></select>');
            }
            else if($('#form_graphs :radio:checked').attr('id')=='type_day'){
				 $('#period').html('<tr><td>Выберите год: </td><td><select name=period_year><option value="2012">2012</option><option value="2013">2013</option><option value="2014">2014</option><option value="2015">2015</option></select></td></tr>');
                 $('#period').append('<tr><td>Выберите месяц:</td><td> <select name=period_month><option value="01">Январь</option><option value="02">Февраль</option><option value="03">Март</option><option value="04">Апрель</option><option value="05">Мая</option><option value="06">Июнь</option><option value="07">Июль</option><option value="08">Август</option><option value="09">Сентябрь</option><option value="10">Октябрь</option><option value="11">Ноябрь</option><option value="12">Декабрь</option></select></td></tr>');
			}
        });
	}	
);

//$('#logtable').columnHover(); 
</script>

<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css" />
<style type="text/css">
#stat td.hover, #logtable tbody tr:hover
{
	font-weight: bold;
	color: black;
	background-color: LightCyan;
}
div.scrollTableContainer {
	height: 400px;
	overflow: auto;
	width: 1100px;
	margin: 15px 0 0 0;
	position: relative;
}
div.scrollTableContainer thead tr {
	position:relative;
	top: expression(offsetParent.scrollTop); /*IE5+ only*/
	left: 0px;
}
div.scrollTableContainer td:last-child {padding-right: 20px;}
</style>
<p id="tabname">Отчет по платежам<p>
<div class="section">
	<!--Отчеты-->
	<form action="?page=report_pay&table=1" autocomplete="off" method=post>
		<table class=forms >
		<tr><th>ФИО/Договор</th><th>Номер транзакции</th><th>Операторы</th></tr>
		<tr>
			<td><input type=text name="pay_dogovor_fio" id="pay_dogovor_fio"></td><td><input type=text name="pay_txn"></td>
			<td>
				<?php
				foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
					if ($ArrTypeValuteKey == 'billing_inner'){
						echo '<input type="checkbox" name="pay_sender[]" value="'.$ArrTypeValuteKey.'" >'.$ArrTypeValuteVal.'<br>';
					}else{
						echo '<input type="checkbox" name="pay_sender[]" value="'.$ArrTypeValuteKey.'" CHECKED>'.$ArrTypeValuteVal.'<br>';
					}
					
				}
				?>
			</td>
		</tr>
		<tr><th>Дата начала</th><th>Дата окончания</th><th>Сальдо</th></tr>
		<tr>
			<td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td>
			<td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td>
			<td>
				<select name="pay_buh" id=pay_buh>
					<option value="2">Любое</option>
					<option value="0">Поступление</option>
					<option value="1">Списание</option>
				</select>
			</td>
		</tr>
		<tr>
			<td colspan=3 align=center><input type=submit value="Найти" ></td>
		</tr>
		</table>
	</form>	
	
	<!--Графики-->
	<form action="?page=report_pay&table=2" autocomplete="off" method=post id="form_graphs">
		<table class=forms >
		<tr><th>Тип отчета</th><th>Период</th><th>Операторы</th><th>Населенный пункт</th></tr>
		<tr>
			<td>
				<input type="radio" name="type" id="type_yearmonth" value="type_yearmonth">Месяца/Год<br>
				<input type="radio" name="type" id="type_day" value="type_day">Дни/Месяц<br>
			</td>
			<td id="period" align=center>

			</td>
			<td>
				<?php
				foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
					if ($ArrTypeValuteKey == 'billing_inner'){
						echo '<input type="checkbox" name="operator[]" value="'.$ArrTypeValuteKey.'" >'.$ArrTypeValuteVal.'<br>';
					}else{
						echo '<input type="checkbox" name="operator[]" value="'.$ArrTypeValuteKey.'" CHECKED>'.$ArrTypeValuteVal.'<br>';
					}	
				}
				?>
			</td>
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
			<td colspan=4 align=center><input type=submit value="Построить график" ></td>
		</tr>
		</table>
	</form>	
			<?php
			/* ОТЧЕТ СПИСОК */
			if (count($_POST)>1 && $_GET["table"]==1){
				echo '<table id="paymenttable" class="tablesorter">
					<thead class="fixedHeader">
						<tr><th width=20>#</th><th width=250>Абонент</th><th>Пользователь</th><th>Оператор</th><th>Транзакция</th><th>Договор</th><th>Сумма</th><th>Новый баланс</th><th width=130>Дата</th><th width=250>Комментарий</th></tr>
						<tr><th colspan=6></th><th id=itog> </th><th colspan=3></th></tr>
					</thead>
					<tbody id=result>';
				$PaymentTable=$tinsp->APIGetPayment($_POST["pay_dogovor_fio"],$_POST["pay_txn"], $_POST["pay_sender"], $_POST["period1"], $_POST["period2"], $_POST["pay_buh"]);
				if(is_array($PaymentTable[1])){
					$i=1;
					foreach($PaymentTable[1] as $PaymentTableVal){
						$ItogSumm=$PaymentTableVal["sum"]+$ItogSumm;
						echo '<tr id="id'.$PaymentTableVal["id"].'"><td>'.$i++.'</td><td><b>'.$PaymentTableVal["user_name"].'</b></td><td>'.$PaymentTableVal["access_name"].'</td><td>'.$PaymentTableVal["sender"].'</td><td>'.$PaymentTableVal["payment_id"].'</td><td>'.$PaymentTableVal["order"].'</td><td>'.$PaymentTableVal["sum"].'</td><td>'.$PaymentTableVal["balance"].'</td><td>'.$PaymentTableVal["date"].'</td><td>'.$PaymentTableVal["comment"].'</td></tr>';
					}
					?>
					<script> $(document).ready(function() { $('#itog').html('<?=$ItogSumm?>');}	); </script>
					<?php
				}else{
					echo '<tr><td colspan=8 align=center>Нет данных</td></tr>';

				}
				echo '</tbody></table>';
			/* ОТЧЕТ ГРАФИК */
			}elseif(count($_POST)>1 && $_GET["table"]==2){
				#за месяца
				if($_POST["type"]=="type_yearmonth"){
					$data_title = 'Месяца/Год';
					$legend_y = $_POST["operator"];
					$legend_x = array('1','2','3','4','5','6','7','8','9','10','11','12');

					#выборка по каждому месяцу
					foreach($legend_y as $operator){
						foreach ($legend_x as $month){
							$array[$month][$operator]=$tinsp->Query("Select sum(sum) from billing_payment where MONTH(date) = '".$month."' and YEAR(date) = '".$_POST["period"]."' and sender = '".$operator."'", false);
						}
					}
					$i=1;
					#собираем данные для графика
					foreach($array as $mon => $op){
						$op_value_array=array();
						foreach(array_keys ($op) as $op_value){
								$op_value_array[]=$op[$op_value][0]["sum(sum)"];
						}
						#график
						$data[] = $op_value_array;
						
						#считаем итоги по месяцам
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
				#за дни в месяце
					$data_title = 'Дни/Месяц';
					$legend_y = $_POST["operator"];
					$legend_x = array('1','2','3','4','5','6','7','8','9','10','11','12', '13', '14', '15', '16', '17', '18', '19', '20', '21', '22', '23', '24', '25', '26', '27', '28', '29', '30', '31');
					
					foreach($_POST["operator"] as $key => $value){
						if(isset($_POST["operator"][$key+1])){
							$SqlSender.= "sender='".$value."' or ";
						}else{
							$SqlSender.= "sender='".$value."'";
						}
					}
					#POST НАСЕЛЕННЫЕ ПУНКТЫ
					if ($_POST["nasp"]!="ALL" && strlen($_POST["nasp"])>5) {
						$sql=$tinsp->Query("Select `sum`,`date`,`sender` from billing_payment LEFT JOIN billing_users ON billing_users.user_dogovor = billing_payment.order where MONTH(date) = '".$_POST["period_month"]."' and YEAR(date) = '".$_POST["period_year"]."' and (".$SqlSender.") and billing_users.user_nasp = '".$_POST["nasp"]."' ", false);
					}else{
						$sql=$tinsp->Query("Select `sum`,`date`,`sender` from billing_payment where MONTH(date) = '".$_POST["period_month"]."' and YEAR(date) = '".$_POST["period_year"]."' and (".$SqlSender.")", false);
					}
					#заполним массив нулями
					foreach ($_POST["operator"] as $operator) {
						foreach ($legend_x as $days){
							$array[(integer)$days][$operator]=$array[(integer)$days][$operator];	
						}
					}
					#заполним реальными значениями
					foreach($sql as $val){
						$d=date("d", strtotime($val["date"]));
						if(in_array((integer)$d, $legend_x)){
							$array[(integer)$d][$val["sender"]]=$array[(integer)$d][$val["sender"]]+$val["sum"];
						}	
					}
					$i=1;
					#собираем данные для графика
					foreach($array as $day => $op){
						$op_value_array=array();
						foreach(array_keys ($op) as $op_value){
							$op_value_array[]=$op[$op_value];	
						}
						#график
						$data[] = $op_value_array;
						
						#считаем итоги по дням
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

	


</div>

