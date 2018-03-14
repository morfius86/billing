<?php
// Network Monitor

?>
<script type="text/javascript" src="js/cal.js"></script>


<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css" />
<style type="text/css">
</style>
<p id="tabname">Монитор сети</p>
<table>
	<tbody>
	<tr>
		<td style="height:50px; vertical-align:top;">
			<!--Отчеты-->
			<form action="?page=network_monit&show=true" autocomplete="off" method=post>
			<table class="forms">
			<tr><th>Место</th></tr>
			<tr><td>
				<select name="select_nasp" id="select_nasp">
					<option value="null">Все населенные пункты</option>
					<?php
					if (is_array($tinsp->APIGetNasp(2,"")))
					foreach($tinsp->APIGetNasp(2,"") as $Nasp){
						echo "<option value='".$Nasp["nasp_id"]."'>".$Nasp["nasp_name"]."</option>";
					}
					?>
				</select>
			</td></tr>
			<tr><td><select name="select_ulica" id="select_ulica"></select></td></tr>
			<tr><td><select name="select_dom" id="select_dom"></select></td></tr>
			<tr><th>Доступность устройств</th></tr>
			<tr><td>
				<select name="pay_buh" id="pay_buh">
					<option value="0">Все</option>
					<option value="1">Доступен</option>
					<option value="2">Недоступен</option>
				</select>
			</td></tr>
			<tr><th>Информация</th></tr>
			<tr><td>Всего устройств: <span id="device_all"></span></td></tr>
			<tr><td>Доступно устройств: <span style="color:green; font-weight:bold;" id="device_online"></span></td></tr>
			<tr><td>Недоступно устройств: <span style="color:red; font-weight:bold;" id="device_offline"></span></td></tr>
			<tr><td align="center"><input type="submit" value="Показать" ></td></tr>
			</table></form>
		</td>
		<td style="height:50px; vertical-align:top;">
			<table id="switch_table" class="tablesorter">
			<thead class="fixedHeader">
				<tr><th>#</th><th>Тип устройства</th><th>Нас. пункт</th><th>Улица</th><th>дом</th><th>место</th><th>Статус</th><th>Статус изменен</th><th>Длительность статуса</th><th>Комментарий</th></tr>
			</thead>
			<tbody>
				<?php
				if ($_GET["show"]=="true"){
					$switch_id = (!empty($_GET["switch_id"])) ? $_GET["switch_id"] : NULL;
					$SwitchArr=$tinsp->APIGetSwitch($switch_id);
					
					if($SwitchArr[0]==0){
						$i=1;
						$StatArr=array(0=>0, 1=>0, 2=>0);
						foreach($SwitchArr[1] as $SwitchKey => $Switch){
							$StatArr[0]++;
							($Switch["switch_monit_status"]==1) ? $StatArr[1]++ : $StatArr[2]++; 
							$SwitchType=$tinsp->APIGetSwitchType($Switch["switch_type_id"]);	// мибы	
							$deff_time=$tinsp->seconds2times((strtotime(date("Y-m-d H:i:s")) - strtotime($Switch["switch_monit_statustime"])));
							$nasp_name=$tinsp->APIGetNasp(2,$Switch["ulica_nasp"]);
							
							echo "<tr id='id".$Switch["switch_id"]."'>
							
							<td>".$i++."</td>
							<td><a onclick='window.open(\"opener.php?opener=netmon_switch&id=".$Switch["switch_id"] . "\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=700\");' href='javascript:void(0);'>".$SwitchType[1]["switch_type"]."</a></td>
							<td>".$nasp_name[0]["nasp_name"]."</td>
							<td>".$Switch["ulica_name"]."</td>
							<td>".$Switch["dom_name"].$Switch["dom_letter"]."</td>
							<td>".$Switch["switch_place"]."</td>
							<td>".($Switch["switch_monit_status"]==1 ? "<font color=green>ONLINE</font>" : "<font color=red>OFFLINE</font>")."</td>
							<td>".$Switch["switch_monit_statustime"]."</td>
							<td>".$deff_time[3]." дн. ".$deff_time[2]." час. ".$deff_time[1]." мин.</td>
							<td>".$Switch["switch_comment"]."</td>";
							

						}
					}else{
						echo '<tr><td colspan=10 align=center>Нет данных</td></tr>';
					}
				}else{
					echo '<tr><td colspan=10 align=center>Нажмите "Показать", чтобы сформировать отчет</td></tr>';
				}
			?>
			</tbody></table>
		</td>
	</tr>
	</tbody>
</table>

<script type="text/javascript">
$("#device_all").html("<?=$StatArr[0]?>");
$("#device_online").html("<?=$StatArr[1]?>");
$("#device_offline").html("<?=$StatArr[2]?>");
	//select nasp
$("#select_nasp").change(function(){
	//данные нас пункта		
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = "";
	arr['2'] = {};
	arr['2']['value'] = $(this).val();
			
	$('#select_ulica').empty();
	$('#select_ulica').append('<option value="null">Все улицы</option>');
	//список нас. пунктов
	$.post("query/json_query.php", {FUNCTION: "APIGetUlica", QUERY:arr}, function(response){
	var data_ulica = eval("("+response+")");
		$.each(data_ulica.error, function(key, ulica){		
			$('#select_ulica').append('<option value="'+ulica.ulica_id+'">'+ulica.ulica_name+'</option>');			
		});
		//передаем данные событию select dom
		$("#select_ulica").trigger("change");
	});
}).change();
	//select dom
$("#select_ulica").change(function(){	
	var ulica_id = $("#select_ulica").val();
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = "";
	arr['2'] = {};
	arr['2']['value'] = ulica_id;
			
	$('#select_dom').empty();
	$('#select_dom').append('<option value="null">Все дома</option>');
	//список нас. пунктов
	$.post("query/json_query.php", {FUNCTION: "APIGetDom", QUERY:arr}, function(response){
	var data_dom = eval("("+response+")");
		$.each(data_dom.error, function(key, dom){		
			$('#select_dom').append('<option value="'+dom.dom_id+'">'+dom.dom_name+''+dom.dom_letter+'</option>');			
		});
	});
}); 
</script>
