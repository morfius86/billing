<?php
session_start();
//
// Query Json API Parser
// MR-Billing
// (c) Bobrov N.I. icq 332277202
//

 #include bill class
include("../class/class.api.php");
 #include config
include("../config.php");

$tinsp = New API;
$tinsp->encode="utf8"; 
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
$tinsp->config = $config;

function JsonReply($error, $message, $array=array()){
	$Array=array(
		"error" 	=> $error,
		"message"	=> $message,
		"array"		=> $array
	);
	return json_encode($Array);
}
function post_get_switch(){
	if (isset($_GET["FUNCTION"])){ return $_GET["FUNCTION"];}
	elseif (isset($_POST["FUNCTION"])){ return $_POST["FUNCTION"];}
	else{ return "";}
}
switch (post_get_switch()){
	/*УПРАВЛЕНИЕ АБОНЕНТАМИ*/
	case "APICreatUser":{
		$Res=$tinsp->APICreatUser($_POST["QUERY"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APICreatUser2":{
		$Res=$tinsp->APICreatUser2($_POST["QUERY"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APISetUser":{
		$Res=$tinsp->APISetUser($_POST);
		echo JsonReply($Res, "");
		break;
	}
	case "APIDelUser":{
		$Res=$tinsp->APIDelUser($_POST["QUERY"][0]["obj"], $_POST["QUERY"][1]["delete_payment"], $_POST["QUERY"][2]["delete_log"], $_POST["QUERY"][3]["delete_abonent"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APISearchAbonent":{
		$Res=$tinsp->APISearchAbonent($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APISearchAbonent2":{
		$Res=$tinsp->APISearchAbonent2($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"], $_POST["QUERY"][6]["value"], $_POST["QUERY"][7]["value"], $_POST["QUERY"][8]["value"]);
		echo JsonReply($Res, "");
		break;
	}
	case "BillingGetAjaxUser":{
		$Res=$tinsp->BillingGetAjaxUser($_GET["q"]);
		echo $Res;
		break;
    }
	case "APISetGroupUserAttr":{
		parse_str($_POST["QUERY"]["selected_userid"], $parse_array);
		$Res=$tinsp->APISetGroupUserAttr($_POST["QUERY"]["attr"], $_POST["QUERY"]["attr_value"], $parse_array["check_user"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
		//return print_r($parse_array["check_user"]);
    }
	/*УПРАВЛЕНИЕ ПЛАТЕЖАМИ*/
	case "APIAddCash":{
		$Res=$tinsp->APIAddCash($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],  $_POST["QUERY"][5]["value"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APIGetPayment":{
		$Res=$tinsp->APIGetPayment($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"]);
		echo JsonReply($Res, "");
		break;
	}
	case "APITransCashBetweenAbon":{
		$Res=$tinsp->APITransCashBetweenAbon($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
	}
	/*----*/
    case "APISetGroup":{
		$Res=$tinsp->APISetGroup($_POST["QUERY"]);
        echo JsonReply($Res[0], $Res[1]);
        break;
    }
    case "APICreatGroup":{
		$Res=$tinsp->APICreatGroup($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
        break;
    }
	case "APIDelBillProp":{
		$Res=$tinsp->APIDelBillProp($_POST["QUERY"]);
        echo JsonReply($Res[0], $Res[1]);
        break;
    }
	case "APIGetBillProp":{
		$Res=$tinsp->APIGetBillProp($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	/*ГРУППЫ ТАРИФОВ*/

	case "APICreatGroupTarif":{
		parse_str($_POST["QUERY"], $parse_array);
		$Res=$tinsp->APICreatGroupTarif($parse_array["grtarif_name"], $parse_array["grtarif_desc"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APIDelGroupTarif":{
		$Res=$tinsp->APIDelGroupTarif($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APISetGroupTarifArray":{
		$Res=$tinsp->APISetGroupTarifArray($_POST);
		echo JsonReply($Res[0], $Res[1]);
		break;
	}
	/*ТАРИФЫ*/
	case "APICreatTarif":{
		parse_str($_POST["QUERY"], $parse_array);
		$Res=$tinsp->APICreatTarif($parse_array["tarif_name"], $parse_array["tarif_desc"], $parse_array["grtarif_id"], $parse_array["type"], $parse_array["type_abon"], $parse_array["abon_cash"], $parse_array["tarif_block"], $parse_array["valute"], $parse_array["tarif_speed_in"], $parse_array["tarif_speed_out"], $parse_array["tarif_prizn"], $parse_array["tarif_ves"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APISetTarif":{
		$Res=$tinsp->APISetTarif($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"], $_POST["QUERY"][6]["value"], $_POST["QUERY"][7]["value"], $_POST["QUERY"][8]["value"], $_POST["QUERY"][9]["value"], $_POST["QUERY"][10]["value"], $_POST["QUERY"][11]["value"], $_POST["QUERY"][12]["value"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APISetTarifArray":{
		$Res=$tinsp->APISetTarifArray($_POST);
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetTarif":{
		$Res=$tinsp->APIGetTarif($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelTarif":{
		$Res=$tinsp->APIDelTarif($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APISetTarifNasp":{
		parse_str($_POST["QUERY"], $parse_array);
		$Res=$tinsp->APISetTarifNasp($parse_array);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APIGetTarifNasp":{
		$Res=$tinsp->APIGetTarifNasp($_POST["QUERY"][0]["value"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	
	/* РАБОТА С ОБЕЩАННЫМИ ПЛАТЕЖАМИ */
	case "APICreatTarifCredit":{
		$Res=$tinsp->APICreatTarifCredit($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APIDelTarifCredit":{
		$Res=$tinsp->APIDelTarifCredit($_POST["QUERY"][0]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetTarifCredit":{
		$Res=$tinsp->APIGetTarifCredit($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APICreatCreditUser":{
		$Res=$tinsp->APICreatCreditUser($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetCreditUser":{
		$Res=$tinsp->APIGetCreditUser($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	
	/* ЗАЯВКИ НА СМЕНУ ТАРИФНОГО ПЛАНА */
	case "APICreatRequestTariff":{
		$Res=$tinsp->APICreatRequestTariff($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetRequestTariff":{
		$Res=$tinsp->APIGetRequestTariff($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APISetRequestTariff":{
		$Res=$tinsp->APISetRequestTariff($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
		
	/*СЕКТОРЫ*/
	
	 case "APICreatSector":{
		$Res=$tinsp->APICreatSector($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"], $_POST["QUERY"][6]["value"], $_POST["QUERY"][7]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно создан сектор");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIGetSector":{
		$Res=$tinsp->APIGetSector($_POST["QUERY"][0]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetSector":{
		$Res=$tinsp->APISetSector($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"], $_POST["QUERY"][6]["value"], $_POST["QUERY"][7]["value"], $_POST["QUERY"][8]["value"], $_POST["QUERY"][9]["value"], $_POST["QUERY"][10]["value"], $_POST["QUERY"][11]["value"], $_POST["QUERY"][12]["value"], $_POST["QUERY"][13]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelSector":{
		$Res=$tinsp->APIDelSector($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	/*НАСЕЛЕННЫЕ ПУНКТЫ*/
	case "APIGetNasp":{
		$Res=$tinsp->APIGetNasp($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetNasp":{
		$Res=$tinsp->APISetNasp($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APICreatNasp":{
		$Res=$tinsp->APICreatNasp($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно создан населенный пункт");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelNasp":{
		$Res=$tinsp->APIDelNasp($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
		/*УЛИЦЫ*/
	case "APIGetUlica":{
		$Res=$tinsp->APIGetUlica($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"],  $_POST["QUERY"][2]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetUlica":{
		$Res=$tinsp->APISetUlica($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APICreatUlica":{
		$Res=$tinsp->APICreatUlica($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно создана");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelUlica":{
		$Res=$tinsp->APIDelUlica($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
		/*ДОМА*/
	case "APIGetDom":{
		$Res=$tinsp->APIGetDom($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetDom":{
		$Res=$tinsp->APISetDom($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"], $_POST["QUERY"][4]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APICreatDom":{
		$Res=$tinsp->APICreatDom($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно создан дом");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelDom":{
		$Res=$tinsp->APIDelDom($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
			/*ОБОРУДОВАНИЕ*/
	case "APIGetSwitch":{
		$Res=$tinsp->APIGetSwitch($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetSwitch":{
		$Res=$tinsp->APISetSwitch($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APICreatSwitch":{
		$Res=$tinsp->APICreatSwitch($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
   	case "APIDelSwitch":{
		$Res=$tinsp->APIDelSwitch($_POST["QUERY"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
				/* NAS /СЕРВЕРА ДОСТУПА  */
	case "APIGetNas":{
		$Res=$tinsp->APIGetNas($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APISetNas":{
		parse_str($_POST["QUERY"], $parse_array);
		$Res=$tinsp->APISetNas($parse_array);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APICreatNas":{
		$Res=$tinsp->APICreatNas($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"],$_POST["QUERY"][4]["value"],$_POST["QUERY"][5]["value"], $_POST["QUERY"][6]["value"], $_POST["QUERY"][7]["value"], $_POST["QUERY"][8]["value"], $_POST["QUERY"][9]["value"],  $_POST["QUERY"][10]["value"]);
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	case "APIDelNas":{
		$Res=$tinsp->APIDelNas($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно");
		echo JsonReply($Res[0], $Res[1]);
		break;
    }
	/* ПРАВА ДОСТУПА */
	case "APISetAccess":{
		$Res=$tinsp->APISetAccess($_POST["QUERY"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APICreatAccess":{
		$Res=$tinsp->APICreatAccess($_POST["QUERY"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APIDelAccess":{
		$Res=$tinsp->APIDelAccess($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	/* ДОПОЛНИТЕЛЬНЫЕ АТРИБУТЫ */
	case "APIGetAdvAttr":{
		$Res=$tinsp->APIGetAdvAttr($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	case "APISetAdvAttr":{
		$Res=$tinsp->APISetAdvAttr($_POST);
		echo JsonReply($Res, "");
		break;
    }
	
	case "APIGetAddAttr":{
		$Res=$tinsp->APIGetAddAttr($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
    case "APISetAddAttr":{
		$Res=$tinsp->APISetAddAttr($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"], $_POST["QUERY"][3]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно создан");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIDelAttr":{
		$Res=$tinsp->APIDelAttr($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"], $_POST["QUERY"][2]["value"]);
		if ($Res==0) echo JsonReply($Res, "Успешно удален");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	
	/* ПАРАМЕТРЫ БИЛЛИНГА */
	case "APISetOptionsArray":{
		$Res=$tinsp->APISetOptionsArray($_POST);
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetOptions":{
		$Res=$tinsp->APIGetOptions($_POST["QUERY"][0]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	
	/* -- */

	case "APIGetTypeTarif":{
		$Res=$tinsp->APIGetTypeTarif($_POST["QUERY"][0]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	case "APIGetTypeTarifAbon":{
		$Res=$tinsp->APIGetTypeTarifAbon($_POST["QUERY"][0]["value"]);
		if ($Res==0 or is_array($Res))  echo JsonReply($Res, "Успешно");
		else echo JsonReply($Res, "Проверьте правильность заполнения.");
		break;
    }
	/*HELP FUNCTION*/
	
	case "APISMSSend":{
		$Res=$tinsp->APISMSSend(str_replace("'","",$_POST["QUERY"]));
		echo JsonReply($Res, "");
		break;
    }
	case "APIGetLastIP":{
		$Res=$tinsp->APIGetLastIP($_POST["QUERY"]);
		echo JsonReply($Res, "");
		break;
    }
	case "Query":{
		$Res=$tinsp->Query($_POST["QUERY"][0]["value"], $_POST["QUERY"][1]["value"]);
		echo JsonReply($Res, "");
		break;
    }
	default:{
       echo "default";
	}
}



?>
