<?php
/*
Справочник секторов/vlan
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#sectortable").tablesorter(); 
    } 
); 

function remove_sector(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelSector",
				QUERY : id
			}, function(response) {
				alert(response);
				var data = eval("("+response+")");
				if (data.error == '0') {
					$('#id'+id).hide('slow', function() {$(this).remove();});
					$.prompt("Успешно удален!");
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
			}});
		}}
	});
}
</script>

<p id="tabname">СПРАВОЧНИК СЕГМЕНТОВ<p>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список сегментов</span></li>
		<li><span>Новый сегмент</span></li>
	</ul>
	<div class="box visible" >
		<!--список групп-->
	<table id="sectortable" class="tablesorter">
		<thead>
			<tr><th>название</th><th>vlan</th><th>сеть</th><th>маска</th><th>dns1</th><th>dns2</th><th>бродкаст</th><th>шлюз</th><th>dhcp</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php

			if (is_array($tinsp->APIGetSector()))
			foreach($tinsp->APIGetSector() as $Sector){
				echo "<tr id='id".$Sector["sector_id"]."'>
				<td id=sector_name>".$Sector["sector_name"]."</td>
				<td id=sector_vlanid>".$Sector["sector_vlanid"]."</td>
				<td id=sector_network>".$Sector["sector_network"]."</td>
				<td id=sector_netmask>".$Sector["sector_netmask"]."</td>
				<td id=sector_dns1>".$Sector["sector_dns1"]."</td>
				<td id=sector_dns2>".$Sector["sector_dns2"]."</td>
				<td id=sector_broadcast>".$Sector["sector_broadcast"]."</td>
				<td id=sector_gateway>".$Sector["sector_gateway"]."</td>
				<td id=sector_dhcp_onoff>".$Sector["sector_dhcp_onoff"]."</td>
				<td><a onclick='window.open(\"opener.php?opener=edit_sector&id=".$Sector["sector_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=500\");' href='#'><img src='img/Setting-icon.png' width='20'></a><a href='javascript:;' onclick='remove_sector(\"".$Sector["sector_id"]."\");'><img src='img/delete.png' width='16'></a></td>
				</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации сектора-->
		<table class=forms>
		<form action="query/json_query.php" id="APICreatSector">
			<tr><th>Название сектора/vlan</th><td><input type=text name=sector_name id=sector_name size=30></td></tr>
			<tr><th>Vlan</th><td><input type=text name=sector_vlanid id=sector_vlanid value="1" size=5></td></tr>
			<tr><th>Сеть</th><td><input type=text name=sector_network id=sector_network value=""></td></tr>
			<tr><th>Маска</th><td><input type=text name=sector_netmask id=sector_netmask value="255.255.255.0"></td></tr>
			<tr><th>Dns1</th><td><input type=text name=sector_dns1 id=sector_dns1 value=""></td></tr>
			<tr><th>Dns2</th><td><input type=text name=sector_dns2 id=sector_dns2 value=""></td></tr>
			<tr><th>Бродкаст</th><td><input type=text name=sector_broadcast id=sector_broadcast value=""></td></tr>
			<tr><th>Шлюз</th><td><input type=text name=sector_gateway id=sector_gateway value=""></td></tr>
			<tr><th colspan=2><b>Маршруты</b></th></tr>
			
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
		</table>
	<div id=result></div>
	<script>
	 $("#result").hide();
	 $("#APICreatSector").submit(function(event) {
	 	$("#result").show();
	    event.preventDefault();       
	    /*VARs*/
	        var form = $(this);
			var ArrayForm = form.serializeArray();
	        url = form.attr("action");	
			funct = form.attr("id");		
	    /*POST*/
	    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
			function(response){
				//json parse
				//alert(response);
				var data = eval("("+response+")");
				$("#result").html("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}
	    );
	  });
	</script>	
	</div>		
</div>
 
