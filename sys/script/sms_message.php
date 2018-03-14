<?php
/*OPTIONS*/
$CountSumm=30; // меньше какой суммы
$Provider="VN-TELECOM";
//$Provider="910-94-90";
$API="123"; //API TI
#$API_Pilot="XXXXXXXXXXXXYYYYYYYYYYYYZZZZZZZZXXXXXXXXXXXXYYYYYYYYYYYYZZZZZZZZ";
$API_Pilot="KVIA68Q6QY885PHA86M9OAR4MGYZE2RFDQL7IFNH1IRO8IC0IV2U4Q79M9NAH98Q";

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);


//$txt = "Uvazhaemii abonent! Teper' Vi mozhete oplachivat' nashi uslugi cherez sistemu Sberbank Online. Podrobnosti na saite www.vn-tel.ru";
//$txt = "Uvazhaemii abonent! V period s 13:00 po 15:00 budut provodit'sya profilakticheskie raboti. Vozmozhni pereboi v rabote.
//Prinosim izvineniya za neudobstva.";

$txt = "Вам доступны новые тарифные планы. Подробности на сайте vn-tel.ru";
//$txt = "Уважаем(ый/ая)! В связи с аварией у вышестоящего оператора, доступ в интернет временно ограничен. Время устранения - завтра до 13 ч. Вам будет сделан перерасчет. Приносим извинения за доставленные неудобства.";
//$txt = "Уважаемый абонент! VN-Телеком проводит акцию «Приведи друга» и получи 300 руб. Подробности на vn-tel.ru и по тел. (812) 910-94-90";
//$txt = "Компании VN-Телеком требуется монтажник ЛВС. Тел.: 910-94-90 доб. 102";
//$txt = "V svyazi s provodimimi rabotami u vishestoyaschego operatora v period s 18:00 po 00:00 chasov, vremenno snizhaetsya skorost' na vseh tarifnih planah.";
//$txt = "Uvazhaemii abonent! V period s 13:30 po 14:30 budut provodit'sya profilakticheskie raboti. Vozmozhni pereboi v rabote.";
//$txt = "Осенний ценопад! Новые тарифы! 10 Мбит за 290 руб., 30 Мбит за 390 руб. Подробности на сайте www.vash-net.ru и по тел. 910-94-90";
$i=0;
$UserBill=$tinsp->APIGetBillProp(3,"");
$GetTarif=$tinsp->APIGetTarif(2,"");
$ArrayTypeTarif=array();
foreach($GetTarif as $tar_k => $tar_v){
	$ArrayTypeTarif[$tar_v["tarif_id"]]=$tar_v["type_abon"];
}


$Json_Array_Send[]=array("id"=>12354343254, "from"=>$Provider, "to"=>"79115076114", "text"=>$txt);
$Json_Array_Send[]=array("id"=>2657258614425, "from"=>$Provider, "to"=>"79213903346", "text"=>$txt);

#$Json_Array_Send[]=array("id"=>1524552421254, "from"=>$Provider, "to"=>"79115076114", "text"=>$txt);
#$Json_Array_Send[]=array("id"=>223327414425, "from"=>$Provider, "to"=>"79213903346", "text"=>'Уважаемый Шефер Виталий Вячеславович! В связи с перебоями в предоставлении услуг, компания "VN-Телеком" возвращает Вам абонентскую плату за октябрь в размере 999.00 руб. Приносим свои извинения и спасибо за понимание.');

$Json_Array=array("apikey"=>$API_Pilot);
	foreach($UserBill as $UsersKey =>$Users){
		#нужные нам нас. пункты
		#смотрим всех у кого есть телефон; 
		if(strlen($Users["user_mobphone"])==10 && $Users["user_block"] == 0){
			
			//print_r($Users);
			#ищем абонентов с ежесуточным тарифным планом
			//if($ArrayTypeTarif[$Users["tarif_id"]]  == "1"){
			if($Users["user_nasp"]== "C5F3F47E-6144-416F-9243-50FCEDD55E9D" or $Users["user_nasp"]== "496CE1B3-7ED3-4880-B5B1-AD8E7D869FFE" or $Users["user_nasp"]== "6B6BACBF-9498-44B7-9F68-2DE7C3A72D64" or $Users["user_nasp"]== "C8A6DD48-EE07-4CA1-845E-8D62803814A5") {
				$i++;
				//$GetPayment = $tinsp->APIGetPayment($Users["user_dogovor"], "", "billing_inner", "2015-10-01", "2015-10-30", "1");
				//print_r($GetPayment);
				//if ($GetPayment[1] == '-1') {
				$Json_Array_Send[]=array("id"=>$sec.$usec, "from"=>$Provider, "to"=>"7".$Users["user_mobphone"]."", "text"=> $txt);
					
				//}else{
					//$tinsp->APIAddCash($Users["user_id"], str_replace("-","",$GetPayment[1][0]["sum"]), "Перерасчет за октябрь.", "false", "billing_pernet");
					//$Json_Array_Send[]=array("id"=>$sec.$usec, "from"=>$Provider, "to"=>"7".$Users["user_mobphone"]."", "text"=>'Уважаемый '.$Users["user_name"].'! В связи с перебоями в предоставлении услуг, компания "VN-Телеком" возвращает Вам абонентскую плату за октябрь в размере '.str_replace("-","",$GetPayment[1][0]["sum"]).' руб. Приносим свои извинения и спасибо за понимание.');					
					
					
			}
				
			#делаем выборку теле2
			//if(in_array($Users["user_mobphone"]{0}.$Users["user_mobphone"]{1}.$Users["user_mobphone"]{2}, array('900', '902', '904', '908', '950', '951', '952', '953'))){
				
				list($usec, $sec) = explode(" ", microtime());
				//print "7".$Users["user_mobphone"]."\n";																									
				//$Json_Array_Send[]=array("id"=>$sec.$usec, "from"=>$Provider, "to"=>"7".$Users["user_mobphone"]."", "text"=>$txt);
				//print $i."\n";
			//}
		}
	}

	print_r($Json_Array_Send);
$Json_Array["send"]=$Json_Array_Send;
//exit;
//print (json_encode($Json_Array));
function post_content ($url,$postdata) {
  $uagent = "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 1.1.4322)";
  $ch = curl_init( $url );
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($ch, CURLOPT_ENCODING, "");
  curl_setopt($ch, CURLOPT_USERAGENT, $uagent);
  curl_setopt($ch, CURLOPT_TIMEOUT, 120);
  curl_setopt($ch, CURLOPT_FAILONERROR, 1);
  curl_setopt($ch, CURLOPT_AUTOREFERER, 1);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $postdata);

  $content = curl_exec( $ch );
  curl_close( $ch );
  return $content;
}
/*Пишем в лог результат*/
$result_array=json_decode(post_content("http://smspilot.ru/api2.php", json_encode($Json_Array)), true);
print_r($result_array);
if (isset($result_array["send"])){
	foreach($result_array["send"] as $value){
		#log
		$date=date("d-m-Y");
		$rec="".date("H:i:s")."	error ".$value["error"]."	status ".$value["status"]."	to ".$value["to"]."
";
		$handle=fopen("/var/www/billing/sys/script/log/sms_".$date.".log",'a');
		fwrite($handle, $rec);
		fclose($handle);
	}
}	
?>