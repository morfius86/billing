<?php
/*	
–едактор абонента: основна¤ информаци¤	
*/
if (!empty($_GET["id"]) && strlen($_GET["id"])>2){
	$Abonent_id_options=$tinsp->APIGetBillProp(3, $_GET["id"]);
	if(!is_array($Abonent_id_options)){
		exit("јбонент с таким id не найден");
	}
}else{
	exit ("Ошибка переданного id");
}
?>
<script>
	$(document).ready(function() {  
		
	});
</script>

	<!--форма редактировани¤ абонента-->
	<?php include("edit_abonent_navigation.inc"); ?>
	<table class="forms" style="width: 100%;">
		<form action="query/json_query.php" id="APISetUser">
			<input type="hidden" name="obj" id=obj value="<?=$Abonent_id_options[0]["user_id"]?>">
				<!--ќ—Ќќ¬Ќјя »Ќ‘ќ–ћј÷»я-->
				<tr>
					<th colspan=2 class="zagolovok">Основная информация</th>
				</tr>
				<tr>
					<th>Группа</th>
					<td>
						<select name="group_id" id="group_id">
							<?php
							
								$GroupList=$tinsp -> APIGetBillProp(2, "");
								if(!isset($_GET["group_selected"])) $_GET["group_selected"] = $Abonent_id_options[0]["group_id"];
								if (is_array($GroupList))
								{
									foreach ($GroupList as $GroupKey => $Group){
										echo "<option value='".$Group["group_id"]."' ".(($Group["group_id"]==$_GET["group_selected"]) ? 'SELECTED':'' )." >" . $Group["group_name"] . "</option>";
									}
									//print_r($_GET["group_selected"]);
								}
							?>
						</select></td>
				</tr>
				<!-- –≈ ¬»«»“џ -->
				<?php
					$GroupSelect=$tinsp -> APIGetBillProp(2, $_GET["group_selected"]);
					if ($GroupSelect[0]["group_type"] == 0){
						#физ лица
					?>
						<tr><th>Пользователь</th><td style="float:left;">
							<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_name"><i>Ф.И.О.</i>*</label><input type="text" id="user_name" name="user_name" value="<?=$Abonent_id_options[0]["user_name"]?>" required="" /></div>
						</td></tr>
					<?php
					}elseif($GroupSelect[0]["group_type"] == 1){
						#юр лица
					?>
						<tr><th>Пользователь</th><td style="float:left;">
							<input type="text" id="user_name" name="user_name" value="<?=$Abonent_id_options[0]["user_name"]?>" required="" size="40" />
						</td></tr>
					<?php						
					}
				?>
				
				<!-- ќЌ“ј “Ќјя »Ќ‘ќ–ћј÷»я-->
				<tr>
					<th colspan=2 class="zagolovok">Адрес подключения</th>
				</tr>
				<tr><th>Адрес</th><td style="float:left;">
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_nasp"><i>Нас. пункт</i>*</label>
						<select name="user_nasp" id="user_nasp">
							<option value="null">Нет</option>
							<?php
								if (is_array($tinsp -> APIGetNasp(2, "")))
								foreach ($tinsp->APIGetNasp(2,"") as $Nasp){
									echo "<option value='" . $Nasp["nasp_id"] . "' ".(($Abonent_id_options[0]["nasp_id"]==$Nasp["nasp_id"]) ? 'SELECTED':'' ).">" . $Nasp["nasp_name"] . "</option>";
								}
							?>
						</select>
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_ulica"><i>Улица</i>*</label>
						<select name="user_ulica" id="user_ulica">
							<option value="null">Нет</option>
							<?php
								if (is_array($tinsp->APIGetUlica(2,"", $Abonent_id_options[0]["user_nasp"])))
								foreach($tinsp->APIGetUlica(2,"", $Abonent_id_options[0]["user_nasp"]) as $Ulica){
									if($Ulica["ulica_id"]==$Abonent_id_options[0]["user_ulica"]){
										echo "<option value='".$Ulica["ulica_id"]."' selected>".$Ulica["ulica_name"]."</option>";
									}else{
										echo "<option value='".$Ulica["ulica_id"]."'>".$Ulica["ulica_name"]."</option>";
									}	
								}
							?>
						</select>
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dom"><i>Дом</i>*</label>
						<select name="user_dom" id="user_dom">
							<option value="null">Нет</option>
							<?php
								if (is_array($tinsp->APIGetDom(2,"", $Abonent_id_options[0]["user_ulica"])))
								foreach($tinsp->APIGetDom(2,"", $Abonent_id_options[0]["user_ulica"]) as $Dom){
									if($Dom["dom_id"]==$Abonent_id_options[0]["user_dom"]){
										echo "<option value='".$Dom["dom_id"]."' selected>".$Dom["dom_name"].$Dom["dom_letter"]."</option>";
									}else{
										echo "<option value='".$Dom["dom_id"]."'>".$Dom["dom_name"].$Dom["dom_letter"]."</option>";
									}	
								}
							?>
						</select>
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_kvof"><i>кв.</i></label><input type="text" name="user_kvof" id="user_kvof" value="<?=$Abonent_id_options[0]["user_kvof"]?>" /></div>
					
				</td></tr>
				<tr><th>Контакты</th><td style="float:left;">
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_phone"><i>дом. телефон</i></label>
						<input type="text" id="user_domphone" name="user_phone" value="<?=$Abonent_id_options[0]["user_phone"]?>" />
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_mobphone"><i>моб. телефон</i></label>
						<input type="text" id="user_mobphone" name="user_mobphone" pattern="[0-9]{10}" title="10 цифр. Например 9217775566." value="<?=$Abonent_id_options[0]["user_mobphone"]?>" />
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dopphone"><i>доп. телефон</i></label>
						<input type="text" id="user_dopphone" name="user_dopphone" value="<?=$Abonent_id_options[0]["user_dopphone"]?>" />
					</div>
					<div style="clear:both; text-align:right; line-height:25px;"><label style="float:left; padding-right:10px;" for="user_dopphone"><i>e-mail</i></label>
						<input type="text" id="user_email" name="user_email" value="<?=$Abonent_id_options[0]["user_email"]?>" />
					</div>
				</td></tr>
				<tr><th>Комментарий</th><td style="float:left;">
						<input type="text" id="comment" name="comment" value="<?=$Abonent_id_options[0]["comment"]?>" size="50"/>
				</td></tr>
				
			<tr><td colspan=2><input type="submit" name="sendBtn" class="button" value="Сохранить" /><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>			
		</form>	
	</table>
