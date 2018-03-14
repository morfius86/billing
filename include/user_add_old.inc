<?php
/*
 Интрфейс создания абонентов
 */
?>
<script>

	$(document).ready(function() {

		$("#user_mac").mask("**:**:**:**:**:**");
		$("#user_mobphone").mask("9999999999");

		$("#user_mac").keyup(function() {
			var mac = $("#user_mac").val();
			if (mac != 0) {
				if (isValidMac(mac)) {
					$("#validmac").html('<img src="img/validyes.png">');
				} else {
					$("#validmac").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validmac").html('');
			}
		});

		$("#user_ip").keyup(function() {
			var ip = $("#user_ip").val();
			if (ip != 0) {
				if (isValidIp(ip)) {
					$("#validip").html('<img src="img/validyes.png">');
				} else {
					$("#validip").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validip").html('');
			}
		});

		$("#user_pppip").keyup(function() {
			var ip = $("#user_pppip").val();
			if (ip != 0) {
				if (isValidIp(ip)) {
					$("#validpppip").html('<img src="img/validyes.png">');
				} else {
					$("#validpppip").html('<img src="img/validno.png">');
					eventObject.preventDefault();
				}
			} else {
				$("#validpppip").html('');
			}
		});
	});

	function isValidMac(obj) {
		var pattern = new RegExp(/^([0-9A-F]{2}([:]|$)){6}$|([0-9A-F]{4}([.]|$)){3}/);
		return pattern.test(obj);
	}

	function isValidIp(obj) {
		var pattern = new RegExp(/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}/);
		return pattern.test(obj);
	}

	
	
</script>

<p id="tabname">
	ДОБАВИТЬ АБОНЕНТА
<p>


	<div class="section">
		<ul class="tabs">
			<li class="current">
				<span>Форма создания</span>
			</li>
		</ul>
		<div class="box visible" >
			<table class="forms">
				<form action="query/json_query.php" id="APICreatUser">
					<!--ТАРИФИКАЦИЯ-->
					<tr>
						<th colspan=2 class="zagolovok">ТАРИФИКАЦИЯ</th>
					</tr>
					<tr>
						<th>Имя</th><td>
						<input type=text name=user_name id=user_name>
						</td>
					</tr>
					<tr>
						<th>Группа</th>
						<td>
						<select name="group_id" id=group_id>
							<?php
                            if (is_array($tinsp -> APIGetBillProp(2, "")))
                                foreach ($tinsp->APIGetBillProp(2,"") as $Group) {
                                    echo "<option value='" . $Group["group_id"] . "'>" . $Group["group_name"] . "</option>";
                                }
							?>
						</select></td>
					</tr>
					<tr>
						<th>Тариф</th>
						<td>
						<select name="tarif_id" id="tarif_id" required>
							<?php
                            if (is_array($TarifArray=$tinsp -> APIGetTarif(2, "")))
							foreach ($TarifArray as $TarifKey => $Tarif) {
								if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
									$TarifGroup=$tinsp->APIGetGroupTarif($Tarif["grtarif_id"]);
									echo '<optgroup label="'.$TarifGroup[0]["grtarif_name"].'">';
								}
								echo "<option value='" . $Tarif["tarif_id"] . "'>" . $Tarif["tarif_name"] . "</option>";
								
								if($TarifArray[$TarifKey+1]["grtarif_id"]!=$Tarif["grtarif_id"]){
									echo '</optgroup>';
								}
								
                            }
							?>
						</select></td>
					</tr>
					<tr>
						<th>На счет</th><td>
						<input type=text name=balance id=balance value=0 size=5>
						</td>
					</tr>

					<tr>
						<th>Состояние счета</th>
						<td>
						<select name="user_block" id=user_block>
							<option value='0'>Работает</option>
							<option value='1'>Заблокирован</option>
							<option value='2'>Приостановлен</option>
						</select></td>
					</tr>

					<!--АДРЕС ПОДКЛЮЧЕНИЯ-->
					<tr>
						<th colspan=2 class="zagolovok">АДРЕС ПОДКЛЮЧЕНИЯ</th>
					</tr>
					<tr>
						<th>Нас. пункт</th>
						<td>
						<select name="user_nasp" id=user_nasp>
							<option value="null">Нет</option>
							<?php
                            if (is_array($tinsp -> APIGetNasp(2, "")))
                                foreach ($tinsp->APIGetNasp(2,"") as $Nasp) {
                                    echo "<option value='" . $Nasp["nasp_id"] . "'>" . $Nasp["nasp_name"] . "</option>";
                                }
							?>
						</select></td>
					</tr>
					<tr>
						<th>Улица</th><td colspan=5><select id="user_ulica" name="user_ulica"></select></td>
					</tr>
					<tr>
						<th>Дом</th><td colspan=5><select id="user_dom" name="user_dom"></select></td>
					</tr>
					<tr>
						<th>Кв./оф.</th><td>
						<input type=text name=user_kvof id=user_kvof size=4>
						</td>
					</tr>
					<tr>
						<th>Моб. телефон</th><td>
						<input type=text name=user_mobphone id=user_mobphone size=15 pattern="[0-9]{10}" title="10 цифр. Например 9217775566.">
						</span></td>
					</tr>
					<tr>
				        <th>Принадлежность к сектору</th><td>
				        <select name="user_sector_id" id="user_sector_id">
						<?php
                        if (is_array($tinsp -> APIGetSector()))
                            foreach ($tinsp->APIGetSector() as $Sector) {
                                echo "<option value='" . $Sector["sector_id"] . "'>" . $Sector["sector_name"] . "</option>";
                            }
						?></select></td>
					</tr>
					<!--СЕТЕВЫЕ НАСТРОЙКИ-->
					<tr>
						<th colspan=2 class="zagolovok">СЕТЕВЫЕ НАСТРОЙКИ</th>
					</tr>
					<?php  $Dogovor = $tinsp -> APIGetOptions("billing_order"); ?>
					<tr>
						<th>Договор</th><td>
						<input type=text name=user_dogovor id=user_dogovor value="<?=$Dogovor[1][0]["value"] + 1 ?>">
						</td>
					</tr>
					<tr>
						<th>Local IP-адрес</th><td>
						<input type=text name=user_ip id=user_ip  title="IP-адрес формата x.x.x.x">
						<span id="validip"></td>
					</tr>
					<tr>
						<th>PPP IP-адрес</th><td>
						<input type=text name=user_pppip id=user_pppip  title="IP-адрес формата x.x.x.x">
						<span id="validpppip"></td>
					</tr>
					<tr>
						<th>MAC-адрес</th><td>
						<input type=text name=user_mac id=user_mac  title="MAC-адрес в формате XX:XX:XX:XX:XX:XX">
						<span id="validmac"></span></td>
					</tr>
					<tr>
						<th>Логин</th><td>
						<input type="text" name="user_login" id=user_login >
						</td>
					</tr>
					<tr>
						<th>Пароль</th><td>
						<input type="text" name="user_password" id="user_password" >
						</td>
					</tr>
				</form>
                    <tr>
                        <td colspan=2>
                        <input type="submit" name="sendBtn" id="create" class="button" value="создать" />
                        </td>
                    </tr>
				<tr>
					<td colspan=2>Зачислить на счет согласно тарифу
					<input type=checkbox name=user_autocash id=user_autocash value=1>
					</td>
				</tr>
				<tr>
					<td colspan=2>Присвоить локальный IP-адрес согласно сектору
					<input type=checkbox name=user_autoip id=user_autoip value=1>
				</tr>
				<!--<tr><td colspan=2>Сформировать № договора согласно IP-адресу<input type=checkbox name=user_autodogovor id=user_autodogovor value=1></tr>-->

			</table>
        <script>
            $("#create").click(function(event) {
                var json_data='';
                $('table.forms').each(function() {
                    var values = {};
                    $('input[type=checkbox],input[type=text],select', this).each(function(i) {
                        values[this.name] = this.value;
                        //alert(this.name);
                    });
                    //json_data=JSON.stringify(values);
                    $.post("query/json_query.php", {
                        FUNCTION : "APICreatUser",
                        'QUERY' : values
                    }, function(response) {
                        //json parse
                        //alert (response);
                        var data = eval("(" + response + ")");
                        if (data.error[0] == '0') { 
                        	$.prompt('Абонент успешно создан! <br> <a onclick="window.open(\'opener.php?opener=edit_abonent_attr&id=' + data.error[1] + '\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="#"><img src="img/Setting-icon.png" width="20">Редактировать дополнительные атрибуты</a>');
                        } else {
                            $.prompt("Ошибка: " + data.error[1] + " Сообщение: " + data.error[1] + "");
                        }   
                    });
                });
            }); 
				//clear
				$("#group_id").change(function() {
					$("#user_autocash").removeAttr("checked");
					$("#balance").val('');
				});
				//Сумма на счет
				$("#user_autocash").click(function() {
					var arr = {};
					arr['0'] = {};
					arr['0']['value'] = 2;
					arr['1'] = {};
					arr['1']['value'] = $('#tarif_id').val();

					if ($('#user_autocash:checked').val() == "1") {
						$.post("query/json_query.php", {
							FUNCTION : "APIGetTarif",
							QUERY : arr
						}, function(response) {
							var data = eval("(" + response + ")");
							if (data.error == "-1") {
								$("#user_autocash").removeAttr("checked");
								$.prompt('Ошибка получения данных по тарификации группы');
							} else {
								$("#balance").val(data.error[0]["abon_cash"]);
							}
						});
					} else {
						$("#balance").val('');
					}
				});
				//LASP IP
				$("#user_autoip").click(function() {
					if ($('#user_autoip:checked').val() == "1") {
						$.post("query/json_query.php", {
							FUNCTION : "APIGetLastIP",
							QUERY : $('#user_sector_id').val()
						}, function(response) {
							var data = eval("(" + response + ")");
							if (data.error[0] != "0") {
								$("#user_autoip").removeAttr("checked");
								$.prompt(data.error[1]);
							} else {
								$("#user_ip").val(data.error[1]);
							}
						});
					} else {
						$("#user_ip").val('');
					}

				});

				//select nasp
				$("#user_nasp").change(function() {
					//данные нас пункта
					var arr = {};
					arr['0'] = {};
					arr['0']['value'] = "2";
					arr['1'] = {};
					arr['1']['value'] = "";
					arr['2'] = {};
					arr['2']['value'] = $(this).val();

					$('#user_ulica').empty();
					$('#user_ulica').append('<option value="null">Нет</option>');
					//список нас. пунктов
					$.post("query/json_query.php", {
						FUNCTION : "APIGetUlica",
						QUERY : arr
					}, function(response) {
						var data_ulica = eval("(" + response + ")");
						$.each(data_ulica.error, function(key, ulica) {
							$('#user_ulica').append('<option value="' + ulica.ulica_id + '">' + ulica.ulica_name + '</option>');
						});
						//передаем данные событию select dom
						$("#user_ulica").trigger("change");
					});
				}).change();
				//select dom
				$("#user_ulica").change(function() {
					var ulica_id = $("#user_ulica").val();
					var arr = {};
					arr['0'] = {};
					arr['0']['value'] = "2";
					arr['1'] = {};
					arr['1']['value'] = "";
					arr['2'] = {};
					arr['2']['value'] = ulica_id;

					$('#user_dom').empty();
					$('#user_dom').append('<option value="null">Нет</option>');
					//список нас. пунктов
					$.post("query/json_query.php", {
						FUNCTION : "APIGetDom",
						QUERY : arr
					}, function(response) {
						var data_dom = eval("(" + response + ")");
						$.each(data_dom.error, function(key, dom) {
							$('#user_dom').append('<option value="' + dom.dom_id + '">' + dom.dom_name + '' + dom.dom_letter + '</option>');
						});
					});
				});

				$("#user_nasp, #user_ulica, #user_dom").change(function() {
					SearchSector($("#user_nasp").val(), $("#user_ulica").val(), $("#user_dom").val());
				});
			</script>
		</div>
	</div>

