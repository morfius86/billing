<?php
/*
1. argv: abon, attr: month - once per month, day - once per day)
2. argv: check, attr: 

*/


#include config & classs
include("/var/www/billing/config.php");
include("/var/www/billing/class/class.api.php");

#connect API billing class
$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);


if (@$argv[1]=="abon"){
	if($argv[2]=="month"){
		#абон. плата за мес¤ц
		TakeAbonMoney("1", date("Y-m-d", mktime(0, 0, 0, date("m")+1, 1, date("Y"))));
    }elseif($argv[2]=="day"){
		#абон. плата за день
		TakeAbonMoney("2", date("Y-m-d", mktime(0, 0, 0, date("m"), date("d")+1, date("Y"))));
		TakeAbonMoney("3", date("Y-m-d", mktime(0, 0, 0, date("m"), date("d")+1, date("Y"))));
	}
}elseif(@$argv[1]=="check"){
	CheckingBlock();
}elseif(@$argv[1]=="credit"){
    CheckCredit();
}elseif(@$argv[1]=="tarif_change"){
	if($argv[2]=="month"){
		#смена в начале нового мес¤ца
		CheckTariffRequest("0");
    }elseif($argv[2]=="day"){
		#смена в начале нового дн¤
		CheckTariffRequest("1");
	}elseif($argv[2]=="period"){
		#смена по окончанию отчетного периода абонента
		CheckTariffRequest("3");
	}
}else{
	echo "not get parametrs";
}


function CheckingBlock(){
Global $tinsp;
$ArrayAbonent=$tinsp->APIGetBillProp(3,"");
	if(is_array($ArrayAbonent)){
		foreach($ArrayAbonent as $ArrayAbonentKey => $ArrayAbonentVal){
			#select tarif
			$AbonTarif=$tinsp->APIGetTarif(2,$ArrayAbonentVal["tarif_id"]);
			#echo $ArrayAbonentVal["user_id"]."--".$ArrayAbonentVal["user_block"]."--".$AbonTarif[0]["tarif_block"]."\n";
				#≈сли абонент разблокирован и баланс < 0  и у тарифа стоит "блокировать" -> заблокируем
			if ($ArrayAbonentVal["user_block"]==0 && $ArrayAbonentVal["balance"]<0 && $AbonTarif[0]["tarif_block"]=="0"){
				$tinsp->Query("UPDATE billing_users SET user_block = '1', user_dropnas = '1' WHERE user_id = '".$ArrayAbonentVal["user_id"]."'");
				$tinsp->APICreatEvent("billing", $ArrayAbonentVal["user_id"], $ArrayAbonentVal["user_name"], "", "1", "Block, balance<0");
			#≈сли абонент заблокирован и баланс > 0  и у тарифа стоит "блокировать" -> разблокируем
			}elseif($ArrayAbonentVal["user_block"]==1 && $ArrayAbonentVal["balance"]>=0 && $AbonTarif[0]["tarif_block"]=="0"){
				$tinsp->Query("UPDATE billing_users SET user_block = '0', user_dropnas = '1' WHERE user_id = '".$ArrayAbonentVal["user_id"]."'");
				$tinsp->APICreatEvent("billing", $ArrayAbonentVal["user_id"], $ArrayAbonentVal["user_name"], "", "0", "Unlock, balance>=0");
			#≈сли абонент заблокирован и у тарифа стоит "не блокировать" -> разблокируем
			}elseif($ArrayAbonentVal["user_block"]==1 && $AbonTarif[0]["tarif_block"]=="1"){
				$tinsp->Query("UPDATE billing_users SET user_block = '0', user_dropnas = '1' WHERE user_id = '".$ArrayAbonentVal["user_id"]."'");
				$tinsp->APICreatEvent("billing", $ArrayAbonentVal["user_id"], $ArrayAbonentVal["user_name"], "", "0", "Unlock, tarif_block = 1");
			}	
		}
	}
}

