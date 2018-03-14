<?php
/*	
Редактор оборудования	
*/
if (isset($_GET["id"])){
	$Switch_id=$_GET["id"];
	if(strlen($Switch_id)>2){
		$Switch_id_options=$tinsp->APIGetSwitch($Switch_id);
		if($Switch_id_options[0]!="0"){
			exit("Оборудование с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
?>
<script>
$(document).ready(function() 
    { 
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
	})
    } 	
); 
</script>
<!--форма редактирования тарифы-->
<table class=forms>
	<input type="hidden" name="obj" id="obj" value="<?=$Switch_id_options[1][0]["switch_id"]?>">
	<tr><td colspan=2 align=center>Редактирование оборудования «<?=$Switch_id_options[1][0]["switch_name"]?>»</td></tr>
	<tr><td>Название</td><td><input type=text name="switch_name" id="switch_name" size=30 value="<?=$Switch_id_options[1][0]["switch_name"]?>"></td></tr>
	<tr><td>Тип оборудования</td><td>
	<select name="switch_type_id" id="switch_type_id">
	<?php
		$SwitchTypeArray=$tinsp->APIGetSwitchType();
		if (is_array($SwitchTypeArray))
		foreach($SwitchTypeArray[1] as $SwitchTypeKey => $SwitchTypeVal){
			echo "<option value='".$SwitchTypeKey."' ".($Switch_id_options[1][0]["switch_type_id"]==$SwitchTypeKey? 'selected': '')." >".$SwitchTypeVal["switch_type"]."</option>";
		}

	?>
	</select></td></tr>
	<tr><td>IP</td><td><input type=text name="switch_ip" id="switch_ip" size=20 value="<?=$Switch_id_options[1][0]["switch_ip"]?>"></td></tr>
	<tr><td>UPlink порты (через запятую)</td><td><input type=text name="switch_uplink" id="switch_uplink" size=20 value="<?=$Switch_id_options[1][0]["switch_uplink"]?>"></td></tr>
	<tr><td>Примечание</td><td><input type=text name="switch_comment" id="switch_comment" size=30 value="<?=$Switch_id_options[1][0]["switch_comment"]?>"></td></tr>		
	<tr><th colspan=2>Доступ</th></tr>
	<tr><td>Логин</td><td><input type=text name="switch_log" id="switch_log" size=10 value="<?=$Switch_id_options[1][0]["switch_log"]?>"></td></tr>
	<tr><td>Пароль</td><td><input type=text name="switch_pas" id="switch_pas" size=10 value="<?=$Switch_id_options[1][0]["switch_pas"]?>"></td></tr>
	<tr><th colspan=2>Местоположение</th></tr>
	<tr><td>Улица</td>
	<td><select name="switch_ulica" id="switch_ulica">
	<?php
		if (is_array($tinsp->APIGetUlica(2,"")))
		foreach($tinsp->APIGetUlica(2,"") as $Ulica){
			echo "<option value='".$Ulica["ulica_id"]."' ".($Switch_id_options[1][0]["switch_ulica"]==$Ulica["ulica_id"]? 'selected': '').">".$Ulica["nasp_name"]." ул. ".$Ulica["ulica_name"]."</option>";
		}
	?>
	</select></td></tr>
	
	<tr><td>Дом</td>
	<td>
	<select name="switch_dom" id="switch_dom">
	<?php
		$GetDomArr=$tinsp->APIGetDom("2", "", $Switch_id_options[1][0]["switch_ulica"]);
		if (is_array($GetDomArr))
		foreach($GetDomArr as $GetDomArrVal){
			echo "<option value='".$GetDomArrVal["dom_id"]."' ".($Switch_id_options[1][0]["switch_dom"]==$GetDomArrVal["dom_id"]? 'selected': '')." >".$GetDomArrVal["dom_name"]."</option>";
		}
	?>
	</select>
	</td>
	</tr>
	<tr><td>Место</td><td><input type=text name="switch_place" id="switch_place" size=20 value="<?=$Switch_id_options[1][0]["switch_place"]?>"></td></tr>
	<tr><th colspan="2">SNMP</th></tr>
	<tr>
		<td>snmp</td>
		<td><select name="switch_snmp" id="switch_snmp">
			<option value='0' <?=($Switch_id_options[1][0]["switch_snmp"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_snmp"]=="1" ? 'selected': '')?>>Да</option>
		</select></td>
	</tr>
	<tr>
		<td>сбор mac-адресов</td>
		<td><select name="switch_snmpcollectmac" id="switch_snmpcollectmac">
			<option value='0' <?=($Switch_id_options[1][0]["switch_snmpcollectmac"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_snmpcollectmac"]=="1" ? 'selected': '')?>>Да</option>
		</select></td>
	</tr>
	<tr>
		<td>сбор состояния портов</td>
		<td><select name="switch_snmpcollectport" id="switch_snmpcollectport">
			<option value='0' <?=($Switch_id_options[1][0]["switch_snmpcollectport"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_snmpcollectport"]=="1" ? 'selected': '')?>>Да</option>
		</select></td>
	</tr>
	<tr>
		<td>сбор трафика по портам</td>
		<td><select name="switch_snmpcollecttraff" id="switch_snmpcollecttraff">
			<option value='0' <?=($Switch_id_options[1][0]["switch_snmpcollecttraff"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_snmpcollecttraff"]=="1" ? 'selected': '')?>>Да</option>
		</select></td>
	</tr>
	<tr>
		<td>сбор ошибок по портам</td>
		<td><select name="switch_snmpcollecterror" id="switch_snmpcollecterror">
			<option value='0' <?=($Switch_id_options[1][0]["switch_snmpcollecterror"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_snmpcollecterror"]=="1" ? 'selected': '')?>>Да</option>
		</select></td>
	</tr>
	<tr>
		<td>snmp comunity</td><td><input type=text name="switch_snmpcomunity" id="switch_snmpcomunity" value="<?=$Switch_id_options[1][0]["switch_snmpcomunity"]?>"></td>
	</tr>	
	<tr><th colspan="2">Монитонинг</th></tr>
	<tr>
		<td>Мониторинг оборудования</td>
		<td><select name="switch_monit" id="switch_monit">
			<option value='0' <?=($Switch_id_options[1][0]["switch_monit"]=="0" ? 'selected': '')?>>Нет</option>
			<option value='1' <?=($Switch_id_options[1][0]["switch_monit"]=="1" ? 'selected': '')?>>PING</option>
			<option value='2' <?=($Switch_id_options[1][0]["switch_monit"]=="2" ? 'selected': '')?>>SNMP</option>
		</select></td>
	</tr>
	<tr><td colspan="2"><input type="submit" name="sendBtn" id ="save" value="Сохранить" /><input type="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>	
</table>
<script>
	//checkbox
	$("#result").hide();

	//save
	$("#save").click(function(event) {
		var json_data='';
		$("#result").show();
		$('table.forms').each(function() {
			var values = {};
			$('input[type=checkbox],input[type=text],input[type=hidden],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.value);
			});
			$.post("query/json_query.php", {
				FUNCTION : "APISetSwitch",
				'QUERY' : values
			}, function(response) {
				//json parse
				var data = eval("("+response+")");
				if (data.error == '0'){
					//обновить данные в таблице			
					var switch_id = $("#obj").val();
					window.opener.$('#id'+switch_id).find('#switch_name').text($('#switch_name').val());
					window.opener.$('#id'+switch_id).find('#switch_type_id').text($('#switch_type_id option:selected').html());
					window.opener.$('#id'+switch_id).find('#switch_dom').text($('#switch_dom option:selected').html());
					window.opener.$('#id'+switch_id).find('#switch_ulica').text($('#switch_ulica option:selected').html());
					window.opener.$('#id'+switch_id).find('#switch_log').text($('#switch_log').val());
					window.opener.$('#id'+switch_id).find('#switch_pas').text($('#switch_pas').val());
					window.opener.$('#id'+switch_id).find('#switch_ip').text($('#switch_ip').val());
					window.close();
				}else{
					alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
				}
				
			});
		});

	});  
</script>