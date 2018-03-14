<?php
/*	
Редактор NAS	
*/

if (isset($_GET["id"])){
	$Nas_id=$_GET["id"];
	$Nas_id_options=$tinsp->APIGetNas(2, $Nas_id);
}else{
	exit ("Fail exist parametr");
}

?>
	<!--форма редактирования nas-->
	<table class=forms>
		<tr><th>Сектор</th>
			<td>
			<select name="nas_sector_val" size=4 multiple id="nas_sector_val">
			<?php
				if (is_array($tinsp->APIGetSector()))
				foreach($tinsp->APIGetSector() as $sector){
					if(eregi(",",$Nas_id_options[0]["nas_sector_id"])){
						if(in_array($sector["sector_id"], explode(",",trim($Nas_id_options[0]["nas_sector_id"])))){
							echo "<option value='".$sector["sector_id"]."' selected>".$sector["sector_name"]."</option>";
						}else{
							echo "<option value='".$sector["sector_id"]."'>".$sector["sector_name"]."</option>";
						}
					}else{
						if($sector["sector_id"]==trim($Nas_id_options[0]["nas_sector_id"])){
							echo "<option value='".$sector["sector_id"]."' selected>".$sector["sector_name"]."</option>";
						}else{
							echo "<option value='".$sector["sector_id"]."'>".$sector["sector_name"]."</option>";
						}
					}	
				}
			?>
			</select>
			</td>
		</tr>
		<form action="query/json_query.php" id="APISetNas">
			<input type=hidden name=obj id=obj value="<?=$Nas_id?>">
			<tr><th>Название</th><td><input type=text name=nas_name id=nas_name size=30 value="<?=$Nas_id_options[0]["nas_name"]?>"></td></tr>
			<!-- скрытый text для занесения выбранных насов через разделитель -->
			<input type=hidden name=nas_sector_id id=nas_sector_id>
			<tr><th>IP</th><td><input type=text name=nas_ip id=nas_ip size=20 value="<?=$Nas_id_options[0]["nas_ip"]?>"></td></tr>
			<tr><th>Логин</th><td><input type=text name=nas_login id=nas_login size=20 value="<?=$Nas_id_options[0]["nas_login"]?>"></td></tr>
			<tr><th>Пароль</th><td><input type=text name=nas_password id=nas_password size=20 value="<?=$Nas_id_options[0]["nas_password"]?>"></td></tr>
			<tr><th>Секрет</th><td><input type=text name=nas_secret id=nas_secret size=20 value="<?=$Nas_id_options[0]["nas_secret"]?>"></td></tr>
			<tr><th>Тип NAS</th>
				<td>
					<select name="nas_type" id=nas_type>
					<option value="6" <?php if($Nas_id_options[0]["nas_type"] == 6)echo("selected");?>>Mikrotik API HotSpot (mac)</option>
					<option value="7" <?php if($Nas_id_options[0]["nas_type"] == 7)echo("selected");?>>Mikrotik API HotSpot (mac+ip)</option>
					<option value="1" <?php if($Nas_id_options[0]["nas_type"] == 1)echo("selected");?>>Mikrotik Radius HotSpot (ip+mac)</option>
					<option value="2" <?php if($Nas_id_options[0]["nas_type"] == 2)echo("selected");?>>Mikrotik Radius HotSpot (ip)</option>
					<option value="3" <?php if($Nas_id_options[0]["nas_type"] == 3)echo("selected");?>>Mikrotik Radius HotSpot (mac)</option>
					<option value="4" <?php if($Nas_id_options[0]["nas_type"] == 4)echo("selected");?>>Mikrotik Radius PPTP (log+pass)</option>
					<option value="5" <?php if($Nas_id_options[0]["nas_type"] == 5)echo("selected");?>>Mikrotik Radius PPTP (log+pass+ip)</option>
					</select>
				</td>
			</tr>
			
			<tr><th>UPLINK интерфейс</th><td><input type="text" name="nasp_uplink_interface" id="nasp_uplink_interface" size=20 value="<?=$Nas_id_options[0]["nasp_uplink_interface"]?>"></td></tr>

			<tr><th>Шейпер</th>
				<td>
					<select name="nas_shaper" id=nas_shaper>
					<option value="0" <?php if($Nas_id_options[0]["nas_shaper"] == 0)echo("selected");?>>Нет</option>
					<option value="1" <?php if($Nas_id_options[0]["nas_shaper"] == 1)echo("selected");?>>Radius</option>
					<option value="2" <?php if($Nas_id_options[0]["nas_shaper"] == 2)echo("selected");?>>Mikrotik Queue Simple</option>
					</select>
				</td>
			</tr>
			<tr><th>Queue Type</th><td><input type="text" name="nasp_queue_type" id="nasp_queue_type" size=20 value="<?=$Nas_id_options[0]["nasp_queue_type"]?>"></td></tr>
			<tr><th>Добавлять в Adress-list согласно <br>группе абонента(Mikrotik/Radius)</th>
				<td>
					<select name="nas_addresslist" id=nas_addresslist>
					<option value="0" <?php if($Nas_id_options[0]["nas_addresslist"] == 0)echo("selected");?>>Нет</option>
					<option value="1" <?php if($Nas_id_options[0]["nas_addresslist"] == 1)echo("selected");?>>Да</option>
					</select>
				</td>
			</tr>
			<tr><th>Добавлять IP/MAC в ARP-Static</th>
				<td>
					<select name="nas_arp" id=nas_arp>
					<option value="0" <?php if($Nas_id_options[0]["nas_arp"] == 0)echo("selected");?> >Нет</option>
					<option value="1" <?php if($Nas_id_options[0]["nas_arp"] == 1)echo("selected");?>>Да</option>
					</select>
				</td>
			</tr>
			<tr><th>DHCP</th>
				<td>
					<select name="nas_dhcp" id=nas_dhcp>
					<option value="0" <?php if($Nas_id_options[0]["nas_dhcp"] == 0)echo("selected");?> >Нет</option>
					<option value="1" <?php if($Nas_id_options[0]["nas_dhcp"] == 1)echo("selected");?>>Да</option>
					</select>
				</td>
			</tr>
			<tr><td colspan=2><input type="submit" name="sendBtn" value="Сохранить" id="save" /><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>	
		</form>	
	</table>
