<?php
/*	
Редактор NAS	
*/
if (isset($_GET["id"])){
	$Tarif_id=$_GET["id"];
	if(strlen($Tarif_id)>2){
		$GrTarif_id=$tinsp->APIGetGroupTarif($Tarif_id);
		if(!is_array($GrTarif_id)){
			exit("Группа тарифов с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
?>
<!--форма редактирования тарифы-->
<table class="forms" id="APISetGroupTarifArray" style="width:100%; height: 100%;">
	<input type=hidden name=obj size=50 id=obj value="<?=$GrTarif_id[0]["grtarif_id"]?>">
	<!-- параметры -->
	<tr><th colspan=2 class="zagolovok">Редактирование группы тарифов «<?=$GrTarif_id[0]["grtarif_name"]?>»</th></tr>
	<tr><th>Название тарифа</th><td><input type="text" name="grtarif_name" id=edit_grtarif_name value='<?=$GrTarif_id[0]["grtarif_name"]?>'></td></tr>
	<tr><th>Описание тарифа</th><td><input type="text" name="grtarif_desc" id="edit_tarif_desc" value='<?=$GrTarif_id[0]["grtarif_desc"]?>' size="40"></td></tr>
	<tr><td colspan=2><input type="submit" name="sendBtn" class="button" value="Сохранить" id="save"/><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>				
</table>
<script>
	$("#result").hide();
	 $("#save").click(function(event) {

		$("#result").show();
		$('#APISetGroupTarifArray').each(function() {
			var values = {};
			$('input[type=hidden],input[type=number],input[type=text],select', this).each(function(i) {
				values[this.name] = this.value;
				//alert(this.name);
			});
			$.post("query/json_query.php", {
				FUNCTION : "APISetGroupTarifArray",
				'QUERY[]' : values
			}, function(response) {
				//json parse
				var data = eval("("+response+")");
				if (data.error == '0') {
					window.close();
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
				}
				
			});
		});

	}); 

</script>