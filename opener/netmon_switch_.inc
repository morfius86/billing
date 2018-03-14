<?php
/*	
Netmon: коммутатор	
*/
if (isset($_GET["id"]) && strlen($_GET["id"])>2){
	$Switch_id_options=$tinsp->APIGetSwitch($_GET["id"]);

	if($Switch_id_options[0]!="0"){
		exit("Не найдено оборудование");
	}else{
		$tinsp->include_network_monitor();
		$SwitchType=$tinsp->APIGetSwitchType($Switch_id_options[1][0]["switch_type_id"]);
	}
}else{
	exit ("Ошибка переданного id");
}

#SetSNMP
$tinsp->NET_MONIT->SetSnmp($Switch_id_options[1][0]["switch_ip"], $Switch_id_options[1][0]["switch_snmpcomunity"], $SwitchType[1]["walk"]);

#UPTIME
$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["mib_uptime"]);
$uptime=$tinsp->seconds2times($tinsp->NET_MONIT->get_snmpwalk_uptime($Switch_id_options[1][0]["switch_type_id"])/100);


?>
<script>
</script>

<table class="forms" style="width: 100%;">
	<!--Информация по оборудованию-->
	<tr><th colspan=2 class="zagolovok">Информация по оборудованию: <?=$Switch_id_options[1][0]["switch_name"]?></th></tr>
	<tr>
		<th>Место установки</th>
		<td><?=$Switch_id_options[1][0]["nasp_name"]?>, ул. <?=$Switch_id_options[1][0]["ulica_name"]?>, дом. <?=$Switch_id_options[1][0]["dom_name"].$Switch_id_options[1][0]["dom_letter"]?>. Место: <?=$Switch_id_options[1][0]["switch_place"]?></td>
	</tr>
	<tr>
		<th>Тип оборудования</th>
		<td><?=$SwitchType[1]["switch_type"]?>, портов: <?=$SwitchType[1]["switch_ports"]?></td>
	</tr>
	<tr>
		<th>Состояние</th>
		<td><?=($Switch_id_options[1][0]["switch_monit_status"]==1 ? "<font color=green>ONLINE</font>" : "<font color=red>OFFLINE</font>")?></td>
	</tr>
	<tr>
		<th>IP-адрес</th>
		<td><?=$Switch_id_options[1][0]["switch_ip"]?></td>
	</tr>
	<tr>
		<th>Системный uptime:</th>
		<td><?php echo $uptime[3].' дн., '.$uptime[2].' ч:'.$uptime[1].' мин:'.(int)$uptime[0].' сек'; ?></td>
	</tr>
	<!--Состояние портов и абонентов-->
	<tr><th colspan=2 class="zagolovok">Состояние</th></tr>
	<?php 
		if (is_array($AbonentList=$tinsp->APIGetBillProp(3, ""))){
			$MacListAbon=array();
			$SwitchListAbon=array();
			foreach ($AbonentList as $a_key => $a_val){
				$SwitchListAbon[$a_val["switch_id"]][$a_val["switch_port"]][] = array("user_id"=>$a_val["user_id"], "name"=>$a_val["user_name"], "mac"=>$a_val["user_mac"]);
				$MacListAbon[$a_val["user_mac"]]=array("user_name"=>$a_val["user_name"], "user_id"=>$a_val["user_id"]);
			}
			//print_r($MacListAbon);
		}
		# IfDescr PORTS
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifdescr"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$IfDescr_Table=$tinsp->NET_MONIT->get_snmpwalk_ifdescr($Switch_id_options[1][0]["switch_type_id"]);
		}
	
		#FDB MAC ADDRESS
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_mibfdb"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$FDB_Table=array();
			$result_mac=$tinsp->NET_MONIT->get_snmpwalk_fdb($Switch_id_options[1][0]["switch_type_id"], $Switch_id_options[1][0]["switch_uplink"], "\d+");
			foreach($result_mac as $m_key => $m_val){
				$FDB_Table[$m_val[2]][$tinsp->GetMac(strtoupper($tinsp->NET_MONIT->mac16_mac10($m_val[1])))] = $m_val[1];
			}
		}
		
		#SPEED PORTS
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifspeed"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$Speed_Table=$tinsp->NET_MONIT->get_snmpwalk_ifspeed($Switch_id_options[1][0]["switch_type_id"], "\d+");
		}
		
		#IN ERROR
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifinerr"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$Inerror_Table=$tinsp->NET_MONIT->get_snmpwalk_iferror($Switch_id_options[1][0]["switch_type_id"], "\d+");
		}
		
		#OUT ERROR
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_ifouterr"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$Outerror_Table=$tinsp->NET_MONIT->get_snmpwalk_iferror($Switch_id_options[1][0]["switch_type_id"], "\d+");
		}
		
		#STATUS PORTS
		$tinsp->NET_MONIT->snmpwalk($SwitchType[1]["switch_mibport"]);
		if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
			$result_port=$tinsp->NET_MONIT->get_snmpwalk_ports($Switch_id_options[1][0]["switch_type_id"]);
			print_r($result_port);
			ksort($result_port);
			foreach ($result_port AS $p_key => $p_val) {
				if ($p_key<=$SwitchType[1]["switch_ports"]){
					echo '<tr><th style="line-height: 1em; text-align: left;">

'.(($p_val == "up") ? '<img src="\img\switch\0.png">':'<img src="\img\switch\1.png">').' '.$IfDescr_Table[$p_key].' - <small>'.($Speed_Table[$p_key]/1000000).' МБИТ</small><br>
<small>ERROR IN/OUT: '.($Inerror_Table[$p_key]).'/'.($Outerror_Table[$p_key]).'</small>


						</th><td>';
					$GetAbonPort=$SwitchListAbon[$Switch_id_options[1][0]["switch_id"]][$p_key];
					echo '<table class="forms">';
					if (is_array($GetAbonPort)){
						foreach($GetAbonPort as $GetAbonVal){
							#смотрим мак абонента на коммутаторе 
							echo'<tr>
								<td>'.((isset($FDB_Table[$p_key][$GetAbonVal["mac"]]))? '<font color="green">ONLINE</font>':'<font color="red">OFFLINE</font>').'</td>
								<td>'.$GetAbonVal["mac"].'</td>';
							?>
								<td><a onclick="window.open('opener.php?opener=edit_abonent&id=<?=$GetAbonVal["user_id"]?>','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598');" href="#"><?=$GetAbonVal["name"]?></a></td>
							<?php	
							echo'</tr>';
						}						
					}
					#ищем неизвестные маки
					if (count($FDB_Table[$p_key])>0){
						#print_r($FDB_Table[$p_key]);
	
						foreach ($FDB_Table[$p_key] as $fdb_mac => $fdb_port){
							if (!isset($MacListAbon[$fdb_mac])) {
								echo'<tr><td></td><td>'.$fdb_mac.'</td><td>неизвестный mac-адрес</td></tr>';	
							}
						}
					}
					echo '</table>';
					echo '</td></tr>';
				}else{
					break;
				}
			}
		}elseif(!$tinsp->NET_MONIT->snmp_reply){
			$status_port = "<font color=red>НЕВОЗМОЖНО ПОЛУЧИТЬ ДАННЫЕ С КОММУТАТОРА</font>";
		}

	?>
</table>
<script>

</script>