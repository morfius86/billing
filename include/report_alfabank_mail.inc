<?php
	ini_set('default_socket_timeout', 60);

	//Поиск безнальных платежей через email уведомления Альфа-банка.
	$CONNECT_MAIL = array (
		"server_imap" => "imap.yandex.ru:993/imap/ssl/novalidate-cert",
		//"server_pop" => "pop.yandex.ru:995/pop3/ssl/novalidate-cert",
		"server_pop" => "imap.yandex.ru:993/imap/ssl/novalidate-cert",
		"login" => "alfa@vash-net.ru",
		"pass" => "140682",
		"find_email" => "alfa-bank"
	);
	
	$message = '';
	
	function get_mail($server, $login, $pass, $find_email){
		$return = false;
		imap_timeout(1, 30);
		imap_timeout(2, 30);
		$connect_imap = imap_open("{".$server."}INBOX", $login, $pass);
		
		if (false !== $connect_imap) {
			// проверим ящик на наличие новых писем
			$count = imap_num_msg($connect_imap);
			// если есть новые письма
			for ($i=1; $i <= $count; $i++) {
				// получаем заголовок
				$header = imap_header($connect_imap, $i);
				$header_convert = iconv_mime_decode($header->subject, 0,"UTF-8");
				$mail_from = $header->sender[0]->mailbox."@".$header->sender[0]->host;
				$Msgno = trim((string)$header->Msgno);
				if (preg_match ("/{$find_email}/i", $mail_from)) {
					// получаем тему письма
					// $subject = $header->subject;
					// получаем содержимое письма
					$text_mail = imap_fetchbody($connect_imap, $i, 1); 
					$return[$Msgno] = array("email"=>$mail_from, "msg" => htmlspecialchars_decode(base64_decode($text_mail)), "header" => $header_convert);
				}else{
					$return[$Msgno] = array("email"=>$mail_from, "msg"=> false, "header" => $header_convert);
					//echo "#{$i} :  {$header->Msgno} from {$mail_from}\n";
				}
			}
			imap_close($connect_imap);
		}else {
			$return = imap_last_error();
		}
		return $return;
	}
	
	function delete_mail($server, $login, $pass, $mailno){
		$return = false;
		imap_timeout(1, 30);
		imap_timeout(2, 30);
		imap_timeout(3, 30);
		imap_timeout(4, 30);
		if ($connect_imap = imap_open("{".$server."}INBOX", $login, $pass)) {
			imap_delete($connect_imap, $mailno);
			imap_expunge($connect_imap);
			imap_close($connect_imap);
			$return = true;
		}else {
			$return = imap_last_error();
		}
		return $return;
	}

	// запрос на удаление привязки
	if (isset($_GET["delete"]) && !empty($_GET["delete"])) {
		$QUERY["QUERY"][] = array("name"=>"user_id", "value"=>$_GET["delete"]);
		$QUERY["QUERY"][] = array("name"=>"yur_bank_num", "value"=>"");
		$SetUser = $tinsp->APISetUser($QUERY, "billing_users_requisites");
		$message.= ($SetUser[0] == '0') ? "Привязка к счету удалена<br>" : "Ошибка удаления<br>";

	}
	// запрос на добавление
	if (isset($_GET["add"]) && !empty($_GET["add"])) {
		$QUERY["QUERY"][] = array("name"=>"user_id", "value"=>$_POST["select_user"]);
		$QUERY["QUERY"][] = array("name"=>"yur_bank_num", "value"=>$_GET["add"]);
		$SetUser = $tinsp->APISetUser($QUERY, "billing_users_requisites");
		$message.= ($SetUser[0] == '0') ? "Привязка к счету добавлена<br>" : "Ошибка привязки счета<br>";
	}
	//запрос на удаление письма
	if (isset($_GET["delete_mail"]) && !empty($_GET["delete_mail"])) {
		$query_delete = delete_mail ($CONNECT_MAIL["server_imap"], $CONNECT_MAIL["login"], $CONNECT_MAIL["pass"], $_GET["delete_mail"]);
		$message.= ($query_delete) ? "Письмо № ".$_GET["delete_mail"]." успешно удалено.<br>" : "Ошибка удаления письма № ".$_GET["delete_mail"].": ".$query_delete."<br>";
	}
	//запрос на проведение платежей
	if (isset($_GET["send"]) && !empty($_GET["send"])) {
		if (is_array($_POST["select_user"])){
			// удаляем письмо
			$query_delete = delete_mail ($CONNECT_MAIL["server_imap"], $CONNECT_MAIL["login"], $CONNECT_MAIL["pass"], $_GET["mail"]);
			if ($query_delete){
				$message.= "Письмо № ".$_GET["mail"]." успешно удалено.<br>";
				//проводим платежи
				foreach ($_POST["select_user"] as $userid => $param) {
					$addcash = $tinsp->APIAddCash($userid, $param["sum"] , $param["comment"], "", "billing_bezn");
					if($addcash[0]=="0"){
						$message.= "Платеж (".$param["comment"].") успешно проведен. <br>";
					}else{
						$message.= "Ошибка ".$addcash[1]." проведения платежа (".$param["comment"].")!<br>";
					}	
				}
			}else{
				$message.= "Ошибка удаления письма № ".$_GET["mail"].": ".$query_delete."<br>";
			}
			
		}else{
			$message.= "Не выбраны получатели перевода! <br>";
		}
		//print_r($_POST);
		//$query_delete = delete_mail ($CONNECT_MAIL["server_imap"], $CONNECT_MAIL["login"], $CONNECT_MAIL["pass"], $_GET["delete_mail"]);
	}
