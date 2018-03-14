<?php
/* mobi.money to simple-billing
(с)Bobrov Nikita
icq 332277202 */

#OPTIONS
define("SENDER", "unitpay");
define("SEKRET_KEY", "b9cb631808158dc395da6afdddcc183c");
define("DATENOW", date("Y-m-d H:i:s"));

#include config & classs
include("../../config.php");
include("../../class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

//header("Content-type: application/json");

function getLogin($login)
	{
		Global $tinsp;
		$UserBill=$tinsp->APISearchAbonent("!".$login, "", "",  "",  "",  "");
		if(is_array($UserBill[1]) && count($UserBill[1])==1){
			$ArrayResult=array("basic_account"=>$login, "balance"=>$UserBill[1][0]["balance"], "GUID"=>$UserBill[1][0]["user_id"]);
			return $ArrayResult;
		}else{
			return array("basic_account"=>"", "balance"=>"", "GUID"=>"");
		}
	}

 function getResponseSuccess($message)
    {
        echo json_encode(array(
            "jsonrpc" => "2.0",
            "result" => array(
                "message" => $message
            ),
            'id' => 1,
        ));
    }

function getResponseError($message)
    {
        echo json_encode(array(
            "jsonrpc" => "2.0",
            "error" => array(
                "code" => -32000,
                "message" => $message
            ),
            'id' => 1
        ));
    }

function getMd5Sign($params, $secretKey)
    {
		if (!empty($params)){
			ksort($params);
			unset($params['sign']);
			return md5(join(null, $params).$secretKey);
		}
    }
	
function fileopen($str)
	{
		$file = fopen ("log/".SENDER."_".date("y-m-d").".txt","a");
		fputs ( $file, $str); fclose ($file);
	}
	
$txt=implode(",", array_keys($_GET));
fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	 [$txt]\n");	

#PAY OR CHECK
$command = $_GET["method"];
$params = $_GET['params'];

if ($params['sign'] != getMd5Sign($params, SEKRET_KEY))
{
	getResponseError('Incorrect digital signature'); exit();
}

if ($command == "check"){
   $txn_id=$params['unitpayId'];
   $account=trim($params['account']);
   $sum=$params['sum'];
   
   #Запрет на проведение платежей
	if (eregi ("(^[0-9]{1,20}$)", $txn_id)) {
		#латиница, цифры и больше от 4 до 8 чисел
		if (eregi ("^[0-9]{4,8}$", $account)) {
			if (eregi ("^(\d+)([,.]\d{0,2})?$", $sum)) {
			
				#Ищем аккаунт в базе ТИ
				$data = getLogin($account);
				$basic_account=$data["basic_account"];
				$balance=$data["balance"];
				if ($basic_account == ""){
					getResponseError('Account not found');
				}elseif ($basic_account != ""){
					getResponseSuccess('Account found');
				}
			}else{
				getResponseError('Incorrect sum format '.$sum.'');
   			}
		}else{
			getResponseError('Incorrect account format');
   		}
	}else{
		getResponseError('Incorrect transaction format');
	}
	
}elseif ($command == "pay"){
	$txn_id=$params['unitpayId'];
	$account=trim($params['account']);
	$sum=$params['sum'];
	
	$data = getLogin($account);
	$GUID=$data["GUID"];
	if (eregi ("(^[0-9]{1,20}$)", $txn_id)) {
		#проверяем аккаунт
		if (eregi ("^[0-9]{4,8}$", $account)) {
			if (eregi ("^(\d+)([,.]\d{0,2})?$", $sum)) {
   				#ищем номер транзакции в БД
   				$datachk = $tinsp->Query("SELECT * FROM billing_payment WHERE payment_id = '".$txn_id."' and res = '0' and sender = '".SENDER."' LIMIT 1", false);
				$payment_ext_number=$datachk[0]["payment_id"];
   				#искомая транзация не равна текущей транзакции
				if ($payment_ext_number != $txn_id){
					#зачисляем 
					$pay=$tinsp->APIAddCash($GUID, $sum, "Пополнение через unitpay (".$params["paymentType"].")", $txn_id, SENDER);
					if ($pay[0]=="0") {
						getResponseSuccess('PAY is successful');	
					}else{
						getResponseError('Payment error '.$pay[0].'');	
					}						
   				}elseif ($payment_ext_number == $txn_id){
					#Такой платеж уже есть в базе
					getResponseSuccess('Payment has already been paid');
   				}
			}else{
				getResponseError('Incorrect sum format '.$sum.'');
			}
  		}else{
  			getResponseError('Incorrect account format');
		}
  	}else{
		getResponseError('Incorrect transaction format');
	}
}else{
	getResponseError('Incorrect method');
}



?>