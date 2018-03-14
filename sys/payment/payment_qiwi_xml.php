<?php
/*
==================================
������ ��� �������� ������� ������ ����� ����� API XML QIWI.
==================================
*/
define("SENDER", "QiwiWallet");
define("DATENOW", date("Y-m-d H:i:s"));
define("DIR", dirname(__FILE__));


#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");


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
	$file = fopen (DIR."/log/".SENDER."_".date("y-m-d").".txt","a");
	fputs ( $file, $str); fclose ($file);
};

function request($url, $body) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-Type' => 'text/xml; encoding=utf-8'));
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    $result = curl_exec($ch);
    curl_close($ch);
    return $result;
}

function check_response($xml) {
    $rc = $xml->{'result-code'};
    $fatality = $rc['fatal'];
    if ($rc != 0) {
		return FALSE;
    } else {
		return TRUE;
    }
}


$QiwiTridDogovorArray=array();
#���� ������������ ��� � ����
$sqlchk = $tinsp->Query("SELECT * FROM billing_payment_temp_qiwi where status = '' or ( status <= 52 and status >= 0) ", false);
if (!is_array($sqlchk))
{		
	#��� ������ �� ��������
}else
{
	#��������� ������
	$x='<request>';
	$x.='<protocol-version>4.00</protocol-version>';
	$x.='<request-type>33</request-type>';
	$x.='<extra name="password">188523vntelecom</extra>';
	$x.='<terminal-id>4003</terminal-id>';
	$x.='<bills-list>';
	foreach($sqlchk as $val){ 
		$x.='<bill txn-id="'.$val["tr_id"].'"></bill>'; 
		$QiwiTridDogovorArray[$val["tr_id"]] = $val["id_user"];
	}
	$x.='</bills-list>';
	$x.='</request>';

	#���������� ������ �� �������� ������� ������
	$r=request("http://ishop.qiwi.ru/xml", $x);
	$xml = simplexml_load_string('<?xml version="1.0" encoding="utf-8"?>'.$r);
	$check_response = check_response($xml);

	if($check_response == TRUE)
	{
		foreach ($xml->{'bills-list'}->children() as $bill) {
			#������� ������
			$tinsp->Query("UPDATE billing_payment_temp_qiwi SET status='".(string)$bill['status']."' where tr_id = '".(string)$bill['id']."'");
			
			#���� ���� ������� (60) -> ��������� ������ �� ����
			if((string)$bill['status'] == '60')
			{
				$guid=getLogin($QiwiTridDogovorArray[(string)$bill['id']]);			
				#������
				$pay=$tinsp->APIAddCash($guid["GUID"], (string)$bill['sum'], "������ ����� qiwi", (string)$bill['id'], SENDER);
				if ($pay[0]=="0") {
					fileopen("".DATENOW."	QiwiWallet �������: ������ ".(string)$bill['status'].", ���� ".(string)$bill['id'].", ����� ".(string)$bill['sum'].", ������� ".$QiwiTridDogovorArray[(string)$bill['id']]." \n");
				}else{
					fileopen("".DATENOW."	QiwiWallet ������: (".$pay[0].") ������ ".(string)$bill['status'].", ���� ".(string)$bill['id'].", ����� ".(string)$bill['sum'].", ������� ".$QiwiTridDogovorArray[(string)$bill['id']]." \n");
				}		
			}
			#������� �� ������� ����� �� ������� �������� ������
			unset($QiwiTridDogovorArray[(string)$bill['id']]);
		};
		#������� ��������� �����
		foreach ($QiwiTridDogovorArray as $k => $v){
			$tinsp->Query("DELETE FROM billing_payment_temp_qiwi where tr_id = '".$k."'");
			fileopen("".DATENOW."	QiwiWallet ������� ����:  ".$k." \n");
		}
	}else
	{
		#������ ��������� ������� ������
		fileopen("".DATENOW."	������ ��������� ������� ������ \n");
	}
}

?> 