<?php
// -----------------------
// Мониторинг коммутатора
// -----------------------

if (isset($_GET["id"]) && strlen($_GET["id"])>2){
	$switch_id = $_GET["id"];
	$SwitchArr=$tinsp->APIGetSwitch($_GET["id"]);

	if($SwitchArr[0]!="0"){
		exit("Не найдено оборудование");
	}else{
		$tinsp->include_network_monitor();
		$Switch = $SwitchArr[1][0];
		$SwitchType=$tinsp->APIGetSwitchType($Switch["switch_type_id"]);
	}
}else{
	exit ("Ошибка переданного id");
}

// задаем параметры коннекта к коммутатору
$tinsp->NET_MONIT->SetSnmp($Switch["switch_ip"], $Switch["switch_snmpcomunity"], $SwitchType[1]["walk"]);

// uptime
if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["mib_uptime"]) != FALSE){
	$uptime = $tinsp->seconds2times($tinsp->NET_MONIT->get_snmpwalk_uptime($Switch["switch_type_id"])/100);
}
?>

<script>
$(function(){
	var hash = window.location.hash;
	hash &&  $(hash).css({'border':  '2px solid #ff8787'})
})

</script>

<table class="tablesorter" style="width: 100%;">
<thead>
	<tr>
		<th colspan="2">Инфомрация по оборудованию</th>
	</tr>
</thead>
	<tr>
		<td>Место установки</td>
		<td><?=$Switch["nasp_name"]?>, ул. <?=$Switch["ulica_name"]?>, дом. <?=$Switch["dom_name"].$Switch["dom_letter"]?>. Место: <?=$Switch["switch_place"]?></td>
	</tr>
	<tr>
		<td>Тип оборудования</td>
		<td><?=$SwitchType[1]["switch_type"]?>, портов: <?=$SwitchType[1]["switch_ports"]?></td>
	</tr>
	<tr>
		<td>Состояние</td>
		<td><?=($Switch["switch_monit_status"]==1 ? "<font color=green>ONLINE</font>" : "<font color=red>OFFLINE</font>")?></td>
	</tr>
	<tr>
		<td>IP-адрес</td>
		<td><?=$Switch["switch_ip"]?></td>
	</tr>
	<tr>
		<td>Системный uptime:</td>
		<td><?php echo $uptime[3].' дн., '.$uptime[2].' ч:'.$uptime[1].' мин:'.(int)$uptime[0].' сек'; ?></td>
	</tr>

</table>
<table id="switch_table" class="tablesorter" style="width: 100%;">
	<thead>
		<tr><th>Порт</th><th colspan="2">Данные порта</th><th>MAC-адрес</th><th>Последний online</th><th>Принадлежность</th></tr>
	</thead>
