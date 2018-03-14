<?php
/*
Поиск пользователей
*/
?>
<style type="text/css">
#menu { 
    float: left; /* Обтекание справа */
}
#content {
	height: 100%;
	overflow-y: scroll;
	overflow: auto;
}

.modal-window {
	background: rgba(0, 0, 0, 0.5);
	display: none;
	height: 100%;
	width: 100%;
	left: 0;
	top: 0;	
	position: fixed;
	overflow-y: scroll;
	z-index: 9999;
}
.window-group_change_tarif, .window-test {
	background: #ffffff;
	display: block;
	margin: 10% auto;
	padding: 20px;
	width: 500px;
	z-index: 9999;
	opacity: 0;
	-webkit-transition: 0.4s;
	-moz-transition: 0.4s;
	-ms-transition: 0.4s;
	-o-transition: 0.4s;
	transition: 0.4s;
}
.animation {
	-webkit-transform:  scale(0.5);
	-moz-transform:  scale(0.5);
	-ms-transform:  scale(0.5);
	transform:  scale(0.5);
}
.visible {
	opacity: 1;
	-webkit-transform:  scale(1);
	-moz-transform:  scale(1);
	-ms-transform:  scale(1);
	transform:  scale(1);
}

</style>
<script> 
$(document).ready(function() 
    { 
    $('#check_user_all').click(function(event) {  //on click 
        if(this.checked) { // check select status
            $('.check_user').each(function() { //loop through each checkbox
                this.checked = true;  //select all checkboxes with class "checkbox1"               
            });
        }else{
            $('.check_user').each(function() { //loop through each checkbox
                this.checked = false; //deselect all checkboxes with class "checkbox1"                       
            });         
        }
    });
	
	$('#group_selected_submit').click(function() {
		
		$('.modal-window').fadeIn(function() {
			if ($('#group_selected_type').val() == "group_change_tarif") {
				$('.window-group_change_tarif').addClass('visible');
			}
		});
	});
	
	$('.close, .modal-window').click(function() {
		$('.modal-window').fadeOut().end().find('.window-group_change_tarif').removeClass('visible');
	});
	
	$('.window-group_change_tarif').click(function(event) { 
		event.stopPropagation()
	});
	
    } 
); 
</script>
<?php
	/*Получаем данные из биллига*/
	$GetTarif=$tinsp->APIGetTarif(2,"");
?>
<p id="tabname">Поиск абонентов</p>
<div id="menu">
	<table class="forms">
	<form action="?page=search&submit=true" id="APISearchAbonent2" method="post">
	<tr><th>ФИО/Договор</th></tr>
	<tr><td><input type="text" name="search_fiodogovor"></td></tr>
	<tr><th>IP/MAC</th></tr>
	<tr><td><input type=text name=search_ipmac></td></tr>
	<tr><th>Телефон</th></tr>
	<tr><td><input type=text name=search_phone></td></tr>
	<tr><th>Адрес</th></tr>
	<tr><td>
		<select name="select_nasp" id="select_nasp">
			<option value="">Все нас. пункты</option>
			<option value="null">Нет нас. пункта</option>
			<?php
			if (is_array($tinsp->APIGetNasp(2,"")))
			foreach($tinsp->APIGetNasp(2,"") as $Nasp){
				echo "<option value='".$Nasp["nasp_id"]."'>".$Nasp["nasp_name"]."</option>";
			}
		?>
		</select>
	</td></tr>
	<tr><td><select id="select_ulica" name="select_ulica"></select></td></tr>
	<tr><td><select id="select_dom" name="select_dom"></select></td></tr>


	<tr><th>Группа/Тариф</th></tr>
	<tr><td>
		<select name="select_group" id="select_group">
		<option value="">Все группы</option>
		<?php
		if (is_array($tinsp->APIGetBillProp(2,"")))
		foreach($tinsp->APIGetBillProp(2,"") as $Group){
			echo "<option value='".$Group["group_id"]."'>".$Group["group_name"]."</option>";
		}
		?>
		</select>
	</td></tr>
	<tr><td>
		<select id="select_tarif" name="select_tarif">
		<option value="">Все тарифы</option>
		<?php
			if (is_array($GetTarif))
			foreach($GetTarif as $Tarif){
				echo "<option value='".$Tarif["tarif_id"]."'>".$Tarif["tarif_name"]."</option>";
			}
		?>
		</select>
	</td></tr>
	<tr><th>Блокировка</th></tr>
	<tr><td>
		<select name="select_block" id="select_block">
			<option value="">Любая</option>
			<option value="0">Работает</option>
			<option value="1">Отключен</option>
			<option value="2">Приостановлен</option>
		</select>
	</td></tr>
	<tr>
		<td><input type="submit" value="Найти" ></td>
	</tr>
	</form>
	</table>
	<div>
		<table class="forms" style="margin-top: 1px; width: 100%;">
		<tr>
			<th>Групповые операции</th>
		</tr>
		<tr>
			<td>
				<select id="group_selected_type">
					<option value="" DISABLED checked>Выберите действие:</option>
					<option value="group_change_tarif">Изменить тариф</option>
					<option value="group_change_status">Изменить статус</option>
					<option value="group_send_sms">Отправить SMS</option>
				</select>
				<input type="submit" value="ОК" id="group_selected_submit">
			</td>
		</tr>
		</table>
	</div>
