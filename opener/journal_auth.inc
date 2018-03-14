<?php
/*	
Журнал авторизаций	
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
	<table class="forms" style="width: 100%;">
		<?php include("edit_abonent_navigation.inc"); ?>
		<form action="opener.php?opener=journal_auth&id=<?=$_GET["id"]?>" method="POST">
			<!--ПЕРИОД-->
			<tr><th>Дата начала</th><th>Дата окончания</th></tr>
			<tr>
				<td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td>
				<td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td>
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
		<thead class="fixedHeader"><tr><th width=20>#</th><th>UserName</th><th>FramedIP</th><th>Мбайт/TX</th><th>Мбайт/RX</th><th>Подключился</th><th>Отключился</th></tr></thead>
		<tbody id=result>';
$SelectAuth = $tinsp->Query("Select * from billing_users_auth where `user_id` like '%".$_GET["id"]."'
and (authdate_start >= '".$period1."' and authdate_start < '".$period2."')",false);
if (is_array($SelectAuth)){
	foreach ($SelectAuth as $SelectAuthVal) {
		echo "<tr><td>".$SelectAuthVal["id"]."</td>
			<td>".$SelectAuthVal["user_name"]."</td>
			<td>".$SelectAuthVal["framed_ip_address"]."</td>
			<td>".number_format(($SelectAuthVal["acctoutputoctets"]/1048576), 2)."</td>
			<td>".number_format(($SelectAuthVal["acctinputoctets"]/1048576), 2)."</td>
			<td>".$SelectAuthVal["authdate_start"]."</td>
			<td>".$SelectAuthVal["authdate_stop"]."</td>
		</tr>";
	}
}else{

}
echo '</tbody></table>';
?>
