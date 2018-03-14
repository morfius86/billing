<?php
/*	
Редактор NAS	
*/
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
<table class="forms" id="APISetTarifArray" style="width:100%;">
	<th colspan=2>
		<table class="forms" width="100%">
			<tr>
				<th><a href="?opener=edit_tarif&id=<?php print $Tarif_id?>">Базовые</a></th>
				<th><a href="?opener=edit_tarif_lk&id=<?php print $Tarif_id?>">Расширенные</a></th>
				<th><a href="?opener=edit_tarif_cred&id=<?php print $Tarif_id?>">Обещанный платеж</a></th>
			</tr>
		</table>
	</th>
	<input type=hidden name=obj size=50 id=obj value="<?=$Tarif_id_options[0]["tarif_id"]?>">
	<!-- параметры -->
	<tr><td colspan=2 align=center>Редактирование тарифа «<?=$Tarif_id_options[0]["tarif_name"]?>»</td></tr>
	<tr><th>Архивный</th>
		<td>
			<select name="tarif_arhive" id="tarif_arhive">
				<option value="0" <?php if($Tarif_id_options[0]["tarif_arhive"]=="0") echo "SELECTED"; ?>>Нет</option>
				<option value="1" <?php if($Tarif_id_options[0]["tarif_arhive"]=="1") echo "SELECTED"; ?>>Да</option>
			</select>
		</td>
	</tr>
	<tr><th>Название тарифа</th><td><input type="text" name=tarif_name id=edit_tarif_name value='<?=$Tarif_id_options[0]["tarif_name"]?>'></td></tr>
	<tr><th>Описание тарифа</th><td><input type="text" name="tarif_desc" id="edit_tarif_desc" value='<?=$Tarif_id_options[0]["tarif_desc"]?>' size="40"></td></tr>
	
	<tr>
		<th>Группа тарифов</th>
		<td>
			<select name="grtarif_id" id="edit_grtarif_id">
				<?php 
					$GroupTarif=$tinsp->APIGetGroupTarif("");
					if (is_array($GroupTarif)){
						foreach($GroupTarif as $GroupTarifVal){
							echo '<option value="'.$GroupTarifVal["grtarif_id"].'" '.(($GroupTarifVal["grtarif_id"]==$Tarif_id_options[0]["grtarif_id"])? 'selected': '' ).'>'.$GroupTarifVal["grtarif_name"].'</option>';
						}
					}
				?>
			</select>
		</td>
	</tr>
	<tr><th>Типа тарификации</th>
		<td>
			<select name="type" id="edit_type">
			<?php
				foreach($tinsp->APIGetTypeTarif("") as $TypeTarif){
					if($TypeTarif["key"]==$Tarif_id_options[0]["type"]){
						echo '<option selected value="'.$TypeTarif["key"].'">'.$TypeTarif["value"].'</option>';
					}else{
						echo '<option value="'.$TypeTarif["key"].'">'.$TypeTarif["value"].'</option>'; 
					}		
				}
			?>
			</select>
		</td>
	</tr>
	<tr><th>Тип абон. платы</th>
		<td>
			<select name="type_abon" id=edit_type_abon>
			<?php
				foreach($tinsp->APIGetTypeTarifAbon("") as $TypeAbonCash){
					if($TypeAbonCash["key"]==$Tarif_id_options[0]["type_abon"]){
						echo '<option selected value="'.$TypeAbonCash["key"].'">'.$TypeAbonCash["value"].'</option>';
					}else{
						echo '<option value="'.$TypeAbonCash["key"].'">'.$TypeAbonCash["value"].'</option>'; 
					}		
				}
			?>
			</select>
		</td>
	</tr>
	<tr><th>Абон. плата</th><td><input type=text name=abon_cash id=edit_abon_cash value='<?=$Tarif_id_options[0]["abon_cash"]?>'></td></tr>
	<tr><th>Блокировать</th>
		<td>
			<select name="tarif_block" id=edit_tarif_block>
			<?php
				if($Tarif_id_options[0]["tarif_block"]==0){
					echo '<option value="1">Нет</option>'; 
					echo '<option selected value="0">Да</option>'; 
				}else{
					echo '<option value="0">Да</option>'; 
					echo '<option selected value="1">Нет</option>'; 
				}
			?>
			</select>
			
		</td>
	</tr>
	<tr><th>Валюта</th><td><input type=text name=valute id=edit_valute value='<?=$Tarif_id_options[0]["valute"]?>'></td></tr>
	<tr><th>Вход. скорость/ Мбит</th><td><input type=text name=tarif_speed_in id=edit_tarif_speed_in value='<?=$Tarif_id_options[0]["tarif_speed_in"]?>'></td></tr>
	<tr><th>Исход. скорость/ Мбит</th><td><input type=text name=tarif_speed_out id=edit_tarif_speed_out value='<?=$Tarif_id_options[0]["tarif_speed_out"]?>'></td></tr>
	<tr>
		<th colspan=2 class="zagolovok">MIKROTIK</th>
	</tr>
	<tr><th title="Посылает атрибут Framed-IP-Pool <br> для использования Pool микротика">Посылать Mik IP-Pool <img src="img/question.png" width=12></th>
		<td>
			<select name="mik_ippool" id=mik_ippool>
				<option value="0" <?php if($Tarif_id_options[0]["mik_ippool"]=="0") echo "SELECTED"; ?>>Нет</option>
				<option value="1" <?php if($Tarif_id_options[0]["mik_ippool"]=="1") echo "SELECTED"; ?>>Да</option>
			</select>
			<input type=text name=mik_ippool_name id=mik_ippool_name value="<?=$Tarif_id_options[0]["mik_ippool_name"]?>">
		</td>
	</tr>
	<tr><th title="Посылает атрибут Mikrotik-Address-list <br> для использования в адрес-лист firewall">Mik Address-list <img src="img/question.png" width=12></th>
		<td>
			<input type="text" name="mik_adlist" id="mik_adlist" value="<?=$Tarif_id_options[0]["mik_adlist"]?>">
		</td>
	</tr>
	<tr>
		<th colspan=2 class="zagolovok">Дополнительно</th>
	</tr>
	<tr><th>Перенести на тариф</th>
		<td>
			<select name="tarif_move_onoff" id=tarif_move_onoff>
				<option value="0" <?php if($Tarif_id_options[0]["tarif_move_onoff"]=="0") echo "SELECTED"; ?>>нет</option>
				<option value="1" <?php if($Tarif_id_options[0]["tarif_move_onoff"]=="1") echo "SELECTED"; ?>>да</option>
			</select>
			<select name="tarif_move_tarif" id=tarif_move_tarif>
				<?php
					foreach($tinsp->APIGetTarif(2, "") as $Tarif){
						if($Tarif["tarif_id"]==$Tarif_id_options[0]["tarif_move_tarif"]){
							echo '<option selected value="'.$Tarif["tarif_id"].'">'.iconv("utf-8", "windows-1251", $Tarif["tarif_name"]).'</option>';
							}else{
							echo '<option value="'.$Tarif["tarif_id"].'">'.iconv("utf-8", "windows-1251", $Tarif["tarif_name"]).'</option>'; 
						}		
					}
				?>
			</select>
			
		</td>
	</tr>
	<tr><th>если нет операций по счету в течении</th>
		<td>
			<input type=number name=tarif_move_day id=tarif_move_day value='<?=$Tarif_id_options[0]["tarif_move_day"]?>' min="0" max="999"> дней
		</td>
	</tr>
	<tr><th>и очистить сетевые настройки </th>
		<td>
			<select name="tarif_move_clearnetwork" id=tarif_move_clearnetwork>
				<option value="0" <?php if($Tarif_id_options[0]["tarif_move_clearnetwork"]=="0") echo "SELECTED"; ?>>нет</option>
				<option value="1" <?php if($Tarif_id_options[0]["tarif_move_clearnetwork"]=="1") echo "SELECTED"; ?>>да</option>
			</select>
		</td>
	</tr>
	<tr><td colspan=2><input type="submit" name="sendBtn" value="Сохранить" id="save"/><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>				
</table>
<script>
	$("#result").hide();
	 $("#save").click(function(event) {

		$("#result").show();
		$('#APISetTarifArray').each(function() {
			var values = {};
			$('input[type=hidden],input[type=number],input[type=text],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.value);
			});
			$.post("query/json_query.php", {
				FUNCTION : "APISetTarifArray",
				'QUERY[]' : values
			}, function(response) {
				//json parse
				alert(response);
				
			});
		});

	}); 

</script>