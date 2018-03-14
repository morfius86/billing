<?php
/*
==================================
Ч†ЦїЊУ ?О€ ЉђУЭЂЉДїЧО?Нї€ ЭЊОЉУЯ QIWI ЊЭЧЦ??ЧУђЭЮ ЊЦЭУЭ†ЭОЉ SOAP.
==================================
*/
define("SENDER", "QiwiWallet");
define("DATENOW", date("Y-m-d H:i:s"));

#include config & classs
include("../../config.php");
include("../../class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

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

//fileopen("test\n");

#SOAP XML
$soap_xml='<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:ns1="http://client.ishop.mw.ru/">
		<SOAP-ENV:Body>
			<ns1:updateBillResponse>
				<updateBillResult>status</updateBillResult>
			</ns1:updateBillResponse>
		</SOAP-ENV:Body>
	</SOAP-ENV:Envelope>';
	
$i = file_get_contents('php://input');

$l = array('/<login>(.*)?<\/login>/', '/<password>(.*)?<\/password>/');
$s = array('/<txn>(.*)?<\/txn>/', '/<status>(.*)?<\/status>/');

preg_match($l[0], $i, $m1);
preg_match($l[1], $i, $m2);

preg_match($s[0], $i, $m3);
preg_match($s[1], $i, $m4);

		#сравнение наш парол§ с полученным,если не равны код "150"
		#ищем выставленный чек в базе
   		$sqlchk = mysql_query("SELECT * FROM billing_payment_temp_qiwi where tr_id = '".$m3[1]."'") or die("".mysql_errno()." ".mysql_error()." " . fileopen("".DATENOW."  ".mysql_errno()." ".mysql_error()."\n"));
  		$datachk = mysql_fetch_array($sqlchk, MYSQL_ASSOC);
   		$dogovor=$datachk["id_user"];
		$sum=$datachk["summ"];
		$txn_id=$datachk["tr_id"];
		if (!is_array($datachk)){		
			$resultCode=300;
			fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet Нет чека $m3[1]\n");
		}else{
			if ($m4[1] == 60){
				$Query=mysql_query("UPDATE billing_payment_temp_qiwi SET status='".$m4[1]."' where tr_id = '".$m3[1]."'");
				if(mysql_errno()==0){
					$guid=getLogin($dogovor);			
					#оплата
					$pay=$tinsp->APIAddCash($guid["GUID"], $sum, "ќплата через qiwi", $txn_id, SENDER);
					if ($pay[0]=="0") {
						fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet статус $m4[1], счет $m3[1], сумма $sum, договор $dogovor\n");
						$resultCode=0;
					}else{
						fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet Эшибка (".$pay[0].") зачислени§ платежа в базу биллинга $m3[1]\n");
						$resultCode=-1;
					}										
				}else{					
					fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet Эшибка обновлени§ статуса в billing_payment_temp_qiwi $m3[1]\n");
					$resultCode=-1;
				}	
			}elseif ($m4[1] > 100) {
				mysql_query("UPDATE billing_payment_temp_qiwi SET status='".$m4[1]."' where tr_id = '".$m3[1]."'");
				fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet статус $m4[1], счет $m3[1], сумма $sum, договор $dogovor\n");
				$resultCode=0;
			}elseif ($m4[1] >= 50 && $m4[1] < 60) {
				mysql_query("UPDATE billing_payment_temp_qiwi SET status='".$m4[1]."' where tr_id = '".$m3[1]."'");
				fileopen("".DATENOW."	".$_SERVER['REMOTE_ADDR']."	QiwiWallet статус $m4[1], счет $m3[1], сумма $sum, договор $dogovor\n");
				$resultCode=0;
			}else{
				$resultCode=-100;
			}
		}

    $text = str_replace('status',$resultCode, $soap_xml);
    header('content-type: text/xml; charset=UTF-8');
    echo $text;
?> 