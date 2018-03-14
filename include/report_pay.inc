<?php
/*
| Поиск платежей
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
#menu { /* Левая колонка */
    float: left; /* Обтекание справа */

}
#content { /* Правая колонка */


}

</style>
<p id="tabname">Отчет по платежам</p>
	<div id="menu">
	<!--Отчеты-->
		<table width=100%>
			<tr>
				<td><form action="?page=report_pay&table=1" autocomplete="off" method=post>
				<table class=forms >
				<tr><th>ФИО/Договор</th></tr>
				<tr><td><input type=text name="pay_dogovor_fio" id="pay_dogovor_fio"></td></tr>
				<tr><th>Номер транзакции</th></tr>
				<tr><td><input type=text name="pay_txn"></td></tr>
				<tr><th>Операторы</th></tr>
				<tr><td>
						<details open="open">
						<summary align=left>Внешние</summary>
						<?php
							foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
								if ($ArrTypeValuteVal[0] == '0'){
									echo '<input type="checkbox" name="pay_sender[]" value="'.$ArrTypeValuteKey.'" CHECKED>'.$ArrTypeValuteVal[1].'<br>';
								}
							}
						?>
						</details>
						<details>
						<summary align=left>Внутренние</summary>
						<?php
							foreach ($tinsp->ArrTypeValute as $ArrTypeValuteKey => $ArrTypeValuteVal){
								if ($ArrTypeValuteVal[0] == '1'){
									echo '<input type="checkbox" name="pay_sender[]" value="'.$ArrTypeValuteKey.'" >'.$ArrTypeValuteVal[1].'<br>';
								}
							}
						?>
						</details>
				</td></tr>
				<tr><th>Пользователь</th></tr>
				<tr><td>
				<select name="pay_access" id=pay_access>
				<option value="">Все</option>
				<?php
					if (is_array($tinsp->Query("Select * From billing_access",false)))
					foreach($tinsp->Query("Select * From billing_access order by access_name",false) as $Access){
						echo "<option value='".$Access["access_id"]."'>".$Access["access_name"]."</option>";
					}
				?>
				</select>
				</td></tr>
				<tr><th>Дата начала</th></tr>
				<tr><td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td></tr>
				<tr><th>Дата окончания</th></tr>
				<tr><td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td></tr>
				<tr><th>Сальдо</th></tr>
				<tr><td>
						<select name="pay_buh" id=pay_buh>
							<option value="2">Любое</option>
							<option value="0">Поступление</option>
							<option value="1">Списание</option>
						</select>
				</td></tr>
				<tr><td align=center><input type=submit value="Показать" ></td></tr>
				</table></form></td>
			</tr>
		</table>
	</div>
<div id="content">
	<table id="paymenttable" class="tablesorter">
		<thead class="fixedHeader">
			<tr><th width=20>#</th><th width=250>Абонент</th><th>Пользователь</th><th>Оператор</th><th>Транзакция</th><th>Договор</th><th>Сумма</th><th>Новый баланс</th><th width=130>Дата</th><th width=250>Комментарий</th></tr>
			<tr><th colspan=6></th><th id=itog> </th><th colspan=3></th></tr>
		</thead>
		<tbody>
			<?php
			/* ОТЧЕТ СПИСОК */
			if (count($_POST)>1 && $_GET["table"]==1){
				$Accesslist = $tinsp->APIGetAccess();
				$Userslist = $tinsp->APIGetBillProp(4, "");
				
				$PaymentTable=$tinsp->APIGetPayment($_POST["pay_dogovor_fio"],$_POST["pay_txn"], $_POST["pay_sender"], $_POST["period1"], $_POST["period2"], $_POST["pay_buh"], $_POST["pay_access"]);
				//print_r($Userslist);
				if(is_array($PaymentTable[1])){
					$i=1;
					foreach($PaymentTable[1] as $PaymentTableVal){
						$ItogSumm=$PaymentTableVal["sum"]+$ItogSumm;
						echo '<tr id="id'.$PaymentTableVal["id"].'"><td>'.$i++.'</td><td><b>'.$Userslist[$PaymentTableVal["user_id"]]["user_name"].'</b></td><td>'.$Accesslist[$PaymentTableVal["access_id"]]["access_name"].'</td><td>'.$PaymentTableVal["sender"].'</td><td>'.$PaymentTableVal["payment_id"].'</td><td>'.$PaymentTableVal["order"].'</td><td>'.$PaymentTableVal["sum"].'</td><td>'.$PaymentTableVal["balance"].'</td><td>'.$PaymentTableVal["date"].' '.$PaymentTableVal["prv_date"].'</td><td>'.$PaymentTableVal["comment"].'</td></tr>';
					}
					?>
					<script> $(document).ready(function() { $('#itog').html('<?=$ItogSumm?>');}	); </script>
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