<script>

	//select nasp
$("#user_nasp").change(function(){
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
	$.post("query/json_query.php", {FUNCTION: "APIGetUlica", QUERY:arr}, function(response){
	var data_ulica = eval("("+response+")");
		$.each(data_ulica.error, function(key, ulica){		
			$('#user_ulica').append('<option value="'+ulica.ulica_id+'">'+ulica.ulica_name+'</option>');			
		});
		//передаем данные событию select dom
		$("#user_ulica").trigger("change");
	});
});
	//select dom
$("#user_ulica").change(function(){	
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
	$.post("query/json_query.php", {FUNCTION: "APIGetDom", QUERY:arr}, function(response){
	var data_dom = eval("("+response+")");
		$.each(data_dom.error, function(key, dom){		
			$('#user_dom').append('<option value="'+dom.dom_id+'">'+dom.dom_name+''+dom.dom_letter+'</option>');			
		});
	});
}); 

//выбор группы
$('#group_id').change(function(){
	var url = '?opener=edit_abonent&id=<?=$_GET["id"]?>&group_selected='+ $(this).val();
	window.location = url;
});


//отправка формы
 $("#APISetUser").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			//
			alert(response);
			var data = eval("("+response+")");
			if (data.error[0] == '0'){
				//обновить данные в таблице			
				var user_id = $("#obj").val();
				window.opener.$('#id'+user_id).find('#user_name').text($('#user_name').val());
				window.opener.$('#id'+user_id).find('#user_nasp').text($('#user_nasp option:selected').html());
				window.opener.$('#id'+user_id).find('#user_ulica').text($('#user_ulica option:selected').html());
				window.opener.$('#id'+user_id).find('#user_dom').text($('#user_dom option:selected').html());
				window.opener.$('#id'+user_id).find('#user_kvof').text($('#user_kvof').val());
				window.opener.$('#id'+user_id).find('#user_mobphone').text($('#user_mobphone').val());
				window.opener.$('#id'+user_id).find('#user_ip').text($('#user_ip').val());
				window.opener.$('#id'+user_id).find('#user_pppip').text($('#user_pppip').val());
				window.opener.$('#id'+user_id).find('#user_mac').text($('#user_mac').val());
				window.opener.$('#id'+user_id).find('#comment').text($('#comment').val());
				alert("Успешно сохранено!");
			}else{
				alert("Ошибка: "+data.error+" сообщение: "+data.message+"");
			}

		}
    );
  });
</script>