</div>	
<div id="content">
	<table id="logtable" class="tablesorter">
		<thead class="fixedHeader">
			<tr><th><input type="checkbox" name="check_user_all" id="check_user_all"></th><th>#</th><th></th><th width=200>Имя</th><th>Группа</th><th>Тариф</th><th>Баланс</th><th>Кредит</th><th>Нас.пункт</th><th>Улица</th><th>Дом</th><th>Кв/оф</th><th>Тел.</th><th>Local IP</th><th>PPP IP</th><th>MAC</th><th>Комментарий</th></tr>
		</thead>
		<tbody id="result_search">
		<?php
			if ($_GET["submit"]=="true"){
				$SearchAbon = $tinsp->APISearchAbonent2($_POST["search_fiodogovor"], $_POST["search_ipmac"], $_POST["search_phone"], $_POST["select_nasp"], $_POST["select_ulica"], $_POST["select_group"], $_POST["select_block"], $_POST["select_dom"], $_POST["select_tarif"]);
				foreach ($SearchAbon[1] as $UserKey => $UserVal) {
					echo '<tr id="id'.$UserVal["user_id"].'">
					<td><input type="checkbox" class="check_user" name="check_user[]" value="'.$UserVal["user_id"].'"></td>
					<td>'.($UserKey+1).'</td>
					<td><a onclick="window.open(\'opener.php?opener=edit_abonent&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)"><img src="img/Setting-icon.png" width="20"></a></td>
					<td><img src="img/users/'.$UserVal["user_block"].'.png"> <b id=user_name>'.$UserVal["user_name"].'</b></td>
					<td id=group_name>'.$UserVal["group_name"].'</td>
					<td id=tarif_name>'.$UserVal["tarif_name"].'</td>
					<td id=user_balance>'.$UserVal["balance"].'</td>
					<td>null</td>
					<td id=user_nasp>'.$UserVal["nasp_name"].'</td>
					<td id=user_ulica>'.$UserVal["ulica_name"].'</td>
					<td id=user_dom>'.$UserVal["dom_name"].'</td>
					<td id=user_kvof>'.$UserVal["user_kvof"].'</td>
					<td id=user_mobphone>'.$UserVal["user_mobphone"].'</td>
					<td id=user_ip>'.$UserVal["user_ip"].'</td>
					<td id=user_pppip>'.$UserVal["user_pppip"].'</td>
					<td id=user_mac>'.$UserVal["user_mac"].'</td>
					<td id=comment>'.$UserVal["comment"].'</td>
					</tr>';
				}
			}
		?>
		</tbody>
	</table>
</div>
<div class="modal-window">
	<div class="window-group_change_tarif animation">
		<p><h3>Групповое изменение тарифа</h3></p>
		<select id="group_selected_tarif">
		<?php
			if (is_array($GetTarif)){
				foreach($GetTarif as $Tarif){
					echo "<option value='".$Tarif["tarif_id"]."'>".$Tarif["tarif_name"]."</option>";
				}
			}
		?>
		</select>
		<input type="submit" value="ОК" id="TestSelect">
		<button class="close">Закрыть</button>
	</div>
</div>



<script>
$("#TestSelect").click(function() {
	var ArrayForm = $("#result_search").find(':input[name="check_user[]"]').serialize(); 
	//alert ();
	//alert (ArrayForm);
	var arr = {};
	arr['attr'] = "tarif_id";
	arr['attr_value'] = $("#group_selected_tarif").val();
	arr['selected_userid'] = ArrayForm;
	
	$.post("query/json_query.php", {FUNCTION: "APISetGroupUserAttr", QUERY:arr},
		function(response){
			var data = eval("("+response+")");
			$('.modal-window').fadeOut().end().find('.window-container').removeClass('visible');
			if (data.error == '0'){
				$.prompt("ОК: "+data.error+" Сообщение: "+data.message+"");
				window.location.reload();
			}else{
				$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}
		}
	);
});


	//select nasp
$("#select_nasp").change(function(){
	//данные нас пункта		
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = "";
	arr['2'] = {};
	arr['2']['value'] = $(this).val();
			
	$('#select_ulica').empty();
	$('#select_ulica').append('<option value="">Все улицы</option>');
	$('#select_ulica').append('<option value="null">Нет улицы</option>');

	//alert('-'+$(this).val()+'-');
	//список улиц у нас. пунктов
	$.post("query/json_query.php", {FUNCTION: "APIGetUlica", QUERY:arr}, function(response){
	var data_ulica = eval("("+response+")");
		$.each(data_ulica.error, function(key, ulica){	
				$('#select_ulica').append('<option value="'+ulica.ulica_id+'">'+ulica.ulica_name+'</option>');		
		});
		
		//передаем данные событию select dom
		$("#select_ulica").trigger("change");
	});
}).change();
	//select dom
$("#select_ulica").change(function(){	
	var ulica_id = $("#select_ulica").val();
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = "";
	arr['2'] = {};
	arr['2']['value'] = ulica_id;
			
	$('#select_dom').empty();
	$('#select_dom').append('<option value="">Все дома</option>');
	$('#select_dom').append('<option value="null">Нет дома</option>');

	//список домов у улиц
	$.post("query/json_query.php", {FUNCTION: "APIGetDom", QUERY:arr}, function(response){
	var data_dom = eval("("+response+")");
		$.each(data_dom.error, function(key, dom){		
			$('#select_dom').append('<option value="'+dom.dom_id+'">'+dom.dom_name+''+dom.dom_letter+'</option>');			
		});
	});
});
</script>


