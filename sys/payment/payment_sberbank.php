<?php
/* sberbank online to simple-billing
(с)Bobrov Nikita
email: nick.bobrow@yandex.ru
*/

#OPTIONS
//ini_set('error_reporting', E_ALL);
define("SENDER", "sberbank_online");
define("DATENOW", date("Y-m-d\TH:i:s"));

#include config & classs
include("../../config.php");
include("../../class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);


#запишем _get параметры
fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	 [".http_build_query($_GET)."] [".http_build_query($_POST)."]\n");	

//поиск абонента
function getLogin($login)
	{
		Global $tinsp;
		$UserBill=$tinsp->APISearchAbonent2("!".$login);
		if(is_array($UserBill[1]) && count($UserBill[1])==1){
			return array("basic_account"=>$login, "balance"=>$UserBill[1][0]["balance"], "GUID"=>$UserBill[1][0]["user_id"]);
		}else{
			return false;
		}
	}
	
//проверка платежа в базе
function getCheckPay($payment_id, $sender)
	{
		Global $tinsp;
		#ищем номер транзакции в БД
		$getpay = $tinsp->Query("SELECT * FROM billing_payment WHERE payment_id = '".$payment_id."' and sender = '".$sender."' LIMIT 1", false);
		
		if ($tinsp->sql_errno==0){
			#платеж найден в базе
			if ($getpay[0]["payment_id"] == $payment_id)
			{
				if ($getpay[0]["res"] == '0') {
					return array ('0', 'Платеж уже зачислен', $getpay[0]["id"]);
				}elseif ($getpay[0]["res"] == '1'){
					return array ('7', 'Платеж уже отменен', $getpay[0]["id"]);
				}
			}
		}else{
			return array ('8', 'Состояние платежа не удается определить из-за ошибки БД '.$this->sql_errno.'');
		}
		return 'true';	
	}
	
//ответ на запрос
function getResponse($code, $message, $date=false, $authcode=false)
	{
		$xml ='<?xml version="1.0" encoding="utf-8"?>';
		$xml.='<response><code>'.$code.'</code>';
		if ($date!=false) { $xml.='<date>'.$date.'</date>'; }
		if ($authcode!=false) { $xml.='<authcode>'.$authcode.'</authcode>'; }
		$xml.='<message>'.$message.'</message></response>';
		header('Content-Length: '.strlen($xml));
		header("Content-type: text/xml");
		exit($xml);
	}
	
//проверка данных на соответствие	
function getCheckValid($get)
	{
		#содержание параметров
		if (@$get["action"]=="check" and count(array_diff(array("action", "number", "type", "amount"), array_keys($get)))!=0) return array('9', 'Неполный запрос для check');
		if (@$get["action"]=="payment" and count(array_diff(array("action", "number","type", "amount", "receipt", "date"), array_keys($get)))!=0) return array('9', 'Неполный запрос для payment');
		if (@$get["action"]=="status" and count(array_diff(array("action", "receipt"), array_keys($get)))!=0) return array('9', 'Неполный запрос для status');
		if (@$get["action"]=="cancel" and count(array_diff(array("action", "number", "type", "amount", "receipt", "date", "mes"), array_keys($get)))!=0) return array('9', 'Неполный запрос для cancel');
		#валидность параметров
		if (isset($get["amount"]) and !preg_match ("/(\d+)([,.]\d{1,2})/", $get["amount"])) return array('3', 'Неверный формат суммы');
		if (isset($get["number"]) and !preg_match ("/[0-9]{4,8}/", $get["number"])) return array('9', 'Неверный формат договора. Число от 4 до 8');
		if (isset($get["type"]) and !in_array($get["type"], array("0"))) return array('-2', 'Неверный type');
		if (isset($get["receipt"]) and !preg_match ("/[0-9]{1,15}/", $get["receipt"])) return array('4', 'Неверный формат receipt');
		
		
		return 'true';
	}

//логи	
function fileopen($str)
	{
		$file = fopen ("log/".SENDER."_".date("y-m-d").".txt","a");
		fputs ( $file, $str); fclose ($file);
	}
	
//принимаем реестры или платежи	
if (count($_FILES)==0) {
	$getCheckValid=getCheckValid($_GET);
	if ($getCheckValid=='true'){
		
		//check
		if (@$_GET["action"] == "check") {
			if (getLogin($_GET["number"]) != false){
				getResponse('0', 'Абонент найден');
			}else{
				getResponse('2', 'Абонент не найден');
			}
		//payment
		}elseif(@$_GET["action"] == "payment"){
			$find_acc=getLogin($_GET["number"]);
			if ($find_acc != false){
				$getCheckPay=getCheckPay($_GET["receipt"], SENDER);
				if ($getCheckPay=='true'){
					$pay=$tinsp->APIAddCash($find_acc["GUID"], $_GET["amount"], "Пополнение через Сбербанк Онлайн", $_GET["receipt"], SENDER);
					if ($pay[0]=="0") {
						getResponse('0', 'Платеж зачислен', DATENOW, $pay[1]);
					}else{
						
						getResponse('-3', 'Ошибка зачисления платежа');
					}					
				}else{
					getResponse($getCheckPay[0], $getCheckPay[1], false, $getCheckPay[2]);
				}
			}else{
				getResponse('2', 'Абонент не найден');
			}
			
		//status	
		}elseif(@$_GET["action"] == "status"){
			$getCheckPay=getCheckPay($_GET["receipt"], SENDER);
			if ($getCheckPay=='true'){
				getResponse('6', 'Платеж не найден');	
			}else{
				getResponse($getCheckPay[0], $getCheckPay[1], false, @$getCheckPay[2]);
			}
			
		//cancel	
		}elseif(@$_GET["action"] == "cancel"){
			$find_acc=getLogin($_GET["number"]);
			if ($find_acc != false){
				$getCheckPay=getCheckPay($_GET["receipt"], SENDER);
				#платеж найден
				if ($getCheckPay!=='true'){
					#платеж еще не отменен
					if ($getCheckPay[0]=='0') {
						#списываем средства
						$pay=$tinsp->APIAddCash($find_acc["GUID"], "-".$_GET["amount"], "Отмена платежа СберБанком. Номер платежа ".$_GET["receipt"]."", "false", "billing_inner");
						if ($pay[0]=="0") {
							#отменяем платеж в биллинге
							$tinsp -> Query("UPDATE billing_payment SET res = '1' WHERE payment_id = '".$_GET["receipt"]."' AND sender = '".SENDER."'");
							getResponse('0', 'Платеж отменен', false, $pay[1]);
						}else{
							getResponse('-3', 'Ошибка списание средств');
						}	
					}else{
						getResponse('7', 'Платеж уже отменен', false, $getCheckPay[2]);
					}			
				}else{
					getResponse('6', 'Платеж не найден');	
				}
			}else{
				getResponse('2', 'Абонент не найден');
			}
		}else{
			getResponse('1', 'Отсутствует либо некорректный action');
		}
		
	}else{
		getResponse ($getCheckValid[0], $getCheckValid[1]);
	}	
	
}else{
	
	
}





?>