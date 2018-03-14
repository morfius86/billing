<script>
    $(function() {
        $('#tarif_sms_sendday').multipleSelect({
			placeholder: "Выберите дни",
			position: 'top',
			width: 200,
            multiple: true,
            multipleWidth: 55
        });
    });
</script>

<?php
/*	
* Редактор дополнительных параметров тарифного плана 	
*/

#change settings tarif
$change_pass='';
$SaveResult='';
$param = array();
if (count($_POST)>2) {
	$_POST["tarif_change"]=json_encode($_POST["tarif_change"]);
	$_POST["tarif_sms_sendday"]=implode(",", $_POST["tarif_sms_sendday"]);
	foreach($_POST as $ch_k => $ch_v){
		$param["QUERY"][][$ch_k] = $ch_v;
	}
	$SaveTarif=$tinsp->APISetTarifArray($param);
	if ($SaveTarif[0] == 0) {
		$SaveResult="<tr><td colspan=2><div id=result>Успешно сохранен</div></td></tr>";
	}else{
		$SaveResult="<tr><td colspan=2><div id=result>Ошибка (".$SaveTarif[1]."). Обратитесь к администратору.</div></td></tr>";
	}
}

if (isset($_GET["id"])){
	$Tarif_id=$_GET["id"];
	if(strlen($Tarif_id)>2){
		$Tarif_id_options=$tinsp->APIGetTarif(2, $Tarif_id);
		if(!is_array($Tarif_id_options)){
			exit("Тариф с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}


?>
<!--форма редактирования тарифы-->

<table class="forms"  style="width:100%;">
<form action="?<?=$_SERVER["QUERY_STRING"]?>" method="post" id="APISetTarifArray">
	<th colspan=2>
		<table class="forms" width="100%">
			<tr>
				<th><a href="?opener=edit_tarif&id=<?php print $Tarif_id?>">Базовые</a></th>
				<th><a href="?opener=edit_tarif_lk&id=<?php print $Tarif_id?>">Расширенные</a></th>
				<th><a href="?opener=edit_tarif_cred&id=<?php print $Tarif_id?>">Обещанный платеж</a></th>
			</tr>
		</table>
	</th>
	<tr>
	</tr>
	<input type=hidden name=obj size=50 id=obj value="<?=$Tarif_id_options[0]["tarif_id"]?>">
	<tr><td colspan=2 align=center>Редактирование тарифа «<?=$Tarif_id_options[0]["tarif_name"]?>»</td></tr>
	<!-- save -->
	<?=$SaveResult?>
	<tr><th>Вес тарифа</th><td><input type="number" name="tarif_ves" id="edit_tarif_ves" value='<?=$Tarif_id_options[0]["tarif_ves"]?>' min="0" max="100"></td></tr>
	<tr><th colspan=2 class="zagolovok">Переход на тарифы группы</th></tr>

	<?php 
	$JsonTarifChange = json_decode($Tarif_id_options[0]["tarif_change"], true);
	if (json_last_error() != JSON_ERROR_NONE) { $JsonTarifChange=array(); }
	
	$ChangeTarif = $tinsp -> Query("SELECT * FROM billing_tarification WHERE grtarif_id = '".$Tarif_id_options[0]["grtarif_id"]."' ORDER BY grtarif_id, tarif_arhive, tarif_ves", false);
	
	if (is_array($ChangeTarif)){
		foreach ($ChangeTarif as $ChTarKey => $ChTarVal) {
			if ($Tarif_id_options[0]["tarif_id"]==$ChTarVal["tarif_id"]) {
				echo '<tr '.($ChTarVal["tarif_arhive"]=="1" ? 'style="text-decoration: line-through;"': '').'><th>'.$ChTarVal["tarif_desc"].'</th><td colspan=2></td></tr>';
				continue;
			}
			echo '<tr '.($ChTarVal["tarif_arhive"]=="1" ? 'style="text-decoration: line-through; color: rgb(179, 179, 179);"': '').'>
				<th>'.$ChTarVal["tarif_desc"].'</th>
				<td colspan=2>
					<select name="tarif_change['.$ChTarVal["tarif_id"].'][change_type]" id="tarif_change" '.(($Tarif_id_options[0]["tarif_id"]==$ChTarVal["tarif_id"])? "disabled":"").'>
					<option value="-1" '.(($JsonTarifChange[$ChTarVal["tarif_id"]]["change_type"]=="-1")? "selected":"").'>Запрещен</option>
					<option value="0" '.(($JsonTarifChange[$ChTarVal["tarif_id"]]["change_type"]=="0")? "selected":"").'>В начале месяца</option>
					<option value="1" '.(($JsonTarifChange[$ChTarVal["tarif_id"]]["change_type"]=="1")? "selected":"").'>В начале дня</option>
					</select>
					<input type="number" name="tarif_change['.$ChTarVal["tarif_id"].'][change_sum]" id=edit_tarif_prizn value="'.(!empty($JsonTarifChange[$ChTarVal["tarif_id"]]["change_sum"])? $JsonTarifChange[$ChTarVal["tarif_id"]]["change_sum"]:"0").'" min="0" max="10000">
				</td>
			</tr>';
		}
	}else{
		echo '<tr><th colspan=2>В группе нет других тарифов</th></tr>';
	}
	?>
	<tr><th colspan=2 class="zagolovok">SMS-уведомление</th></tr>
	<tr><th>Включить уведомления</th>
		<td>
		<select name="tarif_sms_alert" id="tarif_sms_alert">
			<option value="0" <?=($Tarif_id_options[0]["tarif_sms_alert"] == 0 ? 'SELECTED' : '')?>>Выкл</option>
			<option value="1" <?=($Tarif_id_options[0]["tarif_sms_alert"] == 1 ? 'SELECTED' : '')?>>Вкл</option>
		</select>
		</td>
	</tr>
	<tr><th>По каким дням делать рассылку</th>
		<td>
		
		<select name="tarif_sms_sendday[]" id="tarif_sms_sendday" multiple="multiple">
			<?php 
				for ($i=1; $i<=31; $i++) { 
					echo '<option value="'.$i.'" '.(array_search($i, explode(",", $Tarif_id_options[0]["tarif_sms_sendday"]))!==FALSE ? 'SELECTED' : '' ).'>'.$i.'&nbsp; &nbsp; &nbsp;</option>'; 
				}
			?>
		</select>
		</td>
	</tr>
	

					
</form>
<tr><td colspan=2 style="text-align: center;"><button type="submit" form="APISetTarifArray" class="button" value="Применить">Применить</button><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>

</table>
 

