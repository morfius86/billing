<?php
/*
ping switch 
*/
#error_reporting (E_ERROR | E_WARNING | E_PARSE | E_NOTICE);

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect class API
$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
#connect class API::NETWORK_MONITOR;
$tinsp->include_network_monitor();

$SwitchArray = $tinsp->APIGetSwitch();
#проходимся по каждому устройству 
if (is_array($SwitchArray) && $SwitchArray[0]=="0"){
	foreach ($SwitchArray[1] as $Switch){
		switch ($Switch["switch_monit"]){
			case "1":		
				echo "PING check ".$Switch["switch_ip"].": ";			
				$check_ping=$tinsp->NET_MONIT->ping($Switch["switch_ip"]);
				if ($check_ping!=false){
					echo "ok";
					#статус изменен 
					if ($Switch["switch_monit_status"]!=1){
						$tinsp->APISetSwitch(array("obj"=>$Switch["switch_id"], "switch_monit_status"=> 1, "switch_monit_statustime"=> date("Y-m-d H:i:s")));
						#записать в журнал событий 
						echo ";new status";
					}
				}else{
					echo "fail";
					#статус изменен 
					if ($Switch["switch_monit_status"]!=0){
						$tinsp->APISetSwitch(array("obj"=>$Switch["switch_id"], "switch_monit_status"=> 0, "switch_monit_statustime"=> date("Y-m-d H:i:s")));
						#записать в журнал событий 
						echo ";new status";
					}
				}
				echo "\n";
				break;
			case "2":
				echo "SNMP check: ";
				break;
		}
	}
}

?>