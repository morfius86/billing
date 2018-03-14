<?php
/*
| заявки на смену тарифного плана
|
*/
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
//$('#logtable').columnHover(); 
</script>

<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css" />
<style type="text/css">
#menu { /* Левая колонка */
    float: left; /* Обтекание справа */
	#border: 1px solid #333; /* Параметры рамки вокруг */
    padding: 0px; /* Поля вокруг текста */
}
#content { /* Правая колонка */
    margin: 10px 5px 20px 217px; /* Значения отступов */
    padding: 5px; /* Поля вокруг текста */
	#border: 1px solid #333; /* Параметры рамки */
	#width: 100%;
	#height:100%;
}

</style>
<p id="tabname">Отчет по заявкам на изменение тарифного плана</p>
	<div id="menu">
	<!--Отчеты-->
		<table width=100%>
			<tr>
				<td><form action="?page=report_request_tariff&show=1" autocomplete="off" method=post>
				<table class=forms >
				<tr><th>ФИО/Договор</th></tr>
				<tr><td><input type=text name="dogovor_fio" id="dogovor_fio"></td></tr>
				<tr><th>Статус</th></tr>
				<tr><td>
					<input type="checkbox" name="status[]" value="1" checked>Выполнено<br>
					<input type="checkbox" name="status[]" value="2" checked>Отменена<br>
					<input type="checkbox" name="status[]" value="0" checked>Новая заявка<br>
					<input type="checkbox" name="status[]" value="-1" checked>Нет денег<br>

				</td></tr>
				<tr><th>Дата начала</th></tr>
				<tr><td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td></tr>
				<tr><th>Дата окончания</th></tr>
				<tr><td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td></tr>
				<tr><td align=center><input type=submit value="Показать" ></td></tr>
				</table></form></td>
			</tr>
		</table>
	</div>
<div id="content">
	<table id="paymenttable" class="tablesorter">
		<thead class="fixedHeader">
			<tr><th width=20>#</th><th width=250>Абонент</th><th>Дата заявки</th><th>Дата исполнения</th><th>Стоимость</th><th>Тариф</th><th>up or down</th><th>Статус</th></tr>
			
		</thead>
		<tbody id=result>
		<tr><td colspan=4></td><td id="itog"></td><td colspan=3></td></tr>
			<?php
			/* ОТЧЕТ СПИСОК */
			if (count($_POST)>1 && $_GET["show"]==1){
				$ItogSumm=array("0"=>"0", "1"=>"0", "2"=>"0", "-1"=>"0");
				$RequestTariffTable=$tinsp->APIGetRequestTariff($_POST["dogovor_fio"], $_POST["status"], $_POST["period1"], $_POST["period2"]);
				$status=array("0"=>"Новая заявка", "1"=>"Выполнено", "2"=>"Отменена", "-1"=>"Нет денег");
				$updown=array("0"=>"Нижестоящий", "1"=>"Вышестойщий");
				if(is_array($RequestTariffTable[1])){
					$i=1;
					foreach($RequestTariffTable[1] as $RequestTariffTableVal){
						$ItogSumm[$RequestTariffTableVal["status"]]=$RequestTariffTableVal["sum"]+$ItogSumm[$RequestTariffTableVal["status"]];
						echo '<tr id="id'.$RequestTariffTableVal["id"].'"><td>'.$i++.'</td><td><b>'.$RequestTariffTableVal["user_name"].'</b></td><td>'.$RequestTariffTableVal["date_request"].'</td><td>'.$RequestTariffTableVal["date_done"].'</td><td>'.$RequestTariffTableVal["sum"].'</td><td>'.$RequestTariffTableVal["tarif_name"].'</td><td>'.$updown[$RequestTariffTableVal["tarif_updown"]].'</td><td>'.$status[$RequestTariffTableVal["status"]].'</td></tr>';
					}
					?>
					<script> $(document).ready(function() { $('#itog').html('<b>Выполнено на <?=$ItogSumm[1]?>  руб.</b>');}	); </script>
					<?php
				}else{
					echo '<tr><td colspan=10 align=center>Нет данных</td></tr>';

				}
				
			}else{
				echo '<tr><td colspan=10 align=center>Нажмите "Показать", чтобы сформировать отчет</td></tr>';
			}
		?>
		</tbody></table>
	


</div>

