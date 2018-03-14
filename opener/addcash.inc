<?php
/*	
зачислить платеж
*/

if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
}else{
	exit ("Fail exist parametr");
}
//print_r($Abonent_id_options);
?>
	<?php include("edit_abonent_navigation.inc"); ?>
	<!-- форма оплаты -->
		<table class="forms" width="100%">
			<form action="query/json_query.php" id="APIAddCash">
			<input type="hidden" name="obj" id="obj" value="<?=$Abonent_id_options[0]["user_id"]?>" >
			<!--ПЛАТЕЖ-->
			<tr><th colspan=2>Пополнить счет</th></tr>
			<tr>
				<th>Сумма руб.</th>
				<td><input type="number" name="summ" id="summ"  min="0.1" max="100000" step="0.1" value="0"></td>
			</tr>
			<tr>
				<th>Примечание</th>
				<td><input type="text" name="comment" size="35"></td>
			</tr>
			<input type="hidden" name="payment_id" id="payment_id" value="">
			<tr>
				<th>Тип валюты</th>
				<td>
					<select name="sender" id=sender>
				<?php
					if (is_array($tinsp->ArrTypeValute))
					{
						echo "<option value='billing' selected>Наличный платеж</option>";
						echo "<option value='billing_bezn'>Безналичный платеж</option>";
					}
				?>
					</select>
				</td>
			</tr>
			<input type=hidden name=access_id id=access_id value="<?=$_SESSION["access_id"]?>" >
			<tr><td colspan=2 style="text-align: center;"><input type="submit" name="sendBtn" value="OK" class="button" /><input type="button" class="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>			
			</form>		
		</table>
<script>
//отправка формы
 $("#APIAddCash").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			var data = eval("("+response+")");
			if (data.error[0] == '0'){
				//обновить данные в таблице			
				var user_id = $("#obj").val();
				newsumm=parseFloat(window.opener.$('#id'+user_id).find('#user_balance').text())+parseFloat($('#summ').val());
				window.opener.$('#id'+user_id).find('#user_balance').text(newsumm);
				alert("Успешно зачислено!");
				window.location.reload("true");
			}else{
				alert("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			}
		}
    );
  });
</script>