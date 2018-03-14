<?php

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="cp1251"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
#connect class API::NETWORK_MONITOR;
$tinsp->include_network_monitor();

$tinsp->NET_MONIT->SetSnmp ('192.168.88.157', 'public');
$tinsp->NET_MONIT->snmpwalk('.1.3.6.1.2.1.2.2.1.16.25');

$res1=$tinsp->NET_MONIT->snmp_reply;
# IF-MIB::ifInOctets.49 = Counter32: 226616719
if(preg_match("/Octets.(\d+) = Counter32: (.*)/", $res1[0], $matches1)){

}

$time1=time();
$oct1=$matches1[2]*8;
echo "time $time1, tx $oct1";
echo "<br>";

sleep(1);

$tinsp->NET_MONIT->snmpwalk('.1.3.6.1.2.1.2.2.1.16.25');
$res2=$tinsp->NET_MONIT->snmp_reply;
if(preg_match("/Octets.(\d+) = Counter32: (.*)/", $res2[0], $matches2)){

}

$time2=time();
$oct2=$matches2[2]*8;
echo "time $time2, tx $oct2";
echo '<br>';
echo (($oct2-$oct1)/($time2-$time1))/1048576;


$test1=array(1=>0,2=>0,3=>0,4=>0,5=>0,6=>0);
$test2=array(1=>0,2=>0,3=>1,4=>0,5=>1,6=>0);

print_r(array_diff($test2,$test1));


/*
$CreditQuery=$tinsp->Query("SELECT * 
FROM billing_payment
WHERE MONTH( DATE ) =  '09'
AND YEAR( DATE ) =  '2013'
AND DAY( DATE ) =  '01'
AND sender =  'billing_inner'
AND comment LIKE '%Переход на новый%'
" , false);

		$i=1;
		foreach($CreditQuery as $PaymentTableVal){

			//echo $PaymentTableVal["user_id"]."\n";
			#ищем все снятия абонентской платы по абоненту за сегодня__halt_compiler
			$q=$tinsp->Query("SELECT * FROM billing_payment WHERE MONTH( DATE ) =  '09' AND YEAR( DATE ) =  '2013' AND DAY( DATE ) =  '01' AND sender =  'billing_inner' AND user_id = '".$PaymentTableVal["user_id"]."'", false);
			#абоненты по которым снялась 2-а раза абонентская плата__halt_compiler
			if (count($q)==1){
				#проходимся по списаниям и находим списание с нужной нам абонентской платой 
				foreach($q as $v){
					if($v["sum"] < 0 && $v["sum"] > -100){
						#зачисляем деньги обратно и удаляем их базы ошибочное списниае 
						#$tinsp->Query("UPDATE billing_users SET balance=balance+".str_replace("-", "", $v["sum"])." WHERE user_id = '".$v["user_id"]."'", true);
						#$qw=$tinsp->Query("DELETE FROM billing_payment WHERE id = '".$v["id"]."' ", true);
	
						
					}
				
				}
			}
			#$tinsp->APIAddCash($PaymentTableVal["user_id"], $PaymentTableVal["sum"], "Перерасчет за обещанный платеж.", "false", "billing_inner");
		}
	
*/
?>

