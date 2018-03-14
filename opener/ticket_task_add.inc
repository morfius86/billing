<table class="tableticket">
	<form action="query/json_query.php" id="APISetUser">
	<!-- параметры -->
	<tr><td colspan=2 align=center>Добавить задание</td></tr>
	<tr><th>Исполнитель</th>
		<td>
			<select name="ticket_exec">
				<?php
				foreach($tinsp->Query("Select * From billing_access",false) as $Access){
					echo "<option value='".$Access["access_id"]."'>".$Access["access_name"]."</option>";
				}
				?>
			</select>
		</td>
	</tr>
	<tr><th>Дата начала</th>
		<td>
			<input type=text name=start_date id=start_date size=10 class="w8em format-y-m-d divider-dash highlight-days-5" value='<?=date("Y-m-d")?>'> <input type="number" min="00" max="23" value="00" step="01" size=2 name=start_date_h id=start_date_h/>час.
			<input type="number" min="00" max="59"  value="00" step="01" name=start_date_h id=start_date_h />мин.
		</td>
	</tr>
	<tr><th>Дата окончания</th>
		<td>
			<input type=text name=end_date id=end_date size=10 class="w8em format-y-m-d divider-dash highlight-days-5" value='<?=date("Y-m-d")?>'> <input type="number" min="00" max="23" value="00" step="01" size=2 name=end_date_h id=end_date_h/>час.
			<input type="number" min="00" max="59"  value="00" step="01" name=end_date_i id=end_date_i/>мин.
		</td>
	</tr>
	<tr><th>Категория</th>
		<td>
			<select name="ticket_cat">
				<?php
				foreach($tinsp->Query("Select * From billing_ticket_category",false) as $Category){
					echo "<option value='".$Category["id"]."'>".$Category["cat_name"];
				}
				?>
			</select>
		</td>
	</tr>
	<tr><th>Приоритет</th>
		<td>
			<input type="radio" name="ticket_priority" value=1 id="ticket_priority1"><label class="prior0" for="ticket_priority1">ВЫСОКИЙ</label><br>
			<input type="radio" name="ticket_priority" value=2 id="ticket_priority2"><label class="prior2" for="ticket_priority2">СРЕДНИЙ</label><br>
			<input type="radio" name="ticket_priority" value=3 id="ticket_priority3"><label class="prior1" for="ticket_priority3">НИЗКИЙ</label>
		</td>
	</tr>
	<tr><th>Описание задания</th>
		<td><textarea name="message" id="message" cols="30" rows="4"></textarea></td>
	</tr>
	
	<tr><td colspan=2><input type="submit" name="sendBtn" value="Сохранить" id="save"/><input type="button" name="sendBtn" value="Закрыть" onclick="javascript:window.close();"/></td></tr>	
	</form>
</table>
<script>
//отправка формы
 $("#APISetUser").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
	window.opener.location = "https://bill.vash-net.ru/?page=ticket_task&select_day=true&day=10&month=2&year=2013";
	window.close();
    
  });
</script>