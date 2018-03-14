<?php
/*
Справочник Group
*/
?>
<script type="text/javascript">
$(document).ready(function() 
    { 
        $("#grouptable").tablesorter();
	}
); 

function remove_group(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelBillProp",
				QUERY : id
			}, function(response) {
				var data = eval("("+response+")");
				if (data.error == '0') {
					$('#id'+id).hide('slow', function() {$(this).remove();});
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
			}});
		}}
	});
}
</script>
<p id="tabname">СПРАВОЧНИК ГРУПП<p>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список групп</span></li>
		<li><span>Новая группа</span></li>
	</ul>
	<div class="box visible" >
		<!--список групп-->
	<table id="grouptable" class="tablesorter">
		<thead> 
			<tr><th>Название группы</th><th>Тип</th><th>Абонентов кол-во</th><th>Комментарий</th><th width=50>Опции</th></tr>
		</thead> 
		<tbody> 
			<?php
			if (is_array($tinsp->APIGetBillProp(2,"")))
			foreach($tinsp->APIGetBillProp(2,"") as $Group){
				$Count=$tinsp->Query("SELECT count('id') as count FROM `billing_users` where billing_users.group_id = '".$Group["group_id"]."' ", false);
				echo "<tr id='id".$Group["group_id"]."'>
				<td id=group_name>".$Group["group_name"]."</td>
				<td id=group_type>".(($Group["group_type"]==0)? 'Физические лица':'Юридические лица')."</td>
				<td><meter value='".$Count[0]["count"]."' max='".$CountBase[0]["count(id)"]."'></meter> ".$Count[0]["count"]."</td>
				<td id=comment>".$Group["comment"]."</td>
				<td>
					<a onclick='window.open(\"opener.php?opener=edit_group&id=".$Group["group_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=400,Height=140\");' href='#'><img src='img/Setting-icon.png' width='20'></a>
					<a href='javascript:;' onclick='remove_group(\"".$Group["group_id"]."\");'><img src='img/delete.png' width='16'></a>
				</tr>";	
			}
			
			?>
		</tbody> 
	</table>
	</div>
	<div class="box">	
	<!--форма регистрации группы-->
	<table class="forms">
		<form action="query/json_query.php" id="APICreatGroup">
			<tr><th>Название группы</th><td><input type="text" name="group_name" id="group_name"></td></tr>
			<tr><th>Тип</th>
				<td><select name="group_type" id="group_type" ><option value="0">Физические лица</option><option value="1">Юридические лица</option></select></td>
			</tr>
			<tr><th>Комментарий</th><td><input type="text" name="comment" id="comment"></td></tr>
		</form>	
		<tr><td colspan=2><input type="submit" name="sendBtn" id="create" value="Создать" /></td></tr>	
	</table>
<script>
	$("#create").click(function(event) {
		$("table.forms").each(function() {
			var values = {};
			$('input[type=checkbox],input[type=text],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.name);
			});
			//json_data=JSON.stringify(values);
			$.post("query/json_query.php", {
				FUNCTION : "APICreatGroup",
				'QUERY' : values
			}, function(response) {
				//json parse
				var data = eval("(" + response + ")");
				if (data.error == '0') { 
					$.prompt("Успешно создан");
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
				}	
			});
		});

	}); 
</script>

	</div>	
</div>
 