function TakeAbonMoney($TypeAbon, $NewPeriod){
Global $tinsp;
$ArrayAbonent=$tinsp->APIGetBillProp(3,"");
	if(is_array($ArrayAbonent)){
		foreach($ArrayAbonent as $ArrayAbonentKey => $ArrayAbonentVal){	
			#select tarif
			$AbonTarif=$tinsp->APIGetTarif(2,$ArrayAbonentVal["tarif_id"]);
			if($AbonTarif[0]["type_abon"]==$TypeAbon){
			
			#снимаем абонентскую плату
				#рассчитываем абонентскую плату в зависимости от типа тарифа
				if($TypeAbon=="3"){
					$MoneyAbon=$AbonTarif[0]["abon_cash"]/date("t");
				}else{
					$MoneyAbon=$AbonTarif[0]["abon_cash"];
				}
				#сн¤тие в минус
				if($AbonTarif[0]["tarif_block"]=="0" && $ArrayAbonentVal["balance"]>=0 && $ArrayAbonentVal["user_block"]!="2"){
					$tinsp->APIAddCash($ArrayAbonentVal["user_id"], "-".$MoneyAbon, "Абонентская плата", "false", "billing_inner");
					#укажем конец отчетного периода 
					$tinsp->Query("UPDATE billing_users SET tarif_datelimit = '".$NewPeriod."' WHERE user_id = '".$ArrayAbonentVal["user_id"]."'");
					
				}elseif($AbonTarif[0]["tarif_block"]=="1" && $ArrayAbonentVal["user_block"]!="2"){
					$tinsp->APIAddCash($ArrayAbonentVal["user_id"], "-".$MoneyAbon, "Абонентская плата", "false", "billing_inner");
					#укажем конец отчетного периода 
					$tinsp->Query("UPDATE billing_users SET tarif_datelimit = '".$NewPeriod."' WHERE user_id = '".$ArrayAbonentVal["user_id"]."'");
				}
			}	
		}
	}
}

function CheckCredit(){
Global $tinsp;
#смотрим активные просроченные кредиты
$query=$tinsp->APIGetCreditUser("", "0", "", date("Y-m-d H:i:s"));
	if($query[0] == "0"){
		#проходимс¤ по кредитам, которые должны сн¤ть
		foreach($query[1] as $credit){
			#снимаем со счет
			$take_money=$tinsp->APIAddCash($credit["user_id"], "-".$credit["sum"], "Снято за услугу обещанный платеж", "false", "billing_credit");
			#обновл¤ем статус
			$tinsp -> Query("UPDATE billing_users_credit SET credit_state = '1' WHERE id = '".$credit["id"]."' ");
			#добавл¤ем в лог
			//$tinsp -> APICreatEvent("billing", $credit["user_id"], "", "", "0", "—н¤то за услугу обещанный платеж. —умма -".$credit["sum"]."");				
		}
	}else{
	
	}
}

function CheckTariffRequest($type){
Global $tinsp;
$ArrayRequestTariff=$tinsp->APIGetRequestTariff("", array("0", "-1"), "", "");
	if ($ArrayRequestTariff[0]=="0"){
		foreach($ArrayRequestTariff[1] as $RequestTariff){
			$Abonent=$tinsp->APIGetBillProp(3, $RequestTariff["user_id"]);
			$FutureTariff=$tinsp->APIGetTarif(2,$RequestTariff["tarif_future"]);
			
			if (is_array($FutureTariff[0]) && is_array($Abonent[0])){
				#ищем за¤вки согласно переданному типу (в начале дн¤/мес¤ца)
				if ($RequestTariff["change_type"]==$type){
					#бесплатный переход либо достаточно средств дл¤ перехода
					//if($Abonent[0]["balance"]>=$RequestTariff["sum"] || $RequestTariff["sum"]=='0.00'){
					if($Abonent[0]["balance"]>=$RequestTariff["sum"]){
						#снимаем со счет
						$take_money=$tinsp->APIAddCash($RequestTariff["user_id"], "-".$RequestTariff["sum"], "Переход на новый тарифный план ".$FutureTariff[0]["tarif_desc"]."", "false", "billing_inner");
						$tinsp->Query("UPDATE billing_users SET tarif_id = '".$RequestTariff["tarif_future"]."', user_dropnas = '1' WHERE user_id = '".$RequestTariff["user_id"]."'");
						$tinsp->APISetRequestTariff($RequestTariff["user_id"], "1");
					}else{
						#баланса не достаточно для перехода
						$tinsp->APISetRequestTariff($RequestTariff["user_id"], "-1");
					}
				}
				
			}else{
				#не смогли получить данные по тарифу абонента или абоненту, пометим отмененным
				$tinsp->APISetRequestTariff($RequestTariff["user_id"], "2");
			}
		}
	}
	
}


?>