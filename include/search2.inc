<?php
/*
Поиск пользователей
*/
?>
<script src="js/fixer.js"></script>
<script> 
$(document).ready(function() 
    { 
	$('#myLink').click(function(){ alert ('123'); });
    } 
); 
//$('#logtable').columnHover(); 
	
</script>
<style type="text/css">
#nav{
 list-style: none; /* Для списка убираем маркеры */
    margin: 0; /* Нет отступов вокруг */
    padding: 0; /* Убираем поля вокруг текста */
    font-family: Arial, sans-serif; /* Рубленый шрифт для текста меню */
    font-size: 11pt; /* Размер названий в пункте меню */
}
#nav li{
	float:left;
	margin-right:5px;
	display:block;
}
#nav li a{
	color:#fff;
	text-decoration:none;
	text-shadow:1px 1px 1px rgba(0,0,0,0.75); /* Тень текста, чтобы приподнять его на немного */
	border-radius:2px;
}
#nav li a:hover{
	color:#fff;
	background:#6b0c36;
	background:rgba(107,12,54,0.75); /* Выглядит полупрозрачным */
	text-decoration:underline;
}

/*--- ВЫПАДАЮЩИЕ ПУНКТЫ ---*/
#nav ul{
	list-style:none;
	position:absolute;
	left:-9999px; /* Скрываем за экраном, когда не нужно (данный метод лучше, чем display:none;) */
}
#nav ul li{
	padding-top:1px; /* Вводим отступ между li чтобы создать иллюзию разделенных пунктов меню */
	float:none;
}
#nav ul a{
	white-space:nowrap; /* Останавливаем перенос текста и создаем многострочный выпадающий пункт */
	display:block;
}
#nav li:hover ul{ /* Выводим выпадающий пункт при наведении курсора */
	left:0; /* Приносим его обратно на экран, когда нужно */
	opacity:1; /* Делаем непрозрачным */
}
#nav li:hover a{ /* Устанавливаем стили для верхнего уровня, когда выводится выпадающий список */

	background:rgba(51, 124, 192, 0.75);/* Выглядит полупрозрачным */
	text-decoration:underline;
}
#nav li:hover ul a{ /* Изменяем некоторые стили верхнего уровня при выводе выпадающего пункта */
	text-decoration:none;
}
#nav li:hover ul li a:hover{ /* Устанавливаем стили для выпадающих пунктов, когда курсор наводится на конкретный пункт */
	background:#333;
	background:rgba(51,51,51,0.75); /* Будет полупрозрачным */
	text-decoration:underline;
}
   
   .brd {
    border-bottom: 1px solid #ccc; /* Линия снизу */
   }

div.scrollTableContainer {
	height: 400px;
	overflow: auto;
	width: 1400px;
	margin: 15px 0 0 0;
	position: relative;
}



div.scrollTableContainer thead tr {

position:relative;
top: expression(offsetParent.scrollTop); /*IE5+ only*/
left: 0px;
}

div.scrollTableContainer td:last-child {padding-right: 20px;}
</style>
<p id="tabname">Поиск абонентов<p>
<div class="section">
	<table class="forms">
		<form action="?page=search2&submit=true" id="APISearchAbonent2" method="post">
		<tr><th>ФИО/Договор</th><th>IP/MAC</th><th>Телефон</th></tr>
		<tr><td><input type=text name=search_fiodogovor></td><td><input type=text name=search_ipmac></td><td><input type=text name=search_phone></td></tr>
		<tr><th>Нас. пункт</th><th>Сектор</th><th>Устройства</th></tr>
		<tr>
			<td>
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
			</td>
			<td>позже</td>
			<td>позже</td>
		</tr>
		<tr>
			<th>Улица</th>
			<th>Группа</th>
			<th>Блокировка</th>
		</tr>
		<tr>
			<td><select id="select_ulica" name="select_ulica"></select></td>
			<td>
				<select name="select_group" id="select_group">
				<option value="">Все группы</option>
				<?php
				if (is_array($tinsp->APIGetBillProp(2,"")))
				foreach($tinsp->APIGetBillProp(2,"") as $Group){
					echo "<option value='".$Group["group_id"]."'>".$Group["group_name"]."</option>";
				}
			?>
				</select>
			</td>
			<td>
				<select name="select_block" id="select_block">
					<option value="">Любая</option>
					<option value="0">Работает</option>
					<option value="1">Отключен</option>
					<option value="2">Приостановлен</option>
				</select>
			</td>
		</tr>
		<tr>
			<th>Дом</th>
			<th>Тариф</th>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td><select id="select_dom" name="select_dom"></select></td>
			<td>
				<select id="select_tarif" name="select_tarif">
				<option value="">Все тарифы</option>
				<?php
					if (is_array($tinsp->APIGetTarif(2,"")))
					foreach($tinsp->APIGetTarif(2,"") as $Tarif){
						echo "<option value='".$Tarif["tarif_id"]."'>".$Tarif["tarif_name"]."</option>";
					}
				?>
				</select>
			</td>
			<td>&nbsp;</td>
		</tr>
		<tr>
			<td colspan=3><input type="submit" value="Найти" ></td>
		</tr>
		</form>	
	</table>
