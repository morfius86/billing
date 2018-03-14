<?php
/*
Справочник оборудования
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#switchtable").tablesorter(); 
		
		//select ulica->dom
		$('select[name=switch_ulica]').change(function () {
		var selectdom = '';		
        var ulica_id = $(this).val();
		var arr = {};
		arr['0'] = {};
		arr['0']['value'] = "2";
		arr['1'] = {};
		arr['1']['value'] = "";	
		arr['2'] = {};
		arr['2']['value'] = ulica_id;	
		$.post("query/json_query.php", {FUNCTION: "APIGetDom", QUERY:arr}, function(response){
			var data_dom = eval("("+response+")");
			$.each(data_dom.error, function(key, dom_value){
				selectdom+='<option value="'+dom_value.dom_id+'">'+dom_value.dom_name+''+dom_value.dom_letter+'</option>'; 	
			});	
		$("select[name=switch_dom]").html(selectdom);			
		});
	}).change();
    } 	
); 
function remove_switch(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelSwitch",
				QUERY : id
			}, function(response) {
				var data = eval("("+response+")");
				if (data.error == '0') {
					$('#id'+id).hide('slow', function() {$(this).remove();});
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
			}});
		}}
	});
}
</script>

<p id="tabname">СПРАВОЧНИК ОБОРУДОВАНИЯ<p>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список оборудования</span></li>
		<li><span>Новое оборудование</span></li>
	</ul>
	<div class="box visible" >
		<!--список оборудования-->
	<table id="switchtable" class="tablesorter">
		<thead>
			<tr><th>Имя</th><th>Тип</th><th>Ip</th><th>Кол-во портов</th><th>Логин</th><th>UP-link</th><th>Нас. пункт, улица</th><th>Дом</th><th>Комментарий</th><th>Последний snmp-запрос</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php
			$SwitchArray=$tinsp->APIGetSwitch();
			if ($SwitchArray[0]=="0" && is_array($SwitchArray[1]))
			foreach($SwitchArray[1] as $Switch){
				$switch_type=$tinsp->APIGetSwitchType($Switch["switch_type_id"]);
				echo "<tr id='id".$Switch["switch_id"]."'>";
				echo "<td id=switch_name>".$Switch["switch_name"]."</td>";
				echo "<td id=switch_type_id>".$switch_type[1]["switch_type"]."</td>";
				echo "<td id=switch_ip>".$Switch["switch_ip"]."</td>";
				echo "<td id=switch_port>".$switch_type[1]["switch_ports"]."</td>";
				echo "<td id=switch_log>".$Switch["switch_log"]."</td>";
				echo "<td id=switch_pas>".$Switch["switch_uplink"]."</td>";
				echo "<td id=switch_ulica>".$Switch["nasp_name"].", ул. ".$Switch["ulica_name"]."</td>";
				echo "<td id=switch_dom>".$Switch["dom_name"].$Switch["dom_letter"]."</td>";
				echo "<td id=switch_comment>".$Switch["switch_comment"]."</td>";
				echo "<td id=switch_snmp_lasttime>".$Switch["switch_snmp_lasttime"]."</td>";
				echo "<td><a onclick='window.open(\"opener.php?opener=edit_switch&id=".$Switch["switch_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=500\");' href='#'><img src='img/Setting-icon.png' width='20'></a>
<a href='javascript:;'  onclick='remove_switch(\"".$Switch["switch_id"]."\");'><img src='img/delete.png' width='16'></a></td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации оборудования-->
	<table class=forms>
			<tr><td>Название</td><td><input type=text name="switch_name" id="switch_name" size=30></td></tr>
			<tr><td>Тип оборудования</td><td>
			<select name="switch_type_id" id="switch_type_id">
			<?php
				$SwitchTypeArray=$tinsp->APIGetSwitchType();
				if (is_array($SwitchTypeArray))
				foreach($SwitchTypeArray[1] as $SwitchTypeKey => $SwitchTypeVal){
					echo "<option value='".$SwitchTypeKey."'>".$SwitchTypeVal["switch_type"]."</option>";
				}
			?>
			</select></td></tr>
			<tr><td>IP</td><td><input type=text name="switch_ip" id="switch_ip" size=20></td></tr>
			<tr><td>UPlink порты (через запятую)</td><td><input type=text name="switch_uplink" id="switch_uplink" size=20 value="25,26"></td></tr>
			<tr><td>Примечание</td><td><input type=text name="switch_comment" id="switch_comment" size=30></td></tr>		
			<tr><th colspan=2>Доступ</th></tr>
			<tr><td>Логин</td><td><input type=text name="switch_log" id="switch_log" size=10></td></tr>
			<tr><td>Пароль</td><td><input type=text name="switch_pas" id="switch_pas" size=10></td></tr>
			<tr><th colspan=2>Местоположение</th></tr>
			<tr><td>Улица</td>
			<td><select name="switch_ulica" id="switch_ulica">
			<?php
				if (is_array($tinsp->APIGetUlica(2,"")))
				foreach($tinsp->APIGetUlica(2,"") as $Ulica){
					echo "<option value='".$Ulica["ulica_id"]."'>".$Ulica["nasp_name"]." ул. ".$Ulica["ulica_name"]."</option>";
				}
			?>
			</select></td></tr>
			
			<tr><td>Дом</td>
			<td>
			<select name="switch_dom" id="switch_dom"></select>
			</td>
			</tr>
			<tr><td>Место</td><td><input type="text" name="switch_place" id="switch_place" size="20"></td></tr>
			<tr><th colspan=2>SNMP</th></tr>
			<tr>
				<td>snmp</td><td>
				<select name="switch_snmp" id="switch_snmp">
					<option value='0'>Нет</option>
					<option value='1'>Да</option>
				</select></td>
			</tr>
			<tr>
				<td>сбор mac-адресов</td><td>
				<select name="switch_snmpcollectmac" id="switch_snmpcollectmac">
					<option value='0'>Нет</option>
					<option value='1'>Да</option>
				</select></td>
			</tr>
			<tr>
				<td>сбор состояния портов</td><td>
				<select name="switch_snmpcollectport" id="switch_snmpcollectport">
					<option value='0'>Нет</option>
					<option value='1'>Да</option>
				</select></td>
			</tr>
			<tr>
			<td>сбор трафика по портам</td><td>
				<select name="switch_snmpcollecttraff" id="switch_snmpcollecttraff">
					<option value='0'>Нет</option>
					<option value='1'>Да</option>
				</select></td>
			</tr>
			<tr>
			<td>сбор ошибок по портам</td><td>
				<select name="switch_snmpcollecterror" id="switch_snmpcollecterror">
					<option value='0'>Нет</option>
					<option value='1'>Да</option>
				</select></td>
			</tr>
			<tr>
				<td>snmp comunity</td><td><input type="text" name="switch_snmpcomunity" id="switch_snmpcomunity"></td>
			</tr>
			<tr><th colspan=2>Монитонинг</th></tr>
			<tr>
				<td>Мониторинг оборудования</td>
				<td><select name="switch_monit" id="switch_monit">
					<option value='0'>Нет</option>
					<option value='1'>PING</option>
					<option value='2'>SNMP</option>
				</select></td>
			</tr>
			<tr><td colspan=2><input type="submit" name="sendBtn" id ="create" value="создать" /></td></tr>	

	</table>

<script>
	$("#create").click(function(event) {
		var json_data='';
		$("#result").show();
		$('table.forms').each(function() {
			var values = {};
			$('input[type=checkbox],input[type=text],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.name);
			});
			//json_data=JSON.stringify(values);
			$.post("query/json_query.php", {
				FUNCTION : "APICreatSwitch",
				'QUERY' : values
			}, function(response) {
				//json parse
				var data = eval("(" + response + ")");
				if (data.error == '0') { $.prompt("Успешно создан");} else {
				$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");}
				
			});
		});

	}); 
</script>	
	</div>	
</div>


 
