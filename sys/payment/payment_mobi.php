<?php
/* mobi.money to simple-billing
(с)Bobrov Nikita
icq 332277202 */

#OPTIONS
define("SENDER", "MOBI.money");
define("NOWVER", "1.3");
define("DATENOW", date("Y-m-d H:i:s"));
define("BASIC_LOG", "mobi");
define("BASIC_PASS", "supermobi");

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

header("Content-type: text/xml");

function getLogin($login){
Global $tinsp;
$UserBill=$tinsp->APISearchAbonent("!".$login, "", "",  "",  "",  "");
	if(is_array($UserBill[1]) && count($UserBill[1])==1){
		$ArrayResult=array("basic_account"=>$login, "balance"=>$UserBill[1][0]["balance"], "GUID"=>$UserBill[1][0]["user_id"]);
		return $ArrayResult;
	}else{
		return array("basic_account"=>"", "balance"=>"", "GUID"=>"");
	}
}

function fileopen($str){
	$file = fopen ("log/".SENDER."_".date("y-m-d").".txt","a");
	fputs ( $file, $str); fclose ($file);
};


#PAY OR CHECK
$command=$_GET["command"];

if ($command == "check"){
   $txn_id=$_GET["txn_id"];
   $account=$_GET["account"];
   $sum=$_GET["sum"];
   #Запрет на проведение платежей
	if (eregi ("(^[0-9]{1,20}$)", $txn_id)) {
		#латиница, цифры и больше 6 символов
		if (eregi ("^[a-zA-Z0-9.-]{4,32}$", $account)) {
			if (eregi ("(^[0-9]{1,6})([.])([0-9]{1,6}$)", $sum)) {
				#Ищем аккаунт в базе ТИ
				$data = getLogin($account);
				$basic_account=$data["basic_account"];
				$balance=$data["balance"];
				if ($basic_account == ""){
					OutPut($command, NOWVER, SENDER, $id=NULL, 5, "Номер договора $account не найден", "", $txn_id, $basic_account, $sum, DATENOW, DATENOW);
				}elseif ($basic_account != ""){
					OutPut($command, NOWVER, SENDER, $id=NULL, 0, "Номер договора $account найден", "", $txn_id, $basic_account, $sum, DATENOW, DATENOW);
				}
			}else{
				OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Некорректный формат суммы", "", $txn_id, $basic_account, $sum, DATENOW, DATENOW);
   			}
		}else{
			OutPut($command, NOWVER, SENDER, $id=NULL, 4, "Некорректный формат договора", "", $txn_id, $basic_account, $sum, DATENOW, DATENOW);
   		}
	}else{
		OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Некорректный формат транзакции", "", $txn_id, $basic_account, $sum, DATENOW, DATENOW);
	}
}elseif ($command == "pay"){
	$txn_id=$_GET["txn_id"];
	$txn_date=$_GET["txn_date"];
	$account=$_GET["account"];
	$sum=$_GET["sum"];
	$data = getLogin($account);
	$GUID=$data["GUID"];
	if (eregi ("(^[0-9]{1,20}$)", $txn_id)) {
		#проверяем аккаунт
		if (eregi ("^[a-zA-Z0-9.-]{4,32}$", $account)) {
			if (eregi ("(^[0-9]{1,6})([.])([0-9]{1,2}$)", $sum)) {
   				#ищем номер транзакции в БД
   				$datachk = $tinsp->Query("SELECT * FROM billing_payment WHERE payment_id = '".$txn_id."' and res = '0' and sender = '".SENDER."' LIMIT 1", false);
				$payment_ext_number=$datachk[0]["payment_id"];
   				#искомая транзация не равна текущей транзакции
				if ($payment_ext_number != $txn_id){
					#зачисляем 
					$pay=$tinsp->APIAddCash($GUID, $sum, "Оплата через терминал mobi.money", $txn_id, SENDER);
					if ($pay[0]=="0") {
						OutPut($command, NOWVER, SENDER, $id=NULL, 0, "OK", "", $txn_id, $account, $sum, DATENOW, DATENOW);	
					}else{
						OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Ошибка пополнения счета ".$pay[0]."", "", $txn_id, $account, $sum, DATENOW, DATENOW);	
					}						
   				}elseif ($payment_ext_number == $txn_id){
					#Такой платеж уже есть в базе
					OutPut($command, NOWVER, SENDER, $id=NULL, 0, "The transaction $txn_id has been completed ", "", $txn_id, $account, $sum, DATENOW, DATENOW);
   				}
			}else{
				OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Некорректная сумма", "", $txn_id, $account, $sum, DATENOW, DATENOW);
			}
  		}else{
  			OutPut($command, NOWVER, SENDER, $id=NULL, 4, "Некорректный формат договора", "", $txn_id, $account, $sum, DATENOW, DATENOW);
		}
  	}else{
		OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Некорректный формат транзакции", "", $txn_id, $account, $sum, DATENOW, DATENOW);
	}
}else{
	OutPut($command, NOWVER, SENDER, $id=NULL, 300, "Некорректные параметры.", "", $txn_id, $account, $sum, DATENOW, DATENOW);
}

#output
function OutPut($command, $ver, $sender, $id, $res, $result, $message_id, $payment_id, $account, $sum, $datetime, $datenow){
global $tinsp;
global $GUID;

$TEMPLATE["XML_CHECK"] = <<<EOF
<?xml version="1.0" encoding="UTF-8"?>
<response>
<osmp_txn_id>[OSMP_TXN_ID]</osmp_txn_id>
<result>[RESULT]</result>
<comment>[COMMENT]</comment>
</response>
EOF;

$TEMPLATE["XML_PAY"] = <<<EOF
<?xml version="1.0" encoding="UTF-8"?>
<response>
<osmp_txn_id>[OSMP_TXN_ID]</osmp_txn_id>
<prv_txn>[PRV_TXN]</prv_txn>
<sum>[SUM]</sum>
<result>[RESULT]</result>
<comment>[COMMENT]</comment>
</response>
EOF;

	if ($command == "check"){
		$replace = array("[RESULT]" => $res, "[OSMP_TXN_ID]" => $payment_id, "[COMMENT]" => iconv("windows-1251", "utf-8", $result)); 
		echo strtr($TEMPLATE["XML_CHECK"],$replace);
	}elseif ($command == "pay"){
		$replace = array("[RESULT]" => $res, "[OSMP_TXN_ID]" => $payment_id, "[COMMENT]" => iconv("windows-1251", "utf-8", $result), "[PRV_TXN]" => $id, "[SUM]" => $sum); 
		echo strtr($TEMPLATE["XML_PAY"],$replace);
	}else{
		$replace = array("[RESULT]" => $res, "[OSMP_TXN_ID]" => $payment_id, "[COMMENT]" => iconv("windows-1251", "utf-8", $result)); 
		echo strtr($TEMPLATE["XML_CHECK"],$replace);
	}	
	#logs
	$txt=implode(",", array_keys($_GET));
	fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	$result [$txt]\n");	
	
	#log biling
	if ($res!="0") {
		$tinsp->APICreatEvent("pay_error", $GUID, "", "", $res, "Sender: ".$sender.", error:".$res.", order = ".$account."");
	}
}
?>