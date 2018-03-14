<?php
/*
Drop session in NAS
*/

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->config = $config;
$tinsp->encode = "cp1251";
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

foreach($tinsp->APIGetNas(2,"") as $Nas){
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
		$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
		#print .."<br>";	
	}
}
#������ ���������
$ArrayAbonent=$tinsp->APIGetBillProp(3,"");
#���������� �����
$tinsp->NAS("mikrotik");
#���������� �� NAS
foreach ($Nas2Sector as $Nas2SectorKey => $Nas2SectorValue) {
	$s=1;
	$NasOptions=$tinsp->APIGetNas(2, $Nas2SectorKey);
		
	#-->type hotspot mikrotik's bras
	if ($NasOptions[0]["nas_type"]=="1" or $NasOptions[0]["nas_type"]=="2" or $NasOptions[0]["nas_type"]=="3" or $NasOptions[0]["nas_type"]=="6" or $NasOptions[0]["nas_type"]=="7"){
		$ArrayNas=array();
		$ArrayBill=array();
		$tinsp->nas->connect($NasOptions[0]["nas_ip"], $NasOptions[0]["nas_login"], $NasOptions[0]["nas_password"]);	

		$Abonent2Nas = $tinsp->nas->comm("/ip/hotspot/host/print");	
		#all from hotspot
		foreach($Abonent2Nas as $Abonent2NasKey => $Abonent2NasValue){
			$ArrayNas[$Abonent2NasValue[".id"]]=array("mac"=>$Abonent2NasValue["mac-address"], "auth"=>$Abonent2NasValue["authorized"], "address"=>$Abonent2NasValue["address"]);
		}
		//print_r ($ArrayNas);
		
		#all from billing
		foreach($ArrayAbonent as $ArrayAbonentKey=> $ArrayAbonentValue){
			#only isset mac
			if ($ArrayAbonentValue["user_mac"]=="") continue;
			#������ ������ && �������������� 
			if(eregi($ArrayAbonentValue["user_sector_id"], $NasOptions[0]["nas_sector_id"])){
				$ArrayBill[$ArrayAbonentValue["user_mac"]]=array("user_id"=>$ArrayAbonentValue["user_id"], "auth"=>$ArrayAbonentValue["user_block"], "user_name"=>$ArrayAbonentValue["user_name"]);
			}
		}
		

		foreach ($ArrayNas as $ArrayNasKey => $ArrayNasVal){
			#���� � ���� ��������
			if (isset($ArrayBill[$ArrayNasVal["mac"]])){
				#���� ������ ���������� � ������� �� ����� ������� � �������� -> ������ drop ������
				if($ArrayBill[$ArrayNasVal["mac"]]["auth"]==0 && $ArrayNasVal["auth"]=="false"){
					$tinsp->nas->comm("/ip/hotspot/host/remove", array("numbers" => $ArrayNasKey));
					#log
					$tinsp->WriteLog('nas_drop_session', 'drop_session	nas: '.$NasOptions[0]["nas_ip"].'	'.$ArrayNasVal["mac"].'	'.$ArrayNasVal["address"].'		'.$ArrayBill[$ArrayNasVal["mac"]]["user_name"].'');
				}elseif($ArrayBill[$ArrayNasVal["mac"]]["auth"]!=0 && $ArrayNasVal["auth"]=="true"){
					echo 2;
					$tinsp->nas->comm("/ip/hotspot/host/remove", array("numbers" => $ArrayNasKey));	
					#log
					$tinsp->WriteLog('nas_drop_session', 'drop_session	nas: '.$NasOptions[0]["nas_ip"].'	'.$ArrayNasVal["mac"].'	'.$ArrayNasVal["address"].'		'.$ArrayBill[$ArrayNasVal["mac"]]["user_name"].'');					
				}
			}else{

			}
		}
	}
}
		
		



?>