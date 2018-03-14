<!--
Системные опции
-->
<?php
/*Список параметров*/
$ArrayParam = $tinsp -> APIGetOptions("");
$Options = array();
if (is_array($ArrayParam)) {
	foreach ($ArrayParam[1] as $ArrayParamKey => $ArrayParamVal) {
		$Options[$ArrayParamVal["id"]] = $ArrayParamVal["value"];
	}
} else
	echo "<div id=error>Не удалось получить список параметров!</div>";
?>
<script>
	$(document).ready(function() {
		//$("#tariftable").tablesorter();
	});

</script>

<p id="tabname">
	СИСТЕМНЫЕ ПАРАМЕТРЫ
</p>
<div class="section">
	<ul class="tabs">
		<li class="current">
			<span>Основные</span>
		</li>
		<li>
			<span>Услуги</span>
		</li>
		<li>
			<span>Настройки</span>
		</li>
		<li>
			<span>Платежные системы</span>
		</li>
	</ul>
	<div class="box visible" >
		<!--список групп-->
		<table class="tablesorter" style="display: inline-block;">
				<thead>
					<tr>
						<th colspan=2>Собственные реквизиты</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Юридическое название</td><td>
						<input type="text" name="prov_name" value="<?=$Options["prov_name"]; ?>" id="prov_name"/>
						</td>
					</tr>
					<tr>
						<td>ИНН</td><td>
						<input type="text" name="prov_inn" value="<?=$Options["prov_inn"]; ?>" id="prov_inn"/>
						</td>
					</tr>
					<tr>
						<td>Адрес</td><td>
						<input type="text" name="prov_address" value="<?=$Options["prov_address"]; ?>" id="prov_address"/>
						</td>
					</tr>
				</tbody>
			</table>
			<table class="tablesorter" style="display: inline-block;">
				<thead>
					<tr>
						<th colspan=2>Параметры</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Название компании</td><td>
						<input type="text" name="company_name" value="<?=$Options["company_name"]; ?>" id="company_name"/>
						</td>
					</tr>
					<tr>
						<td>API-пароль</td><td>
						<input type="text" name="api_pass" value="<?=$Options["api_pass"]; ?>" id="api_pass"/>
						</td>
					</tr>
					<tr>
						<td>Последний номер договора</td><td>
						<input type="text" name="billing_order" value="<?=$Options["billing_order"]; ?>" id="billing_order"/>
						</td>
					</tr>
					<tr>
						<td>Базовая валюта</td><td>
						<input type="text" name="base_valute" value="<?=$Options["base_valute"]; ?>" id="base_valute"/>
						</td>
					</tr>
				</tbody>
			</table>
			
			<table class="tablesorter" style="display: inline-block;">
				<thead>
					<tr>
						<th colspan=2>Пути</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>SSH</td><td>
						<input type="text" name="dir_ssh" value="<?=$Options["dir_ssh"]; ?>" id="dir_ssh"/>
						</td>
					</tr>
					<tr>
						<td>Sudo</td><td>
						<input type="text" name="dir_sudo" value="<?=$Options["dir_sudo"]; ?>" id="dir_sudo"/>
						</td>
					</tr>
					<tr>
						<td>FreeRadius</td><td>
						<input type="text" name="dir_freeradius" value="<?=$Options["dir_freeradius"]; ?>" id="dir_freeradius"/>
						</td>
					</tr>
					<tr>
						<td>Mysql</td><td>
						<input type="text" name="dir_mysql" value="<?=$Options["dir_mysql"]; ?>" id="dir_mysql"/>
						</td>
					</tr>
					<tr>
						<td>Log</td><td>
						<input type="text" name="dir_log" value="<?=$Options["dir_log"]; ?>" id="dir_log"/>
						</td>
					</tr>
				</tbody>
			</table>
			<table class="tablesorter" style="display: inline-block;">
				<thead>
					<tr>
						<th colspan=2>Почта</th>
					</tr>
				</thead>
				<tbody>
					<tr>
						<td>Email</td><td>
						<input type="text" name="mail_mail" value="<?=$Options["mail_mail"]; ?>" id="mail_mail"/>
						</td>
					</tr>
					<tr>
						<td>SMTP host</td><td>
						<input type="text" name="mail_host" value="<?=$Options["mail_host"]; ?>" id="mail_host"/>
						</td>
					</tr>
					<tr>
						<td>SMTP порт</td><td>
						<input type="text" name="mail_port" value="<?=$Options["mail_port"]; ?>" id="mail_port"/>
						</td>
					</tr>
					<tr>
						<td>SMTP логин</td><td>
						<input type="text" name="mail_login" value="<?=$Options["mail_login"]; ?>" id="mail_login"/>
						</td>
					</tr>
					<tr>
						<td>SMTP пароль</td><td>
						<input type="text" name="mail_pass" value="<?=$Options["mail_pass"]; ?>" id="mail_pass"/>
						</td>
					</tr>
				</tbody>
			</table>
		</div>
	<div class="box">
		<!--услуги-->
		<table class="tablesorter" style="display: inline-block;">
			<thead>
				<tr>
					<th colspan=2>Переводы между абонентами</th>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>Снимать % от перевода</td><td>
					<input type="number" name="usl_persent" value="<?=$Options["usl_persent"]; ?>" id="usl_persent" min="0" max="100"/>
					</td>
				</tr>
			</tbody>
		</table>
	</div>
	
	<div class="box">
	<!-- настройки -->
		<table class="tablesorter" style="display: inline-block;">
			<thead>
				<tr>
					<th colspan=2>SMS Pilot</th>
				</tr>
			</thead>
			<tbody>
				<tr><td>URL Post</td><td><input type="text" name="sms_pilot_url" value="<?=$Options["sms_pilot_url"]; ?>" id="sms_pilot_url" /></td></tr>
				<tr><td>API key</td><td><input type="text" name="sms_pilot_api" value="<?=$Options["sms_pilot_api"]; ?>" id="sms_pilot_api" /></td></tr>
				<tr><td>Provider</td><td><input type="text" name="sms_pilot_provider" value="<?=$Options["sms_pilot_provider"]; ?>" id="sms_pilot_provider" /></td></tr>
			</tbody>
		</table>
	</div>
	
	<div class="box">
	<!-- платежные системы -->
		<table class="tablesorter" style="display: inline-block;">
			<thead>
				<tr>
					<th colspan=2>QIWI VISA Wallet</th>
				</tr>
			</thead>
			<tbody>
				<tr><td>Номер магазина</td><td><input type="text" name="qiwi_number" value="<?=$Options["qiwi_number"]; ?>" id="qiwi_number" /></td></tr>
				<tr><td>Пароль</td><td><input type="text" name="qiwi_password" value="<?=$Options["qiwi_password"]; ?>" id="qiwi_password" /></td></tr>
				<tr><td>Lifetime счета</td><td><input type="text" name="qiwi_lifetime" value="<?=$Options["qiwi_lifetime"]; ?>" id="qiwi_lifetime" /></td></tr>
				<tr><td>URL</td><td><input type="text" name="qiwi_url" value="<?=$Options["qiwi_url"]; ?>" id="qiwi_url" /></td></tr>
			</tbody>
		</table>
		<table class="tablesorter" style="display: inline-block;">
			<thead>
				<tr>
					<th colspan=2>ПСКБ-Банк</th>
				</tr>
			</thead>
			<tbody>
				<tr><td>URL</td><td><input type="text" name="pskb_url" value="<?=$Options["pskb_url"]; ?>" id="pskb_url" /></td></tr>
				<tr><td>Service id</td><td><input type="text" name="pskb_service" value="<?=$Options["pskb_service"]; ?>" id="pskb_service" /></td></tr>
			</tbody>
		</table>
		<table class="tablesorter" style="display: inline-block;">
			<thead>
				<tr>
					<th colspan=2>MOBI.Деньги</th>
				</tr>
			</thead>
			<tbody>
				<tr><td>URL</td><td><input type="text" name="mobimoney_url" value="<?=$Options["mobimoney_url"]; ?>" id="mobimoney_url" /></td></tr>
				<tr><td>http-basic логин</td><td><input type="text" name="mobimoney_login" value="<?=$Options["mobimoney_login"]; ?>" id="mobimoney_login" /></td></tr>
				<tr><td>http-basic пароль</td><td><input type="text" name="mobimoney_password" value="<?=$Options["mobimoney_password"]; ?>" id="mobimoney_password" /></td></tr>
			</tbody>
		</table>
	</div>
	<div>
		<input type="button" class="button" value="Сохранить" id="test"/>
	</div>

</div>
<div id=result></div>
<script>
	$("#result").hide();
	$("#test").click(function(event) {
		$("#result").show();
		$('div.section').each(function() {
			var values = {};
			$('input[type=number],input[type=text]', this).each(function(i) {
				values[this.name] = this.value;

			});
			$.post("query/json_query.php", {
				FUNCTION : "APISetOptionsArray",
				'QUERY[]' : values
			}, function(response) {
				//json parse
				//alert(response);
				var data = eval("(" + response + ")");
				if(data.error[0] == 0){
					$("#result").html("Успешно сохранено");
				}else{
					$("#result").html("Ошибка: " + data.error[0] + " Сообщение: " + data.message + "");
				}
				
			});
		});

	}); 
</script>
