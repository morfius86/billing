<?php
/*
Sync to ARP

*/

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
 $tinsp = New API;
 $tinsp->encode="cp1251"; 
 $tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
 
#список нас-ов		
foreach($tinsp->APIGetNas(2,"") as $Nas){
	if ($Nas["nas_arp"]=="1"){
		if(eregi(",",$Nas["nas_sector_id"])){
			$i=1;
			foreach(explode(",",$Nas["nas_sector_id"]) as $nas_sector_id){
				$nas_sector_id_name=$tinsp->APIGetSector($nas_sector_id);
				$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
				#print .."<br>";
			}	
		}else{
			$i=1;
			$nas_sector_id_name=$tinsp->APIGetSector($Nas["nas_sector_id"]);
			foreach(explode(",",$Nas["nas_sector_id"]) as $nas_sector_id){
				$nas_sector_id_name=$tinsp->APIGetSector($nas_sector_id);
				$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
				#print .."<br>";
			}	
		}
	}	
}		

	
$ArrayAbonent=$tinsp->APIGetBillProp(3,"");
#ѕроходимс€ по NAS
foreach ($Nas2Sector as $Nas2SectorKey => $Nas2SectorValue) {
	$s=1;
	$NasOptions=$tinsp->APIGetNas(2, $Nas2SectorKey);

	#-->mikrotik's hotspot
	if ($NasOptions[0]["nas_type"]=="1" or $NasOptions[0]["nas_type"]=="2" or $NasOptions[0]["nas_type"]=="3"){
		$Array=array();
		#connect
		$tinsp->NAS("mikrotik");
		$tinsp->nas->connect($NasOptions[0]["nas_ip"], $NasOptions[0]["nas_login"], $NasOptions[0]["nas_password"]);	
		$Abonent2Nas = $tinsp->nas->comm("/ip/arp/print");
		#all from arp
		foreach($Abonent2Nas as $Abonent2NasKey => $Abonent2NasValue){
			if (!isset($Abonent2NasValue["comment"])) continue;
			if (!eregi("id_", $Abonent2NasValue["comment"])) continue;
			$Array[str_replace("id_", "", $Abonent2NasValue["comment"])]=array("ip"=>$Abonent2NasValue["address"], "mac"=>$Abonent2NasValue["mac-address"],  "interface"=>$Abonent2NasValue["interface"], "id"=>$Abonent2NasValue[".id"]);
		}
		#all from billing
		foreach($ArrayAbonent as $Abonent2Del=> $Abonent2DelValue){
			if ($Abonent2DelValue["user_ip"]=="" or $Abonent2DelValue["user_mac"]=="") continue;
			$ArrayDel[$Abonent2DelValue["user_id"]]=array("ip"=>$Abonent2DelValue["user_ip"], "mac"=>$Abonent2DelValue["user_mac"]);
		}

		#find && edit -> bill to dhcp
		foreach ($Nas2SectorValue as $Nas2SectorValueValue) {
			if(is_array($ArrayAbonent)){
				foreach($ArrayAbonent as $ArrayAbonentKey => $ArrayAbonentVal){
					#only ip+mac
					if ($ArrayAbonentVal["user_ip"]=="" or $ArrayAbonentVal["user_mac"]=="") continue;
					$UserSector=$tinsp->APIGetSector($ArrayAbonentVal["user_sector_id"]);
					#ищем абонентов принадлежишь данному сектору
					if($Nas2SectorValueValue[0]==$UserSector[0][0]["sector_id"]){
						#есть в микротике, проверим ip, mac, interface
						if (isset($Array[$ArrayAbonentVal["user_id"]])){
							#если какой-нибудь из атрибутов не равен, отредактируем
							if((string)$Array[$ArrayAbonentVal["user_id"]]["ip"]!=(string)$ArrayAbonentVal["user_ip"] or (string)$Array[$ArrayAbonentVal["user_id"]]["mac"]!=(string)$ArrayAbonentVal["user_mac"] or (string)$Array[$ArrayAbonentVal["user_id"]]["interface"]!=(string)$UserSector[0][0]["sector_name"]){
								$edit_dhcp=$tinsp->nas->comm("/ip/arp/set", array(
									"address" => trim($ArrayAbonentVal["user_ip"]),
									"mac-address" => trim($ArrayAbonentVal["user_mac"]), 
									"interface" => (string)$UserSector[0]["sector_name"],
									"numbers" => $Array[$ArrayAbonentVal["user_id"]]["id"]
								));							
							}
						}else{
							#нет в микротике, создадим
							$add_dhcp=$tinsp->nas->comm("/ip/arp/add", array("address" => $ArrayAbonentVal["user_ip"],"mac-address" => $ArrayAbonentVal["user_mac"], "interface" => trim($UserSector[0][0]["sector_name"]), "comment" => "id_".$ArrayAbonentVal["user_id"]));
						}	
					}
				}
			}	
		}	
		#find && edit -> dhcp to bill
		foreach ($Array as $ArrayKey => $ArrayValue){
			#«апись дхцп не обнаружена в биллинге, удалим 
			if (!isset($ArrayDel[$ArrayKey])){
				$add_dhcp=$tinsp->nas->comm("/ip/arp/remove", array("numbers" => $ArrayValue["id"]));
			}else{
			
			}
		}
	}
}
	
		
		
		


?>