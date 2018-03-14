<?php
/*
Collect MAC from switch snmp
*/
error_reporting (E_ERROR | E_WARNING | E_PARSE | E_NOTICE);

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->config=$config;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
#connect class API::NETWORK_MONITOR;
$tinsp->include_network_monitor();
#Users's
$UsersArray = $tinsp->APIGetBillProp(3,"");
#Switch's	
$SwitchArray = $tinsp->APIGetSwitch();
#SnmpTempData
$APIGetSnmpTemp = $tinsp->APIGetSnmpTemp();
$FDB_BillArray=array();

foreach ($UsersArray as $UserVal) {
	if (strlen($UserVal["user_mac"])==17){
		$FDB_BillArray[$UserVal["user_mac"]]=array("switch_id"=>$UserVal["switch_id"], "switch_port"=>$UserVal["switch_port"], "user_id"=>$UserVal["user_id"]);
	}
}

if (is_array($SwitchArray) && $SwitchArray[0]=="0"){
	$Stream_arr=array_chunk($SwitchArray[1], count($SwitchArray[1])/$argv[1]);

	if (is_array($Stream_arr[$argv[2]])){
		foreach ($Stream_arr[$argv[2]] as $Switch){
			if ($Switch["switch_snmp"]==1){
				$GetSwitchType=$tinsp->APIGetSwitchType($Switch["switch_type_id"]);
				$tinsp->NET_MONIT->SetSnmp($Switch["switch_ip"], $Switch["switch_snmpcomunity"], $GetSwitchType[1]["walk"]);
				/*
				*	СБОР MAC-АДРЕСОВ С КОММУТАТОРОВ
				*/
				if ($Switch["switch_snmpcollectmac"]==1 && $argv[3]=="check_mac"){
					$tinsp->NET_MONIT->snmpwalk($GetSwitchType[1]["switch_mibfdb"]);

					echo "\n === ".$Switch["switch_ip"]." SUCCESS GET FDB MAC === \n";
					$get_snmpwalk_fdb=$tinsp->NET_MONIT->get_snmpwalk_fdb($Switch["switch_type_id"], $Switch["switch_uplink"]);
					if ($get_snmpwalk_fdb!=FALSE){
						foreach ($get_snmpwalk_fdb as $get_snmpwalk_fdb_val){
							$mac_str=$tinsp->NET_MONIT->mac16_mac10($get_snmpwalk_fdb_val[1]);
							$mac = strtoupper($tinsp->GetMac($mac_str));
							//print $tinsp->GetMac($mac_str)."	".$get_snmpwalk_fdb_val[2]."\n";	
							#if mac isset in billing base
							if (isset($FDB_BillArray[$mac])){
								#if port or switch changed or empty
								if ($FDB_BillArray[$mac]["switch_id"]!=$Switch["switch_id"] or $FDB_BillArray[$mac]["switch_port"]!=$get_snmpwalk_fdb_val[2]){							
									$tinsp->Query("UPDATE billing_users SET 
									switch_id = '".$Switch["switch_id"]."',
									switch_port = '".$get_snmpwalk_fdb_val[2]."' 
									WHERE user_id = '".$FDB_BillArray[$mac]["user_id"]."'");
									print $mac."	".$get_snmpwalk_fdb_val[2]."\n";		
								}
							}else{
								#write unknown mac to base
							}
							
							// Собираем инфу по макам в новую таблицу
							$tinsp->Query("INSERT INTO billing_switch_mac (switch_id, port, mac, cycle_time) VALUES('".$Switch["switch_id"]."', '".$get_snmpwalk_fdb_val[2]."' , '".$mac."', '".$argv[4]."') 
							ON DUPLICATE KEY UPDATE last_seen=NOW(), cycle_time='".$argv[4]."', switch_id='".$Switch["switch_id"]."', port='".$get_snmpwalk_fdb_val[2]."' ");
						}
					}else{
						echo "\n === ".$Switch["switch_ip"].": ERROR GET FDB MAC === \n";
					}
					
				/*
				*	СБОР СТАТИСТИКИ ПО ПОРТАМ С КОММУТАТОРОВ
				*/
				}elseif($argv[3]=="check_port"){
					$CollectPortData=array();
					#состояние портов
					if ($Switch["switch_snmpcollectport"]==1){
						$tinsp->NET_MONIT->snmpwalk($GetSwitchType[1]["switch_mibport"]);
						$get_port_status=$tinsp->NET_MONIT->get_snmpwalk_ports($Switch["switch_type_id"]);
						if ($get_port_status!=FALSE){			
							echo "\n === ".$Switch["switch_ip"]." SUCCESS GET STATUS PORT === \n";
							$tinsp->APISetSnmpData(false, $APIGetSnmpTemp, $Switch["switch_id"], "PORT_CHECK", $get_port_status);
						}
						
					}
					#TX_RX траффик
					if ($Switch["switch_snmpcollecttraff"]==1){
						$tinsp->NET_MONIT->snmpwalk($GetSwitchType[1]["switch_port_tx"]);
						$get_port_tx=$tinsp->NET_MONIT->get_snmpwalk_ifoctets($Switch["switch_type_id"]);
						if ($get_port_tx!=FALSE){			
							echo "\n === ".$Switch["switch_ip"]." SUCCESS GET TX PORT === \n";
							$tinsp->APISetSnmpData(false, $APIGetSnmpTemp, $Switch["switch_id"], "PORT_TX", $get_port_tx);
							
						}
						$tinsp->NET_MONIT->snmpwalk($GetSwitchType[1]["switch_port_rx"]);
						$get_port_rx=$tinsp->NET_MONIT->get_snmpwalk_ifoctets($Switch["switch_type_id"]);
						if ($get_port_rx!=FALSE){			
							echo "\n === ".$Switch["switch_ip"]." SUCCESS GET RX PORT === \n";
							$tinsp->APISetSnmpData(false, $APIGetSnmpTemp, $Switch["switch_id"], "PORT_RX", $get_port_rx);
						}		
								
					} if ($Switch["switch_snmpcollecterror"]==1){ }
				}	
			}
		}
	}
}
$tinsp->APISetSnmpData(true);
?>