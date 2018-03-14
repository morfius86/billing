<?php
/*OPTIONS*/
$CountDay=1; 
#$API_Pilot="XXXXXXXXXXXXYYYYYYYYYYYYZZZZZZZZXXXXXXXXXXXXYYYYYYYYYYYYZZZZZZZZ";

#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);

//print_r($tinsp->APISMSSend('[{"to":"9115076114","text":"qwe"},{"to":"9115076114","text":"hello, world"},{"to":"9115076114","text":"hello, world"},{"to":"9115076114","text":"hello, world"},{"to":"9115076114","text":"hello, world"}]'));
$json_query_array=array();
$UserBill=$tinsp->APIGetBillProp(3,"");

foreach($UserBill as $UsersKey =>$Users){
	$tarif=$tinsp->APIGetTarif("2", $Users["tarif_id"]);
	
	#абонентская плата
	$abon_money=($tarif[0]["type_abon"]=="3" ? $tarif[0]["abon_cash"]/date("t") : $tarif[0]["abon_cash"]);

		#рассылка для тарифа разрешена & в текущий день рассылка разрешена
	if ($tarif[0]["tarif_sms_alert"]==1 && array_search(date("j"), explode(",", $tarif[0]["tarif_sms_sendday"]))!==FALSE){
		#смотрим всех у кого есть телефон;баланс >=0; рабочее состояние; 
		if(strlen($Users["user_mobphone"])==10 && $Users["balance"]>=0 && $Users["user_block"]==0){
			#баланс 
			if((float)$Users["balance"] < ((float)$abon_money*$CountDay)){
				$json_query_array[]=array("to"=>$Users["user_mobphone"], "text"=>"Уважаемый абонент! Вы у порога отключения. Ваш баланс ".number_format((float)$Users["balance"], 2)." руб.");
			}
		}	
	}else{
		#для текущего тарифа у абонента рассылка не разрешена
	}
}


#print $json_query;
$Query=$tinsp->APISMSSend(json_encode($json_query_array));
print_r($Query);

if ($Query[0]=="0"){
	foreach($Query[1] as $value){
		#log
		$date=date("d-m-Y");
		$rec="".date("H:i:s")."	error ".$value["error"]."	status ".$value["status"]."	to ".$value["to"]."
";
		$handle=fopen("/var/www/billing/sys/script/log/sms_".$date.".log",'a');
		fwrite($handle, $rec);
		fclose($handle);
	}
}else{

}	
?>