?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='"text"/javascript' src='js/jquery.autocomplete.js'></script>
<script> 
$(function() {
	$(':checkbox[name=paycheck]').click(function() {
			
			id_checkbox = $(this).attr('id');
			//alert (id_checkbox);
			//$('input:text').attr('disabled',!this.checked);
			$("#sum_"+id_checkbox+", #comm_"+id_checkbox).prop("disabled", !this.checked);
			//$("#comm"+id_checkbox).prop("disabled", !this.checked);
		});
});

$(document).ready(function() 
    { 
        $("#reporttable").tablesorter(); 
	}	
);



//$('#logtable').columnHover(); 
</script>

<link rel="stylesheet" type=""text"/css" href="css/jquery.autocomplete.css" />
<style type=""text"/css">
#menu { /* Левая колонка */
    float: left; /* Обтекание справа */

}
#content { /* Правая колонка */


}

</style>
<p id="tabname">Уведомление о поступлении (Альфа-Банк)</p>
	<div id="menu">
	<!--Отчеты-->
		<table width="100%">
			<tr>
				<td><form action="?page=report_alfabank_mail&check=1" autocomplete="off" method="post">
				<table class="forms">
				<tr><th>Информация</th></tr>
				<tr><td>Email:<?=$CONNECT_MAIL["login"]?><br>Ищем: <?=$CONNECT_MAIL["find_email"]?></td></tr>
				<tr><th>Плательщики</th></tr>
				<tr><td>
					<input type="checkbox" name="pay_sender[]" value="123" CHECKED>Будущий плательщик<br>
					<input type="checkbox" name="pay_sender[]" value="123" CHECKED>Будущий плательщик<br>
					<input type="checkbox" name="pay_sender[]" value="123" CHECKED>Будущий плательщик<br>
					<input type="checkbox" name="pay_sender[]" value="123" CHECKED>Будущий плательщик<br>
				</td></tr>
				
				<tr><td align="center"><input type="submit" value="Запрос" ></td></tr>
				<tr><th>Результат операции</th></tr>
				<tr><td align="center">
					<?php
					if (strlen($message)>1){
						echo '<div id="result">'.$message.'</div>';
					}
					?></td>
				</tr>

				</table></form></td>
			</tr>
		</table>
	</div>
