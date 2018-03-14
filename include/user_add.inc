<?php
/*
 Интрфейс создания абонентов
 */
?>

<script>

	$(document).ready(function() {
		$("#user_mac").mask("**:**:**:**:**:**");
		$("#user_mobphone").mask("9999999999");
		$("#birth_day").mask("9999-99-99");

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
	
	function PassGens (lens)
	{
	  var PassGen = (function(){
		var Chars = '0123456789',
			Length = Chars.length,
			RndChar = function() {
				return Chars.charAt(Math.floor(Math.random() * Length));
			};
		return function(len) {
			var s = '', i;
			for (i = 0; i < len; i++) s += RndChar();
			return s;
		};
		})();
		$('#user_perspass').val(PassGen(lens));
	}
	
	
</script>

<p id="tabname">ДОБАВИТЬ АБОНЕНТА</p>
	<div class="section">
		<ul class="tabs">
			<li class="current">
				<span>Форма создания</span>
			</li>
		</ul>
		<div class="box visible" >
			<table class="forms">
				<form action="query/json_query.php" id="APICreatUser2">
					<!--ОСНОВНАЯ ИНФОРМАЦИЯ-->
					<tr>
						<th colspan=2 class="zagolovok">ОСНОВНАЯ ИНФОРМАЦИЯ</th>
					</tr>
					<tr>
						<th>Группа</th>
						<td>
						<select name="group_id" id=group_id>
							<?php
							$GroupList=$tinsp -> APIGetBillProp(2, "");
                            if (is_array($GroupList))
							{
                                foreach ($GroupList as $GroupKey => $Group){
                                    echo "<option value='".$Group["group_id"]."' ".(($Group["group_id"]==$_GET["group_selected"]) ? 'SELECTED':'' )." >" . $Group["group_name"] . "</option>";
                                }
								if(!isset($_GET["group_selected"])) $_GET["group_selected"] = $GroupList[0]["group_id"];
							}
							?>
						</select></td>
					</tr>
				<?php
					$GroupSelect=$tinsp -> APIGetBillProp(2, $_GET["group_selected"]);
					if ($GroupSelect[0]["group_type"] == 0){
						#физ лица
				?>
						<tr><th>Пользователь</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_name"><i>Ф.И.О.</i>*</label><input type="text" id="user_name" name="user_name" value="" required="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="birth_day"><i>Дата Рождения</i>*</label><input type="date" name="birth_day" id="birth_day" /></div>
						</td></tr>
						<tr><th>Паспорт</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Серия</i></label><input type="text" id="pasp_ser" name="pasp_ser" value="" required="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Номер</i></label><input type="text" id="pasp_num" name="pasp_num" value="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_date"><i>Когда</i></label><input type="date" id="pasp_date" name="pasp_date"  value="" required="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_who"><i>Кем</i></label><input type="text" name="pasp_who" id="pasp_who" /></div>
						</td></tr>
						<tr><th>Адрес регистрации</th><td style="float:left;">
							<input type="text" id="pasp_addr" name="pasp_addr" value="" size=40 />
						</td></tr>
				<?php
					}elseif($GroupSelect[0]["group_type"] == 1){
						#юр лица
				?>
						<tr><th>Название фирмы</th><td style="float:left;">
							<input type="text" id="user_name" name="user_name" value="" required="" size="40" />
						</td></tr>
						<tr><th>Ф.И.О. ген. директора или<br> лица по доверенности.</th><td style="float:left;">
							<input type="text" id="yur_gen" name="yur_gen" value="" required="" size="40" />
						</td></tr>
						<tr><th>Адреса</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Юридический</i></label><input type="text" id="yur_ad_yur" name="yur_ad_yur" value="" required="" size="30"/></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Почтовый</i></label><input type="text" id="yur_ad_postal" name="yur_ad_postal" value="" size="30"/></div>
						</td></tr>
						<tr><th>ИНН/КПП</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>ИНН</i></label><input type="text" id="yur_inn" name="yur_inn" value="" required="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>КПП</i></label><input type="text" id="yur_kpp" name="yur_kpp" value="" /></div>
						</td></tr>
						<tr><th>Счет</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_ser"><i>Счет №</i></label><input type="text" id="yur_bank_num" name="yur_bank_num" value="" required="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>в банке</i></label><input type="text" id="yur_bank_name" name="yur_bank_name" value="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>Кор. счет</i></label><input type="text" id="yur_bank_kornum" name="yur_bank_kornum" value="" /></div>
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="pasp_num"><i>БИК</i></label><input type="text" id="yur_bank_bik" name="yur_bank_bik" value="" /></div>
						</td></tr>
				<?php						
					}
				?>

					<!--КОНТАКТНАЯ ИНФОРМАЦИЯ-->
					<tr>
						<th colspan=2 class="zagolovok">КОНТАКТНАЯ ИНФОРМАЦИЯ</th>
					</tr>
					<tr><th>Адрес</th><td style="float:left;">
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_nasp"><i>Нас. пункт</i>*</label>
							<select name="user_nasp" id="user_nasp">
								<option value="null">Нет</option>
								<?php
									if (is_array($tinsp -> APIGetNasp(2, "")))
									foreach ($tinsp->APIGetNasp(2,"") as $Nasp) {
										echo "<option value='" . $Nasp["nasp_id"] . "'>" . $Nasp["nasp_name"] . "</option>";
									}
								?>
							</select>
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_ulica"><i>Улица</i>*</label>
							<select id="user_ulica" name="user_ulica"></select>
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dom"><i>Дом</i>*</label>
							<select id="user_dom" name="user_dom"></select>
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_kvof"><i>Кв.</i></label><input type="text" name="user_kvof" id="user_kvof" /></div>
						
					</td></tr>
					<tr><th>Контакты</th><td style="float:left;">
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_phone"><i>дом. телефон</i></label>
							<input type="text" id="user_domphone" name="user_phone" />
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_mobphone"><i>моб. телефон</i></label>
							<input type="text" id="user_mobphone" name="user_mobphone"  pattern="[0-9]{10}" title="10 цифр. Например 9217775566." />
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dopphone"><i>доп. телефон</i></label>
							<input type="text" id="user_dopphone" name="user_dopphone" />
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dopphone"><i>e-mail</i></label>
							<input type="text" id="user_email" name="user_email" />
						</div>
					</td></tr>

					<!--БИЛИНГОВАЯ ИНФО-->
					<tr>
						<th colspan=2 class="zagolovok">БИЛИНГОВАЯ ИНФОРМАЦИЯ</th>
					</tr>
					<tr>
				        <th>Сегмент</th><td>
				        <select name="user_sector_id" id="user_sector_id">
						<?php
                        if (is_array($tinsp -> APIGetSector()))
                            foreach ($tinsp->APIGetSector() as $Sector) {
                                echo "<option value='" . $Sector["sector_id"] . "'>" . $Sector["sector_name"] . "</option>";
                            }
						?></select></td>
					</tr>
					<tr>
						<th>Тариф</th>
						<td><select name="tarif_id" id="tarif_id" required >
							<?php
							/*
                            if (is_array($TarifArray=$tinsp -> APIGetTarif(2, "")))
							foreach ($TarifArray as $TarifKey => $Tarif) {
								if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
									echo '<optgroup label="'.$Tarif["grtarif_name"].'">';
								}
								echo "<option value='" . $Tarif["tarif_id"] . "'>".$Tarif["tarif_desc"]." --- [".$Tarif["tarif_name"]."]</option>";
								
								if($TarifArray[$TarifKey+1]["grtarif_id"]!=$Tarif["grtarif_id"]){
									echo '</optgroup>';
								}
								
                            }*/
							?>
						</select></td>
					</tr>

					<!--СЕТЕВЫЕ НАСТРОЙКИ-->
					<?php  $Dogovor = $tinsp -> APIGetOptions("billing_order"); ?>
					<tr>
						<th>Договор</th><td>
						<input type="text" name="user_dogovor" id="user_dogovor" value="<?=$Dogovor[1][0]["value"] + 1 ?>">
						</td>
					</tr>
					<tr>
						<th>Пароль от ЛК</th><td>
						<input type="text"  name="user_perspass" id="user_perspass"  onfocus="PassGens(6);" value="">
						</td>
					</tr>
					<tr><th>IPoE</th><td style="float:left;">
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_ip"><i>IP-адрес</i></label>
							<input type="checkbox" name="user_autoip" id="user_autoip" value="1"><input type="text" id="user_ip" name="user_ip" title="IP-адрес формата x.x.x.x" /><span id="validip">
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_mac"><i>MAC-адрес</i></label>
							<input type="text" id="user_mac" name="user_mac"  title="MAC-адрес в формате XX:XX:XX:XX:XX:XX" /><span id="validmac"></span>
						</div>
					</td></tr>
					<tr><th>PPP</th><td style="float:left;">
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_pppip"><i>IP-адрес</i></label>
							<input type="text" id="user_pppip" name="user_pppip" title="IP-адрес формата x.x.x.x" /><span id="validpppip"></span>
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_login"><i>Логин</i></label>
							<input type="text" id="user_login" name="user_login"  />
						</div>
						<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_password"><i>Пароль</i></label>
							<input type="text" id="user_password" name="user_password"  />
						</div>
					</td></tr>
				</form>
                    <tr>
                        <td colspan=2>
                        <input type="submit" name="sendBtn" id="create" class="button" value="создать" />
                        </td>
                    </tr>
				<!--<tr><td colspan=2>Сформировать № договора согласно IP-адресу<input type=checkbox name=user_autodogovor id=user_autodogovor value=1></tr>-->

			</table>
<script>
//создаем абонента
$("#create").click(function(event) {
	var json_data='';
	$('table.forms').each(function() {
		var values = {};
		$('input[type=text],input[type=date],select', this).each(function(i) {
			values[this.name] = this.value;
			//alert(this.value);
		});
		//json_data=JSON.stringify(values);
		$.post("query/json_query.php", {
			FUNCTION : "APICreatUser2",
			'QUERY' : values
		}, function(response) {
			//json parse
			//alert (response);
			var data = eval("(" + response + ")");
			if (data.error[0] == '0') { 
				$.prompt('Абонент успешно создан!');
			} else {
				$.prompt("Ошибка: " + data.error[1] + " Сообщение: " + data.error[1] + "");
			}   
		});
	});
}); 



$('#group_id').change(function(){
	var url = '?page=user_add&group_selected='+ $(this).val();
	window.location = url;

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
	
	var arr_tar = {};
	arr_tar['0'] = {};
	arr_tar['0']['value'] = "3";
	arr_tar['1'] = {};
	arr_tar['1']['value'] =  $(this).val();
	
	$('#tarif_id').empty();
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
	
	$.post("query/json_query.php", {
		FUNCTION : "APIGetTarif",
		QUERY : arr_tar
	}, function(response) {
		//alert (response);
		var data = eval("(" + response + ")");
		console.log (data);
		$.each(data.error, function(key, val) {
			
			if(key == 0 || val.grtarif_id != data.error[key-1].grtarif_id ){
				$('#tarif_id').append('<optgroup label="' + val.grtarif_name + '">');
			}
			if(val.tarif_arhive == '1'){
				$('#tarif_id').append('<option value="' + val.tarif_id + '" disabled>' + val.tarif_desc + '- - - - ' + val.abon_cash + ' руб. (архивный)</option>');
			}else{
				$('#tarif_id').append('<option value="' + val.tarif_id + '" >' + val.tarif_desc + '- - - - ' + val.abon_cash + ' руб.</option>');
			}
			
			
			
		});
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

</script>

		</div>
	</div>

