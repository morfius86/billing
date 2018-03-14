<?php
//
// API CLASS MR-BILLING
// version: 1.0.0
//

Class API {
	public $sql_error;
	public $sql_errno;
	public $link;
	public $config;
	public $encode;
	public $nas;
	public $ArrTypeAbon;
	public $ArrTypeAbonCash;
	public $ArrTypeEvent;
	public $ArrTypeValute;
	public $NET_MONIT;
	public $NET_MONIT_INSERT = array();

	
	public function __construct()
	{
        $this->ArrTypeAbon = array("1" => "Безлимитный", "2" => "Лимитный");
        $this->ArrTypeAbonCash = array("-1" => "Не снимать", "1" => "Раз в месяц 1 числа", "2" => "Раз в сутки", "3" => "Раз в сутки (пропорционально месяцу)", "4" => "Раз в месяц (плавающая дата)");;
        $this->ArrTypeEvent =array
		(
			"creat_abon" => "Создание абонентов", 
			"billing" => "Биллинг", 
			"pay_error" => "Платежные ошибки", 
			"sms" => "Рассылка SMS", 
			"dhcp" => "DHCP", 
			"stat" => "Личный кабинет",
			"snmp" => "SNMP",
		);
        $this->ArrTypeValute = array
		(
			"billing_inner" => array("1", "Внутренние операции"), 
			"billing_trans" => array("1", "Перевод другу"), 
			"billing_credit" => array("1", "Обещанный платеж"), 
			#акции
			"billing_action" => array("1", "Акция"),
			"billing_podarok" => array("1", "Подарок"),
			#перерасчет/ошибки
			"billing_per" => array("1", "Перерасчет"), 
			"billing_pernet" => array("1", "Возврат из-за перебоев сети"), 
			#подключение/ремнт
			"billing_podkl" => array("1", "Подключение к сети"), 
			"billing_remont" => array("1", "Устранение неисправности"), 
			#платежи
			"billing" => array("0", "Наличный платеж"), 
			"billing_bezn" => array("0", "Безналичный платеж"), 
			
			"sberbank_online" => array("0", "СберБанк-Онлайн"), 
			"Kassira.net" => array("0", "Кассира.Нет"), 
			"Pskb_bank" => array("0", "Банк ПСКБ"),
			"ComePay" => array("0", "ComePay"), 
			"MOBI.money" => array("0", "Моби.Моней"), 
			"QiwiWallet" => array("0", "Qiwi-кошелек"), 
			"LiqPay" => array("0", "LiqPay"), 
			"SKassa" => array("0", "Свободная Касса (Почта России)"),
			"Pochta" => array("0", "Почта России"),
			"unitpay" => array("0", "UnitPay")
		);
    }
	
	#получить типы тарификации
	function APIGetTypeTarif($obj) {
		#все
		if (strlen($obj) >= 1) {
			foreach ($this->ArrTypeAbon as $key => $value) {
				if (trim($key) == trim($obj) or trim($value) == trim($obj)) {
					$Res[] = array("key" => "$key", "value" => $value);
				}
			}
		} else {
			foreach ($this->ArrTypeAbon as $key => $value) {
				$Res[] = array("key" => "$key", "value" =>  $value);
			}
		}
		return $Res;
	}

	#получить тип начисления абонентских плат
	function APIGetTypeTarifAbon($obj) {
		#все
		if (strlen($obj) >= 1) {
			foreach ($this->ArrTypeAbonCash as $key => $value) {
				if (trim($key) == trim($obj) or trim($value) == trim($obj)) {
					$Res[] = array("key" => "$key", "value" => $value);
				}
			}
		} else {
			foreach ($this->ArrTypeAbonCash as $key => $value) {
				$Res[] = array("key" => "$key", "value" =>  $value);
			}
		}
		return $Res;
	}

	/*
	 ==================================
	 Функции для работы с абонентами
	 APICreatUser, APIDelUser, APISearchAbonent, APISetUser, APIGetAuthTypeUser, APISetAttr
	 ==================================
	 */
	#создание абонента
	function APICreatUser($query) {
		#$user_name, $group_id, $tarif_id, $balance, $user_block, $user_nasp, $user_ulica, $user_dom, 
		#$user_kvof, $user_mobphone, $user_dogovor, $user_ip, $user_pppip, $user_mac, $user_login, $user_password
		if ($this -> CheckArgsArray($query, array("user_name", "group_id", "tarif_id","user_nasp", "user_sector_id"))) {
			#проверка на дубликаты
			@$dublicat = $this -> Query("SELECT * FROM billing_users WHERE 
			(user_name != '' and user_name = '" . $query["user_name"] . "') or 
			(user_dogovor != '' and user_dogovor = '" . $query["user_dogovor"]."') or 
			(user_ip != '' and user_ip = '" . $query["user_ip"]. "') or
			(user_pppip != '' and user_pppip = '" . $query["user_pppip"]."') or
			(user_mac != '' and user_mac = '" . $query["user_mac"]. "') or
			(user_login != '' and user_login = '" . $query["user_login"]. "') or
			(user_password != '' and user_password = '" . $query["user_password"]. "')
			LIMIT 1", false);
			#найден дубликат
			if (is_array($dublicat)) {
				return array("-1", "Dublicat attribute with abonent " . $dublicat[0]["user_name"] . "");
			} else {
				$this -> Query('START TRANSACTION');
				$GUID = $this -> GUID();
				$this -> Query("INSERT INTO billing_users(
				user_id,user_name,user_ip,user_pppip,user_mac,
				user_login,user_password,group_id, tarif_id,
				balance,user_block,user_sector_id, user_nasp, user_ulica,
				user_dom,user_kvof,user_mobphone, user_dogovor, user_perspass
				) VALUES 
				('".$GUID."', 
				'" .$query["user_name"]. "',
				'" .$query["user_ip"]. "', 
				'" .$query["user_pppip"]. "', 
				'" .$query["user_mac"]. "', 
				'" .$query["user_login"]. "', 
				'" .$query["user_password"]. "',
				'" .$query["group_id"]. "',
				'" .$query["tarif_id"]. "',
				'" .$query["balance"]. "',
				'" .$query["user_block"]. "', 
				'" .$query["user_sector_id"]. "', 
				'" .$query["user_nasp"]. "',
				'" .$query["user_ulica"]. "',
				'" .$query["user_dom"]. "',
				'" .$query["user_kvof"]. "',
				'" .$query["user_mobphone"]. "',
				'" .$query["user_dogovor"]. "',  
				'" .$query["user_perspass"]. "'
				)");
				
				if ($this->sql_errno==0)
				{
					$this -> APISetOptions("billing_order", $query["user_dogovor"]);
					$this -> APICreatEvent("creat_abon", $GUID, $query["user_name"], @$_SESSION["access_id"], "0", "Creat abonent: order = ".$query["user_dogovor"]."");
					$this -> Query('COMMIT');
					return array("0", $GUID);
				}else{
					$this -> Query('ROLLBACK');
					return array($this->sql_errno, $this->sql_error);
				}
			}
		} else {
			return array(-1, "Attribute error");
		}
	}

	/*
		table - string название таблицы
		data - array. ключ => значение
	*/
	function fix_before_sql ($table, $data) {
		$table_header = array();
		foreach ($this->Query("SHOW COLUMNS FROM {$table}", false) as $v){
			$table_header[$v["Field"]] = $v;
		}
		foreach ($data as $key => $val) {
			if (isset($table_header[$key])) {
				if ($table_header[$key]["Null"] == "YES" and empty($val)) {
					$new_data[] = "" . $key . " = NULL";
				}else{
					$new_data[] = "" . $key . " = '".$val."'";
				}
			}
		}
		return $new_data;
	}
	#создание абонента
		#создание абонента
	function APICreatUser2($query) {
		#$user_name, $group_id, $tarif_id, $balance, $user_block, $user_nasp, $user_ulica, $user_dom, 
		#$user_kvof, $user_mobphone, $user_dogovor, $user_ip, $user_pppip, $user_mac, $user_login, $user_password
		if ($this -> CheckArgsArray($query, array("user_name", "group_id", "tarif_id","user_nasp", "user_sector_id"))) {
			#проверка на дубликаты
			@$dublicat = $this -> Query("SELECT * FROM billing_users WHERE 
			(user_name != '' and user_name = '" . $query["user_name"] . "') or 
			(user_dogovor != '' and user_dogovor = '" . $query["user_dogovor"]."') or 
			(user_ip != '' and user_ip = '" . $query["user_ip"]. "') or
			(user_pppip != '' and user_pppip = '" . $query["user_pppip"]."') or
			(user_mac != '' and user_mac = '" . $query["user_mac"]. "') or
			(user_login != '' and user_login = '" . $query["user_login"]. "') or
			(user_password != '' and user_password = '" . $query["user_password"]. "')
			LIMIT 1", false);
			#найден дубликат
			if (is_array($dublicat)) {
				return array("-1", "Dublicat attribute with abonent " . $dublicat[0]["user_name"] . "");
			} else {
				$this -> Query('START TRANSACTION');
				$GUID = $this -> GUID();
				$insert_1 = $this -> Query("INSERT INTO billing_users(
				user_id,user_name,user_ip,user_pppip,user_mac, user_login,
				user_password,group_id, tarif_id, balance,user_block,user_sector_id, 
				user_nasp, user_ulica, user_dom,user_kvof,user_mobphone, user_phone, 
				user_dopphone, birth_day, user_email, user_dogovor, user_perspass
				) VALUES 
				(
					'" .$GUID."', 
					'" .$query["user_name"]. "',
					'" .$query["user_ip"]. "', 
					'" .$query["user_pppip"]. "', 
					'" .$query["user_mac"]. "', 
					'" .$query["user_login"]. "', 
					'" .$query["user_password"]. "',
					'" .$query["group_id"]. "',
					'" .$query["tarif_id"]. "',
					'" .$query["balance"]. "',
					'" .$query["user_block"]. "', 
					'" .$query["user_sector_id"]. "', 
					'" .$query["user_nasp"]. "',
					'" .$query["user_ulica"]. "',
					'" .$query["user_dom"]. "',
					'" .$query["user_kvof"]. "',
					'" .$query["user_mobphone"]. "',
					'" .$query["user_phone"]. "',
					'" .$query["user_dopphone"]. "',
					'" .$query["birth_day"]. "',
					'" .$query["user_email"]. "',
					'" .$query["user_dogovor"]. "',  
					'" .$query["user_perspass"]. "'
				)");
				$insert_2 = $this -> Query("INSERT INTO billing_users_requisites(user_id, pasp_ser, pasp_num, pasp_date, pasp_who, yur_name, yur_gen, yur_ad_yur, yur_ad_postal, yur_inn, yur_kpp, yur_bank_num, yur_bank_name, yur_bank_kornum, yur_bank_bik) VALUES 
				(
					'" .$GUID."', 
					'" .@$query["pasp_ser"]. "',
					'" .@$query["pasp_num"]. "',
					'" .@$query["pasp_date"]. "',
					'" .@$query["pasp_who"]. "',
					
					'" .@$query["user_name"]. "',
					'" .@$query["yur_gen"]. "',
					'" .@$query["yur_ad_yur"]. "',
					'" .@$query["yur_ad_postal"]. "',
					'" .@$query["yur_inn"]. "',
					'" .@$query["yur_kpp"]. "',
					'" .@$query["yur_bank_num"]. "',
					'" .@$query["yur_bank_name"]. "',
					'" .@$query["yur_bank_kornum"]. "',
					'" .@$query["yur_bank_bik"]. "'
				)");
				
				if ($insert_1 == 0 && $insert_2 == 0 ){
					$this -> APISetOptions("billing_order", $query["user_dogovor"]);
					$this -> APICreatEvent("creat_abon", $GUID, $query["user_name"], $_SESSION["access_id"], "0", "Creat abonent: order = ".$query["user_dogovor"]."");
					$this -> Query('COMMIT');
					return array("0", $GUID);
				}else{
					$this -> Query('ROLLBACK');
					return array('-100', 'Other error');
				}
			}
		} else {
			return array(-1, "Attribute error");
		}
	}
	
	#редактирование абонента
	function APISetUser($array_value, $table="billing_users") {
		if (is_array($array_value)) {
			#параметры функции
			$table_header = array();
			foreach ($this->Query("SHOW COLUMNS FROM ".$table."", false) as $v){
				$table_header[$v["Field"]] = $v;
			}

			$sql["update"] = "";
			#данные по абоненту
			$user_id = $array_value["QUERY"][0]["value"];
			$User = $this -> APIGetBillProp(3, $user_id);
			//print_r($User);
			unset($array_value["QUERY"][0]);

			#формируем sql запрос на проверку дубликата и на обновление данных

			foreach ($array_value["QUERY"] as $AttrKey => $AttrValue) {
				if (isset($table_header[$AttrValue["name"]])) {
					if ($table_header[$AttrValue["name"]]["Null"] == "YES" and empty($AttrValue["value"])) {
						$sql["update"][] = "" . $AttrValue["name"] . " = NULL";
					}else{
						$sql["update"][] = "" . $AttrValue["name"] . " = '" . $AttrValue["value"] . "'";
					}
					
					#список переданных параметров
					$ArrayGetAttr[$AttrValue["name"]] = $AttrValue["value"];
				}
			}

			#Получим список недопереданных параметров и заполним текущими значениями абонента
			$array_diff_key = array_diff_key($User[0], $ArrayGetAttr);
			$array_diff_key2 = array_diff($ArrayGetAttr, $User[0]);
			$ArrayAttr = $array_diff_key + $ArrayGetAttr;

			$this -> Query('START TRANSACTION');
			$this -> Query("UPDATE ".$table." SET " . implode(",", $sql["update"]) . " WHERE user_id = '" . $user_id . "';");
			if ($this->sql_errno != "0") {
				$Res = array($this->sql_errno, $this->sql_error);
				$this -> Query('ROLLBACK');	
			} else {
				#были-ли изменены ip/mac/log/pas/tarif/group (Необходимо для дропа сессии)
				if ((isset($ArrayAttr["user_ip"]) and $ArrayAttr["user_ip"] != $User[0]["user_ip"]) or (isset($ArrayAttr["user_mac"]) and $ArrayAttr["user_mac"] != $User[0]["user_mac"]) or (isset($ArrayAttr["user_login"]) and $ArrayAttr["user_login"] != $User[0]["user_login"]) or (isset($ArrayAttr["user_password"]) and $ArrayAttr["user_password"] != $User[0]["user_password"]) or (isset($ArrayAttr["group_id"]) and $ArrayAttr["group_id"] != $User[0]["group_id"]) or (isset($ArrayAttr["tarif_id"]) and $ArrayAttr["tarif_id"] != $User[0]["tarif_id"])) {
					$this -> Query("UPDATE ".$table." SET user_dropnas = '1' WHERE user_id = '" . $user_id . "'");
					$this -> APICreatEvent("billing", $user_id, $User[0]["user_name"], @$_SESSION["access_id"], "", "Edit abonent: " . str_replace("&", "<br>", http_build_query($array_diff_key2)) . "");
					$this -> Query('COMMIT');
					$Res = array("0", "OK");
				} else {
					$this -> APICreatEvent("billing", $user_id, $User[0]["user_name"], @$_SESSION["access_id"], "", "Edit abonent: " . str_replace("&", "<br>", http_build_query($array_diff_key2)) . "");
					$this -> Query('COMMIT');
					$Res = array("0", $array_diff_key2);
				}
			}
			
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}
	
	
	function APISetGroupUserAttr($attr, $value, $array_user) {
		if (!empty($attr) && !empty($value) && is_array($array_user)) {
			$str = "";
			foreach	($array_user as $user_id) {
				$str.= "WHEN '".$user_id."' THEN '".$value."' ";
			}
			$this -> Query("UPDATE billing_users SET ".$attr." = CASE user_id ".$str." ELSE ".$attr." END ");
			return ($this->sql_errno==0 ? array(0, "OK") : array($this->sql_errno, $this->sql_error));
		}else{
			return array("-1", "empty object!");
		}
	}


	
	#удаление абонента
	function APIDelUser($obj, $del_payment="0", $del_log="0", $del_abon="1") {
		if (!isset($obj) || !empty($obj)) {
			$User = $this -> APIGetBillProp(3, $obj);
			$mess="";
			$mess_arr=array(
				0=>array("mess"=>"удалены платежи", "val"=>$del_payment), 
				1=>array("mess"=>"удалены логи и накопительные данные", "val"=>$del_log), 
				2=>array("mess"=>"удалена учетная запись", "val"=>$del_abon), 
			);
			foreach ($mess_arr as $mess_arr_k=>$mess_arr_v){
				if ($mess_arr_v["val"]=="1"){
					$mess.=$mess_arr_v["mess"];
					if (isset($mess_arr[$mess_arr_k+1]) && $mess_arr[$mess_arr_k+1]["val"]=="1"){
						$mess.=", ";
					}
				}
			}
			#Удаляем из billing_log
			if ($del_log=="1") $this->Query("DELETE FROM billing_log WHERE user_id = '".$obj."';");
			#удаляем платежи
			if ($del_payment=="1") $this->Query("DELETE FROM billing_payment WHERE user_id = '".$obj."';");
			#удаляем абонента
			if ($del_abon=="1"){
				$this -> Query("DELETE FROM billing_users WHERE user_id = '" . $obj . "' LIMIT 1;");
				$this -> Query("DELETE FROM billing_users_attr WHERE user_id = '" . $obj . "';");
			}
			$this -> APICreatEvent("billing", $obj, $User[0]["user_name"], @$_SESSION["access_id"], "", "Договор ".$User[0]["user_dogovor"].": ".$mess.". ");
		} else {
			return array("-1", "empty object!");
		}
		return array("0", "ok");
	}

	#поиск абонентов
	function APISearchAbonent($search_fiodogovor, $search_ipmac, $search_phone, $select_nasp, $select_ulica, $select_dom) {
		if (!empty($search_fiodogovor) &&  $search_fiodogovor{0} == "!") { $search_fiodogovor = substr($search_fiodogovor, 1);
		} else { $search_fiodogovor = "%" . $search_fiodogovor . "%";
		}
		if (!empty($search_ipmac) && $search_ipmac{0} == "!") { $search_ipmac = substr($search_ipmac, 1);
		} else { $search_ipmac = "%" . $search_ipmac . "%";
		}
		#print $search_fiodogovor;
		#сформируем запрос
		$ConstArray = func_get_args();
		foreach ($ConstArray as $key => $value) {
			if ($key == 0 && $value != "") { $query .= "(billing_users.user_name LIKE '" . $search_fiodogovor . "' or billing_users.user_dogovor LIKE '" . $search_fiodogovor . "')";
			}
			if ($key == 1 && $value != "") {
				if ($ConstArray[0] != "") {$query .= " AND ";
				} $query .= "(billing_users.user_ip LIKE '" . $search_ipmac . "' or billing_users.user_mac LIKE '" . $search_ipmac . "')";
			}
			if ($key == 2 && $value != "") {
				if ($ConstArray[1] != "" or $ConstArray[0] != "") {$query .= " AND ";
				} $query .= "(billing_users.user_mobphone LIKE '%" . $search_phone . "%')";
			}
			if ($key == 3 && $value != "") {
				if ($ConstArray[2] != "" or $ConstArray[1] != "" or $ConstArray[0] != "") {$query .= " AND ";
				} $query .= "(billing_users.user_nasp = '" . $select_nasp . "')";
			}
			if ($key == 4 && $value != "") {
				if ($ConstArray[3] != "" or $ConstArray[2] != "" or $ConstArray[1] != "" or $ConstArray[0] != "") {$query .= " AND ";
				} $query .= "(billing_users.user_ulica = '" . $select_ulica . "')";
			}
			if ($key == 5 && $value != "") {
				if ($ConstArray[4] != "" or $ConstArray[3] != "" or $ConstArray[2] != "" or $ConstArray[1] != "" or $ConstArray[0] != "") {$query .= " AND ";
				} $query .= "(billing_users.user_dom = '" . $select_dom . "')";
			}
		}
		#список абонентов
		$IFQuery = ($query == "") ? "" : "where " . $query . "";
		$Res = $this -> Query("SELECT billing_users.*,
			billing_group.group_name,
			billing_group.group_id,
			billing_nasp.nasp_id,
			billing_nasp.nasp_name,
			billing_ulica.ulica_id,
			billing_ulica.ulica_name,
			billing_dom.dom_id,
			billing_dom.dom_name
			FROM billing_users
			LEFT JOIN billing_group ON billing_group.group_id = billing_users.group_id
			LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_users.user_nasp
			LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_users.user_ulica
			LEFT JOIN billing_dom ON billing_dom.dom_id = billing_users.user_dom
			" . $IFQuery . " ", false);
		return array("0", $Res);
	}

	#поиск абонентов ver 2.
	function APISearchAbonent2($search_fiodogovor = "", $search_ipmac = "", $search_phone = "", $select_nasp = "", $select_ulica = "", $select_group = "", $select_block = "", $select_dom = "", $select_tarif = "") {
		if (!empty($search_fiodogovor) && $search_fiodogovor{0} == "!") { $search_fiodogovor = substr($search_fiodogovor, 1);
		} else { $search_fiodogovor = "%" . $search_fiodogovor . "%";
		}
		if (!empty($search_ipmac) && $search_ipmac{0} == "!") {
			$search_ipmac = substr($search_ipmac, 1);
		} else {
			$search_ipmac = "%" . $search_ipmac . "%";
		}

		$IFQuery = "(billing_users.user_name LIKE '" . $search_fiodogovor . "' or billing_users.user_dogovor LIKE '" . $search_fiodogovor . "') AND";
		$IFQuery .= "(billing_users.user_ip LIKE '" . $search_ipmac . "' or billing_users.user_mac LIKE '" . $search_ipmac . "') AND";
		$IFQuery .= "(billing_users.user_mobphone LIKE '%" . $search_phone . "%') AND";
		$IFQuery .= "(billing_users.user_nasp LIKE '%" . $select_nasp . "') AND";
		$IFQuery .= "(billing_users.user_ulica LIKE '%" . $select_ulica . "') AND";
		$IFQuery .= "(billing_users.user_block LIKE '%" . $select_block . "') AND";
		$IFQuery .= "(billing_users.group_id LIKE '%" . $select_group . "') AND";
		$IFQuery .= "(billing_users.user_dom LIKE '%" . $select_dom . "') AND";
		$IFQuery .= "(billing_users.tarif_id LIKE '%" . $select_tarif . "')";

		$Res = $this -> Query("SELECT billing_users.*,
			billing_tarification.tarif_id,
			billing_tarification.tarif_name,
			billing_tarification.tarif_desc,
			billing_tarification.abon_cash,
			billing_tarification.type_abon,
			billing_tarification.tarif_arhive,
			billing_tarification.tarif_ves,
			billing_group.group_name,
			billing_group.group_id,
			billing_nasp.nasp_id,
			billing_nasp.nasp_name,
			billing_ulica.ulica_id,
			billing_ulica.ulica_name,
			billing_dom.dom_id,
			billing_dom.dom_name
			FROM billing_users
			LEFT JOIN billing_group ON billing_group.group_id = billing_users.group_id
			LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_users.user_nasp
			LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_users.user_ulica
			LEFT JOIN billing_dom ON billing_dom.dom_id = billing_users.user_dom
			LEFT JOIN billing_tarification ON billing_tarification.tarif_id = billing_users.tarif_id
			Where " . $IFQuery . " ORDER BY billing_users.user_nasp,billing_users.user_ulica", false);
		return array("0", $Res);
	}

	function BillingGetAjaxUser($obj) {
		if (!$obj)
			return;
		$UserBill = $this -> APIGetBillProp(3, "");
		foreach ($UserBill as $Users) {
			#echo "".$obj."|123\n";
			if (strpos(strtolower($Users["user_name"]), strtolower($obj)) !== false) {
				echo "" . $Users["user_name"] . "|123\n";
			}
		}
	}

	/*
	 ==================================
	 Функции для работы с группами абонентов
	 APICreatGroup, APIGetBillProp
	 ==================================
	 */

	function APICreatGroup($query) {
		if ($this -> CheckArgsArray($query, array("group_name"))) {
			$this -> Query("INSERT INTO billing_group(group_id, group_name, group_type,comment) VALUES ('".$this->GUID() . "','" . $query["group_name"]. "', '" . $query["group_type"]. "', '" .@$query["comment"]. "')");
			return ($this->sql_errno==0 ? array(0, "OK") : array($this->sql_errno, $this->sql_error));
		} else {
			return array(-1, "No required parameters");
		}
	}

	#список абонентов/групп
	function APIGetBillProp($type, $obj = false) {
		if ($type == 2) {
			#список групп
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT * FROM billing_group", false);
			} else {
				$Res = $this -> Query("SELECT * FROM billing_group
				WHERE billing_group.group_id = '" . $obj . "' or billing_group.group_name = '" . $obj . "'", false);
			}
		} elseif ($type == 3) {
			#список абонентов
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT billing_users.*,
				billing_users_requisites.*, 
				billing_group.group_name,
				billing_group.group_id,
				billing_group.group_type,
				billing_nasp.nasp_id,
				billing_nasp.nasp_name,
				billing_ulica.ulica_id,
				billing_ulica.ulica_name,
				billing_dom.dom_id,
				billing_dom.dom_name
				FROM billing_users
				LEFT JOIN billing_users_requisites ON billing_users_requisites.user_id = billing_users.user_id
				LEFT JOIN billing_group ON billing_group.group_id = billing_users.group_id
				LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_users.user_nasp
				LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_users.user_ulica
				LEFT JOIN billing_dom ON billing_dom.dom_id = billing_users.user_dom order by user_name", false);
			} else {
				$Res = $this -> Query("SELECT billing_users.*,
				billing_users_requisites.*, 
				billing_group.group_name,
				billing_group.group_id,
				billing_nasp.nasp_id,
				billing_nasp.nasp_name,
				billing_ulica.ulica_id,
				billing_ulica.ulica_name,
				billing_dom.dom_id,
				billing_dom.dom_name
				FROM billing_users
				LEFT JOIN billing_users_requisites ON billing_users_requisites.user_id = billing_users.user_id
				LEFT JOIN billing_group ON billing_group.group_id = billing_users.group_id
				LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_users.user_nasp
				LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_users.user_ulica
				LEFT JOIN billing_dom ON billing_dom.dom_id = billing_users.user_dom
				WHERE billing_users.user_id = '" . $obj . "' or billing_users.user_name = '" . $obj . "'", false);
			}
		}elseif ($type == 4){
			$Query= $this -> Query("SELECT billing_users.* FROM billing_users ".($obj != NULL ? "billing_users.user_id = '".$obj."'" : " ")."", false);
			if (is_array($Query)) {
				foreach(@$Query as $v){
					$Res[$v["user_id"]]=$v;
				}
			}
		} else {
			#аргементы не определены
			$Res = -1;
		}
		return $Res;
	}

	#редактор абонентов/групп
	function APISetGroup($query) {
		$str = '';
		$i = 0;
		if ($this -> CheckArgsArray($query, array("obj"))) {
			$obj = $query["obj"];
			unset($query["obj"]);
			foreach	($query as $key => $value) {
				$i++;
				$str.= " ".$key." = '".$value."' ".($i<count($query) ? ',': '')."";
			}
			$this -> Query("UPDATE billing_group SET ".$str." WHERE group_id = '".$obj."'");
			return ($this->sql_errno==0 ? array(0, "OK") : array($this->sql_errno, $this->sql_error));
		}else{
			return array(-1, "No required parameters");
		}
	}

	function APIDelBillProp($obj) {
	    if ($this -> CheckArgsArray(func_get_args(), array(0))) {
            #связанность между sector_id и billing_users.user_sector_id
            $this -> Query("DELETE FROM billing_group WHERE group_id = '".$obj."' LIMIT 1;");
            if ($this->sql_errno==0){
                return array(0, "OK");
            }elseif($this->sql_errno==1451){
                return array(-1, "This group linked to objects! Remove them.");
            }else{
                return array($this->sql_errno, $this->sql_error);
            }
        }else{
            return array(-1, "No parameters");
        }
	}

	/*
	 ==================================
	 Функции для работы с денежными средставми
	 APIAddCash, APIGetPayment, APITransCashBetweenAbon
	 ==================================
	 */
	function APIAddCash($obj, $summ, $comment = "", $payment_id = "false", $sender = "billing", $access_id = "") {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$User = $this -> APIGetBillProp(3, $obj);
			if (is_array($User)) {
				$Itog = (float)$User[0]["balance"] + (float)$summ;
				if ($payment_id == "") $payment_id = "false";
				if ($sender == "") $sender = "billing";
				
				#проверить тип передаваемой валюты.
				if(isset($this->ArrTypeValute[$sender])){
					$this -> Query('START TRANSACTION');
					#обновим баланс
					$Query = $this -> Query("UPDATE billing_users SET balance = '" . $Itog . "', user_lastbill = '".date("Y-m-d H:i:s")."' WHERE user_id = '" . $obj . "'");
					if ($Query == "0") {
						#gen payment_id
						if ($payment_id == "false") {
							//list($mk, $ms) = explode(" ", str_replace(".", "", microtime()));
							//$payment_id = $ms . $mk;
							$payment_id = 'NULL';
						}
						#добавить в базу платеж
						$Query = $this -> Query("INSERT INTO billing_payment(`user_id`,`access_id`,`sender`,`res` ,`payment_id`,`order`,`sum`,`balance`,`date`,`prv_date`,`comment`) VALUES 
						('" . $obj . "', '" . $access_id . "', '" . $sender . "', '0', {$payment_id},'" . $User[0]["user_dogovor"] . "','" . $summ . "','" . $Itog . "','" . date("Y-m-d H:i:s") . "','" . date("H:i:s") . "','" . $comment . "')");
						if ($Query == "0") {
							$insert_id=mysql_insert_id();
							$this -> Query('COMMIT');
							$Res = array("0", $insert_id);
						} else {
							$this -> Query('ROLLBACK');
							$Res = array("-3", time());
							$this -> APICreatEvent("pay_error", $obj, $User[0]["user_name"], @$_SESSION['access_id'], "-3", "Failed to add the payment to the base payment. sender = " . $sender . ", dogovor = " . $User[0]["user_dogovor"] . "");
						}
					} else {
						$this -> Query('ROLLBACK');
						$Res = array("-3", "Error update balance");
						$this -> APICreatEvent("pay_error", $obj, $User[0]["user_name"], @$_SESSION['access_id'], "-3", "Failure to update the balance. sender = " . $sender . ", dogovor = " . $User[0]["user_dogovor"] . "");
					}
				}else{
					$Res = array("-2", "Not suppot valute!");
					$this -> APICreatEvent("pay_error", $obj, $User[0]["user_name"], @$_SESSION['access_id'], "-2", "Valute ".$sender." not suppot!");	
				}
			} else {
				$Res = array("-2", "Abonent not search");
				$this -> APICreatEvent("pay_error", $obj, '', @$_SESSION['access_id'], "-2", "The abonent can not be found. sender = " . $sender . ", guid = " . $obj . "");
			}
		} else {
			$Res = array("-1", "Error attribute");
			$this -> APICreatEvent("pay_error", $obj, '', @$_SESSION['access_id'], "-1", "Error sent attributes");
		}
		return $Res;
	}
	
	function APITransCashBetweenAbon($obj_find, $obj_from, $obj_to, $summa) {
		$usl_persent=$this->APIGetOptions("usl_persent");
		if ($this -> CheckArgsArray(func_get_args(), array(0,1,2,3))) {
			#ищем абонентов в зависимости от переданного типа find
			if ($obj_find=="guid"){
			
			}elseif($obj_find=="order"){
				$obj_f=$this->APISearchAbonent2('!'.$obj_from);
				$obj_t=$this->APISearchAbonent2('!'.$obj_to);
				#не могут быть равны
				if($obj_from==$obj_to){
					$Res = array("-8", "Should not match");
				}elseif(!is_array($obj_f[1][0]) or !is_array($obj_t[1][0])){
					$Res = array("-9", "Договор не найден.");
				}elseif(!eregi ("^[0-9]{1,32}$", $summa)){
					$Res = array("-7", "Invalid characters. Only integers.");
				}elseif(((float)$obj_f[1][0]["balance"])<$summa){
					$Res = array("-6", "Not enough amount on account");
				}elseif($summa<=0){
					$Res = array("-5", "Сумма не может быть меньше нуля");
				}elseif($obj_t[1][0]["user_perstrans"]=='0' or $obj_f[1][0]["user_perstrans"]=='0'){
					$Res=array("-4","Операция запрещена.");
				}else{
					$summa=(int)$summa;
					$procsumm=(($usl_persent[0]["value"]/100)*$summa);

					#снимаем у переводящего 
					$Trans1=$this->APIAddCash($obj_f[1][0]["user_id"], '-'.$summa, "Перевод для договора ".$obj_to." (".$obj_t[1][0]["user_name"].")", "false", "billing_trans");
					#добавляем получателю
					$Trans2=$this->APIAddCash($obj_t[1][0]["user_id"], ($summa-$procsumm), "Перевод от договора ".$obj_from." (".$obj_f[1][0]["user_name"].")", "false", "billing_trans");
					if ($Trans1[0]=="0" && $Trans2[0]=="0"){
						$Res = array("0", "Success");
					}else{
						$Res = array("-3", "fail");
					}
				}	
			}else{
				$Res = array("-2", "not support find type");
			}
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}
	
	#поиск платежей ver. 2
	function APIGetPayment($order = "", $pay_txn = "", $pay_sender = "", $period1 = "", $period2 = "", $pay_buh = "", $pay_access = "") {
		$IFQuery = '';
		#сформируем запрос
		if(!empty($order)) $IFQuery.= "(billing_payment.order = '".$order."') AND ";
		if(!empty($pay_txn)) $IFQuery.= "(billing_payment.payment_id LIKE '%".$pay_txn."%') AND ";
		if(!empty($pay_access)) $IFQuery .= "(billing_payment.access_id LIKE '%" . $pay_access . "%') AND ";
		#список платежных операторов
		if (is_array($pay_sender)) {
			$IFQuery .= "(";
			foreach ($pay_sender as $pa_sender_key => $pa_sender_val) {
				if (isset($pay_sender[$pa_sender_key + 1])) {
					$IFQuery .= "billing_payment.sender LIKE '" . $pa_sender_val . "' or ";
				} else {
					$IFQuery .= "billing_payment.sender LIKE '" . $pa_sender_val . "'";
				}
			}
			$IFQuery .= ") AND ";
		}

		$IFQuery .= "(billing_payment.date BETWEEN '".$period1."' AND '".$period2."')";
		
		if ($pay_buh == "1") {
			$IFQuery .= " AND (billing_payment.sum<0)";
		} elseif ($pay_buh == "0") {
			$IFQuery .= " AND (billing_payment.sum>0)";
		} elseif ($pay_buh == "2") {
			#$IFQuery.="";
		}
		/*
		#список платежей 
		$Res = $this -> Query("SELECT billing_payment.*,
			billing_users.user_name,
			billing_access.access_name
			FROM billing_payment
			LEFT JOIN billing_users ON billing_users.user_id = billing_payment.user_id
			LEFT JOIN billing_access ON billing_access.access_id = billing_payment.access_id
			where " . $IFQuery . " ORDER BY billing_payment.id", false);
		*/
		#список платежей 
		$Res = $this -> Query("SELECT billing_payment.* FROM billing_payment where " . $IFQuery . " ORDER BY billing_payment.id", false);
		return array("0", $Res);
	}

	/*
	 ==================================
	 Функции для работы с тарифными планами
	 APICreatTarif, APIGetTarif, APICreatGroupTarif
	 ==================================
	 */
	#создание тарифа
	function APICreatTarif($tarif_name,$tarif_desc, $grtarif_id, $type = 0, $type_abon = 0, $abon_cash = 0, $tarif_block = 0, $valute = "руб.", $tarif_speed_in = "0", $tarif_speed_out = "0", $tarif_arhive="0", $tarif_ves="0", $mik_ippool = "0", $mik_ippool_name = "") {
		if (!is_numeric($abon_cash) or !is_numeric($tarif_speed_in) or !is_numeric($tarif_speed_out))
			return array("-1", "Error attribute");
		if (!isset($tarif_name) || !empty($tarif_name)) {
			$Query = $this -> Query("INSERT INTO billing_tarification(
			tarif_name,
			tarif_desc,
			tarif_id,
			grtarif_id,
			type,
			type_abon,
			abon_cash,
			tarif_block,
			valute,
			tarif_speed_out,
			tarif_speed_in,
			tarif_arhive,
			tarif_ves,
			mik_ippool,
			mik_ippool_name) VALUES (
			'" . $tarif_name . "',
			'" . $tarif_desc."',
			'" . $this -> GUID() . "', 
			'" . $grtarif_id."',
			'" . $type . "',
			'" . $type_abon . "', 
			'" . $abon_cash . "', 
			'" . $tarif_block . "', 
			'" . $valute . "',
			'" . $tarif_speed_out . "',
			'" . $tarif_speed_in . "', 
			'" . $tarif_arhive . "',
			'" . $tarif_ves . "', 
			'" . $mik_ippool . "', 
			'" . $mik_ippool_name . "'
			)");
			if ($Query != 0) {
				$Res = array($Query, mysql_error());
			} else {
				$Res = array($Query, "Success create tarification");
			}
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}

	#создание группы тарифов
	function APICreatGroupTarif($grtarif_name, $grtarif_desc) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#связанность между sector_id и billing_users.user_sector_id
			 $this -> Query("INSERT INTO billing_tarification_group(grtarif_id, grtarif_name,grtarif_desc) VALUES ('" . $this -> GUID() . "', '" . $grtarif_name . "', '" . $grtarif_desc."')");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This TarifGroup linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}
	
	#редактирование параметров группы тарифов (Данные в виде массива)
	function APISetGroupTarifArray($obj) {
		$str = "";
		if (is_array($obj["QUERY"])){
			if(isset($obj["QUERY"][0]["obj"])){
				$grtarif_id = $obj["QUERY"][0]["obj"];
				unset($obj["QUERY"][0]);
				foreach	($obj["QUERY"] as $key => $value) {
					list($attr, $attrval) = each($value);
					$str.= " ".$attr." = '".$attrval."' ";
					if(isset($obj["QUERY"][$key+1])){
						$str.= ",";
					}
				}
				$this -> Query("UPDATE `billing_tarification_group` SET ".$str." WHERE grtarif_id = '".$grtarif_id."'");
				if ($this->sql_errno==0){
					return array(0, "OK");
				}else{
					return array($this->sql_errno, $this->sql_error);
				}
			}else{
				return array("-1", "IN ARRAY NO SET OBJ");
			}	
		}else{
			return array("-1", "QUERY IS NOT ARRAY");
		}
	}
	
	#редактирование параметров группы тарифов (Данные в виде массива)
	function APISetTarifNasp($obj) {
		if (is_array(@$obj["nasp_tarif"])){
			$count_nasp=0;
			foreach ($obj["nasp_tarif"] as $obj_nasp=> $obj_tar_arr){
				$count_nasp++;
				$count_tar=0;
				foreach ($obj_tar_arr as $obj_tarif){
					$count_tar++;
					$write_sql_temp.="('".$obj_nasp."', '".$obj_tarif."') ".((count($obj["nasp_tarif"])==$count_nasp && count($obj_tar_arr)==$count_tar) ? '':',')."";
				}
			}
			$this->Query("TRUNCATE TABLE billing_tarification_nasp");
			$this->Query("INSERT INTO billing_tarification_nasp(nasp_id, tarif_id) VALUES ".$write_sql_temp." ON DUPLICATE KEY UPDATE nasp_id = VALUES(nasp_id), tarif_id = VALUES(tarif_id)");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array("-1", "Not selected");
		}
	}
	
	function APIGetTarifNasp($obj=""){
		if (!empty($obj)) {
			$Res=$this -> Query("SELECT * FROM billing_tarification_nasp WHERE nasp_id = '".$obj."'", false);
		}else{
			$Res=$this -> Query("SELECT * FROM billing_tarification_nasp", false);
		}
		
		if ($this->sql_errno==0){
			return array(0, $Res);
		}else{
			return array($this->sql_errno, $this->sql_error);
		}
	}
	
	#список групп тарифов
	function APIGetGroupTarif($obj=false) {
		if (!empty($obj)) {
			$Res = $this -> Query("SELECT * FROM billing_tarification_group WHERE grtarif_id = '".$obj."'", false);
		}else{
			$Res = $this -> Query("SELECT * FROM billing_tarification_group ", false);
		}
		return $Res;
	}
	
	#удаление группы тарифа
	function APIDelGroupTarif($obj) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#связанность между sector_id и billing_users.user_sector_id
			$this -> Query("DELETE FROM billing_tarification_group WHERE grtarif_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This group tarif linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}
	
	#список тарификаций
	function APIGetTarif($type, $obj) {
		# 2: список тарифов
		if ($type == 2) {
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT billing_tarification.*, billing_tarification_group.grtarif_name FROM billing_tarification
				LEFT JOIN billing_tarification_group ON billing_tarification_group.grtarif_id = billing_tarification.grtarif_id ORDER BY billing_tarification.grtarif_id, billing_tarification.tarif_arhive, billing_tarification.tarif_ves", false);
			} else {
				$Res = $this -> Query("SELECT billing_tarification.*, billing_tarification_group.grtarif_name FROM billing_tarification
				LEFT JOIN billing_tarification_group ON billing_tarification_group.grtarif_id = billing_tarification.grtarif_id
				WHERE billing_tarification.tarif_id = '" . $obj . "' or billing_tarification.tarif_name = '" . $obj . "' LIMIT 1", false);
			}
		# 3: список тарифов в населенному пункте
		}elseif($type == 3){
			if (!empty($obj)) {
				$Res = $this -> Query("SELECT billing_tarification.*, billing_tarification_group.grtarif_name FROM billing_tarification_nasp, billing_tarification
				LEFT JOIN billing_tarification_group ON billing_tarification_group.grtarif_id = billing_tarification.grtarif_id
				WHERE billing_tarification_nasp.nasp_id =  '".$obj."' AND billing_tarification_nasp.tarif_id = billing_tarification.tarif_id ORDER BY billing_tarification.grtarif_id, billing_tarification.tarif_ves", false);
			} else {
				#аргементы не определены
				$Res = -2;
			}
		}else{
			#аргементы не определены
			$Res = -2;
		}
		return $Res;
	}

	#редактирование тарифа
	function APISetTarif($obj, $tarif_name, $type = 0, $type_abon = 0, $abon_cash = 0, $tarif_block = 0, $valute = "руб.", $tarif_speed_in = "0", $tarif_speed_out = "0", $tarif_arhive="0", $tarif_ves="0", $mik_ippool = "0", $mik_ippool_name = "") {
		if (!is_numeric($abon_cash) or !is_numeric($tarif_speed_in) or !is_numeric($tarif_speed_out))
			return array("-1", "Error attribute");
		if (!isset($tarif_name) || !empty($tarif_name)) {
			$Query = $this -> Query("UPDATE billing_tarification SET 
			tarif_name = '" . $tarif_name . "',
			type = '" . $type . "',
			type_abon = '" . $type_abon . "',
			abon_cash = '" . $abon_cash . "',
			tarif_block = '" . $tarif_block . "',
			valute = '" . $valute . "',
			tarif_speed_in = '" . $tarif_speed_in . "',
			tarif_speed_out = '" . $tarif_speed_out . "',
			tarif_arhive = '" . $tarif_arhive . "',
			tarif_ves = '" . $tarif_ves . "',
			mik_ippool = '" . $mik_ippool . "',
			mik_ippool_name = '" . $mik_ippool_name . "'
			WHERE tarif_name = '" . $obj . "' or tarif_id = '" . $obj . "'");
			if ($Query != 0) {
				$Res = array($Query, mysql_error());
			} else {
				$Res = array($Query, "Success edit tarification");
			}
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}
	#редактирование параметров тарифа (Данные в виде массива)
	function APISetTarifArray($obj) {
		$str = "";
		if (is_array($obj["QUERY"])){
			if(isset($obj["QUERY"][0]["obj"])){
				$tarif_id = $obj["QUERY"][0]["obj"];
				unset($obj["QUERY"][0]);
				foreach	($obj["QUERY"] as $key => $value) {
					list($attr, $attrval) = each($value);
					$str.= " ".$attr." = '".$attrval."' ";
					if(isset($obj["QUERY"][$key+1])){
						$str.= ",";
					}
				}
				$this -> Query("UPDATE `billing_tarification` SET ".$str." WHERE tarif_id = '".$tarif_id."'");
				if ($this->sql_errno==0){
					return array(0, "OK");
				}else{
					return array($this->sql_errno, $this->sql_error);
				}
			}else{
				return array("-1", "IN ARRAY NO SET OBJ");
			}	
		}else{
			return array("-1", "QUERY IS NOT ARRAY");
		}
	}
	
	#удаление тарифа
	function APIDelTarif($obj) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#связанность между sector_id и billing_users.user_sector_id
			$this -> Query("DELETE FROM billing_tarification WHERE tarif_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This sector linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}
	

	/*
	==================================
	 Функции для работы с кредитами у тарифов
	 APICreatTarifCredit, APISetTarifCredit, APIGetTarifCredit, APIC
	==================================
	*/
	function APICreatTarifCredit($tarif_id, $tarif_cred_name,$tarif_cred_day, $tarif_cred_sum, $tarif_cred_dopsum, $tarif_cred_limitmonth) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$Res = $this -> Query("INSERT INTO billing_tarification_credit(tarif_cred_id, tarif_id, tarif_cred_name, tarif_cred_day, tarif_cred_sum, tarif_cred_dopsum, tarif_cred_limitmonth) VALUES 
			('" . $this -> GUID() . "', 
			'" . $tarif_id . "',
			'" . $tarif_cred_name . "', 
			'" . $tarif_cred_day . "', 
			'" . $tarif_cred_sum . "', 
			'" . $tarif_cred_dopsum . "',
			'" . $tarif_cred_limitmonth . "' )");
			
			return array($Res, mysql_error());
		} 
		return array("-1", "Error attribute");
	}
	
	function APIDelTarifCredit($obj) {
		#получить данные по тарифу
		$Credit=$this->APIGetTarifCredit("1", $obj);
		if($Credit[0]=="0"){
			#проверить активные кредиты по данному кредиту.
			$Query = $this -> Query("SELECT count(id) as count FROM billing_users_credit WHERE tarif_cred_id = '".$obj."' AND credit_state = '0' ", false);
			if($Query[0]["count"]==0){
				#удалим
				$Res = $this -> Query("DELETE FROM billing_tarification_credit where tarif_cred_id = '".$obj."'");
				return array($Res, mysql_error());
			}else{
				return array("-2", "Have active credit!");
			}
		}else{
			return array("-1", "not search this credit");
		}
		#удалить
	}
	
	function APIGetTarifCredit($type, $obj) {
		if ($type == 1){
			#ищем tarif_cred_id
			if (!empty($obj)) {
				$Query = $this -> Query("SELECT * FROM billing_tarification_credit WHERE tarif_cred_id = '" . $obj . "' LIMIT 1", false);	
			}else{
				return array("-1", "2 parameter is empty");
			}	
		}elseif($type == 2){
			#ищем по tarif_id
			if (!empty($obj)) {
				$Query = $this -> Query("SELECT * FROM billing_tarification_credit WHERE tarif_id = '" . $obj . "' ", false);
			}else{
				$Query = $this -> Query("SELECT * FROM billing_tarification_credit", false);
			}	
		}
		#return
		if (is_array($Query)) {
			return array("0", $Query);
		}else{
			return array("-1", "error attribute");
		}
	}
	
	function APICreatCreditUser($user_id, $tarif_cred_id) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			#проверять есть ли несписанные кредиты 
			$Query = $this -> Query("SELECT count(id) as count FROM billing_users_credit WHERE user_id = '" . $user_id . "' AND credit_state = '0' ", false);
			if($Query[0]["count"]==0){
				
				#получаем параметры кредита
				$GetCredit=$this->APIGetTarifCredit("1", $tarif_cred_id);
				if($GetCredit[0]=="0"){
					#проверить кол-во кредитов взятое ранее в этом месяце	
					$Query = $this -> Query("SELECT count(id) as count FROM billing_users_credit WHERE user_id = '" . $user_id . "' AND start_date >= '".date("Y")."-".date("m")."-01 00:00:00'", false);
					if($GetCredit[1][0]["tarif_cred_limitmonth"] > $Query[0]["count"]){
					
						#зачислить кредит на счет и добавить в базу
						$add_summ=$GetCredit[1][0]["tarif_cred_sum"]+$GetCredit[1][0]["tarif_cred_dopsum"];
						$Query = $this -> Query("INSERT INTO billing_users_credit(user_id, tarif_cred_id, sum, start_date, end_date, credit_state) VALUES 
						('".$user_id."', 
							'".$tarif_cred_id."',
							'".$add_summ."', 
							'".date("Y-m-d H:i:s")."', 
							'".date("Y-m-d H:i:s", mktime(date("H"), date("i"), date("s"), date("m"), date("d")+$GetCredit[1][0]["tarif_cred_day"], date("Y")))."', 
						'0'
						)");
						if($Query == "0"){
							$add_cash=$this->APIAddCash($user_id, $GetCredit[1][0]["tarif_cred_sum"], "Активирована услуга обещанный платеж. Комиссия ".$GetCredit[1][0]["tarif_cred_dopsum"]." руб.", "", "billing_credit");
							if($add_cash[0]=="0"){
								$this -> APICreatEvent("billing", $user_id, "", "", "0", "Активирован обещанный платеж. Сумма ".$GetCredit[1][0]["tarif_cred_sum"]." + комиссия ".$GetCredit[1][0]["tarif_cred_dopsum"].", дней ".$GetCredit[1][0]["tarif_cred_day"]."");
								return array("0", "OK");
							}else{
								return array("-6", $add_cash[1]);
							}	
						}else{
							return array("-5", mysql_error());
						}
					}else{
						return array("-4", "Превышено кол-во активаций обещанного платежа.");
					}
				}else{
					return array("-3", "Ошибка получения параметров кредита.");
				}
			}else{
				return array("-2", "Есть активный обещанный платеж");
			}
		}else{
			return array("-1", "Error attribute");
		}
	}
	
	function APIGetCreditUser($user_id="", $active="0", $date_start="", $date_stop=""){
		if ($this -> CheckArgsArray(func_get_args(), array(1))) {
			$Query_SQL=array();
			$Query_TEXT="";
			if(!empty($user_id)) $Query_SQL[]="user_id = '".$user_id."'";
			if(!empty($date_start)) $Query_SQL[]="start_date >= '".$date_start."'";
			if(!empty($date_stop)) $Query_SQL[]="end_date <= '".$date_stop."'";
			
			foreach ($Query_SQL as $Query_Key => $Query_Val){
				if(isset($Query_SQL[$Query_Key+1])){
					$Query_TEXT.=$Query_Val." AND ";
				}else{
					$Query_TEXT.=$Query_Val;
				}
			}
			
			$Query = $this -> Query("SELECT * FROM billing_users_credit WHERE credit_state = '".$active."' ".($Query_TEXT != "" ? 'AND '.$Query_TEXT.'' : '')." ", false);
			if(is_array($Query)){
				return array("0", $Query);
			}else{
				return array("-2", "No entries!");
			}
			
		}else{
			return array("-1", "Error attribute");
		}
	}
		

	/*
	==================================
	 Функции для работы с заявками на смену тарифного плана
	 APICreatRequestTariff, APISetRequestTariff
	==================================
	*/
	
	function APICreatRequestTariff($user_id, $tarif_future) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			#проверим существование абонента
			$User = $this -> APIGetBillProp(3, $user_id);
			if (is_array($User)) {
				#проверять есть ли существующие заявки 
				$Query = $this -> Query("SELECT count(id) as count FROM billing_users_request_tariff WHERE user_id = '" . $user_id . "' AND status <= 0 ", false);
				if($Query[0]["count"]==0){
					
					#получаем параметры выбранного тарифа
					$GetFutureTariff=$this->APIGetTarif(2, $tarif_future);
					#получаем параметры абонентского тарифа
					$GetCurrentTariff=$this->APIGetTarif(2, $User[0]["tarif_id"]);
					$GetCurrTarChange = json_decode($GetCurrentTariff[0]["tarif_change"], true);
					if (json_last_error() != JSON_ERROR_NONE) { $GetCurrTarChange=array(); }
					
					if(is_array($GetFutureTariff[0]) && is_array($GetCurrentTariff[0])){
						#проверим параметры перехода на выбранный тариф у текущего тарифа
						if (isset($GetCurrTarChange[$GetFutureTariff[0]["tarif_id"]])){
							$sum = $GetCurrTarChange[$GetFutureTariff[0]["tarif_id"]]["change_sum"];
							$change_type = $GetCurrTarChange[$GetFutureTariff[0]["tarif_id"]]["change_type"];
							#проверим принадлежность тарифов к одному признаку группировки
							if ($GetFutureTariff[0]["grtarif_id"] == $GetCurrentTariff[0]["grtarif_id"]){
								#узнаем вверх или вниз проиходит переход а так же стоимость перехода
								if ((int)$GetCurrentTariff[0]["tarif_ves"]>(int)$GetFutureTariff[0]["tarif_ves"]) {
									$tarif_updown=0;
								}else{
									$tarif_updown=1;
								}
								$Query = $this -> Query("INSERT INTO billing_users_request_tariff(user_id, tarif_future, tarif_updown, sum, change_type, date_request, date_done, status) VALUES 
								('".$user_id."', '".$tarif_future."','".$tarif_updown."', '".$sum."', '".$change_type."', '".date("Y-m-d H:i:s")."', '', '0')");
								if($Query == "0"){
									return array("0", "OK");	
								}else{
									return array("-6", mysql_error());
								}
							}else{
								return array("-5", "Тарифы принадлежат разным признакам группировки");
							}	
						}else{
							return array("-4", "Ошибка получения данных тарифов");
						}
					}else{
						return array("-4", "Ошибка получения данных тарифов");
					}
				}else{
					return array("-3", "Уже есть активные заявки, отмените существующие.");
				}
			}else{
				return array("-2", "Ошибка получения данных по абоненту.");
			}
		}else{
			return array("-1", "Error attribute");
		}
	}
	
	function APISetRequestTariff($user_id, $status) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			#проверим существование абонента
			$User = $this -> APIGetBillProp(3, $user_id);
			if (is_array($User)) {
				#проверять есть ли существующие заявки у абонента
				$Query = $this -> Query("SELECT count(id) as count FROM billing_users_request_tariff WHERE user_id = '" . $user_id . "' AND status <= 0 ", false);
				if($Query[0]["count"]==1){
					$Query = $this -> Query("UPDATE billing_users_request_tariff SET status='".$status."' ".($status>0 ? ", date_done='".date("Y-m-d H:i:s")."' " : "")." WHERE user_id = '" . $user_id . "' AND status <= 0");
					if($Query == "0"){
						return array("0", "OK");	
					}else{
						return array("-4", mysql_error());
					}
					
				}else{
					return array("-3", "Нет заявок");
				}
			}else{
				return array("-2", "Ошибка получения данных по абоненту.");
			}
		}else{
			return array("-1", "Error attribute");
		}
	}
	
	function APIGetRequestTariff($user_id="", $status=array("1", "2"), $date_request1="", $date_request2=""){
		if (is_array($status)) {
			$Query_SQL=array();
			$Query_TEXT="";
			$Query_STATUS="";
			foreach ($status as $k => $v) {
				if (isset($status[$k+1])) {
					$Query_STATUS.="status = '".$v."' OR ";
				}else{
					$Query_STATUS.="status = '".$v."'";
				}
			}

			if(!empty($user_id)) $Query_SQL[]="billing_users_request_tariff.user_id = '".$user_id."'";
			if(!empty($date_request1)) $Query_SQL[]="date_request >= '".$date_request1."'";
			if(!empty($date_request2)) $Query_SQL[]="date_request <= '".$date_request2."'";
			
			foreach ($Query_SQL as $Query_Key => $Query_Val){
				if(isset($Query_SQL[$Query_Key+1])){
					$Query_TEXT.=$Query_Val." AND ";
				}else{
					$Query_TEXT.=$Query_Val;
				}
			}
			
			
			$Query = $this -> Query("SELECT billing_users_request_tariff.*, billing_users.user_name, billing_tarification.tarif_name FROM billing_users_request_tariff 
			LEFT JOIN billing_users ON billing_users.user_id = billing_users_request_tariff.user_id 
			LEFT JOIN billing_tarification ON billing_tarification.tarif_id = billing_users_request_tariff.tarif_future
			WHERE (".$Query_STATUS.") ".($Query_TEXT != "" ? 'AND '.$Query_TEXT.'' : '')." ", false);
			if(is_array($Query)){
				return array("0", $Query);
			}else{
				return array("-2", "");
			}
			
		}else{
			return array("-1", "Error attribute");
		}
	}
	/*
	 ==================================
	 Функции для работы с Секторами/vlan
	 APICreatSector, APIGetSector, APISetSector
	 ==================================
	 */
	#создание сектора
	function APICreatSector($sector_name, $sector_vlanid, $sector_network, $sector_netmask, $sector_dns1, $sector_dns2, $sector_broadcast, $sector_gateway) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1, 2, 3, 4, 6))) {
			$Res = $this -> Query("INSERT INTO billing_sector(sector_id, sector_name, sector_vlanid, sector_network, sector_netmask, sector_dns1, sector_dns2, sector_broadcast, sector_gateway) VALUES 
			('" . $this -> GUID() . "', 
			'" . $sector_name . "',
			'" . $sector_vlanid . "', 
			'" . $sector_network . "', 
			'" . $sector_netmask . "', 
			'" . $sector_dns1 . "',
			'" . $sector_dns2 . "',
			'" . $sector_broadcast . "',
			'" . $sector_gateway . "')");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#список секторов
	function APIGetSector($obj=NULL) {
		if (isset($obj)) {
			$query = $this -> Query("SELECT * FROM billing_sector WHERE sector_id = '".$obj."' LIMIT 1", false);
		}else{
			$query = $this -> Query("SELECT * FROM billing_sector", false);
		}
		return ($this->sql_errno==0 ? $query : array($this->sql_errno, $this->sql_error));
	}

	#редактирование сектора
	function APISetSector($obj, $sector_name, $sector_vlanid, $sector_network, $sector_netmask, $sector_dns1, $sector_dns2, $sector_broadcast, $sector_gateway, $sector_dhcp_onoff, $sector_dhcpdyn, $sector_dhcpstat, $sector_ippool, $sector_ippool_name) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1, 2, 3, 4, 5, 7))) {
			$Res = $this -> Query("UPDATE billing_sector SET 
			sector_name = '" . $sector_name . "',
			sector_vlanid = '" . $sector_vlanid . "',
			sector_network = '" . $sector_network . "',
			sector_netmask = '" . $sector_netmask . "',
			sector_dns1 = '" . $sector_dns1 . "',
			sector_dns2 = '" . $sector_dns2 . "',
			sector_broadcast = '" . $sector_broadcast . "',
			sector_gateway = '" . $sector_gateway . "',
			sector_dhcp_onoff = '" . $sector_dhcp_onoff . "',
			sector_dhcpdyn = '" . $sector_dhcpdyn . "',
			sector_dhcpstat = '" . $sector_dhcpstat . "',
			sector_ippool = '" . $sector_ippool . "',
			sector_ippool_name = '" . $sector_ippool_name . "'
			WHERE sector_name = '" . $obj . "' or sector_id = '" . $obj . "'");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#удаление тарифа
	function APIDelSector($obj) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#связанность между sector_id и billing_users.user_sector_id
			$this -> Query("DELETE FROM billing_sector WHERE sector_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This sector linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}
	/*
	 ==================================
	 Функции для работы с населенными пунктами
	 APICreatNasp, APIGetNasp, APISetSNasp, APIDelNasp
	 ==================================
	 */
	#создание нас. пункта
	function APICreatNasp($nasp_name, $nasp_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$Res = $this -> Query("INSERT INTO billing_nasp(nasp_id, nasp_name, nasp_comment) VALUES 
			('" . $this -> GUID() . "', 
			'" . $nasp_name . "',
			'" . $nasp_comment . "')");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#список нас. пункта [type int(2), obj string - название/id Сектора, sector integer(1,2) ]
	function APIGetNasp($type, $obj) {
		if ($type == 2) {
			#список групп
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT billing_nasp.id, billing_nasp.nasp_id, billing_nasp.nasp_name, billing_nasp.nasp_comment
				FROM billing_nasp order by billing_nasp.nasp_name", false);
			} else {
				$Res = $this -> Query("SELECT billing_nasp.id, billing_nasp.nasp_id, billing_nasp.nasp_name, billing_nasp.nasp_comment
				FROM billing_nasp
				WHERE billing_nasp.nasp_id = '" . $obj . "' or billing_nasp.nasp_name = '" . $obj . "' LIMIT 1", false);
			}
		} else {
			#аргементы не определены
			$Res = -2;
		}

		return $Res;
	}

	#редактирование нас. пункта
	function APISetNasp($obj, $nasp_name, $nasp_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$Res = $this -> Query("UPDATE billing_nasp SET 
			nasp_name = '" . $nasp_name . "',
			nasp_comment = '" . $nasp_comment . "'
			WHERE nasp_name = '" . $obj . "' or nasp_id = '" . $obj . "'");
		} else {
			$Res = -1;
		}
		return $Res;
	}

		#удаление тарифа
	function APIDelNasp($obj) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$this -> Query("DELETE FROM billing_nasp WHERE nasp_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This location linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}

	/*
	 ==================================
	 Функции для работы с улицами
	 APICreatUlica, APIGetUlica, APISetUlica, APIDelUlica
	 ==================================
	 */
	#создание улиц
	function APICreatUlica($ulica_name, $ulica_nasp, $ulica_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$Res = $this -> Query("INSERT INTO billing_ulica(ulica_id, ulica_name, ulica_nasp, ulica_comment) VALUES 
			('" . $this -> GUID() . "', 
			'" . $ulica_name . "',
			'" . $ulica_nasp . "',
			'" . $ulica_comment . "')");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#список улиц
	function APIGetUlica($type, $obj, $nasp = false) {
		if ($type == 2) {
			#список групп
			if ($obj == NULL) {
				if ($nasp != false) {
					$Res = $this -> Query("SELECT billing_ulica.*,billing_nasp.nasp_id, billing_nasp.nasp_name
					FROM billing_ulica	
					LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp
					WHERE billing_nasp.nasp_id = '" . $nasp . "' or billing_nasp.nasp_name = '" . $nasp . "' ORDER BY billing_ulica.ulica_nasp, billing_ulica.ulica_name", false);
				} else {
					$Res = $this -> Query("SELECT billing_ulica.*, billing_nasp.nasp_id, billing_nasp.nasp_name 
					FROM billing_ulica	
					LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp ORDER BY billing_ulica.ulica_nasp, billing_ulica.ulica_name", false);
				}
			} else {
				$Res = $this -> Query("SELECT billing_ulica.*, billing_nasp.nasp_id, billing_nasp.nasp_name 
				FROM billing_ulica	
				LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp
				WHERE billing_ulica.ulica_id = '" . $obj . "' or billing_ulica.ulica_name = '" . $obj . "' LIMIT 1", false);
			}
		} else {
			#аргементы не определены
			$Res = -2;
		}
		return $Res;
	}

	#редактирование улицы
	function APISetUlica($obj, $ulica_name, $ulica_nasp, $ulica_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1, 2))) {
			$Res = $this -> Query("UPDATE billing_ulica SET 
			ulica_name = '" . $ulica_name . "',
			ulica_nasp = '" . $ulica_nasp . "',
			ulica_comment = '" . $ulica_comment . "'
			WHERE ulica_name = '" . $obj . "' or ulica_id = '" . $obj . "'");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#удаление улицы
	function APIDelUlica($obj) {
		//Связанные индексы с billing_nasp
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$this -> Query("DELETE FROM billing_ulica WHERE ulica_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This location linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}

	/*
	 ==================================
	 Функции для работы с домами
	 APICreatDom, APIGetDom, APISetDom, APIDelDom
	 ==================================
	 */
	#создание дома
	function APICreatDom($dom_name, $dom_letter, $dom_ulica, $dom_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 2))) {
			$Res = $this -> Query("INSERT INTO billing_dom(dom_id, dom_name, dom_letter, dom_ulica, dom_comment) VALUES 
			('" . $this -> GUID() . "', 
			'" . $dom_name . "',
			'" . $dom_letter . "',
			'" . $dom_ulica . "',
			'" . $dom_comment . "')");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#список дома
	function APIGetDom($type, $obj, $ulica = false) {
		if ($type == 2) {
			#список групп
			if ($obj == NULL) {
				#показать дома для конкретной улицы
				if ($ulica != false) {
					$Res = $this -> Query("SELECT billing_dom.*,
					billing_ulica.ulica_id, 
					billing_ulica.ulica_name
					FROM billing_dom	
					LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_dom.dom_ulica
					WHERE billing_ulica.ulica_id = '" . $ulica . "' or billing_ulica.ulica_id  = '" . $ulica . "'", false);
				} else {
					$Res = $this -> Query("SELECT billing_dom.*, billing_ulica.ulica_id, billing_ulica.ulica_name, billing_ulica.ulica_nasp, billing_nasp.nasp_name
					FROM billing_dom	
					LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_dom.dom_ulica 
					LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp
					ORDER BY billing_ulica.ulica_name ASC, billing_dom.dom_name ASC", false);
				}
			} else {
				$Res = $this -> Query("SELECT billing_dom.*, billing_ulica.ulica_id, billing_ulica.ulica_name 
				FROM billing_dom	
				LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_dom.dom_ulica
				WHERE billing_dom.dom_id = '" . $obj . "' or billing_dom.dom_name = '" . $obj . "' LIMIT 1", false);
			}
		} else {
			#аргементы не определены
			$Res = -2;
		}
		return $Res;
	}

	#редактирование дома
	function APISetDom($obj, $dom_name, $dom_letter, $dom_ulica, $dom_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1, 3))) {
			$Res = $this -> Query("UPDATE billing_dom SET 
			dom_name = '" . $dom_name . "',
			dom_letter = '" . $dom_letter . "',
			dom_ulica = '" . $dom_ulica . "',
			dom_comment = '" . $dom_comment . "'
			WHERE dom_name = '" . $obj . "' or dom_id = '" . $obj . "'");
		} else {
			$Res = -1;
		}
		return $Res;
	}

	#удаление дома
	function APIDelDom($obj) {
		//Связанные индексы с billinf_ulica
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$this -> Query("DELETE FROM billing_dom WHERE dom_id = '".$obj."' LIMIT 1;");
			if ($this->sql_errno==0){
				return array(0, "OK");
			}elseif($this->sql_errno==1451){
				return array(-1, "This location linked to objects! Remove them.");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		}else{
			return array(-1, "No parameters");
		}
	}

	/*
	 ==================================
	 Функции для работы с оборудованием
	 APICreatSwitch, APIGetSwitch, APISetSwitch, APIDelSwitch, APIGetSnmpTemp
	 ==================================
	 */
	#создание оборудования
	function APICreatSwitch($query) {
		if ($this -> CheckArgsArray($query, array("switch_name", "switch_ip"))) {
			$Res = $this -> Query("INSERT INTO billing_switch(switch_id, switch_name, switch_type_id, switch_ip, switch_uplink, switch_log, switch_pas, switch_ulica, switch_dom, switch_place, switch_comment, switch_snmp, switch_snmpcollectmac, switch_snmpcollectport, switch_snmpcollecttraff, switch_snmpcollecterror, switch_snmpcomunity, switch_monit) VALUES 
			('".$this -> GUID()."', 
			'".$query["switch_name"]."',
			'".$query["switch_type_id"]."',
			'".$query["switch_ip"]."',
			'".$query["switch_uplink"]."',
			'".$query["switch_log"]."',
			'".$query["switch_pas"]."',
			'".$query["switch_ulica"]."',
			'".$query["switch_dom"]."',
			'".$query["switch_place"]."',
			'".$query["switch_comment"]."',
			'".$query["switch_snmp"]."',
			'".$query["switch_snmpcollectmac"]."',
			'".$query["switch_snmpcollectport"]."',
			'".$query["switch_snmpcollecttraff"]."',
			'".$query["switch_snmpcollecterror"]."',
			'".$query["switch_snmpcomunity"]."',
			'".$query["switch_monit"]."'
			)");
			if ($Res != 0) {
				return array($Res, mysql_error());
			} 
		}else{
			return array(-1, "Error attribute");
		}
		return array(0, "OK");
	}
	#получить список типа оборудования
	function APIGetSwitchType($obj=NULL) {
		#dlink 1..99
		$SwitchTypeArray[1]=array("type"=> "SWITCH", "switch_type"=>"D-link 3526",		"walk"=>"snmpbulkwalk",	"switch_ports"=>"26", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[2]=array("type"=> "SWITCH", "switch_type"=>"D-link 3028", 		"walk"=>"snmpbulkwalk",	"switch_ports"=>"28", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[3]=array("type"=> "SWITCH", "switch_type"=>"D-link 1210-28", 	"walk"=>"snmpbulkwalk", "switch_ports"=>"28", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[4]=array("type"=> "SWITCH", "switch_type"=>"D-link 3200 28F", 	"walk"=>"snmpbulkwalk",	"switch_ports"=>"28", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[5]=array("type"=> "SWITCH", "switch_type"=>"D-link 3100", 		"walk"=>"snmpbulkwalk", "switch_ports"=>"24", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[6]=array("type"=> "SWITCH", "switch_type"=>"SNR 2970 SFP", 	"walk"=>"snmpbulkwalk", "switch_ports"=>"48", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[7]=array("type"=> "SWITCH", "switch_type"=>"D-link 3052", 		"walk"=>"snmpbulkwalk",	"switch_ports"=>"52", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		$SwitchTypeArray[8]=array("type"=> "SWITCH", "switch_type"=>"D-link 1228/ME", 	"walk"=>"snmpbulkwalk", "switch_ports"=>"28", "switch_mibfdb" =>"1.3.6.1.2.1.17.7.1.2.2.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");

		#cisco 100..150
		$SwitchTypeArray[100]=array("type"=> "SWITCH", "switch_type"=>"Cisco Catalyst 3550-48", "walk"=>"snmpbulkwalk", "switch_ports"=>"50", "switch_mibfdb" =>"1.3.6.1.2.1.17.4.3.1.2", "switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>".1.3.6.1.2.1.2.2.1.5", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>".1.3.6.1.2.1.2.2.1.14", "switch_ifouterr"=>".1.3.6.1.2.1.2.2.1.20", "mib_uptime"=>".1.3.6.1.2.1.1.3", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10");
		
		#bdcom 151..200
		$SwitchTypeArray[151]=array("type"=> "OLT", "switch_type"=>"OLT BDCOM 3310B", "walk"=>"snmpwalk", "switch_ports"=>"256", "switch_mibfdb" =>"1.3.6.1.4.1.3320.152.1.1.1", 	"switch_mibport" =>"1.3.6.1.2.1.2.2.1.8", "switch_ifspeed"=>"ifspeed", "switch_ifdescr"=>"ifDescr", "ifindex"=> ".1.3.6.1.2.1.17.1.4.1.2", "switch_ifinerr"=>"ifInErrors", "switch_ifouterr"=>"ifOutErrors", "mib_uptime"=>"sysUpTime.0", "switch_port_tx"=>".1.3.6.1.2.1.2.2.1.16", "switch_port_rx"=>".1.3.6.1.2.1.2.2.1.10", "onu_mac"=>"iso.3.6.1.4.1.3320.101.10.1.1.3", "onu_rx_power"=>"iso.3.6.1.4.1.3320.101.10.5.1.5", "onu_vendor"=>"iso.3.6.1.4.1.3320.101.10.1.1.1");

		if (isset($obj)) {
			#возвращаем выбранный
			return array(0, $SwitchTypeArray[$obj]);
		} else {
			#возвращаем весь список типов
			return array(0, $SwitchTypeArray);
			
		}
	}

	#список оборудования
	function APIGetSwitch($obj=NULL) {
		if (isset($obj)) {
			#возвращаем выбранный
			$Res = $this -> Query("SELECT billing_switch.*,
				billing_ulica.ulica_id, 
				billing_ulica.ulica_name,
				billing_ulica.ulica_nasp,
				billing_nasp.nasp_name,
				billing_dom.dom_id, 
				billing_dom.dom_name, 	
				billing_dom.dom_letter 					
				FROM billing_switch	
				LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_switch.switch_ulica
				LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp
				LEFT JOIN billing_dom ON billing_dom.dom_id = billing_switch.switch_dom
				WHERE billing_switch.switch_id = '".$obj."' LIMIT 1", false);
				if(!is_array($Res)) return array("-2", mysql_error());
		}else{
			$Res = $this -> Query("SELECT billing_switch.*,
				billing_ulica.ulica_id, 
				billing_ulica.ulica_name,
				billing_ulica.ulica_nasp,
				billing_nasp.nasp_name,
				billing_dom.dom_id, 
				billing_dom.dom_name, 	
				billing_dom.dom_letter 					
				FROM billing_switch	
				LEFT JOIN billing_ulica ON billing_ulica.ulica_id = billing_switch.switch_ulica
				LEFT JOIN billing_nasp ON billing_nasp.nasp_id = billing_ulica.ulica_nasp
				LEFT JOIN billing_dom ON billing_dom.dom_id = billing_switch.switch_dom
				ORDER BY billing_ulica.ulica_nasp, billing_switch.switch_ulica, billing_dom.dom_name", false);
				if(!is_array($Res)) return array("-1", $Res);
		}					
		return array(0, $Res);	
	}

	#редактирование оборудования
	function APISetSwitch($query) {
		$str = "";
		$i = 0;
		if ($this -> CheckArgsArray($query, array("obj"))) {
			$obj = $query["obj"];
			unset($query["obj"]);
			foreach	($query as $key => $value) {
				$i++;
				$str.= " ".$key." = '".$value."' ".($i<count($query) ? ',': '')."";
			}
			$Res = $this -> Query("UPDATE billing_switch SET ".$str." WHERE switch_id = '" . $obj . "'");
			if ($Res != 0) {
				return array($Res, mysql_error());
			} 
		}else{
			return array(-1, "No required parameters");
		}
		return array(0, "OK");
	}

	#удаление оборудования
	function APIDelSwitch($obj) {
		//! ПРЕДУСМОТРЕТЬ УДАЛЕНИЕ СВЯЗАННЫХ ДАННЫХ
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$GetSwitch=$this->APIGetSwitch($obj);
			if($GetSwitch[0]=="0"){
				$Res = $this -> Query("DELETE FROM billing_switch WHERE switch_id = '".$obj."'");
				if ($Res != 0) {
					return array($Res, mysql_error());
				} 
			}else{
				return array($Res, mysql_error());
			}
		} else {
			return array("-1", "Not all attributes are specified");
		}
		return array(0, "OK");
	}
	/*
	 ==================================
	 Функции для работы с SNMP данными
	 APICreatSwitch, APIGetSwitch, APISetSwitch, APIDelSwitch, APIGetSnmpTemp
	 ==================================
	*/
	 
	// список собранных мак-адресов с коммутаторов. 1 - поиск по свитчу, 2 - поиск по маку
	function APIGetSwitchMac ($switch_id='', $mac=''){
		$param = array ("billing_switch_mac.switch_id" => $switch_id, "billing_switch_mac.mac" => $mac);
		$param = array_diff($param, array('')); // удалим пустые элементы массива
		$param_new = array ();
		foreach ($param as $param_k => $param_val) {
			$param_new[] = "{$param_k} = '{$param_val}'";
		}		
		
		$param_where = (count($param_new)>=1) ? 'WHERE '.implode (" AND ", $param_new) : '';
		
		$query = $this -> Query("SELECT billing_users.user_id, billing_users.user_name, billing_switch_mac.* FROM billing_switch_mac 
		LEFT JOIN billing_users ON billing_users.user_mac = billing_switch_mac.mac {$param_where} order by last_seen desc", false);
		
		return ($this->sql_errno==0 ? array(0, $query) : array($this->sql_errno, $this->sql_error));
		
	}
	#список временных записей по сбору данных с оборудования через SNMP
	function APIGetSnmpTemp($obj=NULL) {
		$return=array();
		if (isset($obj)) {
			$query = $this -> Query("SELECT * FROM billing_switch_snmp_temp WHERE switch_id = '".$obj."'", false);
		}else{
			$query = $this -> Query("SELECT * FROM billing_switch_snmp_temp", false);
		}
		if($this->sql_errno==0){
			foreach(@$query as $v){
				$return[$v["switch_id"]]=$v;
			}
		}else{ 
			$return=array($this->sql_errno, $this->sql_error);
		}
		return $return;
	}
	
	function APISetSnmpData($write=false, $switch_temp_arr='', $switch_id='', $switch_type='', $switch_data='') {
		$write_sql_temp=''; $write_sql_data='';
		switch ($switch_type) {
			case "PORT_TX" :
				echo "PORT_TX\n";
				#смотрим есть ли в temp -> обновляем 
				if (isset($switch_temp_arr[$switch_id])){
					$json_ports_tx=json_decode($switch_temp_arr[$switch_id]["json_ports_tx"], true);
					#сравниваем массив предыдущих и текущих значений, находим разницу и работаем с ней
					$switch_arr_diff=array_diff($switch_data, $json_ports_tx);
					#если текущее значение у порта меньше предыдущего, значит счетчик сброшен. В Data порт не добавляем
					foreach (@$switch_arr_diff as $switch_arr_diff_port => $switch_arr_diff_val) {
						if ($switch_arr_diff_val < $json_ports_tx[$switch_arr_diff_port]){
							print($switch_arr_diff_port." - fail \n");
						}else{
							#находим делту
							print($switch_arr_diff_port." - success ");
							echo ((( ($switch_arr_diff_val*8)-($json_ports_tx[$switch_arr_diff_port]*8)) - (time()-strtotime($switch_temp_arr[$switch_id]["time"])))/1048576)." \n";
							#обновляем
							$this->NET_MONIT_INSERT["TEMP"][$switch_id]["port_tx"]=json_encode($switch_data);
						}
					}
					//print_r($json_ports_tx); #пред значение
					//print_r($switch_data); #текущие значения
					//print_r(array_diff($switch_data, $json_ports_tx));
				
				}else{
					#первый input
					$this->NET_MONIT_INSERT["TEMP"][$switch_id]["port_tx"]=json_encode($switch_data);
				}
				break;
			case "PORT_RX":
				echo "PORT_RX\n";
				#смотрим есть ли в temp -> обновляем 
				if (isset($switch_temp_arr[$switch_id])){
				
				}else{
					#первый input
					$this->NET_MONIT_INSERT["TEMP"][$switch_id]["port_rx"]=json_encode($switch_data);
				}
				break;
			CASE "PORT_CHECK":
				echo "PORT_CHECK\n";
				$this->NET_MONIT_INSERT["DATA"][$switch_id]["port_check"]=json_encode($switch_data);
				break;
			
		}
		
		#Записываем в БД
		if ($write==true){
			#пишем в TEMP
			if (is_array(@$this->NET_MONIT_INSERT["TEMP"])){
				$t_count=0;
				foreach ($this->NET_MONIT_INSERT["TEMP"] as $swt_k=> $swt_v){
					$t_count++;
					$write_sql_temp.="('".$swt_k."', '".$swt_v["port_rx"]."', '".$swt_v["port_tx"]."') ".(count($this->NET_MONIT_INSERT["TEMP"])==$t_count ? '':',')."";
				}
				$this -> Query("INSERT INTO billing_switch_snmp_temp(switch_id, json_ports_rx, json_ports_tx) VALUES ".$write_sql_temp." ON DUPLICATE KEY UPDATE json_ports_rx = VALUES(json_ports_rx), json_ports_tx = VALUES(json_ports_tx)");
				if ($this->sql_errno==0){
					//return array(0, "OK");
				}else{
					//return array($this->sql_errno, $this->sql_error);
				}
			}
			#пишем в DATA
			if (is_array(@$this->NET_MONIT_INSERT["DATA"])){
				$d_count=1;
				foreach ($this->NET_MONIT_INSERT["DATA"] as $swd_k=> $swd_v){
					$d_count++;
					if (!isset($swd_v["port_rx"])) $swd_v["port_rx"] = 'NULL';
					if (!isset($swd_v["port_tx"])) $swd_v["port_tx"] = 'NULL';
					if (!isset($swd_v["port_check"])) $swd_v["port_check"] = 'NULL';
					$write_sql_data.="('".$swd_k."', '".date("Y-m-d")."', '".date("H:i:s")."', ".$swd_v["port_rx"].", ".$swd_v["port_tx"].", '".$swd_v["port_check"]."') ".(count($this->NET_MONIT_INSERT["DATA"])==$d_count ? ',':'')."";
				}
				$this -> Query("INSERT INTO billing_switch_snmp(switch_id, date, time, json_ports_rx, json_ports_tx, json_ports_status) VALUES ".$write_sql_data."");
				if ($this->sql_errno!=0){
					//return array(0, "OK");
				}else{
					//return array($this->sql_errno, $this->sql_error);
				}
			}

		}else{

		}
	}

	/*
	 ==================================
	 Функции для работы с NAS/Серверами доступа
	 APICreatNas, APIGetNas, APISetNas, APIDelNas, APIGetNasOnline
	 ==================================
	 */
	#создание оборудования
	function APICreatNas($nas_name, $nas_sector, $nas_ip, $nas_login, $nas_password, $nas_secret, $nas_type, $nas_shaper, $nas_addresslist, $nas_arp, $nas_dhcp) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1, 2, 3, 4))) {
			$Res = $this -> Query("INSERT INTO billing_nas(nas_id, nas_sector_id, nas_name, nas_ip, nas_login, nas_password, nas_secret, nas_type, nas_shaper, nas_arp, nas_addresslist, nas_dhcp) VALUES 
			('" . $this -> GUID() . "', 
			'" . $nas_sector . "',
			'" . $nas_name . "',
			'" . $nas_ip . "',
			'" . $nas_login . "',
			'" . $nas_password . "',
			'" . $nas_secret . "',
			'" . $nas_type . "',
			'" . $nas_shaper . "',
			'" . $nas_arp . "',
			'" . $nas_addresslist . "',
			'" . $nas_dhcp . "
			')");
			if ($Res != 0) {
				return array($Res, mysql_error());
			} else {
				return array($Res, "Success creat NAS-server");
			}
		} else {
			return array("-100", "Not all attributes are specified");
		}
	}

	#список оборудования
	function APIGetNas($type, $obj, $sector_id='') {
		if ($type == 2) {
			#список nas
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT billing_nas.*				
				FROM billing_nas ".((!empty($sector_id)) ? "WHERE billing_nas.nas_sector_id LIKE '%{$sector_id}%'" : "")."" , false);
			} else {
				$Res = $this -> Query("SELECT billing_nas.*				
				FROM billing_nas
				WHERE billing_nas.nas_id = '" . $obj . "' or billing_nas.nas_name = '" . $obj . "' LIMIT 1", false);
			}
		} else {
			#аргементы не определены
			$Res = -2;
		}
		return $Res;
	}

	#редактирование nas
	function APISetNas($query) {
		$str = '';
		$i = 0;
		if ($this -> CheckArgsArray($query, array("obj"))) {
			$obj = $query["obj"];
			unset($query["obj"]);
			foreach	($query as $key => $value) {
				$i++;
				$str.= " ".$key." = '".$value."' ".($i<count($query) ? ',': '')."";
			}
			$this -> Query("UPDATE billing_nas SET ".$str." WHERE nas_id = '".$obj."'");
			return ($this->sql_errno==0 ? array(0, "OK") : array($this->sql_errno, $this->sql_error));
		}else{
			return array(-1, "No required parameters");
		}
	}

	#удаление nas
	function APIDelNas($obj = false, $comment) {
		if ($obj) {
			$QueryLastIP = $this -> APIGetNas(2, $obj);
			$Query = $this -> Query("DELETE FROM billing_nas WHERE 
			nas_id = '" . $obj . "' or nas_name = '" . $obj . "' LIMIT 1;");
			if ($Query != 0) {
				$Res = array($Query, mysql_error());
			} else {
				$Res = array($Query, "Success delete NAS-server");
			}
		} else {
			$Res = array("-100", "Not all attributes are specified");
		}
		return $Res;
	}

	function APIGetNasOnline($obj) {
		$NasObj = $this -> APIGetNas(2, $obj);
		if (!is_array($NasObj)) {
			return array("-1", "Not search nas"); 
		}
		$SortTable = array();
		$SearchAbonent = array();
		// список абонентов с ключами MAC
		$Users = $this->APIGetBillProp(3, '');
		foreach ($Users as $User) {
			if (!empty($User["user_mac"])){
				$SearchAbonent[$User["user_mac"]] = $User;
			}
		} 
		
		if ($NasObj[0]["nas_type"] == "1" or $NasObj[0]["nas_type"] == "2" or $NasObj[0]["nas_type"] == "3" or $NasObj[0]["nas_type"] == "6" or $NasObj[0]["nas_type"] == "7") {
			$this -> NAS("mikrotik");
			$this -> nas -> connect($NasObj[0]["nas_ip"], $NasObj[0]["nas_login"], $NasObj[0]["nas_password"]);
			//$OnlineTable = $this -> nas -> comm("/ip/hotspot/active/print/stats");
		
			$this -> nas -> write('/ip/hotspot/active/print');
			$time = array_sum( explode( ' ' , microtime() ) );
			#$this -> nas -> write('=stats=');
			$OnlineTable = $this -> nas ->read();
			
			//print_r($OnlineTable);
			
			
			foreach ($OnlineTable as $OnlineTableKey => $OnlineTableValue) {
				preg_match_all("/\d+/", $OnlineTableValue["uptime"], $matches);
				if (count($matches[0]) == 3) {
					$time = (($matches[0][0]*60)*60)+($matches[0][1]*60)+$matches[0][2];
				}elseif (count($matches[0]) == 2) {
					$time = ($matches[0][0]*60)+$matches[0][1];
				}elseif (count($matches[0]) == 1) {
					$time = $matches[0][0];
				}
				$SortTable[] = array(
					".id" => $OnlineTableValue[".id"], 
					"user_id" => $SearchAbonent[$OnlineTableValue["user"]]["user_id"], 
					"abonent" => $SearchAbonent[$OnlineTableValue["user"]]["user_name"], 
					"dogovor" => $SearchAbonent[$OnlineTableValue["user"]]["user_dogovor"], 
					"user" => $OnlineTableValue["user"], 
					"address" => $OnlineTableValue["address"], 
					"pptp_address" => "", 
					"uptime" => $OnlineTableValue["uptime"], 
					"session" => $OnlineTableValue["session-time-left"],
					"bytes-in" => $OnlineTableValue["bytes-in"],
					"bytes-out" => $OnlineTableValue["bytes-out"],
					"time" => $time
				);
			}
			return array("0", $SortTable);
		#ppp mikrotik
		} elseif ($NasObj[0]["nas_type"] == "4" or $NasObj[0]["nas_type"] == "5") {
			// ...
		} else {
			return array("-1", "Not support online table");
		}
	}
	function APIGetNasInfoAbon($nas_id = '', $user_id = '') {
		$Users = $this->APIGetBillProp(3, $user_id);
		$mik_param = array ();
		$return_table = array();
		$SearchAbonent = array();

		// не указан нас, значит ищем по сегменту указаного абонента
		if (empty($nas_id) && !empty($user_id)){
			// узнать принадлежность сегмента nas'у
			if (isset($Users[0]["user_sector_id"])){
				$NasObj = $this -> APIGetNas(2, '', $Users[0]["user_sector_id"]);
				$mik_param["?mac-address"] = $Users[0]["user_mac"];
			}else{
				return array("-1", "Abonent not found"); 
			}
		//nas указан
		}elseif (!empty($nas_id)) {
			$NasObj = $this -> APIGetNas(2, $nas_id);
		}else{
			return array("-1", "Invalid parametr function"); 
		}
				
		if (!is_array($NasObj)) {
			return array("-1", "Not search nas"); 
		}
		
		foreach ($Users as $User) {
			if (!empty($User["user_mac"])){
				$SearchAbonent[$User["user_mac"]] = $User;
			}
		} 
		
		if ($NasObj[0]["nas_type"] == "1" or $NasObj[0]["nas_type"] == "2" or $NasObj[0]["nas_type"] == "3" or $NasObj[0]["nas_type"] == "6" or $NasObj[0]["nas_type"] == "7") {
			$this -> NAS("mikrotik");
			$this -> nas -> connect($NasObj[0]["nas_ip"], $NasObj[0]["nas_login"], $NasObj[0]["nas_password"]);
			
			/*
			$this -> nas ->write('/ping',false);
			$this -> nas ->write('=arp-ping=yes',false);
			$this -> nas ->write('=interface='.iconv( "utf-8", "windows-1251","LAG1").'',false);
			$this -> nas ->write('=address=192.168.88.249',false); 
			$this -> nas ->write('=count=3',false);
			$this -> nas ->write('=interval=1');
			$ARRAY4 = $this -> nas -> read(false);
			echo "<pre>". print_r($ARRAY4) ." </pre>";
			$this -> nas ->disconnect();
			exit;
			*/

			$OnlineTable = $this -> nas -> comm("/ip/dhcp-server/lease/print", $mik_param);
			
			

			foreach ($OnlineTable as $OnlineTableKey => $OnlineTableValue) {
				$OnlineTableValue["expires-after"] = str_replace (array("w", "d", "h","m","s"), array("н. ", "д. ", "ч. ","м. ","с. "), $OnlineTableValue["expires-after"]);
				$OnlineTableValue["last-seen"] = str_replace (array("w", "d", "h","m","s"), array("н. ", "д. ", "ч. ","м. ","с. "), $OnlineTableValue["last-seen"]);
				
				$return_table[] = array(
					".id" => $OnlineTableValue[".id"], 
					"user_id" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_id"], 
					"abonent" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_name"], 
					"dogovor" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_dogovor"], 
					"status" => $OnlineTableValue["status"],  
					"expires-after" => $OnlineTableValue["expires-after"], 
					"last-seen" => $OnlineTableValue["last-seen"], 
					"address" => $OnlineTableValue["address"],
					"active-address" => $OnlineTableValue["active-address"],
					"active-mac-address" => $OnlineTableValue["active-mac-address"],
					"mac-address" => $OnlineTableValue["mac-address"],
					"host-name" => $OnlineTableValue["host-name"],
					"dynamic" => $OnlineTableValue["dynamic"],
				);
			}
			return array("0", $return_table);
		#ppp mikrotik
		} elseif ($NasObj[0]["nas_type"] == "4" or $NasObj[0]["nas_type"] == "5") {
			// ...
		} else {
			return array("-1", "Not support online table");
		}
	}
	
	function APIGetDhcpOnline($nas_id = '', $user_id = '') {
		$Users = $this->APIGetBillProp(3, $user_id);
		$mik_param = array ();
		$return_table = array();
		$SearchAbonent = array();

		// не указан нас, значит ищем по сегменту указаного абонента
		if (empty($nas_id) && !empty($user_id)){
			// узнать принадлежность сегмента nas'у
			if (isset($Users[0]["user_sector_id"])){
				$NasObj = $this -> APIGetNas(2, '', $Users[0]["user_sector_id"]);
				$mik_param["?mac-address"] = $Users[0]["user_mac"];
			}else{
				return array("-1", "Abonent not found"); 
			}
		//nas указан
		}elseif (!empty($nas_id)) {
			$NasObj = $this -> APIGetNas(2, $nas_id);
		}else{
			return array("-1", "Invalid parametr function"); 
		}
				
		if (!is_array($NasObj)) {
			return array("-1", "Not search nas"); 
		}
		
		foreach ($Users as $User) {
			if (!empty($User["user_mac"])){
				$SearchAbonent[$User["user_mac"]] = $User;
			}
		} 
		
		if ($NasObj[0]["nas_type"] == "1" or $NasObj[0]["nas_type"] == "2" or $NasObj[0]["nas_type"] == "3" or $NasObj[0]["nas_type"] == "6" or $NasObj[0]["nas_type"] == "7") {
			$this -> NAS("mikrotik");
			$this -> nas -> connect($NasObj[0]["nas_ip"], $NasObj[0]["nas_login"], $NasObj[0]["nas_password"]);
			
			/*
			$this -> nas ->write('/ping',false);
			$this -> nas ->write('=arp-ping=yes',false);
			$this -> nas ->write('=interface='.iconv( "utf-8", "windows-1251","LAG1").'',false);
			$this -> nas ->write('=address=192.168.88.249',false); 
			$this -> nas ->write('=count=3',false);
			$this -> nas ->write('=interval=1');
			$ARRAY4 = $this -> nas -> read(false);
			echo "<pre>". print_r($ARRAY4) ." </pre>";
			$this -> nas ->disconnect();
			exit;
			*/

			$OnlineTable = $this -> nas -> comm("/ip/dhcp-server/lease/print", $mik_param);
			
			

			foreach ($OnlineTable as $OnlineTableKey => $OnlineTableValue) {
				$OnlineTableValue["expires-after"] = str_replace (array("w", "d", "h","m","s"), array("н. ", "д. ", "ч. ","м. ","с. "), $OnlineTableValue["expires-after"]);
				$OnlineTableValue["last-seen"] = str_replace (array("w", "d", "h","m","s"), array("н. ", "д. ", "ч. ","м. ","с. "), $OnlineTableValue["last-seen"]);
				
				$return_table[] = array(
					".id" => $OnlineTableValue[".id"], 
					"user_id" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_id"], 
					"abonent" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_name"], 
					"dogovor" => $SearchAbonent[$OnlineTableValue["mac-address"]]["user_dogovor"], 
					"status" => $OnlineTableValue["status"],  
					"expires-after" => $OnlineTableValue["expires-after"], 
					"last-seen" => $OnlineTableValue["last-seen"], 
					"address" => $OnlineTableValue["address"],
					"active-address" => $OnlineTableValue["active-address"],
					"active-mac-address" => $OnlineTableValue["active-mac-address"],
					"mac-address" => $OnlineTableValue["mac-address"],
					"host-name" => $OnlineTableValue["host-name"],
					"dynamic" => $OnlineTableValue["dynamic"],
				);
			}
			return array("0", $return_table);
		#ppp mikrotik
		} elseif ($NasObj[0]["nas_type"] == "4" or $NasObj[0]["nas_type"] == "5") {
			// ...
		} else {
			return array("-1", "Not support online table");
		}
	}
	
	
	/*
	 ==================================
	 Функции для работы с Правами доступа
	 APICreatAccess, APISetAccess, APIDelAccess
	 ==================================
	 */
	#создание учетной записи
	function APICreatAccess($array_value) {
		if (is_array($array_value)) {
			$access_access = "";
			$access_name = $array_value[0]["value"];
			$access_pass = $array_value[1]["value"];
			$access_comm = $array_value[2]["value"];
			if (strlen($access_name) > 1 && strlen($access_pass) > 1) {
				#sql access
				foreach ($array_value as $array_value) {
					if ($array_value["name"] == "access_access") {
						$access_access .= $array_value["value"] . ";";
					}
				}
				#sql insert
				$Res = $this -> Query("INSERT INTO billing_access(access_id, access_name, access_pass, access, comment) VALUES 
				('" . $this -> GUID() . "', 
				'" . $access_name . "',
				'" . $access_pass . "',
				'" . $access_access . "',
				'" . $access_comm . "')");
				if ($Res != 0) {
					return array($Res, mysql_error());
				} else {
					#log
					$this -> APICreatEvent("billing", "", "", @$_SESSION['access_id'], "", "Создана учетная запись " . $access_name . "");
					return array($Res, "Success creat Access");
				}
			} else {
				return array("-100", "Not all attributes are specified");
			}
		} else {
			return array("-100", "Fail attribute");
		}
	}

	#список пользователей
	function APIGetAccess($obj=NULL) {
		$return=array();
		if (isset($obj)) {
			$query = $this -> Query("SELECT * FROM billing_access WHERE access_id = '".$obj."'", false);
		}else{
			$query = $this -> Query("SELECT * FROM billing_access", false);
		}
		if($this->sql_errno==0){
			foreach(@$query as $v){
				$return[$v["access_id"]]=$v;
			}
		}
		return ($this->sql_errno==0 ? $return : array($this->sql_errno, $this->sql_error));
	}
	
	#редактирование прав доступа
	function APISetAccess($array_value) {
		if (is_array($array_value)) {
			$obj = $array_value[0]["value"];
			unset($array_value["QUERY"][0]);

			$access_name = $array_value[1]["value"];
			$access_pass = $array_value[2]["value"];
			$access_comm = $array_value[3]["value"];
			#sql access
			foreach ($array_value as $array_value) {
				if ($array_value["name"] == "access_access") {
					$access_access .= $array_value["value"] . ";";
				}
			}
			#update
			$Res = $this -> Query("UPDATE billing_access SET `access_name` = '" . $access_name . "', `access_pass` = '" . $access_pass . "', `access` = '" . $access_access . "', `comment` = '" . $access_comm . "' WHERE access_id = '" . $obj . "' or access_name = '" . $obj . "'");
			if ($Res != 0) {
				return array($Res, mysql_error());
			} else {
				$this -> APICreatEvent("billing", "", "", @$_SESSION['access_id'], "", "Редактирование учетной записи " . $access_name . "");
				return array($Res, "Success update Access");
			}
		} else {
			return array("-100", "Fail attribute");
		}
		return array("-100", "Error");
		;
	}
	
	#удаление прав доступа учетной записи
	function APIDelAccess($obj) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$Query = $this -> Query("DELETE FROM billing_access WHERE access_id = '" . $obj . "' or access_name = '" . $obj . "' LIMIT 1;");
			if ($this->sql_errno==0){
				$this -> APICreatEvent("billing", "", "", @$_SESSION['access_id'], "", "Удалена учетная запись");
				return array(0, "OK");
			}else{
				return array($this->sql_errno, $this->sql_error);
			}
		} else {
			return array(-1, "No parameters");
		}
	}

	/*
	 ==================================
	 Функции для работы с доп-ыми атрибутами абонентов
	 APIGetAddAttr, APISetAddAttr, APIDelAttr, APIGetAdvAttr
	 ==================================
	 */
	#редактирование значений атрибутов абонентов
	function APISetAdvAttr($array_value) {
		if (is_array($array_value)) {
			$obj = $array_value["QUERY"][0]["value"];
			unset($array_value["QUERY"][0]);
			foreach ($array_value["QUERY"] as $Attr) {
				$Query = $this -> Query("SELECT * FROM billing_users_attr WHERE attr_id = '" . $Attr["name"] . "' AND user_id = '" . $obj . "'", false);
				if (is_array($Query)) {
					#если атрибут существует, обновить
					$Return = $this -> Query("UPDATE billing_users_attr SET value = '" . $Attr["value"] . "' WHERE attr_id = '" . $Attr["name"] . "' AND user_id = '" . $obj . "'");
					$Res = array($Return, "");
				} else {
					#если атрибут не существует, создадим
					$Return = $this -> Query("INSERT INTO billing_users_attr(user_id,attr_id,value) VALUES ('" . $obj . "', '" . $Attr["name"] . "', '" . $Attr["value"] . "')");
					$Res = array($Return, "");
				}
			}
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}

	#Значения атрибутов абонентов
	function APIGetAdvAttr($obj, $attr) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$Query = $this -> Query("SELECT * FROM billing_users_attr WHERE attr_id = '" . $attr . "' AND user_id = '" . $obj . "'", false);
			if (is_array($Query)) {
				$Res = array("0", $Query);
			} else {
				$Res = array("-1", "error get attribute");
			}
		} else {
			$Res = array("-2", "Error attribute");
		}
		return $Res;
	}

	#СПРАВОЧНИК АТРИБУТОВ
	function APIGetAddAttr($type, $obj = false) {
		#2 - абонентов 3 - групп
		if ($type == 2) {
			#атрибуты абонентов
			if ($obj == NULL) {
				$Res = $this -> Query("SELECT id,attr_id,attr_name,comment FROM billing_attribute_users ORDER BY id", false);
			} else {
				$Res = $this -> Query("SELECT id,attr_id,attr_name,comment FROM billing_attribute_users WHERE attr_id = '" . $obj . "' or attr_name = '" . $obj . "' LIMIT 1", false);
			}
		} elseif ($type == 3) {
			#аргементы не определены
			$Res = -1;
		} else {
			#аргементы не корректны
			$Res = -1;
		}
		return $Res;
	}

	function APISetAddAttr($type, $obj, $value, $comment) {
		if (!isset($type) || empty($type))
			return -1;
		if (!isset($value) || empty($value))
			return -1;

		#абонентские атрибуты
		if ($type == 2) {
			#новый атрибут
			if ((!isset($obj) || empty($obj))) {
				$Res = $this -> Query("INSERT INTO billing_attribute_users(attr_id,attr_name,comment) VALUES 
				('" . $this -> GUID() . "', '" . $value . "', '" . $comment . "')");
				return $Res;
			} else {
				$Res = $this -> Query("UPDATE billing_attribute_users SET attr_name = '" . $value . "',comment = '" . $comment . "'  
				WHERE attr_name = '" . $obj . "'");
				return $Res;
			}
		}
		#атрибуты групп
		if ($type == 3) {
			#новый атрибут
			if ((!isset($obj) || empty($obj))) {
				$Res = -1;
			} else {
				$Res = -1;
			}
		}
		return $Res;
	}

	function APIDelAttr($type, $obj, $comment) {
		if (!isset($type) || empty($type))
			return -1;
		if (!isset($obj) || empty($obj))
			return -1;

		#абонентские атрибуты
		if ($type == 2) {
			//! ПРЕДУСМОТРЕТЬ УДАЛЕНИЕ СВЯЗАННЫХ ДАННЫХ С АТРИБУТОМ
			$Res = $this -> Query("DELETE FROM billing_attribute_users WHERE 
			attr_id = '" . $obj . "' or attr_name = '" . $obj . "' LIMIT 1");
			return $Res;
		}
		#атрибуты групп
		if ($type == 3) {
			$Res = -1;
		}
		return $Res;
	}

	/*
	 ==================================
	 Функции для работы с логами
	 APICreatEvent, APIGetEvent
	 ==================================
	 */
	#создание cобытия
	function APICreatEvent($log_type, $user_id, $user_name, $access_id, $log_errnum, $log_comment) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#проверяем событие
			if (array_key_exists($log_type, $this -> ArrTypeEvent)) {
				$Res = $this -> Query("INSERT INTO billing_log(log_type, user_id, user_name, access_id, log_errnum, log_comment, log_date, log_time) VALUES 
				('" . $log_type . "',
				'" . $user_id . "',
				'" . $user_name . "',
				'" . $access_id . "',
				'" . $log_errnum . "',
				'" . $log_comment. "',
				'" . date("Y-m-d")."',
				'" . date("H:i:s")."'
				)");
				if ($Res != 0) {
					return array($Res, mysql_error());
				} else {
					return array($Res, "Success");
				}
			} else {
				return array("-100", "Error event type");
			}
		} else {
			return array("-100", "Not all attributes are specified");
		}
	}
	/*
	==================================
	SMS - Service (default smspilot.ru)
	==================================
	*/
	
	#отправка sms шлюзу
	function APISMSSend ($json_request){
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#get options
			$sms_pilot_api=$this->APIGetOptions("sms_pilot_api");
			$sms_pilot_url=$this->APIGetOptions("sms_pilot_url");
			$sms_pilot_provider=$this->APIGetOptions("sms_pilot_provider");
			$Json_query=array();
			$Json_query_send=array();

			$query=json_decode($json_request, true);
			if (json_last_error() == JSON_ERROR_NONE) {
				$Json_query=array("apikey"=>$sms_pilot_api[1][0]["value"]);
				foreach($query as $j_key => $j_val){
					list($usec, $sec) = explode(" ", microtime());
					$Json_query_send[]=array("id"=>$sec.$usec, "from"=>$sms_pilot_provider[1][0]["value"], "to"=>"7".$j_val["to"]."", "text"=>$j_val["text"]);
				}
				$Json_query["send"]=$Json_query_send;
				#SEND GATEWAY
				$result= file_get_contents($sms_pilot_url[1][0]["value"], false, stream_context_create(array(
					'http' => array(
					'method' => 'POST',
					'header' => "Content-Type: application/json\r\n",
					'content' => json_encode($Json_query),
					),
				)));
				#result
				$result_array=json_decode($result, true);
				print_r($result_array);
				if (isset($result_array["send"])){
					return array("0", $result_array["send"]);
				}else{
					return array("-3", "Error post to gateway");
				}	
			}else{
				return array($json_request, "Error in json format");
			}
		}else{
			return array("-1", "Error attribute");
		}
	}

	/*
	 ==================================
	 APIGetLastIP, NAS, APIGetSectorWithSector, APIGetOptions, APISetOptions
	 ==================================
	*/
	#Подключаем класс для работы с NAS
	function NAS($obj) 
	{ 
		include_once ("class." . $obj . ".php");
		$this -> nas = New routeros_api;
	}
	
	function include_network_monitor() 
	{
		include_once ("class.network_monitor.php");
		$this -> NET_MONIT = New NETWORK_MONITOR;
	}

	#к какому сектору принадлежит ip
	#последний ип для сектора
	function APIGetSectorWithSector($ip) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			#список секторов
			$ArraySector = $this -> APIGetSector();
			if (is_array($ArraySector)) {
				foreach ($ArraySector as $ArraySectorValue) {
					$UnixMask = $this -> ConvertIPMask($ArraySectorValue["sector_netmask"]);
					$mask = split('/', $ArraySectorValue["sector_network"] . "/" . $UnixMask);
					$start = sprintf('%u', ip2long($mask[0]));
					$end = $start + pow(2, 32 - $mask[1]) - 1;
					if ($this -> GetIntIP($ip) >= $start and $this -> GetIntIP($ip) <= $end) {
						$sector = $ArraySectorValue["sector_name"];
						break;
					}
				}
				if ($sector != "") {
					$Res = array("0", $sector);
				} else {
					$Res = array("-1", "Not search $ip any sector");
				}
			} else {
				$Res = array("-1", "Nothin sector");
			}
		} else {
			$Res = array("-1", "Fail arguments");
		}
		return $Res;
	}

	#последний ип для сектора
	function APIGetLastIP($sector_id) {
		if ($this -> CheckArgsArray(func_get_args(), array(0))) {
			$SearchSector = $this -> APIGetSector($sector_id);
			#найден сектор для объекта
			if (is_array($SearchSector[0])) {
				$ArraySector = $this -> APIGetSector($SearchSector[0]["sector_id"]);
				$UnixMask = $this -> ConvertIPMask($ArraySector[0]["sector_netmask"]);
				$mask = split('/', $ArraySector[0]["sector_network"] . "/" . $UnixMask);
				$start = sprintf('%u', ip2long($mask[0]));
				$end = $start + pow(2, 32 - $mask[1]) - 1;
				#заполянем массив ip адресами
				while ($start++ < $end) {
					#255 и 0 нам не нужны
					$ErrorIP = explode(".", $this -> GetStringIP($start));
					if ($ErrorIP[3] == "255" or $ErrorIP[3] == "0")
						continue;
					$ArrayPool[] = $this -> GetStringIP($start);
				}
				#абоненты
				$ArrayAbonentSector = $this -> Query("SELECT * FROM billing_users", false);
				$ArrayPoolAbonent = array();
				if (is_array($ArrayAbonentSector)) {
					#создадим массив только из ип адресов абонентов
					foreach ($ArrayAbonentSector as $ValueArrayAbonentSector) {
						$ArrayPoolAbonent[] = $ValueArrayAbonentSector["user_ip"];
					}
				}
				$compare = (array_diff($ArrayPool, $ArrayPoolAbonent));
				if (count($compare) >= 1) {
					$Res = array("0", reset($compare));
				} else {
					$Res = array("-1", "Nothing ip address in this sector!");
				}
			} else {
				$Res = array("-1", "For this address not search sector!");
			}
		} else {
			$Res = array("-1", "Fail arguments");
		}
		return $Res;
	}

	#Параметры биллинга
	function APIGetOptions($obj) {
		if (!empty($obj)) {
			$Query = $this -> Query("SELECT * FROM billing_options WHERE id = '" . $obj . "' LIMIT 1", false);
			if (is_array($Query)) {
				$Res = array("0", $Query);
			} else {
				$Res = array("-1", "error get attribute");
			}
		} else {
			$Query = $this -> Query("SELECT * FROM billing_options", false);
			if (is_array($Query)) {
				$Res = array("0", $Query);
			} else {
				$Res = array("-2", "error get attribute");
			}
		}
		return $Res;
	}

	#редактирование параметров биллинга
	function APISetOptions($obj, $value) {
		if ($this -> CheckArgsArray(func_get_args(), array(0, 1))) {
			$Query = $this -> Query("UPDATE billing_options SET value = '" . $value . "' WHERE id = '" . $obj . "'");
			if ($Query != "0") {
				$Res = array($Query, "Error update abonent");
			} else {
				$Res = array("0", "Успешно");
			}
		} else {
			$Res = array("-1", "Error attribute");
		}
		return $Res;
	}
	#редактирование параметров биллинга (Данные в виде массива)
	function APISetOptionsArray($obj) {
		$str = "";
		if (is_array($obj["QUERY"])){
			foreach	($obj["QUERY"] as $key => $value) {
				list($attr, $attrval) = each($value);
				$str.= "WHEN '".$attr."' THEN '".$attrval."' ";
			}
			$Query = $this -> Query("UPDATE `billing_options` SET value = CASE id " . $str . " ELSE value END ");
			$Res = array($Query, mysql_error());
		}else{
			$Res = array("-1", "QUERY IS NOT ARRAY");
		}
		return $Res;
	}
	
	/*
		==================================
		MYSQL
		==================================
	 */
	
	function ConnSQL($server, $user, $psw, $bd) {
		$link = mysql_connect($server, $user, $psw);
		mysql_query("SET NAMES ".$this->encode."");
		if ($link) {
			if (mysql_select_db($bd, $link)) {
				$this -> link = $link;
			} else {
				echo "БД не найдена";
				#LOG...
			}
		} else {
			echo "Ошибка подключения";
			#LOG...
		}
	}
	
	function Query($query, $type = true) {
		$result = mysql_query($query);
		//print mysql_error();
		
		if (!$result) {
			$Return = mysql_errno();
			$mysql_errnum;
			#LOG...
			$this->WriteLog("mysql", "Query:".$query."	".mysql_errno()."	".mysql_error());
		} else {
			#insert/update/delete
			if ($type == true) {
				$Return = mysql_errno();
				#echo 123;
			} else {
				if (mysql_num_rows($result) == 0) {
					$Return = -1;
				} else {
					while ($row = mysql_fetch_assoc($result)) {
						$Return[] = $row;
					}
				}
				
			}
			
		}
		
		$this->sql_errno=mysql_errno();
		$this->sql_error=mysql_error();
		
		return $Return;
	}
	
	Function mysql_close() {
		mysql_close($this -> link);
	}
	
	/*
		==================================
		HELP FUNCTION
		==================================
	 */
	#Пишем логи
	function WriteLog($filename, $str) {
		$DirLog=$this->config["dir_log"];
		if(is_dir($DirLog)){
			if ($handle = fopen($DirLog.$filename.".log", 'a')) {
				// Write $somecontent to our opened file.
				if (fwrite($handle, date("d-m-y H:i:s")."	".$str."\n") === FALSE) {
					fclose($handle);
					return array(-1, "Cannot write to file ($filename)");
				}else{
					fclose($handle);
					return array(0, "OK");
				}
			}else{
				fclose($handle);
				return array(-1, "Cannot open file ($filename)");
			}
		}else{
			return array(-1, "it is not directory");
		}
	}
	
	#Поддержка браузеров веб-интерфейсом
	public function SupportBrowser(){
		#Not supported browsers
		$Browsers=array("firefox", "opera", "Chrome", "safari");
		foreach($Browsers as $browser) 
		{ 
			if (preg_match("#($browser)[/ ]?([0-9.]*)#", strtolower($_SERVER['HTTP_USER_AGENT']), $match))
			{ 
				return true;
				break; 
			}
		}
		return false;
	}

	#Преобразование utf2win
	function utf82win($str) {
		return iconv("utf-8", "Windows-1251", $str);
	}

	
	#Преобразование IP адресов
	function GetIntIP($src) {
		$tmp = explode('.', $src);
		return (count($tmp) != 4) ? 0 : (floatval($tmp[0]) * 256 * 256 * 256 + floatval($tmp[1]) * 256 * 256 + floatval($tmp[2]) * 256 + floatval($tmp[3]));
	}

	function GetStringIP($src) {
		$res = '';
		for ($i = 0; $i < 4; $i++) {
			$res = (($i != 3) ? '.' : '') . strval($src - (floatval(256) * intval($src / 256))) . $res;
			$src = intval($src / 256);
		}
		return $res;
	}

	#создает гуид для объектов
	function GUID() {
		if (function_exists('com_create_guid') === true) {
			return trim(com_create_guid(), '{}');
		}
		return sprintf('%04X%04X-%04X-%04X-%04X-%04X%04X%04X', mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(16384, 20479), mt_rand(32768, 49151), mt_rand(0, 65535), mt_rand(0, 65535), mt_rand(0, 65535));
	}

	#mask-nix mask
	function ConvertIPMask($obj) {
		$MaskArray = array("255.255.255.254" => "31", "255.255.255.252" => "30", "255.255.255.248" => "29", "255.255.255.240" => "28", "255.255.255.224" => "27", "255.255.255.192" => "26", "255.255.255.128" => "25", "255.255.255.0" => "24", "255.255.254.0" => "23", "255.255.252.0" => "22", "255.255.248.0" => "21", "255.255.240.0" => "20", "255.255.224.0" => "19", "255.255.192.0" => "18", "255.255.128.0" => "17", "255.255.0.0" => "16");
		return ($MaskArray[$obj]);
	}
	
	#разбиваем секунды на сек, мин, часы, дни, года
	function seconds2times($seconds)
	{
		$times = array(0=>0,1=>0,2=>0,3=>0);
		$count_zero = false;
		$periods = array(60, 3600, 86400, 31536000);
		
		for ($i = 3; $i >= 0; $i--)
		{
			$period = floor($seconds/$periods[$i]);
			if (($period > 0) || ($period == 0 && $count_zero))
			{
				$times[$i+1] = $period;
				$seconds -= $period * $periods[$i];
				
				$count_zero = true;
			}
		}
		
		$times[0] = $seconds;
		return $times;
	}
	
	function GetMac($str) {
		$tstr='';
		for ($i = 0; $i < strlen($str); $i++) {
			if ($i == 2 || $i == 4 || $i == 6 || $i == 8 || $i == 10)
				$tstr .= ':';
			$tstr .= $str[$i];
		}
		return strtoupper($tstr);
	}

	//проверяем переменные
	function CheckArgsArray($args, $numargs) {
		if (!is_array($args) and !is_array($numargs))
			return false;
		foreach ($args as $key => $value) {
			foreach ($numargs as $numargs_value) {
				if ($key == $numargs_value) {
					if (isset($value) and strlen($value) > 0) {
						#...
					} else {
						return false;
					}
				}
			}
		}
		return true;
	}

}
?>