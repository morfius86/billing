<?php
set_time_limit(120);
/*
Sync to DHCP
*/
//error_reporting (E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
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
		}	
	}else{
		$i=1;
		$nas_sector_id_name=$tinsp->APIGetSector($Nas["nas_sector_id"]);
		$Nas2Sector[$Nas["nas_id"]][$i++]=array($nas_sector_id_name[0]["sector_id"]);
	}
}	
	
#список секторов/тарифов в биллинге в виду массива
@$GetSectors=$tinsp->APIGetSector();
@$GetTarif=$tinsp->APIGetTarif(2, '');
$SectorArray = array();
$TarifArray = array();

foreach($GetSectors as $SectorKey => $SectorVal){ 
	$SectorArray[$SectorVal["sector_id"]]=$SectorVal["sector_name"]; 
}

foreach($GetTarif as $TarifKey => $TarifVal){ 
	$TarifArray[$TarifVal["tarif_id"]]=array(
		"tarif_name" => $TarifVal["tarif_name"],
		"mik_adlist" => $TarifVal["mik_adlist"],
		"tarif_speed_in" => $TarifVal["tarif_speed_in"],
		"tarif_speed_out" => $TarifVal["tarif_speed_out"]
	); 
}

#Проходимся по NAS
foreach ($Nas2Sector as $Nas2SectorKey => $Nas2SectorValue) {
	$s=1;
	$NasOptions=$tinsp->APIGetNas(2, $Nas2SectorKey);
	
/* 
 * Подключаемся к микротику.
 * Получаем список профилей и юзеров и заносим в массив
 * Получаем список абонентов из биллинга и заносим в массив
*/
	
	#-->mikrotik only API HotSpot
	if ($NasOptions[0]["nas_type"]=="6" or $NasOptions[0]["nas_type"]=="7"){
		#print_r($NasOptions);
		$HotSpotProfileArray=array();
		$HotSpotUserArray=array();
		$HotSpotQueueTypes=array();
		$HotSpotTarifProfile=array();
		$HotSpotActive=array();
		$QueueSimple=array();
		
		#connect
		$tinsp->NAS("mikrotik");
		$tinsp->nas->connect($NasOptions[0]["nas_ip"], $NasOptions[0]["nas_login"], $NasOptions[0]["nas_password"]);	
		
		# hotspot profile from mikrotik
		$GetHotSpotProfile = $tinsp->nas->comm("/ip/hotspot/user/profile/print");
		foreach($GetHotSpotProfile as $ProfileKey => $ProfileVal){
			if (!eregi("id_", @$ProfileVal["name"])) continue;
			$HotSpotProfileArray[str_replace("id_", "", $ProfileVal["name"])]=array(
				"name"=>$ProfileVal["name"], "address-list"=>$ProfileVal["address-list"], 
				"idle-timeout"=>$ProfileVal["idle-timeout"], "insert-queue-before"=>$ProfileVal["insert-queue-before"], 
				"queue-type"=>$ProfileVal["queue-type"], "rate-limit" => $ProfileVal["rate-limit"], "id"=>$ProfileVal[".id"]
			);
		}
		
		# hotspot user from mikrotik
		$GetHotSpotUser = $tinsp->nas->comm("/ip/hotspot/user/print");
		foreach($GetHotSpotUser as $UserKey => $UserVal){
			if (!eregi("id_", @$UserVal["comment"])) continue;
			//print_r($UserVal);
			$HotSpotUserArray[str_replace("id_", "", $UserVal["comment"])]=array("name"=>$UserVal["name"], "disabled" => $UserVal["disabled"], "server"=>@$UserVal["server"], "mac" => $UserVal["mac-address"], "address" => @$UserVal["address"], "profile" => $UserVal["profile"],"id"=>$UserVal[".id"]);
		}
		
		# queue type from mikrotik
		$GetHotSpotQueueTypes= $tinsp->nas->comm("/queue/type/print");
		foreach($GetHotSpotQueueTypes as $QueueKey => $QueueVal){
			if (!eregi("id_", @$QueueVal["name"])) continue;
			$HotSpotQueueTypes[str_replace("id_", "", $QueueVal["name"])]=array("name"=>$QueueVal["name"], "pcq-rate"=>$QueueVal["pcq-rate"], "kind"=>$QueueVal["kind"], "id"=>$QueueVal[".id"]);
		}
		
		# active from mikrotik
		$GetHotSpotActive= $tinsp->nas->comm("/ip/hotspot/active/print");
		foreach($GetHotSpotActive as $HotSpotActiveKey => $HotSpotActiveVal){
			if (!eregi("id_", @$HotSpotActiveVal["comment"])) continue;
			$HotSpotActive[str_replace("id_", "", $HotSpotActiveVal["comment"])]=$HotSpotActiveVal;
		}
		//print_r($HotSpotActive);
		
		# queue simple from mikrotik
		$GetQueueSimple= $tinsp->nas->comm("/queue/simple/print");
		foreach($GetQueueSimple as $QueueSimpleKey => $QueueSimpleVal){
			if (!eregi("id_", @$QueueSimpleVal["name"])) continue;
			$QueueSimple[str_replace("id_", "", $QueueSimpleVal["name"])]=$QueueSimpleVal;
		}

		#all from billing
		foreach($tinsp->APIGetBillProp(3,"") as $ArrayAbonentKey=> $ArrayAbonentValue){
			#ищем сектор абонента
			$UserSector=$SectorArray[$ArrayAbonentValue["user_sector_id"]];
			if(isset($UserSector)){
				$ArrayAbonent[$ArrayAbonentValue["user_id"]]=array(
					"user_id"=>$ArrayAbonentValue["user_id"], 
					"user_ip"=>$ArrayAbonentValue["user_ip"], 
					"user_mac"=>$ArrayAbonentValue["user_mac"], 
					"sector_id"=>$ArrayAbonentValue["user_sector_id"], 
					"sector_name"=>$UserSector,
					"tarif_id"=>$ArrayAbonentValue["tarif_id"], 
					"user_block"=>(($ArrayAbonentValue["user_block"] == 0)? 'false':'true'),
				);
				
				$HotSpotTarifProfile[$ArrayAbonentValue["user_sector_id"]][$ArrayAbonentValue["tarif_id"]]='0';
			}
		}
		
/* 
 * Синхронизируем тарифы для сектора из биллинга с профилями hotspot
 * Список тарифов получаем из списка абонентов в секторе.
 */
		$ArrTarifNasSector = array();
		foreach ($Nas2SectorValue as $Nas2SectorValueValue) {
			# выбираем используемые тарифы для выбранного сектора
			if (isset($HotSpotTarifProfile[$Nas2SectorValueValue[0]])){	
				foreach($HotSpotTarifProfile[$Nas2SectorValueValue[0]] as $TarHotSpotKey => $TarHotSpotVal){		
					$ArrTarifNasSector[] = $TarHotSpotKey;
					
					## queue type
					if (isset($HotSpotQueueTypes[$TarHotSpotKey])){
						# если какой-нибудь из атрибутов не равен, отредактируем
						$edit_queue_attr = array ();
						
						if((string)$HotSpotQueueTypes[$TarHotSpotKey]["kind"]!="pcq") {
							$edit_queue_attr["kind"] = "pcq";
						}				
						
						if(($HotSpotQueueTypes[$TarHotSpotKey]["pcq-rate"]/1000000)!=$TarifArray[$TarHotSpotKey]["tarif_speed_in"]){
							$edit_queue_attr["pcq-rate"] = $TarifArray[$TarHotSpotKey]["tarif_speed_in"]."M";				
						}
						
						if (count($edit_queue_attr)>0){
							$edit_queue_attr["numbers"] = $HotSpotQueueTypes[$TarHotSpotKey]["id"];
							$tinsp->nas->comm("/queue/type/set", $edit_queue_attr);
							# log
							$tinsp->WriteLog('nas_hotspot_sync', 'edit queue: '.http_build_query($edit_queue_attr, "", ",").'');	
						}									
					}else{
						# моздадим queue type
						$tinsp->nas->comm(
						"/queue/type/add", array(
							"name" => "id_".$TarHotSpotKey,
							"kind" => "pcq", 
							"pcq-classifier"=> "dst-address",
							"pcq-rate" => $TarifArray[$TarHotSpotKey]["tarif_speed_in"]."M"
						));
						#log
						$tinsp->WriteLog("nas_hotspot_sync", "create queue type: ".$TarifArray[$TarHotSpotKey]["tarif_name"]."");	
					}
					
					## hotspot user profile
					if (isset($HotSpotProfileArray[$TarHotSpotKey])){
						# если какой-нибудь из атрибутов не равен, отредактируем
						$edit_profile_attr = array ();
						
						if((string)$HotSpotProfileArray[$TarHotSpotKey]["address-list"]!=(string)$TarifArray[$TarHotSpotKey]["tarif_name"]){
							$edit_profile_attr["address-list"] = (string)$TarifArray[$TarHotSpotKey]["tarif_name"];				
						}
						
						if((string)$HotSpotProfileArray[$TarHotSpotKey]["idle-timeout"]!="5m"){
							$edit_profile_attr["idle-timeout"] = "5m";				
						}
						
						if((string)$HotSpotProfileArray[$TarHotSpotKey]["rate-limit"]!=""){
							$edit_profile_attr["rate-limit"] = "";				
						}
						
						# если для тарифа есть queue type, то отредактируем
						if (isset($HotSpotQueueTypes[$TarHotSpotKey])) {
							if((string)$HotSpotProfileArray[$TarHotSpotKey]["insert-queue-before"]!=""){
								$edit_profile_attr["insert-queue-before"] = "";				
							}
							
							if((string)$HotSpotProfileArray[$TarHotSpotKey]["queue-type"]!=""){
								$edit_profile_attr["queue-type"] = "";				
							}
						}
						
						if (count($edit_profile_attr)>0){
							$edit_profile_attr["numbers"] = $HotSpotProfileArray[$TarHotSpotKey]["id"];
							$q = $tinsp->nas->comm("/ip/hotspot/user/profile/set", $edit_profile_attr);
							# log
							$tinsp->WriteLog('nas_hotspot_sync', 'edit profile: '.http_build_query($edit_profile_attr, "", ",").'');	
						}
															
					}else{
						# нет в микротике, создадим
						$add_profile=$tinsp->nas->comm(
						"/ip/hotspot/user/profile/add", array(
							"name" => 'id_'.$TarHotSpotKey.'',
							"session-timeout" => '1d', 
							"idle-timeout" => '5m',
							"keepalive-timeout" => '1m',
							"shared-users" => "2",
							"address-list" => $TarifArray[$TarHotSpotKey]["mik_adlist"],
							"add-mac-cookie"=> 'yes',
							"mac-cookie-timeout" => '1d'
						));
						#log
						$tinsp->WriteLog('nas_hotspot_sync', 'create profile: '.$TarifArray[$TarHotSpotKey]["tarif_name"].'');	
					}
				}	
			}else{
				//тарифов для сектора нет ..
			}	
			
			
/* 
 * 
 * Синхронизируем абонентов с ip hotspot user (mac/mac+ip)
 * 
 */
			if(is_array($ArrayAbonent)){
				foreach($ArrayAbonent as $ArrayAbonentId => $ArrayAbonentVal){
					# ищем абонентов принадлежащих данному сектору					
					if($Nas2SectorValueValue[0]==$ArrayAbonentVal["sector_id"]){					

						/* MAC */
						if (strlen($ArrayAbonentVal["user_mac"])>5 && $NasOptions[0]["nas_type"]=="6" ) {
							#есть в микротике, проверим ip, mac, profile
							if (isset($HotSpotUserArray[$ArrayAbonentId])){
								#если какой-нибудь из атрибутов не равен, отредактируем
								//(string)$HotSpotUserArray[$ArrayAbonentId]["server"]!=(string)$ArrayAbonentVal["sector_name"]
								### (string)$HotSpotUserArray[$ArrayAbonentId]["address"]!=(string)$ArrayAbonentVal["user_ip"] or 
								if( (string)$HotSpotUserArray[$ArrayAbonentId]["disabled"]!=(string)$ArrayAbonentVal["user_block"] or 
									(string)$HotSpotUserArray[$ArrayAbonentId]["mac"]!=(string)$ArrayAbonentVal["user_mac"] or 
									(string)$HotSpotUserArray[$ArrayAbonentId]["name"]!=(string)$ArrayAbonentVal["user_mac"] or 
									(string)$HotSpotUserArray[$ArrayAbonentId]["profile"]!='id_'.$ArrayAbonentVal["tarif_id"].''){	
									
									if ((string)$ArrayAbonentVal["user_ip"]==''){ $ArrayAbonentVal["user_ip"] = '0.0.0.0'; }
									
									$edit_user=$tinsp->nas->comm("/ip/hotspot/user/set", array(
										#"address" => $ArrayAbonentVal["user_ip"],
										"mac-address" => (string)$ArrayAbonentVal["user_mac"],
										"name" => (string)$ArrayAbonentVal["user_mac"],
										"profile" => 'id_'.$ArrayAbonentVal["tarif_id"],
										//"server" => (string)$ArrayAbonentVal["sector_name"],
										"disabled" => (string)$ArrayAbonentVal["user_block"],
										"numbers" => $HotSpotUserArray[$ArrayAbonentId]["id"]
									));		
									#log
									$tinsp->WriteLog('nas_hotspot_sync', 'edit user: '.$ArrayAbonentVal["user_mac"].'	profile: '.$TarifArray[$ArrayAbonentVal["tarif_id"]]["tarif_name"].'	disabled: '.$ArrayAbonentVal["user_block"].'');	
									print_r($ArrayAbonentVal);
								}
							}else{
								if ((string)$ArrayAbonentVal["user_ip"]==''){ $ArrayAbonentVal["user_ip"] = '0.0.0.0'; }
								# нет в микротике, создадим
								$add_profile=$tinsp->nas->comm(
									"/ip/hotspot/user/add", array(
									### "address" => $ArrayAbonentVal["user_ip"],
									"mac-address" => (string)$ArrayAbonentVal["user_mac"],
									"name" => (string)$ArrayAbonentVal["user_mac"],
									"profile" => 'id_'.$ArrayAbonentVal["tarif_id"],
									//"server" => (string)$ArrayAbonentVal["sector_name"],
									"comment" => 'id_'.$ArrayAbonentVal["user_id"],
								));
								$tinsp->WriteLog('nas_hotspot_sync', 'creat user: '.$ArrayAbonentVal["user_mac"].'	profile: '.$TarifArray[$ArrayAbonentVal["tarif_id"]]["tarif_name"].'	disabled: '.$ArrayAbonentVal["user_block"].'');	
							}
						
						/* IP */
						}
					}
				}
			}
		}	
		print $NasOptions[0]["nas_ip"]."\n";
		//print_r($ArrTarifNasSector[$Nas2SectorKey]);
##
##	Queue Simple
##

if ($NasOptions[0]["nas_shaper"] == 2) {
	# ищем несозданные queue, сравниваем active с simple
	$CreateQueueArray = array_diff_key($HotSpotActive, $QueueSimple);
	foreach ($CreateQueueArray as $CreateQueueKey => $CreateQueueVal) {
		$tinsp->nas->comm(
			"/queue/simple/add", array(
			"name" => $CreateQueueVal["comment"],
			"target" => $CreateQueueVal["address"]."/32",
			"dst" => $NasOptions[0]["nasp_uplink_interface"],
			"queue" => "default-small/".$NasOptions[0]["nasp_queue_type"], 
			"limit-at" => $TarifArray[$ArrayAbonent[$CreateQueueKey]["tarif_id"]]["tarif_speed_out"]."M/".$TarifArray[$ArrayAbonent[$CreateQueueKey]["tarif_id"]]["tarif_speed_in"]."M",
			"max-limit" => $TarifArray[$ArrayAbonent[$CreateQueueKey]["tarif_id"]]["tarif_speed_out"]."M/".$TarifArray[$ArrayAbonent[$CreateQueueKey]["tarif_id"]]["tarif_speed_in"]."M",
			"total-queue" => $NasOptions[0]["nasp_queue_type"]
		));
		//
	}
	print count($CreateQueueArray)."-";
	
	# ищем simple, которых нет в active. Удаляем из queue.
	$DelQueueArray = array_diff_key($QueueSimple, $HotSpotActive);
	foreach ($DelQueueArray as $DelQueueKey => $DelQueueVal) {
		$tinsp->nas->comm("/queue/simple/remove", array("numbers" => $DelQueueVal[".id"]));
	}
	
	print count($DelQueueArray);
		


}

		
/*
* 	Ищем удаленные учетные записи из биллинга, удалим их hotspot user
* - удалять абонентов, которых нет в сегментах для этого микротика
*/
		foreach ($HotSpotUserArray as $HotSpotUserKey => $HotSpotUserVal){
			#Запись не обнаружена в биллинге, удалим 
			if (!isset($ArrayAbonent[$HotSpotUserKey])){
				$remove_user=$tinsp->nas->comm("/ip/hotspot/user/remove", array("numbers" => $HotSpotUserVal["id"]));
				$tinsp->WriteLog('nas_hotspot_sync', 'delete user(fail bill): '.$HotSpotUserVal["mac"].'');	
			# Абонент есть в биллинге, но мак-адрес не соответствует (подчищаем)
			}elseif (isset($ArrayAbonent[$HotSpotUserKey]) && (string)$HotSpotUserVal["mac"] != (string)$ArrayAbonent[$HotSpotUserKey]["user_mac"])  {
				$tinsp->nas->comm("/ip/hotspot/user/remove", array("numbers" => $HotSpotUserVal["id"]));
				$tinsp->WriteLog('nas_hotspot_sync', 'delete user(fail mac): '.$HotSpotUserVal["mac"].'');	
			}
		}
/*
* Ищем удаленные тарифы записи из биллинга, удалим из hotspot profile
*
*/
		foreach ($HotSpotProfileArray as $HotSpotProfileKey => $HotSpotProfileVal){
			# profile не найден в тарифах, значит удаляем.
			if (!isset($TarifArray[$HotSpotProfileKey])){
				$tinsp->nas->comm("/ip/hotspot/user/profile/remove", array("numbers" => $HotSpotProfileVal["id"]));
				$tinsp->nas->comm("/queue/type/remove", array("numbers" => $HotSpotQueueTypes[$HotSpotProfileKey]["id"]));
				$tinsp->WriteLog('nas_hotspot_sync', 'delete profile(no tarif): '.$HotSpotProfileVal["name"].'');	
			# hotspot profile не найден в масиве уникальных тарифов сектора у nas
			}elseif(!in_array($HotSpotProfileKey, array_unique($ArrTarifNasSector))){
				$tinsp->nas->comm("/ip/hotspot/user/profile/remove", array("numbers" => $HotSpotProfileVal["id"]));
				$tinsp->nas->comm("/queue/type/remove", array("numbers" => $HotSpotQueueTypes[$HotSpotProfileKey]["id"]));
				$tinsp->WriteLog('nas_hotspot_sync', 'delete profile(no users in tarif): '.$HotSpotProfileVal["name"].'');	
			}
		}
	}

}

		
		
		


?>