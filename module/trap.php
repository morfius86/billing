#!/usr/bin/php -q
<?php
$test='91.233.47.131
UDP: [91.233.47.131]:161->[91.233.47.147]
DISMAN-EVENT-MIB::sysUpTimeInstance 476:19:35:33.33
SNMPv2-MIB::snmpTrapOID.0 SNMPv2-SMI::enterprises.171.11.64.1.2.15.0.3
SNMPv2-SMI::enterprises.171.11.64.1.2.15.1 "01 1C AF F7 E3 BA 0D 00 01 00 19 00 "
SNMP-COMMUNITY-MIB::snmpTrapAddress.0 192.168.88.45
SNMP-COMMUNITY-MIB::snmpTrapCommunity.0 "private"
SNMPv2-MIB::snmpTrapEnterprise.0 SNMPv2-SMI::enterprises.171.11.64.1.2.15
';
  $message = "";
  $fd = fopen("php://stdin", "r");
  while (!feof($fd)){
    $message .= fread($fd, 1024);
  }
    #traplog($message);
    $trim=explode("\n", $message);
    foreach ($trim as $key => $value){
	#ищем адрес источника
	if (eregi("snmpTrapAddress", $value)){
	    list($non, $ip_switch)=explode(" ",$value);
	    if (preg_match('!"(.*?)"!si',$trim[$key-1],$ok)){
		$port=hexdec(substr(trim($ok[1]), -5, -3));
		$mac=substr(trim($ok[1]), 3, 17);
		$type=substr(trim($ok[1]), 0, 2);
		traplog($ip_switch."	".$type."	".$mac."	".$port."");
		#echo $ip_switch."	".$type."--".$mac."--".$port;
	    }else{
		echo "Тег не найден";
	    }
	}
    }
    function traplog($str){
		$file=fopen("/var/www/billing/module/log/".date("Y-m-d")."_trap.log", "a");
		fwrite($file,$str."\n");
		fclose($file);
    }
	
	traplog($message);
    
?>