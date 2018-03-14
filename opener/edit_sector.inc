<?php
/*	
Редактор NAS	
*/

if (isset($_GET["id"])){
	$Sector_id=$_GET["id"];
	$Sector_id_options=$tinsp->APIGetSector($Sector_id);
}else{
	exit ("Fail exist parametr");
}
?>

	<!--форма редактирования сектора-->
	<table class="forms" width="100%" height="100%">		
		<form action="query/json_query.php" id="APISetSector"></form>	
			<input type="hidden" name="obj" id="obj" value="<?=$Sector_id?>" form="APISetSector">
			<tr><th>Название сектора/vlan</th><td><input type="text" name="sector_name" id="sector_name" size="30" value="<?=$Sector_id_options[0]["sector_name"]?>" form="APISetSector"></td></tr>
			<tr><th>Vlan</th>
				<td>
				<select name="sector_vlanid[]" id="sector_vlanid" size="5" multiple form="APISetSector">
					<option value="30">30</option>
					<option value="10">10</option>
					<option value="40">40</option>
				</select>
				<input type="button" name="vlan_remove" id="vlan_remove" value="-"/>
				<input type="number" name="vlan_num" id="vlan_num" min="0" max="4098" value="0">
				<input type="button" name="vlan_add" id="vlan_add" value="+"/>
				</td></tr>
			<tr><th>Сеть</th><td><input type="text" name="sector_network" id="sector_network" size="20" value="<?=$Sector_id_options[0]["sector_network"]?>" form="APISetSector"></td></tr>
			<tr><th>Маска</th><td><input type="text" name="sector_netmask" id="sector_netmask" size="20" value="<?=$Sector_id_options[0]["sector_netmask"]?>" form="APISetSector"></td></tr>
			<tr><th>Dns1</th><td><input type="text" name="sector_dns1 id="sector_dns1" size="20" value="<?=$Sector_id_options[0]["sector_dns1"]?>" form="APISetSector"></td></tr>
			<tr><th>Dns2</th><td><input type="text" name="sector_dns2 id="sector_dns2" size="20" value="<?=$Sector_id_options[0]["sector_dns2"]?>" form="APISetSector"></td></tr>
			<tr><th>Бродкаст</th><td><input type="text" name="sector_broadcas"t id="sector_broadcast" size="20" value="<?=$Sector_id_options[0]["sector_broadcast"]?>" form="APISetSector"></td></tr>
			<tr><th>Шлюз</th><td><input type="text" name="sector_gateway" id="sector_gateway" size="20" value="<?=$Sector_id_options[0]["sector_gateway"]?>" form="APISetSector"></td></tr>
			<tr><th>DHCP<br></th>
				<td>
					<input  type="radio" name="sector_dhcp_onoff" id="sector_dhcp_onoff" value="1" <?php if ($Sector_id_options[0]["sector_dhcp_onoff"]=="1"){echo "CHECKED";} ?> form="APISetSector">Вкл
					<input type="radio" name="sector_dhcp_onoff" id="sector_dhcp_onoff" value="0" <?php if ($Sector_id_options[0]["sector_dhcp_onoff"]=="0"){echo "CHECKED";} ?> form="APISetSector">Выкл
					<br> DHCP
					<hr>
					<input type="text" name="sector_dhcp_servername" id="sector_dhcp_servername" size="20" value="<?=$Sector_id_options[0]["sector_dhcp_servername"]?>" form="APISetSector">
					<br> DHCP название сервера на микротике
					<hr>
					<input type="radio" name="sector_dhcpdyn" id="sector_dhcpdyn" value="1" <?php if ($Sector_id_options[0]["sector_dhcpdyn"]=="1"){echo "CHECKED";} ?> form="APISetSector">Вкл
					<input type="radio" name="sector_dhcpdyn" id="sector_dhcpdyn" value="0" <?php if ($Sector_id_options[0]["sector_dhcpdyn"]=="0"){echo "CHECKED";} ?> form="APISetSector">Выкл
					<br> Динамичный пул (создание записи с mac+pool)
					<hr>					
					<input type="radio" name="sector_dhcpstat" id="sector_dhcpstat" value="1" <?php if ($Sector_id_options[0]["sector_dhcpstat"]=="1"){echo "CHECKED";} ?> form="APISetSector">Вкл
					<input type="radio" name="sector_dhcpstat" id="sector_dhcpstat" value="0" <?php if ($Sector_id_options[0]["sector_dhcpstat"]=="0"){echo "CHECKED";} ?> form="APISetSector">Выкл
					<br> Статичный (обычное создание с mac+ip)
				</td>
			</tr>
			<tr><th>IP-Pool<br></th>
				<td>
					<input type="radio" name="sector_ippool" id="sector_ippool" value="1" <?php if ($Sector_id_options[0]["sector_ippool"]=="1"){echo "CHECKED";} ?> form="APISetSector">Вкл
					<input type="radio" name="sector_ippool" id="sector_ippool" value="0" <?php if ($Sector_id_options[0]["sector_ippool"]=="0"){echo "CHECKED";} ?> form="APISetSector">Выкл
					<br> 
					<input type="text" name="sector_ippool_name" id="sector_ippool_name" size="20" value="<?=$Sector_id_options[0]["sector_ippool_name"]?>" form="APISetSector">
					

				</td>
			</tr>
			<tr><td colspan="2"><input type="submit" name="sendBtn" value="Сохранить" form="APISetSector"/><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>	
		
	</table>
<script>
$("#vlan_add").click(function () {
	 $('#sector_vlanid').append($("<option></option>").attr("value",$('#vlan_num').val()).text($('#vlan_num').val())); 
});
$("#vlan_remove").click(function () {
	vlan_sel = $('#sector_vlanid').val();
	$("#sector_vlanid option[value='"+vlan_sel+"']").remove();
});
	
 $("#APISetSector").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
	console.log (form.serialize());
	//alert(form.serialize());
	/*
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			//alert(response);
			var data = eval("("+response+")");
			if (data.error == '0'){
				//обновить данные в таблице
				var nas_id = $("#obj").val();
				window.opener.$('#id'+nas_id).find('#sector_name').text($('#sector_name').val());
				window.opener.$('#id'+nas_id).find('#sector_dhcp_servername').text($('#sector_dhcp_servername').val());
				window.opener.$('#id'+nas_id).find('#sector_vlanid').text($('#sector_vlanid').val());
				window.opener.$('#id'+nas_id).find('#sector_network').text($('#sector_network').val());
				window.opener.$('#id'+nas_id).find('#sector_netmask').text($('#sector_netmask').val());
				window.opener.$('#id'+nas_id).find('#sector_dns1').text($('#sector_dns1').val());
				window.opener.$('#id'+nas_id).find('#sector_dns2').text($('#sector_dns2').val());
				window.opener.$('#id'+nas_id).find('#sector_broadcast').text($('#sector_broadcast').val());
				window.opener.$('#id'+nas_id).find('#sector_gateway').text($('#sector_gateway').val());

				window.close();
			}else{
				alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}

		}
    );
	*/
  });
</script>