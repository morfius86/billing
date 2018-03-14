<?php
/*	
Подключение/ремонт
*/

if (!isset($_GET["check_user"]) or !is_array($_GET["check_user"])){
	exit ("Fail exist parametr");
}

?>
<!-- форма групповой смены тарифа -->
<table class="forms" width="100%">
	<form action="query/json_query.php" id="APIAddCash">
	<input type="hidden" name="obj" id="obj" value="<?=$Abonent_id_options[0]["user_id"]?>" >
	<tr><th colspan="2">Групповое изменение тарифа (выбрано <?=count($_GET["check_user"])?>)</th></tr>
	<tr>
		<th>Тариф</th>
		<td><select name="tarif_id" id="tarif_id" required >
			<?php
			if (is_array($TarifArray=$tinsp -> APIGetTarif(2, "")))
			foreach ($TarifArray as $TarifKey => $Tarif) {
				if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
					echo '<optgroup label="'.$Tarif["grtarif_name"].'">';
				}
				echo "<option value='" . $Tarif["tarif_id"] . "'>".$Tarif["tarif_desc"]." --- [".$Tarif["tarif_name"]."]</option>";
				
				if($TarifArray[$TarifKey+1]["grtarif_id"]!=$Tarif["grtarif_id"]){
					echo '</optgroup>';
				}
				
			}
			?>
		</select></td>
	</tr>
	<tr><td colspan="2" style="text-align: center;">
		<input type="submit" name="sendBtn" value="OK" class="button" />
		<input type="button" class="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/>
	</td></tr>			
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