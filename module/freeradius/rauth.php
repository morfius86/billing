<?php
/*	FreeRadius 2.x	*/
error_reporting(E_ALL ^ E_NOTICE);
	
#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

function ReturnRequest($nas_type, $param){
	$a = array (
		"1" => array (
			"nas_addresslist" => array(
				"0"	=>	"",
				"1"	=>	"MikroTik-Address-List := ".$param["nas_addresslist"][1]."",
			),
			"nas_shaper" => array(
				"0"	=>	"",
				"1"	=>	""
			)
		),
		"2" => array (
			"nas_addresslist" => array(
				"0"	=>	"",
				"1"	=>	"MikroTik-Address-List := ".$param["nas_addresslist"][1]."",
			),
			"nas_shaper" => array(
				"0"	=>	"",
				"1"	=>	""
			)
		)
	);
	foreach ($param as $k => $v){
		#print_r($param);
		return $a[$nas_type][$k][$v[0]].",";
	}
}

function Find2NasUser($nas_type, $auth_type){
	global $tinsp;
	#*******************************************************************
	# hotspot mikrotik ip+mac/ip/mac
	#*******************************************************************
	if ($nas_type[0]["nas_type"] == "1" or $nas_type[0]["nas_type"] == "2" or $nas_type[0]["nas_type"] == "3"){
	
		$sql_add=array(
			"1" => "`user_ip` like '".$_SERVER["FRAMED_IP_ADDRESS"]."' and `user_mac` like '".str_replace('"', '', $_SERVER["USER_NAME"])."'", 
			"2" => "`user_ip` like '".$_SERVER["FRAMED_IP_ADDRESS"]."'",
			"3" => "`user_mac` like '".str_replace('"', '', $_SERVER["USER_NAME"])."'"
			);
			
		$find = $tinsp->Query("Select * from `billing_users` where ".$sql_add[$nas_type[0]["nas_type"]]." and `user_block` = '0' limit 1",false);	
		if (is_array($find)) {
			#pre/post/auth
			if ($auth_type == "pre_auth"){
				return "Cleartext-Password := \"\"";
			}elseif($auth_type == "post_auth"){
				return "0";
			}else{
				if (trim($find[0]["tarif_id"])==""){
					#групповой тариф
					$Address_list=$tinsp->APIGetBillProp(2, $find[0]["group_id"]);
				}else{
					#индивидуальный тариф
					$Address_list=$tinsp->APIGetTarif(2, $find[0]["tarif_id"]);
				}
				#add to auth_table
				#$tinsp->Query("INSERT INTO `billing_users_auth`(user_id,user_name,framed_ip_address) VALUES ('".$find[0]["user_id"]."','".$_SERVER["USER_NAME"]."', '".$_SERVER["FRAMED_IP_ADDRESS"]."')");
				return ReturnRequest ($nas_type[0]["nas_type"], array("nas_addresslist" => array (trim($nas_type[0]["nas_addresslist"]), trim($Address_list[0]["tarif_name"]))));
			}
		}else{
			return "1";
		}
	}
	#+any nas
}

#ищем nas
$GetNas = $tinsp->Query("Select * from `billing_nas` where `nas_ip` = '".$_SERVER["NAS_IP_ADDRESS"]."' limit 1",false);
if (is_array($GetNas)){		
	#post auth, pre auth, auth
	echo Find2NasUser($GetNas, $argv[1]);
}else{
	exit (1);	
}

?>