<div id="content">
	<table id="reporttable" class="tablesorter">
		<thead class="fixedHeader">
			<tr><th width=20>#</th><th>Плательщик</th><th>Счет плательщика</th><th>№ платежа</th><th>Дата</th><th>Сумма</th><th>Операция</th></tr>
		</thead>
		<tbody>
			<?php
			/* ОТЧЕТ СПИСОК */
			$Users = $tinsp->APIGetBillProp("3");

			foreach ($Users as $UserAttr) {
				if ($UserAttr["group_type"] == '1'){
					$test[$UserAttr["user_id"]] = $UserAttr;
				}
			}
						
			function SelectYur ($Users, $yur_bank_num=false, $sum=0, $msgno, $payment_id, $date){
				$sum = str_replace(",", ".", $sum);
				$form_id = rand();
				$return = '';
				$search = false;
				$YurCount=0;
				foreach ($Users as $k1 => $v1){
					//$YurCount=1;
					// ищем всех юриков с указанным счетов
					if ($yur_bank_num && trim($yur_bank_num) == $v1["yur_bank_num"]){
						$YurCount++;
						$search = true;
						$return.= '<tr><td>';
						//$return.= Select ($yur_bank_num, $Users, $k1);
						$return.= '<input type="checkbox" name="paycheck" id="'.$msgno.'_'.$k1.'" ><u>'.$v1["user_name"].'</u>';
						// input на проведение
						$return.= '</td><td><input type="number" id="sum_'.$msgno.'_'.$k1.'" form="send_'.$msgno.'" name="select_user['.$k1.'][sum]" size="6" value="'.(($YurCount > 1) ? '0': $sum).'" step="0.01" required disabled > руб.</td>
						<td><input type="text" form="send_'.$msgno.'" id="comm_'.$msgno.'_'.$k1.'" name="select_user['.$k1.'][comment]" value="№ '.$payment_id.' от '.$date.'" disabled></td>
						<td><a href="?page=report_alfabank_mail&check=1&delete='.$k1.'" >удалить</a></td></tr>';
					}
				}
				
				$return.= '<tr><td>';
				//$return.= Select ($yur_bank_num, $Users, '', true);
				$return.= '<select name=select_user form="add_'.$form_id.'" required><option></option>';
				foreach ($Users as $UserAttr) {
					$return.='<option value="'.$UserAttr["user_id"].'" >'.$UserAttr["user_name"].'</option>';
				}
				$return.= '</select>';
				
				$return.= '</td><td><form id="add_'.$form_id.'" action="?page=report_alfabank_mail&check=1&add='.$yur_bank_num.'" method="post"></form><input type="submit" form="add_'.$form_id.'" value="Добавить"></td><td></td></tr>';

				$return = array($search, $return);
				return $return;
			}
			
			if ($_GET["check"]==1){
				$query_mail = get_mail ($CONNECT_MAIL["server_pop"], $CONNECT_MAIL["login"], $CONNECT_MAIL["pass"], $CONNECT_MAIL["find_email"]);
				//print_r($query_mail);
				//print_r($Userslist);
				if(is_array($query_mail)){
					foreach($query_mail as $msgno => $msg_string){
						
						if ($msg_string["msg"]){
							// если в письме найдено "Р/С" используем другой паттерн
							if (preg_match ("/Р\/С/", $msg_string["msg"], $msg_param)){
								$pattern = "счет (?P<my_order>\d+).*дату (?P<date>.*) платеж №(?P<payment>\d+) от (?P<client>.*)Р\/С (?P<payment_order>\d+).* сумму (?P<sum>\d+[.,]\d+)"; 
							}else {
								$pattern = "счет (?P<my_order>\d+).*дату (?P<date>.*) платеж №(?P<payment>\d+) от (?P<client>.*)[(].*со счета (?P<payment_order>\d+).* сумму (?P<sum>\d+[.,]\d+)"; 
							}

		
							// 
							
							if (preg_match ("/".$pattern."/", $msg_string["msg"], $msg_param)){
								$SelectYur = SelectYur($test, $msg_param["payment_order"], $msg_param["sum"], $msgno, $msg_param["payment"], $msg_param["date"]);
								//ищем назначение платежа
								preg_match ("/Назначение платежа:(.*)С уважением/", strip_tags($msg_string["msg"]), $msg_nazna);
								//print_r($msg_nazna);
								$nazn = $msg_nazna[1];
								echo '
								<tr>
								<th rowspan="3">'.$msgno.'</th>
								<td style="font-weight: bold;">'.$msg_param[4].'</td>
								<td>'.$msg_param["payment_order"].'</td>
								<td>'.$msg_param["payment"].'</td>
								<td>'.$msg_param["date"].'</td>
								<td><input type="text" value="'.$msg_param["sum"].'" name="sum_all" disabled size="6" form="'.$msg_param["payment_order"].'"/></td>
								<td>';
								echo '<form action="?page=report_alfabank_mail&check=1&send=1&mail='.$msgno.'" method="post" id="send_'.$msgno.'"></form>';
								echo '<form action="?page=report_alfabank_mail&check=1&delete_mail='.$msgno.'" method="post" id="deletemail_'.$msgno.'"></form>';
								if ($SelectYur[0]) {
									echo '<button type="submit" form="send_'.$msgno.'">Провести</button>&nbsp;';
									echo '<button type="submit" form="deletemail_'.$msgno.'">Удалить</button>';
								}else{
									echo '<button type="submit" form="deletemail_'.$msgno.'">Удалить</button>';
								}
								
								echo '</td></tr>';
								echo '<tr><td colspan="6">'.$nazn.'</td></tr>';
								echo '
								<tr>
								<td colspan="6">
									<table class="tablesorter">
										'.$SelectYur[1].'
									</table>
									 
								</td>
								</tr>';
								
							}else{
								echo '<tr>
								<td>'.$msgno.'</td>
								<td>'.$msg_string["email"].'</td>
								<td>'.$msg_string["header"].'</td>
								<td colspan="4">Корректный отправитель, но проблемы с парсингом письма!</td>
								<td><form id="data"></form></td>
								</tr>';
							}
						}else{
							/*
							echo '<tr>
							<td>'.$msgno.'</td>
							<td>'.$msg_string["email"].'</td>
							<td>'.$msg_string["header"].'</td>
							<td colspan="4">Неизвестный отправитель!</td>
							<td>. . .</td>
							</tr>';
							*/
						}
						
					}

				}else{
					echo '<tr><td colspan=10 align=center>Нет данных</td></tr>';

				}
				
			}else{
				echo '<tr><td colspan=10 align=center>Нажмите "Запрос", чтобы сформировать отчет</td></tr>';
			}
		?>
		</tbody>
	</table>
</div>