<script>
$(function() {
	$('#nas_sector_val').change(function() {
		
	}).multipleSelect({
		width: 200
	});
});


				
$("#APISetNas").submit(function(event) {
    event.preventDefault();    
	$('#nas_sector_id').val($('#nas_sector_val').val());	
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serialize();
        url = form.attr("action");	
		funct = form.attr("id");	
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			var data = eval("("+response+")");
			
			if (data.error == '0'){
				//обновить данные в таблице
				var nas_id = $("#obj").val();
				window.opener.$('#id'+nas_id).find('#nas_name').text($('#nas_name').val());
				window.opener.$('#id'+nas_id).find('#nas_ip').text($('#nas_ip').val());
				window.opener.$('#id'+nas_id).find('#nas_login').text($('#nas_login').val());
				window.opener.$('#id'+nas_id).find('#nas_password').text($('#nas_password').val());
				window.opener.$('#id'+nas_id).find('#nas_type').text($('#nas_type option:selected').val());
				window.opener.$('#id'+nas_id).find('#nas_shaper').text($('#nas_shaper option:selected').val());
				//перебираем все селекты нас-ов
				window.opener.$('#id'+nas_id).find('#nas_sector_id').html("");
				$("#nas_sector_val option:selected").each(function () {
					//alert($(this).html());
					window.opener.$('#id'+nas_id).find('#nas_sector_id').append($(this).html() + "<br>");
					//str += $(this).text() + " ";
				});

				window.close();
			}else{
				alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}

		}
    );
  });
</script>