<?php
/*	
Редактор абонентов: Реквизиты
*/
#change pass settings

if (count($_POST)>2) {
	$result='';
	$update='';
	$i=0;
	foreach ($_POST["q"] as $k => $v){
		$i++;
		$update.= "".$k." = '".$v."'".((count($_POST["q"]) == $i)? '':', ')."";
	}
	$tinsp->Query("UPDATE billing_users_requisites SET ".$update." WHERE user_id = '".$_POST["obj"]."'");
	if($tinsp->sql_errno!=0){
		$result="<tr><td colspan=2><div id=result>Ошибка сохранения. Обратитесь к администратору.</div></td></tr>";
	}else{
		$qqq=$tinsp->APICreatEvent("billing", $_POST["obj"], "", $_SESSION["access_id"], "", "Edit abonent: ".str_replace(array("'", "&"), array("", "<br>"), $update)."");;
		$result="<tr><td colspan=2><div id=result>Успешно сохранен.</div></td></tr>";
	}
}
#valid guid
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
//print_r($Abonent_id_options);
?>
<script type='text/javascript'>


</script>

<!--форма редактирования параметров личного кабинета абонента -->
<?php include("edit_abonent_navigation.inc"); ?>
<table class="forms" style="width: 100%;">
	
	<form action="<?=$_SERVER["REQUEST_URI"]?>" method="post" id="APISetUserReq">
	<input type="hidden" name="obj" id="obj" value="<?=$Abonent_id_options[0]["user_id"]?>">

	<!-- РЕКВИЗИТЫ -->
	<tr>
		<th colspan=2 class="zagolovok">РЕКВИЗИТЫ</th>
	</tr>
	<?php
		$GroupSelect=$tinsp -> APIGetBillProp(2, $Abonent_id_options[0]["group_id"]);
		if ($GroupSelect[0]["group_type"] == 0){
			#физ лица
	?>
		<tr><th>Паспорт</th><td style="float:left;">
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="birth_day"><i>Дата Рождения</i>*</label><input type="date" size="40" name="q[birth_day]" id="birth_day" value="<?=$Abonent_id_options[0]["birth_day"]?>" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Серия</i></label><input type="text" size="40" id="pasp_ser" name="q[pasp_ser]" value="<?=$Abonent_id_options[0]["pasp_ser"]?>" required="" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Номер</i></label><input type="text" size="40" id="pasp_num" name="q[pasp_num]" value="<?=$Abonent_id_options[0]["pasp_num"]?>" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_date"><i>Когда</i></label><input type="date" size="40" id="pasp_date" name="q[pasp_date]"  value="<?=$Abonent_id_options[0]["pasp_date"]?>" required="" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_who"><i>Кем</i></label><input type="text" size="40" id="pasp_who" name="q[pasp_who]" value="<?=$Abonent_id_options[0]["pasp_who"]?>" /></div>
		</td></tr>
		<tr><th>Адрес регистрации</th><td style="float:left;">
			<input type="text" id="pasp_addr" size="40" name="q[pasp_addr]" value="<?=$Abonent_id_options[0]["pasp_addr"]?>" />
		</td></tr>
	<?php
		}elseif($GroupSelect[0]["group_type"] == 1){
			#юр лица
	?>
		<tr><th>Название фирмы</th><td style="float:left;">
			<input type="text" id="yur_name" name="yur_name" value="<?=$Abonent_id_options[0]["yur_name"]?>" required="" size="40" />
		</td></tr>
		<tr><th>Ф.И.О. ген. директора или<br> лица по доверенности.</th><td style="float:left;">
			<input type="text" id="yur_gen" name="yur_gen" value="<?=$Abonent_id_options[0]["yur_gen"]?>" required="" size="40" />
		</td></tr>
		<tr><th>Адреса</th><td style="float:left;">
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Юридический</i></label><input type="text" id="yur_ad_yur" name="q[yur_ad_yur]" value="<?=$Abonent_id_options[0]["yur_ad_yur"]?>" required="" size="30"/></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Почтовый</i></label><input type="text" id="yur_ad_postal" name="q[yur_ad_postal]" value="<?=$Abonent_id_options[0]["yur_ad_postal"]?>" size="30"/></div>
		</td></tr>
		<tr><th>ИНН/КПП</th><td style="float:left;">
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>ИНН</i></label><input type="text" id="yur_inn" name="q[yur_inn]" value="<?=$Abonent_id_options[0]["yur_inn"]?>" required="" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>КПП</i></label><input type="text" id="yur_kpp" name="q[yur_kpp]" value="<?=$Abonent_id_options[0]["yur_kpp"]?>" /></div>
		</td></tr>
		<tr><th>Счет</th><td style="float:left;">
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Счет №</i></label><input type="text" id="yur_bank_num" name="q[yur_bank_num]" value="<?=$Abonent_id_options[0]["yur_bank_num"]?>" required="" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>в банке</i></label><input type="text" id="yur_bank_name" name="q[yur_bank_name]" value="<?=$Abonent_id_options[0]["yur_bank_name"]?>" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Кор. счет</i></label><input type="text" id="yur_bank_kornum" name="q[yur_bank_kornum]" value="<?=$Abonent_id_options[0]["yur_bank_kornum"]?>" /></div>
			<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>БИК</i></label><input type="text" id="yur_bank_bik" name="q[yur_bank_bik]" value="<?=$Abonent_id_options[0]["yur_bank_bik"]?>" /></div>
		</td></tr>
	<?php						
		}
	?>
		<tr><td colspan="2" align="center"><input type="submit" name="sendBtn" class="button" value="Сохранить"/><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>	
	</form>	
			
	<?=$result?>		
</table>


