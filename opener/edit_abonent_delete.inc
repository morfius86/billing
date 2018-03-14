<?php
/*	
Редактор абонентов: Удаление данных абонента и абонента	
*/

#valid guid
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
		if(!is_array($Abonent_id_options)){
			exit("Абонент с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}	
}else{
	exit ("Fail exist parametr");
}
//print_r($Abonent_id_options);
?>

	<?php include("edit_abonent_navigation.inc"); ?>
	<!--форма редактирования параметров личного кабинета абонента -->
	<table class="forms" width="100%">
			<input type=hidden name=obj size=50 id=obj value="<?=$Abonent_id_options[0]["user_id"]?>">
			<thead><tr><th colspan=2 align=center>УДАЛЕНИЕ ДАННЫХ И АБОНЕНТА</td></tr></thead>
			<tbody>
				<tr>
					<td>Удалить платежи</td>
					<td>
						<select name="delete_payment" id=delete_payment>
						<option value="0">Нет</option>
						<option value="1">Да</option>
						</select>
					</td>
				</tr>
				<tr>
					<td>Удалить накопительные данные<br><font color=gray>(статистика авторизации, журнал изменений)</font></td>
					<td>
						<select name="delete_log" id=delete_log>
						<option value="0">Нет</option>
						<option value="1">Да</option>
						</select>
					</td>
				</tr>
				<tr>
					<td><b>Удалить учетную запись</b></td>
					<td>
						<select name="delete_abonent" id=delete_abonent>
						<option value="1">Да</option>
						<option value="0">Нет</option>
						</select>
					</td>
				</tr>
			</tbody>
			<tr><td colspan=2 align=center><input type="submit" id="submit" value="Применить" /><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>				
			<tr><td colspan=2 align=center id=result></td></tr>				

	</table>
	
<script type='text/javascript'>
$("#result").hide();
	$("#submit").click(function(event) {
		$("#result").show();
			var ArrayForm = {};
			$('table select, input[type=hidden]').each(function(i) {
				ArrayForm[this.name] = this.value;
			});

			$.post("query/json_query.php", {
				FUNCTION : "APIDelUser",
				'QUERY[]' : ArrayForm
			}, function(response) {
				//json parse
				//alert(response);
				var data = eval("(" + response + ")");
				if(data.error[0] == 0){
					$("#result").html("Успешно");
					//удаляем из таблицы, если было выбрно удаление 
					if($('#delete_abonent option:selected').val() == 1) {			
						var user_id = $("#obj").val();
						window.opener.$('#id'+user_id).hide('slow', function(){ $(this).remove(); });
					}
				}else{
					$("#result").html("Ошибка: " + data.error[0] + " Сообщение: " + data.message + "");
				}
				
			});


	}); 
</script>