<?php

	// список мак-адресов абонентов
	if (is_array($Users=$tinsp->APIGetBillProp(3, ""))){
		$MacUsers=array();
		foreach ($Users as $User){
			$MacUsers[$User["user_mac"]]=array("user_name"=>$User["user_name"], "user_id"=>$User["user_id"]);
		}
	}
	// -----------------------------------------------------	
	// 	Получаем данные с устройства по SNMP
	// -----------------------------------------------------
	
	// статус портов
	$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_mibport"]);	
	
	if ($tinsp->NET_MONIT->snmp_reply!=FALSE){

		// обрабатываем статусы и список портов
		$SNMP_PORTS_STATUS=$tinsp->NET_MONIT->get_snmpwalk_ports($Switch["switch_type_id"]);	
		ksort($SNMP_PORTS_STATUS); // сортируем порты по порядку
		// $SNMP_PORTS_STATUS = array_slice($SNMP_PORTS_STATUS, 0, $SwitchType[1]["switch_ports"], true);	// срезаем массив с нужным кол-ом интерфейсов
		// описания портов	
		if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifdescr"])!=FALSE){
			$SNMP_PORTS_DESCR = $tinsp->NET_MONIT->get_snmpwalk_ifdescr($Switch["switch_type_id"]);
		}
		// скорость портов	
		if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifspeed"])!=FALSE){
			$SNMP_PORTS_SPEED = $tinsp->NET_MONIT->get_snmpwalk_ifspeed($Switch["switch_type_id"]);
		}
		// ошибки портов in	
		if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifinerr"])!=FALSE){
			$SNMP_PORTS_INERR = $tinsp->NET_MONIT->get_snmpwalk_iferror($Switch["switch_type_id"]);
		}
		// ошибки портов out
		if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifouterr"])!=FALSE){
			$SNMP_PORTS_OUTERR = $tinsp->NET_MONIT->get_snmpwalk_iferror($Switch["switch_type_id"]);
		}
		// если устройство OLT, делаем доп запросы по snmp
		if ($SwitchType[1]["type"] == "OLT") {
			// mac onu-интерфейсов
			if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["onu_mac"])!=FALSE){
				$SNMP_ONU_MAC = $tinsp->NET_MONIT->get_mac_onu($Switch["switch_type_id"]);
			}
			// onu-rx-power интерфейсов
			if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["onu_rx_power"])!=FALSE){
				$SNMP_ONU_RX_POWER = $tinsp->NET_MONIT->get_onu_rx_power($Switch["switch_type_id"]);
			}
			// onu-vendor интерфейсов
			if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["onu_vendor"])!=FALSE){
				$SNMP_ONU_VENDOR = $tinsp->NET_MONIT->get_onu_vendor($Switch["switch_type_id"]);			
			}
		}
		
		// -----------------------------------------------------	
		// 	MAC-адреса (с коммутатора / из базы)
		// -----------------------------------------------------
		
		// собираем маки только с коммутатора (bdcom лагает при сборе)
		if ($SwitchType[1]["type"] == "SWITCH") {
			if ($tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_mibfdb"])){
				$get_fdb=$tinsp->NET_MONIT->get_snmpwalk_fdb($Switch["switch_type_id"], $Switch["switch_uplink"]);
				foreach ($get_fdb as $get_fdb_val){
					$mac_format = $tinsp->GetMac($tinsp->NET_MONIT->mac16_mac10($get_fdb_val[1]));
					$GET_MAC_FDB[$mac_format] = array(
						"mac" 		=> $mac_format, 
						"port"		=> $get_fdb_val[2], 
						"last_seen"	=> "<b style='color: #43c76a;'>ONLINE</b>");
					
					// найден мак в биллинге, добавим значения абонента в массив
					if (isset($MacUsers[$mac_format])) {
						$GET_MAC_FDB[$mac_format] = array_merge ($GET_MAC_FDB[$mac_format], $MacUsers[$mac_format]);
					} else {
						$GET_MAC_FDB[$mac_format] = array_merge ($GET_MAC_FDB[$mac_format], array("user_name" => "<b style='color: #fb9097'>неизвестный</b>"));
					} 
				}
			}
		}	
		
		// собираем историю маков из базы (снимается биллингом по snmp)
		$APIGetSwitchMac = $tinsp->APIGetSwitchMac($switch_id);
		if ($APIGetSwitchMac[0] == 0 && is_array($APIGetSwitchMac[1])) {
			foreach ($APIGetSwitchMac[1] as $mac){
				$GET_MAC_DB[$mac["mac"]] = array(
					"mac" 		=> $mac["mac"], 
					"port"		=> $mac["port"], 
					"last_seen"	=> $mac["last_seen"]);
					
				// найден мак в биллинге, добавим значения абонента в массив
				if (isset($MacUsers[$mac["mac"]])) {
					$GET_MAC_DB[$mac["mac"]] = array_merge ($GET_MAC_DB[$mac["mac"]], $MacUsers[$mac["mac"]]);
				} else {
					$GET_MAC_DB[$mac["mac"]] = array_merge ($GET_MAC_DB[$mac["mac"]], array("user_name" => "<b style='color: #fb9097'>неизвестный</b>"));
				}
			}
			unset ($mac);
		}

		// итоговый списов мак-адресов на коммутаторе [ объеденяем с заменой массива с FDB-маками и маками из базы (актуализируем данные) ]
		foreach (array_merge((array)$GET_MAC_DB, (array)$GET_MAC_FDB) as $mac => $mac_val){
			$SNMP_SWITCH_MAC[$mac_val["port"]][] = $mac_val;
		}
		
		// -----------------------------------------------------	
		// 	Подготавливаем список портов
		// -----------------------------------------------------
		
		// найдены PON-порты, сформируем массив с учетом подчинения ONU-интерфейсов к PON-портам
		$PORTS_LIST_PON = array (); $PORTS_LIST = array ();
		foreach ($SNMP_PORTS_DESCR as $k => $v) {
			$olt_epon_int = explode(":", $v);
			// найден pon интерфейс
			if (count($olt_epon_int) > 1) {
				// ищем pon-порт этого интерфейса
				$olt_epon_int_key = array_search($olt_epon_int[0], $SNMP_PORTS_DESCR);
				$PORTS_LIST_PON[$olt_epon_int_key][$k] = $v;

			}else{
				$PORTS_LIST[$k] = $v;
			}
		}
		
		// итоговый список портов 
		$SNMP_PORTS_LIST = array_replace($PORTS_LIST, $PORTS_LIST_PON);
		

		// -----------------------------------------------------	
		// 	Проходимся по ETHERNET портам
		// -----------------------------------------------------
		
		foreach ($SNMP_PORTS_LIST as $port => $port_stat) {
			
			$port_status 	= $SNMP_PORTS_STATUS[$port];
			$port_descr		= $SNMP_PORTS_DESCR[$port];
			$port_speed		= ($SNMP_PORTS_SPEED[$port]/1000000);
			$port_inerr		= $SNMP_PORTS_INERR[$port];
			$port_outerr	= $SNMP_PORTS_OUTERR[$port];
			$port_color		= ($port_status == "up") ? '#43c76a' : '#fb9097';
			$port_color		= ($port_status == "up" && $port_speed == 10) ? '#ffc505' : $port_color;
			
			// инфа по обычному порту
			echo '<thead id="port_'.$port.'"><tr>
				<td style="border-left: 11px solid '.$port_color.';">
					<b>'.$port_descr.'</b>
				</td>
				<td style="font-size: 10px;" >
					SPEED: '.$port_speed.' Мбит
				</td>
				<td style="font-size: 10px;" >
					ERROR: '.$port_inerr.'/'.$port_outerr.'
				</td>
				<td colspan="3" style="font-size: 10px;">

				</td>
				</tr></thead>';
			// найдены маки на порту устройства
			if (isset($SNMP_SWITCH_MAC[$port])) {
				foreach ($SNMP_SWITCH_MAC[$port] as $mac) {
					echo '<tr>
						<th colspan=3></th>
						<td>'.$mac["mac"].'</td>
						<td>'.$mac["last_seen"].'</td>
						<td id="'.$mac["user_id"].'">'.((isset($mac["user_id"])) ? "<a id=\"{$mac["user_id"]}\" onclick=\"window.open('opener.php?opener=edit_abonent&id={$mac["user_id"]}','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598');\" href=\"javascript:void(0)\">{$mac["user_name"]}</a>" : $mac["user_name"]).'</td>
					</tr>';
				}
			}
			
			// -------------------------------------------------	
			// 	Проходимся по PON портам
			// ------------------------------------------------- 

			if ($SwitchType[1]["type"] == "OLT" && is_array($port_stat)) {
				foreach ($port_stat as $port_onu_key => $port_onu_descr) {
					
					$onu_port_status 	= $SNMP_PORTS_STATUS[$port_onu_key];
					$onu_mac 			= $SNMP_ONU_MAC[$port_onu_key];
					$onu_rx_power 		= $SNMP_ONU_RX_POWER[$port_onu_key];
					$onu_rx_power 		= ($onu_rx_power == '-214748364.8') ? 'fail' : $onu_rx_power;
					$onu_vendor 		= $SNMP_ONU_VENDOR[$port_onu_key];
					$onu_vendor 		= (empty($onu_vendor) or $onu_vendor == '----') ? $tinsp->NET_MONIT->get_mac_vendor($onu_mac) : $onu_vendor; // определим вендора по маку
					
					echo '<thead id="port_'.$port_onu_key.'"><tr>
						<td style="padding-left: 55px;">
							'.(($onu_port_status == "up") ? '<img src="\img\switch\0.png">':'<img src="\img\switch\1.png">').'
							<b>'.$port_onu_descr.'</b>	
						</td>
						<td style="font-size: 10px;">
							ONU: '.$onu_mac.'<br>
							LEVEL: <meter value="'.str_replace("-","", $onu_rx_power).'" min="0" low="24" high="28" max="33"></meter> '.$onu_rx_power.'
						</td>
						<td style="font-size: 10px;">
							ERROR: '.($SNMP_PORTS_INERR[$port_onu_key]).'/'.($SNMP_PORTS_OUTERR[$port_onu_key]).'<br>
							VENDOR: '.$onu_vendor.'
						</td>
						<td colspan="3" style="font-size: 10px;">
						
						</td
						</tr></thead>';	
					// найдены маки на порту устройства
					if (isset($SNMP_SWITCH_MAC[$port_onu_key])) {
						foreach ($SNMP_SWITCH_MAC[$port_onu_key] as $mac) {
							// убираем мак ону-терминала
							if ($mac["mac"] != $onu_mac){
								echo '<tr>
								<th colspan=3></th>
								<td>'.$mac["mac"].'</td>
								<td>'.$mac["last_seen"].'</td>
								<td id="'.$mac["user_id"].'">'.((isset($mac["user_id"])) ? "<a id=\"{$mac["user_id"]}\" onclick=\"window.open('opener.php?opener=edit_abonent&id={$mac["user_id"]}','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598');\" href=\"javascript:void(0)\">{$mac["user_name"]}</a>" : $mac["user_name"]).'</td>
								</tr>';
							}
						}
					}	
				}
			}		
		}	
	}
?>

</table>
<a id="123"></a>