<?php
/* 
liqpay to simple-billing
(с)Bobrov Nikita
icq 332277202 
*/

#OPTIONS
define("NOW_VER", "Kassira.net");
define("SENDER", "LiqPay");
define("DATENOW", date("Y-m-d H:i:s"));
define("merchant_id", "i9883957495");
define("sign", "TAF0cJ982GtC4njDJayd9IoEI32WTQ4Hp9MvENf");

#include config & classs
include("../../config.php");
include("../../class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

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


#Если переданы запросы
$xml = base64_decode($_POST['operation_xml']);
$signature = base64_encode(sha1(sign.$xml.sign, 1));
$orderID = $_POST['order_id'];
$log = '';


if($_POST['signature']==$signature) {
	$xml_arr = simplexml_load_string($xml, 'SimpleXMLElement', LIBXML_NOCDATA);
	#Порядковый номер сообщения по БД СМР
	$Message_id="-";
	#Версия протокола
	$Ver=$xml_arr->version;
	#номер платежа
	$Payment_id=$xml_arr->transaction_id;
	#идентификатор плательщика
	$Order=$xml_arr->order_id;
	#сумма
	$Sum=$xml_arr->amount;
	#время
	$datetime=DATENOW;
	#Берем GUID и прочее
	$data_gost = getLogin($Order);
	#успешный перевод
	if($xml_arr->status=='success'){
		#зачисляем 
		$pay=$tinsp->APIAddCash((string)$data_gost["GUID"], $Sum, "Оплата через liqpay.com", $Payment_id, SENDER);
		if ($pay[0]=="0") {
			OutPut(NOW_VER, SENDER, $id=NULL, 0, "OK", $Message_id, $Payment_id, $Order, $Sum, $datetime, DATENOW);
		}else{
			OutPut(NOW_VER, SENDER, $id=NULL, -100, "failure", $Message_id, $Payment_id, $Order, $Sum, $datetime, DATENOW);
		}
	#на проверке
	}elseif($xml_arr->status=='wait_secure') {
		OutPut(NOW_VER, SENDER, $id=NULL, -100, "wait_secure", $Message_id, $Payment_id, $Order, $Sum, $datetime, DATENOW);
	#ошибка
	}elseif($xml_arr->status=='failure') {
		OutPut(NOW_VER, SENDER, $id=NULL, -100, "failure", $Message_id, $Payment_id, $Order, $Sum, $datetime, DATENOW);	
	}
}

#output
function OutPut($ver, $sender, $id, $res, $result, $message_id, $payment_id, $order, $sum, $datetime, $datenow){	
global $tinsp;
global $data_gost;
	$txt=implode(",", array_keys($_POST));
	#записать лог
	fileopen("$datenow	$result [$txt]\n");
	
	#log biling
	if ($res!="0") {
		$tinsp->APICreatEvent("pay_error", (string)$data_gost["GUID"], "", "", $res, "Sender: ".$sender.", error:".$result);
	}
}

?>