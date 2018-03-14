<?php
/*	
Редактор Обещанных платежей у тарифы	
*/
if (isset($_GET["id"])){
	$Tarif_id=$_GET["id"];
	if(strlen($Tarif_id)>2){
		$Tarif_id_options=$tinsp->APIGetTarif(2, $Tarif_id);
		if(!is_array($Tarif_id_options)){
			exit("Тариф с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
?>
<!--форма редактирования тарифы-->
<table class="forms" style="width:100%;">
	<th colspan=2>
		<table class="forms" width="100%">
			<tr>
				<th><a href="?opener=edit_tarif&id=<?php print $Tarif_id?>">Базовые</a></th>
				<th><a href="?opener=edit_tarif_lk&id=<?php print $Tarif_id?>">Расширенные</a></th>
				<th><a href="?opener=edit_tarif_cred&id=<?php print $Tarif_id?>">Обещанный платеж</a></th>
			</tr>
		</table>
	</th>
	<!-- создать кредит -->
	<tr><td colspan=2 align=center>Редактирование тарифа «<?=$Tarif_id_options[0]["tarif_name"]?>» : Обещанный платеж</td></tr>
	<form action="query/json_query.php" id="APICreatTarifCredit">
	<tr>
		<td colspan=2>
		  <details>
			<summary align=center>Создать обещанный платеж</summary>
			<fieldset>
				<input type="hidden" name=tarif_id id=tarif_id value="<?=$Tarif_id_options[0]["tarif_id"]?>">
				<br>
				<input type="text" name=tarif_cred_name id=tarif_cred_name value=""><label for="tarif_cred_name" title="Название кредита ">Название кредита</label><br>
				<input type="number" name=tarif_cred_day id=tarif_cred_day value="1" min=1 max=31><label for="tarif_cred_day" title="Выдаваемая сумма">Кол-во /дн.</label><br>
				<input type="number" name=tarif_cred_sum id=tarif_cred_sum value="1" min=1 max=10000><label for="tarif_cred_sum" title="На сколько дней выдавать тариф">Сумма /руб.</label><br>
				<input type="number" name=tarif_cred_dopsum id=tarif_cred_dopsum value="0" min=0 max=10000><label for="tarif_cred_dopsum" title="Сколько списывать доп. коммиссии по окончанию кредита">Коммиссия /руб.</label><br>
				<input type="number" name=tarif_cred_limitmonth id=tarif_cred_limitmonth value="1" min=1 max=31><label for="tarif_cred_limitmonth" title="Кол-во раз разрешенное брать в месяц">Кол-во активаций в месяц</label>
				
			  <p><input type="submit" name="sendBtn" value="Создать" /></p>
			  <div id="result"></div>
			</fieldset>
		</details>
		</td>
	</tr>			
	</form>	
	<!-- список кредов  
		<input type="radio" name=tarif_cred_onoff id=tarif_cred_onoff value="1"><label for="tarif_cred_onoff" title="Резрешен">Вкл</label>
		<input type="radio" name=tarif_cred_onoff id=tarif_cred_onoff value="0"><label for="tarif_cred_onoff" title="Запрещен">Выкл</label>
	-->
	<tr>
		<td colspan=2>
			<details open=open>
			<summary align=center>Список обещанный платежей</summary>
				<table class="tablesorter">
				 <thead>
					<tr>
					 <th>#</th><th>Название</th><th>Кол-во дней</th><th>Сумма /руб.</th><th>Комиссия /руб.</th><th>Лимит в месяц</th><th>on/off</th><th>del</th>
					</tr> 
				 </thead>
				 <tbody>
				<?php
				$GetCredit=$tinsp->APIGetTarifCredit("2", $Tarif_id_options[0]["tarif_id"]);
				if($GetCredit[0]=="0"){
					$i=1;
					foreach($GetCredit[1] as $GetCreditKey => $GetCreditVal){
						print "<tr id='cred_".$GetCreditVal["tarif_cred_id"]."'>
							<td>".$i++."</td>
							<td>".$GetCreditVal["tarif_cred_name"]."</td>
							<td>".$GetCreditVal["tarif_cred_day"]."</td>
							<td>".$GetCreditVal["tarif_cred_sum"]."</td>
							<td>".$GetCreditVal["tarif_cred_dopsum"]."</td>
							<td>".$GetCreditVal["tarif_cred_limitmonth"]."</td>
							<td><input type=checkbox name='check_onoff' id='check_onoff' value='".$GetCreditVal["tarif_cred_id"]."' ".($GetCreditVal["tarif_cred_onoff"] == '0' ? 'checked' : '')."></td>
							<td><button name='delete' value='".$GetCreditVal["tarif_cred_id"]."'><img src='img/delete.png' width='16' style='vertical-align: middle'></button></td>
						</tr>";
					}
				}
				?>
				</tbody>
				</table>
			</details>
		</td>
	</tr>
</table>
<script type="text/javascript">
//delete
 $("button[name=delete]").click(function(event) {
	var del = $(this);
	var arr = {};
		arr['0'] = {};
		arr['0']['value'] = ""+del.attr("value")+"";

	$.post("query/json_query.php", {FUNCTION: "APIDelTarifCredit", QUERY:arr},
		function(response){
			//alert(response);
			//json parse
			var data = eval("("+response+")");
			if(data.error[0] != 0){
				alert("Не удаеться удалить кредит, ошибка: "+data.error[1]+"");
			}else{
				$("#cred_"+del.attr("value")+"").hide();
			}
		}
    );
 });
//check off/on
 $("input[name=check_onoff]").click(function(event) {
	var check = $(this);
	var arr = {};
		arr['0'] = {};
		if (check.attr("checked") == true){	
			arr['0']['value'] = "update billing_tarification_credit set `tarif_cred_onoff` = 0 where `tarif_cred_id` = '"+check.attr("value")+"'";
		}else{
			arr['0']['value'] = "update billing_tarification_credit set `tarif_cred_onoff` = 1 where `tarif_cred_id` = '"+check.attr("value")+"'";
		}
		arr['1'] = {};
		arr['1']['value'] = "true";
	$.post("query/json_query.php", {FUNCTION: "Query", QUERY:arr},
		function(response){
			//json parse
			var data = eval("("+response+")");
			if(data.error != 0){
				alert("Ошибка про сихранении параметров "+data.error+"");
			}
		}
    );
 });

$("#result").hide();
 $("#APICreatTarifCredit").submit(function(event) {
 $("#result").show();
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
	//alert(form.serialize());
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			//alert(response);
			var data = eval("("+response+")");
			if(data.error[0] == 0){
				$("#result").html("Успешно создано");
				window.location.reload(true);
			}else{
				$("#result").html("Ошибка: " + data.error[0] + " Сообщение: " + data.error[1] + "");
			}

		}
    );
  });
 $("#APISetTarifCredit").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
     	$('table.forms').each(function() {
			var values = {};
			$('input[type=hidden],input[type=number],input[type=text]', this).each(function(i) {
				values[this.name] = this.value;

			});
			$.post("query/json_query.php", {
				FUNCTION : "APICreatTarifCredit",
				'QUERY[]' : values
			}, function(response) {
				//json parse
				alert(response);
				var data = eval("(" + response + ")");
				if(data.error[0] == 0){
					$("#result").html("Успешно сохранено");
				}else{
					$("#result").html("Ошибка: " + data.error[0] + " Сообщение: " + data.error[1] + "");
				}
				
			});
		});
  });
</script>