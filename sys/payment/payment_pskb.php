<?php
/* kassira.net to simple-billing
(с)Bobrov Nikita
icq 332277202 */

#OPTIONS
define("SENDER", "Pskb_bank");
define("DATENOW", date("Y-m-d H:i:s"));
define("BASIC_LOG", "pskb_bank");
define("BASIC_PASS", "super_pskb_bank");

#include config & classs
include("../../config.php");
include("../../class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

#any php ver.
if (!isset($PHP_AUTH_USER)){$PHP_AUTH_USER = $_SERVER["PHP_AUTH_USER"];}
if (!isset($PHP_AUTH_PW)){$PHP_AUTH_PW = $_SERVER["PHP_AUTH_PW"];}

#basic-auth
if (!isset($PHP_AUTH_USER)){
	Header('WWW-Authenticate: Basic realm="Authenticate"');
	Header("HTTP/1.0 401 Unauthorized");
	exit();
}else{	
	if ($PHP_AUTH_USER!=BASIC_LOG or $PHP_AUTH_PW!=BASIC_PASS){
		fileopen("".DATENOW." Error Authenticate. IP: ".getenv("REMOTE_ADDR")."\n");
		Header('WWW-Authenticate: Basic realm="Authenticate"');
		Header("HTTP/1.0 401 Unauthorized");
		exit();
	}
}

Header("Content-type: text/plain");


#поиск абонента по атрибуту
function getLogin($login){
Global $tinsp;
$UserBill=$tinsp->APISearchAbonent("!".$login, "", "",  "",  "",  "");
#print_r($UserBill);
	if(is_array($UserBill[1]) && count($UserBill[1])==1){
		$ArrayResult=array("basic_account"=>$login, "balance"=>$UserBill[1][0]["balance"], "GUID"=>$UserBill[1][0]["user_id"]);
		return $ArrayResult;
	}else{
		return array("basic_account"=>"", "balance"=>"", "GUID"=>"");
	}
}

function fileopen($str){
	$file = fopen ("log/".SENDER."_".date("y-m-d").".txt","a-");
	fputs ( $file, $str); fclose ($file);
};


#Проверяем на валидность _GET
$Array1=array("duser"=>1, "dpass"=>2, "cid"=>3, "uact"=>4);
$Array2=array_change_key_case($_GET, CASE_LOWER);

if(!each(array_diff_key($Array1, $Array2))){
   #Ключи к нижнему регистру, чтобы не заебавали  
   $GetArray=array_change_key_case($_GET, CASE_LOWER);
   #логин
   $duser=$GetArray["duser"];
   #пароль
   $dpass=$GetArray["dpass"];
   #идентификатор счета
   $cid=$GetArray["cid"];
   #ищем в биллинг
   $data_gost = getLogin($cid);
   #типа запроса
   $uact=$GetArray["uact"];
   #сумма
   $sum=$GetArray["sum"];
   #терминал
   $term=$GetArray["term"];
   #транзакиция
   $trans=$GetArray["trans"];

   	if (eregi ("(^[0-9]{3,8}$)", $cid)) {
		if($data_gost["basic_account"]!=""){

			$basic_account=$data_gost["basic_account"];
			$balance=$data_gost["balance"];
			$guid=$data_gost["GUID"];
			if($uact=="get_info"){
				OutPut(SENDER, $id=NULL, 0, "get_info", $cid, $uact, $term, $trans, $balance, "", DATENOW);			
			}elseif($uact=="payment"){
				if (eregi ("^[0-9.-]{1,32}$", $sum)){
					if (eregi ("(^[0-9]{1,12}$)", $term)){
						if (eregi ("(^[0-9]{1,20}$)", $trans)){
							#ищем номер транзакции в БД
							$datachk = $tinsp->Query("SELECT * FROM billing_payment WHERE payment_id = '".$term.$trans."' and res = '0' and sender = '".SENDER."' LIMIT 1", false);
							$payment_ext_number=$datachk[0]["payment_id"];
							#искомая транзация не равна текущей транзакции, значит зачислим
							if ($payment_ext_number != $term.$trans){
								#зачисляем 
								$pay=$tinsp->APIAddCash($guid, $sum, "Оплата через терминал kassiran.net", $term.$trans, SENDER);
								if ($pay[0]=="0") {
									OutPut(SENDER, $id=NULL, 0, "OK", $cid, $uact, $term, $trans, $sum, $sum+$bal_old, DATENOW);
								}else{
									OutPut(SENDER, $id=NULL, -100, "Error payment", $cid, $uact, $term, $trans, $sum, $sum, DATENOW);
								}
							}elseif ($payment_ext_number == $term.$trans){
								#Такой платеж уже есть в базе
								OutPut(SENDER, $id=NULL, 1, "Payment_id ".$term.$trans." already exist in base", $cid, $uact, $term, $trans, $sum, "", DATENOW);
							}
						}else{
							OutPut(SENDER, $id=NULL, -3, "trans not correct", $cid, $uact, $term, $trans, $sum, "", DATENOW);
						}
					}else{
						OutPut(SENDER, $id=NULL, -3, "term not correct", $cid, $uact, $term, $trans, $sum, "", DATENOW);			
					}
				}else{
					OutPut(SENDER, $id=NULL, -5, "sum not correct", $cid, $uact, $term, $trans, $sum, "", DATENOW);
				}
			}else{
				OutPut(SENDER, $id=NULL, -4, "Uact not correct", $cid, $uact, $term, $trans, $sum, "", DATENOW);			
			}
		}else{
			OutPut(SENDER, $id=NULL, -1, "Personal account $cid not found", $cid, $uact, $term, $trans, $sum, "", DATENOW);			
		}
	}else{
		OutPut(SENDER, $id=NULL, -3, "Personal account $cid incorrect format", $cid, $uact, $term, $trans, $sum, "", DATENOW);			
	}
}else{
	OutPut(SENDER, $id=NULL, -100, "Incorrect _GET parameters ".implode(",", array_keys($_GET))."", $cid, $uact, $term, $trans, $sum, "", DATENOW);	
}

#output
function OutPut($sender, $id, $res, $result, $cid, $uact, $term, $trans, $sum, $newsum, $datenow){
	global $tinsp;
	global $guid;
	$TEMPLATE["RES_CHECK"] = "status=[STATUS]";
	if($uact=="payment"){
		$replace = array("[STATUS]" => $res, "[SUMMA]" => $newsum);
	}elseif($uact=="get_info"){
		$replace = array("[STATUS]" => $res, "[SUMMA]" => "");
	}else{
		$replace = array("[STATUS]" => "-100", "[SUMMA]" => "");
	}
	
	$txt=implode(",", array_keys($_GET));
	$txt_val=implode(",", array_values($_GET));
	fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	$result key:[$txt] val:[$txt_val] return:[".strtr($TEMPLATE["RES_CHECK"],$replace)."]\n");
	
	#log billing
	if ($res!="0") {
		$tinsp->APICreatEvent("pay_error", $guid, "", "", $res, "Sender: ".$sender.", order = ".$cid.", sum = ".$sum.", error:".$result);
	}
	

	echo strtr($TEMPLATE["RES_CHECK"], $replace);
}


?>