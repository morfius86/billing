<?php
/*	
Редактор оборудования	
*/
if (isset($_GET["id"]) && strlen($_GET["id"])>2){
	$Group_ID=$_GET["id"];
	$Group_ID_opt=$tinsp->APIGetBillProp(2, $Group_ID);
	if(!is_array($Group_ID_opt[0])){
		exit("Группа с таким id не найден");
	}
}else{
	exit ("Fail exist parametr");
}
?>

<!--форма редактирования группы-->
<table class="forms" style="width: 100%;">
	<thead><tr><th colspan="2" class="zagolovok" align="center">Редактор группы "<?=$Group_ID_opt[0]["group_name"]?>"</td></tr></thead>
	<form action="">
		<input type="hidden" name="obj" id="obj" value="<?=$Group_ID?>">
		<tr><td>Название группы</td><td><input type="text" name="group_name" id="group_name" value="<?=$Group_ID_opt[0]["group_name"]?>" /></td></tr>
		<tr><td>Тип</td><td>
			<select name="group_type" id="group_type">
				<option value="0" <?=($Group_ID_opt[0]["group_type"]==0)? 'selected':'' ?>>Физические лица</option>
				<option value="1" <?=($Group_ID_opt[0]["group_type"]==1)? 'selected':'' ?>>Юридические лица</option>
			</select>
		</td></tr>
		<tr><td>Комментарий</td><td><input type="text" name="comment" id="comment" value="<?=$Group_ID_opt[0]["comment"]?>" /></td></tr>
	</form>	
	<tr><td colspan="2" align="center"><input type="submit" name="save" id="save" class="button" value="Сохранить" /></td></tr>
</table>
<script>
	//save
	$("#save").click(function(event) {
		var json_data='';
		$('.forms').each(function() {
			var values = {};
			$('input[type=checkbox],input[type=text],input[type=hidden],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.value);
			});
			$.post("query/json_query.php", {
				FUNCTION : "APISetGroup",
				'QUERY' : values
			}, function(response) {
				//alert(response);
				//json parse
				var data = eval("("+response+")");
				if (data.error == '0'){
					//обновить данные в таблице			
					var group_id = $("#obj").val();
					window.opener.$('#id'+group_id).find('#group_name').text($('#group_name').val());
					window.opener.$('#id'+group_id).find('#comment').text($('#comment').val());
					window.opener.$('#id'+group_id).find('#group_type').text($('#group_type option:selected').html());
					window.close();
				}else{
					alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
				}
				
			});
		});

	});  
</script>