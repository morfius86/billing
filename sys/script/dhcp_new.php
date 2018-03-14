<?php
set_time_limit(120);
/*
Sync to DHCP
*/
error_reporting (E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
#params
define ("LEASE_TIME", "2h");
#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->config=$config;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

$APIGetBillProp = $tinsp->APIGetBillProp(3,"");
$APIGetBillProp = (is_array($APIGetBillProp)) ? $APIGetBillProp : exit("error sql 1");

$APIGetNas = $tinsp->APIGetNas(2,"");
$APIGetNas = (is_array($APIGetNas)) ? $APIGetNas : exit("error sql 2");


foreach($APIGetNas as $Nas){
	if ($Nas["nas_dhcp"]=="1"){
		if(eregi(",",$Nas["nas_sector_id"])){
			$i=1;
			foreach(explode(",",$Nas["nas_sector_id"]) as $nas_sector_id){
				$nas_sector_id_name=$tinsp->APIGetSector($nas_sector_id);
				if ($nas_sector_id_name[0]["sector_dhcp_onoff"]==1) {
					$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
					#print "<br>";
				}
			}	
		}else{
			$i=1;
			$nas_sector_id_name=$tinsp->APIGetSector($Nas["nas_sector_id"]);
			if ($nas_sector_id_name[0]["sector_dhcp_onoff"]==1) {
				$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
				#print "<br>";
			}
		}
	}	
}

		
#Проходимся по NAS
foreach ($Nas2Sector as $Nas2SectorKey => $Nas2SectorValue) {
	$s=1;
	$NasOptions=$tinsp->APIGetNas(2, $Nas2SectorKey);
	
	#-->mikrotik
	if ($NasOptions[0]["nas_type"]=="1" or $NasOptions[0]["nas_type"]=="2" or $NasOptions[0]["nas_type"]=="3" or $NasOptions[0]["nas_type"]=="4" or $NasOptions[0]["nas_type"]=="5" or $NasOptions[0]["nas_type"]=="6" or $NasOptions[0]["nas_type"]=="7"){
		#print_r($NasOptions);
		$Array=array();
		#connect
		$tinsp->NAS("mikrotik");
		$tinsp->nas->connect($NasOptions[0]["nas_ip"], $NasOptions[0]["nas_login"], $NasOptions[0]["nas_password"]);	
		$Abonent2Nas = $tinsp->nas->comm("/ip/dhcp-server/lease/print");
		#all from dhcp
		foreach($Abonent2Nas as $Abonent2NasKey => $Abonent2NasValue){
			if (!eregi("id_", @$Abonent2NasValue["comment"])) continue;
			$Array[str_replace("id_", "", $Abonent2NasValue["comment"])]=array("ip"=>$Abonent2NasValue["address"], "mac"=>$Abonent2NasValue["mac-address"],  "server"=>$Abonent2NasValue["server"], "lease-time"=>@$Abonent2NasValue["lease-time"], "id"=>$Abonent2NasValue[".id"]);
		}
		//print_r($Array);
		#all from billing
		foreach($APIGetBillProp as $ArrayAbonentKey=> $ArrayAbonentValue){
			#смотрим чтобы присутствовал мак или ип+мак
			if ($ArrayAbonentValue["user_mac"]!=""){
				#ищем сектор абонента
				$UserSector=$tinsp->APIGetSector($ArrayAbonentValue["user_sector_id"]);
				if(is_array($UserSector[0])){
					$ArrayAbonent[$ArrayAbonentValue["user_id"]]=array(
						"user_ip"=>$ArrayAbonentValue["user_ip"], 
						"user_mac"=>$ArrayAbonentValue["user_mac"], 
						"sector_id"=>$UserSector[0]["sector_id"], 
						"sector_dhcpstat"=>$UserSector[0]["sector_dhcpstat"], 
						"sector_dhcpdyn"=>$UserSector[0]["sector_dhcpdyn"],
						"sector_name"=>$UserSector[0]["sector_name"],
						"sector_dhcp_servername"=>$UserSector[0]["sector_dhcp_servername"]
					);
				}
			}	
		}
		
		#find && edit -> bill to dhcp
		foreach ($Nas2SectorValue as $Nas2SectorValueValue) {
			if(is_array($ArrayAbonent)){
				foreach($ArrayAbonent as $ArrayAbonentId => $ArrayAbonentVal){
					#ищем абонентов принадлежащих данному сектору					
					if($Nas2SectorValueValue[0]==$ArrayAbonentVal["sector_id"]){					
					#смотрим параметры dhcp у сектора 
					
						#static ip+mac
						if ($ArrayAbonentVal["sector_dhcpstat"]==1 and strlen($ArrayAbonentVal["user_ip"])>5 and strlen($ArrayAbonentVal["user_mac"])>5) {
							#есть в микротике, проверим ip, mac, sector
							if (isset($Array[$ArrayAbonentId])){
								#если какой-нибудь из атрибутов не равен, отредактируем
								if(	(string)$Array[$ArrayAbonentId]["ip"]!=(string)$ArrayAbonentVal["user_ip"] or 
									(string)$Array[$ArrayAbonentId]["mac"]!=(string)$ArrayAbonentVal["user_mac"] or 
									(string)$Array[$ArrayAbonentId]["server"]!=(string)$ArrayAbonentVal["sector_dhcp_servername"] or 
									(string)$Array[$ArrayAbonentId]["lease-time"]!=LEASE_TIME){	
									$edit_dhcp=$tinsp->nas->comm("/ip/dhcp-server/lease/set", array(
										"address" => trim($ArrayAbonentVal["user_ip"]),
										"mac-address" => trim($ArrayAbonentVal["user_mac"]), 
										"server" => (string)$ArrayAbonentVal["sector_dhcp_servername"],
										"lease-time" => LEASE_TIME,
										"numbers" => $Array[$ArrayAbonentId]["id"]
									));									
								}
							}else{
								#нет в микротике, создадим
								$add_dhcp=$tinsp->nas->comm("/ip/dhcp-server/lease/add", array("address" => $ArrayAbonentVal["user_ip"],"mac-address" => $ArrayAbonentVal["user_mac"], "server" => trim($ArrayAbonentVal["sector_dhcp_servername"]), "lease-time" => "4h", "comment" => "id_".$ArrayAbonentId));
							}
						}
						
						#dynamic pool mac
						if($ArrayAbonentVal["sector_dhcpstat"]==1 and strlen($ArrayAbonentVal["user_mac"])>5 and strlen($ArrayAbonentVal["user_ip"])<2){
							#есть в микротике, проверим name-pool, mac, sector
							if (isset($Array[$ArrayAbonentId])){
								#если какой-нибудь из атрибутов не равен, отредактируем
								if(	(string)$Array[$ArrayAbonentId]["ip"]!=(string)$ArrayAbonentVal["sector_dhcp_servername"] or 
									(string)$Array[$ArrayAbonentId]["mac"]!=(string)$ArrayAbonentVal["user_mac"] or 
									(string)$Array[$ArrayAbonentId]["server"]!=(string)$ArrayAbonentVal["sector_dhcp_servername"] or
									(string)$Array[$ArrayAbonentId]["lease-time"]!=LEASE_TIME){	
									$edit_dhcp=$tinsp->nas->comm("/ip/dhcp-server/lease/set", array(
										"address" => trim((string)$ArrayAbonentVal["sector_dhcp_servername"]),
										"mac-address" => trim($ArrayAbonentVal["user_mac"]), 
										"server" => (string)$ArrayAbonentVal["sector_dhcp_servername"],
										"lease-time" => LEASE_TIME,
										"numbers" => $Array[$ArrayAbonentId]["id"]
									));	
								}
							}else{
								#нет в микротике, создадим
								$add_dhcp=$tinsp->nas->comm("/ip/dhcp-server/lease/add", array("address" => trim((string)$ArrayAbonentVal["sector_dhcp_servername"]),"mac-address" => $ArrayAbonentVal["user_mac"], "server" => trim($ArrayAbonentVal["sector_dhcp_servername"]), "lease-time" => "4h", "comment" => "id_".$ArrayAbonentId));
							}	
						}
					}
				}
			}	
		}	
		#find && edit -> dhcp to bill
		foreach ($Array as $ArrayKey => $ArrayValue){
			#Запись дхцп не обнаружена в биллинге, удалим 
			if (!isset($ArrayAbonent[$ArrayKey])){
				$add_dhcp=$tinsp->nas->comm("/ip/dhcp-server/lease/remove", array("numbers" => $ArrayValue["id"]));
			}else{
			
			}
		}
	}
}
	
		
		
		


?>