<?php
/*	
–едактор устройств:  арточка абонента	
*/
#change pass settings
$change_pass="";
if (count($_POST)>2) {
	$Query=$tinsp->Query("UPDATE billing_users SET switch_id = '".$_POST["switch_id"]."', switch_port = '".$_POST["switch_port"]."' WHERE user_id = '".$_POST["obj"]."'");
	if($Query!="0"){
		$change_pass="<tr><td colspan=2><div id=result>Ошибка сохранени¤. ќбратитесь к администратору.</div></td></tr>";
	}else{
		$change_pass="<tr><td colspan=2><div id=result>Успешно сохранен.</div></td></tr>";
	}
}
#valid guid
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
		if(!is_array($Abonent_id_options)) exit("јбонент с таким id не найден");
	}else exit("ќшибка переданных параметров");
}else exit ("Fail exist parametr");

?>
<!--форма выбора устройства и порта -->
	<?php include("edit_abonent_navigation.inc"); ?>
	<table class="forms" style="width: 100%;">

		<form action="<?=$_SERVER["REQUEST_URI"]?>" method="post" id="FormSwitchSave"></form>	
		<input type=hidden name=obj size=50 id=obj value="<?=$Abonent_id_options[0]["user_id"]?>" form="FormSwitchSave">
		
		<tr><th colspan="2" class="zagolovok">Оборудование</th></tr>
		<tr><th>Устройство</th><td>
			<select name="switch_id" id="switch_id" size=3 form="FormSwitchSave">
				<option value="" disabled>Выберите устройство</option>
				<?php
					$GetSwitch=$tinsp->APIGetSwitch();
					if ($GetSwitch[0]=="0"){
						foreach ($GetSwitch[1] as $Switch){
							$switch_type=$tinsp->APIGetSwitchType($Switch["switch_type_id"]);
							echo "<option value='".$Switch["switch_id"]."' ".($Switch["switch_id"]==$Abonent_id_options[0]["switch_id"]? 'selected':'')." >".$Switch["nasp_name"].", ул. ".$Switch["ulica_name"]." ".$Switch["dom_name"].$Switch["dom_letter"]." ".$Switch["switch_place"]." [".$Switch["switch_name"]." ]";
						}
					}
				?>
			</select>
		</td></tr>
		<tr><th>Порт</th><td><input type="number" name="switch_port" id="switch_port" value="<?=$Abonent_id_options[0]["switch_port"]?>" min="0" max="99" form="FormSwitchSave"></td></tr>
		<!-- SNMP -->
		<?php
		/* 
		$GetSwitchAbon=$tinsp->APIGetSwitch($Abonent_id_options[0]["switch_id"]);
		if ($GetSwitchAbon[0]==0 && $GetSwitchAbon[1][0]["switch_snmp"]==1){
			$GetSwitchTypeAbon=$tinsp->APIGetSwitchType($GetSwitchAbon[1][0]["switch_type_id"]);
			$tinsp->include_network_monitor();
			#SetSnmpParam
			$tinsp->NET_MONIT->SetSnmp($GetSwitchAbon[1][0]["switch_ip"], $GetSwitchAbon[1][0]["switch_snmpcomunity"]);
			
			#get fdb
			$tinsp->NET_MONIT->snmpwalk($GetSwitchTypeAbon[1]["switch_mibfdb"]);

			if ($tinsp->NET_MONIT->snmp_reply!=FALSE){
				$result_mac=$tinsp->NET_MONIT->get_snmpwalk_fdb($GetSwitchAbon[1][0]["switch_type_id"], $GetSwitchAbon[1][0]["switch_uplink"], "\d+", $Abonent_id_options[0]["user_mac"]);
				//print_r($result_mac);
				if (count($result_mac)==1){
					$status_mac = $Abonent_id_options[0]["user_mac"].": <font color=green>ONLINE</font> ".($result_mac[0][3]!=$Abonent_id_options[0]["switch_port"] ? '<br>(Данные о порте не соответствуют. Абонент в '.$result_mac[0][3].' порту)':'')."";
				}else{
					$status_mac = $Abonent_id_options[0]["user_mac"].": <font color=red>OFFLINE</font>";
				}
			}else{
				$status_mac = "<font color=red>Устройство не отвечает.</font>";
			} 
			
			#get status port
			$tinsp->NET_MONIT->snmpwalk($GetSwitchTypeAbon[1]["switch_mibport"]);
			if ($tinsp->NET_MONIT->snmp_reply!=FALSE && $Abonent_id_options[0]["switch_port"]!=""){
				$result_port=$tinsp->NET_MONIT->get_snmpwalk_ports($GetSwitchAbon[1][0]["switch_type_id"], $Abonent_id_options[0]["switch_port"]);
				//print_r($result_port);
				if (count($result_port)==1){
					$status_port = "".($result_port[$Abonent_id_options[0]["switch_port"]]=="down") ? '<font color=red>Выключен/font> ': '<font color=green>Включен</font>'." ";
				}else{
					$status_port = "<font color=red>Нет информации о порте</font>";
				}
			}elseif(!$tinsp->NET_MONIT->snmp_reply && $Abonent_id_options[0]["switch_port"]!=""){
				$status_port = "<font color=red>Устройство не отвечает</font>";
			}
		}
		
		*/ 
		?>		
		<tr><td colspan="2">
			<input type="submit" class="button" value="Открыть" onclick="window.open(&quot;opener.php?opener=netmon_switch&amp;id=<?=$Abonent_id_options[0]["switch_id"];?>#<?=$Abonent_id;?>&quot;,&quot;contents&quot;,&quot;Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=700&quot;);" href="javascript:void(0);">
			<input type="submit" name="sendBtn" class="button" value="Сохранить" form="FormSwitchSave" />
			<input type="button" class="button" value="Закрыть" onclick="javascript:window.close();"/>
		</td></tr>			

	<?=$change_pass?>		
</table>


