<?php
/*	
Редактор абонента: биллинговая информация
*/
if (isset($_GET["id"]) && strlen($_GET["id"])>2){
	$Abonent_id_options=$tinsp->APIGetBillProp(3, $_GET["id"]);
	if(!is_array($Abonent_id_options)){
		exit("Абонент с таким id не найден");
	}
}else{
	exit ("Ошибка переданного id");
}
?>
<script>
	$(document).ready(function() {
		$("#user_mac").mask("**:**:**:**:**:**");
		
		$("#user_mac").keyup(function() {
			var mac = $("#user_mac").val();
			if (mac != 0) {
				if (isValidMac(mac)) {
					$("#validmac").html('<img src="img/validyes.png">');
				} else {
					$("#validmac").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validmac").html('');
			}
		});
		
		$("#user_ip").keyup(function() {
			var ip = $("#user_ip").val();
			if (ip != 0) {
				if (isValidIp(ip)) {
					$("#validip").html('<img src="img/validyes.png">');
				} else {
					$("#validip").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validip").html('');
			}
		});
		
		$("#user_pppip").keyup(function() {
			var ip = $("#user_pppip").val();
			if (ip != 0) {
				if (isValidIp(ip)) {
					$("#validpppip").html('<img src="img/validyes.png">');
				} else {
					$("#validpppip").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validpppip").html('');
			}
		});
	});
	
	function isValidMac(obj) {
		var pattern = new RegExp(/^([0-9A-F]{2}([:]|$)){6}$|([0-9A-F]{4}([.]|$)){3}/);
		return pattern.test(obj);
	}
	
	function isValidIp(obj) {
		var pattern = new RegExp(/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/);
		return pattern.test(obj);
	}
	
</script>

	<!--форма редактирования абонента-->
	<?php include("edit_abonent_navigation.inc"); ?>
	<table class="forms" style="width: 100%;">
		<form action="query/json_query.php" id="APISetUser">
			<input type="hidden" name="obj" id="obj" value="<?=$Abonent_id_options[0]["user_id"]?>">
			<!--БИЛИНГОВАЯ ИНФО-->
			<tr>
				<th colspan=2 class="zagolovok">БИЛИНГОВАЯ ИНФОРМАЦИЯ</th>
			</tr>
			<tr>
				<th>Сегмент</th>
				<td>
					<select name="user_sector_id" id="user_sector_id">
					<?php
						if (is_array($GetSector=$tinsp -> APIGetSector()))
						foreach ($GetSector as $Sector) {
							echo "<option value='".$Sector["sector_id"]."' ".(($Sector["sector_id"]==$Abonent_id_options[0]["user_sector_id"])? 'SELECTED':'').">".$Sector["sector_name"]."</option>";
						}
					?></select>
				</td>
			</tr>		
			<tr><th>Состояние</th>
				<td>
				<select name="user_block" id=user_block>
					<option value='0'<?php if ($Abonent_id_options[0]["user_block"]=="0") echo "selected"; ?>>Работает</option>
					<option value='1' <?php if ($Abonent_id_options[0]["user_block"]=="1") echo "selected"; ?>>Заблокирован</option>
					<option value='2' <?php if ($Abonent_id_options[0]["user_block"]=="2") echo "selected"; ?>>Приостановлен</option>
				</select>
				</td>
			</tr>
			<tr>
				<th>Тариф</th>
				<td><select name="tarif_id" id="tarif_id" required >
					<?php
						if (is_array($TarifArray=$tinsp -> APIGetTarif(2, "")))
						foreach ($TarifArray as $TarifKey => $Tarif) {
							if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
								$TarifGroup=$tinsp->APIGetGroupTarif($Tarif["grtarif_id"]);
								echo '<optgroup label="'.$TarifGroup[0]["grtarif_name"].'">';
							}
							echo "<option ".(($Tarif["tarif_id"]==$Abonent_id_options[0]["tarif_id"])? 'SELECTED':'')." value='" . $Tarif["tarif_id"] . "'>".$Tarif["tarif_desc"]." --- [".$Tarif["tarif_name"]."]</option>";
							
							if($TarifArray[$TarifKey+1]["grtarif_id"]!=$Tarif["grtarif_id"]){
								echo '</optgroup>';
							}
						}
					?>
				</select></td>
			</tr>
			
			<!--СЕТЕВЫЕ НАСТРОЙКИ-->
			<?php  $Dogovor = $tinsp -> APIGetOptions("billing_order"); ?>
			<tr>
				<th>Договор</th><td>
					<input type="text" name="user_dogovor" id="user_dogovor" value="<?=$Abonent_id_options[0]["user_dogovor"]?>">
				</td>
			</tr>
			<tr><th>IPoE</th><td style="float:left;">
				<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_ip"><i>IP-адрес</i></label>
					<input type="checkbox" name="user_autoip" id="user_autoip" value="1"><input type="text" id="user_ip" name="user_ip" title="IP-адрес формата x.x.x.x" value="<?=$Abonent_id_options[0]["user_ip"]?>" /><span id="validip"></span>
				</div>
				<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_mac" ><i>MAC-адрес</i></label>
					<input type="text" id="user_mac" name="user_mac" value="<?=$Abonent_id_options[0]["user_mac"]?>"  title="MAC-адрес в формате XX:XX:XX:XX:XX:XX" /><span id="validmac"></span>
				</div>
			</td></tr>
			<tr><th>PPP</th><td style="float:left;">
				<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_pppip"><i>IP-адрес</i></label>
					<input type="text" id="user_pppip" name="user_pppip" value="<?=$Abonent_id_options[0]["user_pppip"]?>" title="IP-адрес формата x.x.x.x" /><span id="validpppip"></span>
				</div>
				<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_login"><i>Логин</i></label>
					<input type="text" id="user_login" value="<?=$Abonent_id_options[0]["user_login"]?>" name="user_login"  />
				</div>
				<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_password"><i>Пароль</i></label>
					<input type="text" id="user_password" value="<?=$Abonent_id_options[0]["user_password"]?>" name="user_password"  />
				</div>
			</td></tr>	
			<tr><td colspan=2><input type="submit" name="sendBtn" class="button" value="Сохранить" /><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>				
		</form>	
	</table>
<script>
//LASP IP
$("#user_autoip").click(function() {
	if ($('#user_autoip:checked').val() == "1") {
		$.post("query/json_query.php", {
			FUNCTION : "APIGetLastIP",
			QUERY : $('#user_sector_id').val()
			}, function(response) {
			var data = eval("(" + response + ")");
			if (data.error[0] != "0") {
				$("#user_autoip").removeAttr("checked");
				$.prompt(data.error[1]);
			} else {
				$("#user_ip").val(data.error[1]);
			}
		});
	} else {
		$("#user_ip").val('');
	}
});

//отправка формы
$("#APISetUser").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			alert(response);
			var data = eval("("+response+")");
			if (data.error[0] == '0'){
				//обновить данные в таблице			
				var user_id = $("#obj").val();
				window.opener.$('#id'+user_id).find('#user_name').text($('#user_name').val());
				window.opener.$('#id'+user_id).find('#user_nasp').text($('#user_nasp option:selected').html());
				window.opener.$('#id'+user_id).find('#user_ulica').text($('#user_ulica option:selected').html());
				window.opener.$('#id'+user_id).find('#user_dom').text($('#user_dom option:selected').html());
				window.opener.$('#id'+user_id).find('#user_kvof').text($('#user_kvof').val());
				window.opener.$('#id'+user_id).find('#user_mobphone').text($('#user_mobphone').val());
				window.opener.$('#id'+user_id).find('#user_ip').text($('#user_ip').val());
				window.opener.$('#id'+user_id).find('#user_pppip').text($('#user_pppip').val());
				window.opener.$('#id'+user_id).find('#user_mac').text($('#user_mac').val());
				alert("Успешно сохранено!");
			}else{
				alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}
		}
    );
  });
</script>