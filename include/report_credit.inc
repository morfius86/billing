<?php
/*
Отчеты по обещанным платежам 
*/


?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='text/javascript' src='js/jquery.autocomplete.js'></script>
<script> 
$(function() {
	$("#event_fio").autocomplete({
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
        $("#table").tablesorter(); 
	}	
);

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
<p id="tabname">Отчет по обещанным платежам<p>
<div class="section">
	<!--Отчеты-->
	<form action="?page=report_credit" autocomplete="off" method=post>
		<table class=forms >
		<tr><th>ФИО/Договор</th><th>Состояние</th></tr>
		<tr>
			<td>
				<input type=text name="credit_fio" id="credit_fio">
			</td>
			<td>
				<select name="credit_type" id=credit_type>
				<option value="">Все</option>
					<option value="0">Активен</option>
					<option value="1">Не активен</option>
				</select>
			</td>
		</tr>
		<tr><th>Дата начала</th><th>Дата окончания</th></tr>
		<tr>
			<td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td>
			<td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td>
		</tr>
		<tr>
			<td colspan=2 align=center><input type=submit value="Показать" ></td>
		</tr>
		</table>
	</form>	
	
			<?php
			/* ОТЧЕТ */
			if (count($_POST)>1){
				$i=1;
				$sumitog=0;
				#заголовок
				echo '<table id="table" class="tablesorter">
					<thead class="fixedHeader">
						<tr><th width=20>#</th><th>Абонент</th><th>Когда взят</th><th>Когда снят</th><th>Сумма</th><th>Статус</th></tr>
					</thead>
					<tbody id=result>';
				#ищем абонента по фио и договору
				if(strlen($_POST["credit_fio"])>2){
					$User=$tinsp->APISearchAbonent("!".$_POST["credit_fio"], "", "",  "",  "",  "");
					if(is_array($User[1])){
						$_POST["credit_fio"]=$User[1][0]["user_id"];
					}
				}else{
					$_POST["credit_fio"]="";
				}
				#формируем скуль запрос
				$CreditQuery=$tinsp->Query("SELECT billing_users_credit.*, billing_users.user_name as user_name_real 
				FROM billing_users_credit 
				LEFT JOIN billing_users ON billing_users.user_id = billing_users_credit.user_id	
				WHERE billing_users_credit.credit_state LIKE '%".$_POST["credit_type"]."%'
				AND billing_users_credit.user_id like '%".$_POST["credit_fio"]."%'
				AND (UNIX_TIMESTAMP(billing_users_credit.start_date) >= UNIX_TIMESTAMP('".$_POST["period1"]."') and UNIX_TIMESTAMP(billing_users_credit.start_date) < UNIX_TIMESTAMP('".$_POST["period2"]."'))", false);
				#print_r($CreditQuery);
				#имя автора
				if(is_array($CreditQuery)){
					foreach ($CreditQuery as $CreditVal) {
						echo '<tr id="id'.$CreditVal["id"].'"><td>'.$i++.'</td><td>'.$CreditVal["user_name_real"].'</td><td>'.$CreditVal["start_date"].'</td><td>'.$CreditVal["end_date"].'</td><td>'.$CreditVal["sum"].'</td><td>'.($CreditVal["credit_state"] == 0 ? 'Активный' : 'Не активный').'</td></tr>';
						$sumitog=$CreditVal["sum"]+$sumitog;
					}
				}else{
					echo '<tr><td colspan=8 align=center>Нет данных</td></tr>';

				}
				echo'<tfoot>
					<tbody>
						<tr><td colspan=4></td><td>Итого: '.$sumitog.' руб.</td><td></td></tr>
					</tbody>
				</tdfoot>';
				echo '</tbody></table>';
			}
		?>

	


</div>