<script>
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



 
  /*удаление абонентов*/
function remove(id){				
	var txt = '<table class="forms">'+
		'<form action="query/json_query.php" id="APIDelUser">'+
		'	<input type=hidden name=obj id=obj value="'+id+'">'+
		'	<tr><th>Комментарий</th><td><input type="text" id="comment" name="comment" value="" /></td></tr>'+
		'</form>'+	
		'</table>';
			$.prompt(txt,{ 
				buttons:{Delete:true, Cancel:false},
				callback: function(v,m,f){
					if(v){
						var form = m.find("#APIDelUser");
						var url = form.attr("action");
						var ArrayForm = m.find(':input').serializeArray();
						$.post(url, {FUNCTION: "APIDelUser", QUERY:ArrayForm},
							function(response){
								var data = eval("("+response+")");
								if (data.error[0] == '0'){
									$('#id'+f.obj).hide('slow', function(){ $(this).remove(); });
								}else{
									$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
								}
							}
						);
					}else{}
					
					}
				});
			} 
 </script>

	
	<div class="scrollTableContainer" >
	<table id="logtable" class="tablesorter">
		<thead class="fixedHeader">
			<tr><th width=20>#</th><th width=10></th><th width=200>Имя</th><th>Группа</th><th>Тариф</th><th>Баланс</th><th>Кредит</th><th>Нас.пункт</th><th>Улица</th><th>Дом</th><th>Кв/оф</th><th>Тел.</th><th>Local IP</th><th>PPP IP</th><th>MAC</th></tr>
		</thead>
		<tbody id=result_search>
		<?php
			if ($_GET["submit"]=="true"){
				$SearchAbon = $tinsp->APISearchAbonent2($_POST["search_fiodogovor"], $_POST["search_ipmac"], $_POST["search_phone"], $_POST["select_nasp"], $_POST["select_ulica"], $_POST["select_group"], $_POST["select_block"], $_POST["select_dom"], $_POST["select_tarif"]);
				foreach ($SearchAbon[1] as $UserKey => $UserVal) {
					echo '<tr id="id'.$$UserVal["user_id"].'"><td>'.$UserKey.'</td><td align=left> 
					<ul id="nav" align=left>
					<li><a onclick="window.open(\'opener.php?opener=edit_abonent&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)"><img src="img/Setting-icon.png" width="20"></a><ul>
					<li><a onclick="window.open(\'opener.php?opener=edit_abonent_attr&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)">АТРИБУТЫ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=journal_auth&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)">СЕССИИ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=journal_bill&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)">ИСТОРИЯ ПЛАТЕЖЕЙ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=edit_abonent_lk&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598\');" href="javascript:void(0)">ЛИЧНЫЙ КАБИНЕТ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=edit_abonent_switch&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=400\');" href="javascript:void(0)">УСТРОЙСТВО</a></li>
					<li><a onclick="window.open(\'opener.php?opener=edit_abonent_delete&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=220\');" href="javascript:void(0)">УДАЛЕНИЕ</a></li>
					</ul></li>
					<li><a onclick="window.open(\'opener.php?opener=addcash&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=330\');" href="javascript:void(0)"><img src="img/wallet.png" width="20"></a><ul>
					<li><a onclick="window.open(\'opener.php?opener=addcash_per&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=330\');" href="javascript:void(0)">ПЕРЕРАСЧЕТ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=addcash_podkl&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=330\');" href="javascript:void(0)">ПОДКЛЮЧЕНИЕ/РЕМОНТ</a></li>
					<li><a onclick="window.open(\'opener.php?opener=addcash_action&id='.$UserVal["user_id"].'\',\'\',\'Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=330\');" href="javascript:void(0)">АКЦИИ/ПОДАРОК</a></li>
					</ul></li></ul>
					</td><td><img src="img/users/'.$UserVal["user_block"].'.png"> <b id=user_name>'.$UserVal["user_name"].'</b></div></td><td id=group_name>'.$UserVal["group_name"].'</td><td id=tarif_name>'.$UserVal["tarif_name"].'</td><td id=user_balance>'.$UserVal["balance"].'</td><td>null</td><td id=user_nasp>'.$UserVal["nasp_name"].'</td><td id=user_ulica>'.$UserVal["ulica_name"].'</td><td id=user_dom>'.$UserVal["dom_name"].'</td><td id=user_kvof>'.$UserVal["user_kvof"].'</td><td id=user_mobphone>'.$UserVal["user_mobphone"].'</td><td id=user_ip>'.$UserVal["user_ip"].'</td><td id=user_pppip>'.$UserVal["user_pppip"].'</td><td id=user_mac>'.$UserVal["user_mac"].'</td>
				</tr>';
				}
			}
		?>
		</tbody>
	</table>
	</div>

</div>

