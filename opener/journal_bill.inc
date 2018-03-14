<?php
/*	
Биллинг отчет	
*/
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
		if(!is_array($Abonent_id_options)){
			exit("Абонент с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
#print_r($Abonent_id_options);
?>

	<!--форма выбора периода-->
	<?php include("edit_abonent_navigation.inc"); ?>
	<table class="forms" style="width: 100%;">

		<form action="opener.php?opener=journal_bill&id=<?=$_GET["id"]?>" method="POST">
			<!--ПЕРИОД-->
			<tr><th>Дата начала</th><th>Дата окончания</th></tr>
			<tr>
				<td><input type="text" name="period1" id="period1" class="w8em format-y-m-d divider-dash highlight-days-12" value="<?php print date("Y-m-d",time()); ?>" ></td>
				<td><input type="text" name="period2" id="period2" class="w8em format-y-m-d divider-dash highlight-days-12" value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td>
			</tr>	
			<tr align=center><td colspan=2><input type="submit" name="sendBtn" value="Показать" /><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>			
		</form>	
	</table>
	<!-- Вывод результата -->
<?php
if (isset($_POST["period1"]) || isset($_POST["period2"])){
	$period1 = $_POST["period1"];
	$period2 = $_POST["period2"];
}else{
	#за 3 дня
	$period1 = date("Y-m-d",time()-(86400*3));
	$period2 = date("Y-m-d",time()+(86400*1));
}
echo '<table class="tablesorter">
		<thead class="fixedHeader"><tr><th>Дата</th><th>Оператор</th><th>Транзакция</th><th>Сумма</th><th>Новый баланс</th><th>Пользователь</th><th>Комментарий</th><th>Печать</th</tr></thead>
		<tbody id=result>';
$SelectAuth = $tinsp->APIGetPayment($Abonent_id_options[0]["user_dogovor"], NULL, "", $period1, $period2, "");
$prov_name = $tinsp->APIGetOptions("prov_name");
$prov_inn = $tinsp->APIGetOptions("prov_inn");
if (is_array($SelectAuth)){
	foreach ($SelectAuth[1] as $SelectAuthVal) {
		echo "<tr>
			<td>".$SelectAuthVal["date"]." ".$SelectAuthVal["prv_date"]."</td>
			<td>".$tinsp->ArrTypeValute[$SelectAuthVal["sender"]][1]."</td>
			<td>".$SelectAuthVal["payment_id"]."</td>
			<td>".$SelectAuthVal["sum"]."</td>
			<td>".$SelectAuthVal["balance"]."</td>
			<td>".$SelectAuthVal["access_name"]."</td>
			<td>".$SelectAuthVal["comment"]."</td>
			<td><a href='sys/print_layout/tcheck.php?user_name=".$Abonent_id_options[0]["user_name"]."&user_kvof=".$Abonent_id_options[0]["user_kvof"]."&dom_name=".$Abonent_id_options[0]["dom_name"]."&ulica_name=".$Abonent_id_options[0]["ulica_name"]."&nasp_name=".$Abonent_id_options[0]["nasp_name"]."&user_dogovor=".$Abonent_id_options[0]["user_dogovor"]."&prov_name=".$prov_name[1][0]["value"]."&prov_inn=".$prov_inn[1][0]["value"]."&sum=".$SelectAuthVal["sum"]."&payment_id=".$SelectAuthVal["id"]."&date=".$SelectAuthVal["date"]."' title='Товарный чек' target='_blank'><img src='img/print.png' width='20px'></a></td>
		</tr>";
	}
}else{

}
echo '</tbody></table>';
?>
