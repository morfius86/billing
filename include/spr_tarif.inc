<?php
/*
 Справочник Group
 */
?>
<script type="text/javascript">
	$(document).ready(function() {
	
		$('[multiple]').each(function() {
			$('#'+this.id+'').multipleSelect({
				placeholder: "Выберите тарифы",
				width: 300,
			});
		});
	
		$("#tariftable").tablesorter();
	});

	function remove_tarif(id) {
	$.prompt("Уверены в своем желании?", {
		title: "Вы уверены",
		buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
		submit: function(e,v,m,f){
			if (v) {
				$.post("query/json_query.php", {
					FUNCTION : "APIDelTarif",
					QUERY : id
				}, function(response) {
					//alert(response);
					var data = eval("("+response+")");
					if (data.error == '0') {
						$('#id'+id).hide('slow', function() {$(this).remove();});
						$.prompt("Успешно удален!");
					} else {
						$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
				}});
			}}
		});
	}
	function remove_tarif_group(id) {
		$.prompt("Уверены в своем желании?", {
			title: "Вы уверены",
			buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
			submit: function(e,v,m,f){
				if (v) {
					$.post("query/json_query.php", {
						FUNCTION : "APIDelGroupTarif",
						QUERY : id
						}, function(response) {
						//alert(response);
						var data = eval("("+response+")");
						if (data.error == '0') {
							$('#grtar'+id).hide('slow', function() {$(this).remove();});
							$.prompt("Успешно удален!");
						} else {
							$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
					}});
				}}
		});
	}
</script>
<?
/* Получаем данные */
$TarifNaspArray=$tinsp->APIGetTarifNasp();

if ($_GET["filter"]=="true") {
	$TarifArray=$tinsp -> APIGetTarif(3, $_POST["filter_nasp"]);
}else{
	$TarifArray=$tinsp -> APIGetTarif(2, "");
}

$NaspArray=$tinsp -> APIGetNasp(2, "");
?>
<p id="tabname">
	СПРАВОЧНИК ТАРИФОВ
<p>
	<div class="section">
		<ul class="tabs">
			<li class="current">
				<span>Список тарифов</span>
			</li>
			<li>
				<span>Группы тарифов</span>
			</li>
			<li>
				<span >Фильтры</span>
			</li>
			<li>
				<span>Новый тариф</span>
			</li>
		</ul>
		<div class="box visible" >
			<!--список групп-->
			<table id="tariftable" class="tablesorter">
				<thead>
					<tr>
						<th>Название тарифа</th><th>Тип</th><th>Тип абон. платы</th><th>Абон. плата</th><th>Вх/Исх Мбит.</th><th>Признак группы</th><th>Вес</th><th>Абонентов</th><th width=20>Опции</th>
					</tr>
					<tr>
						<td colspan="9">
							<form action="?page=spr_tarif&filter=true" method="post">
							<?php
								if (is_array($NaspArray)){
									echo '<select name="filter_nasp">';
									foreach ($NaspArray as $NaspKey => $NaspVal) {
										echo "<option value='".$NaspVal["nasp_id"]."' ".(isset($_POST["filter_nasp"]) && $_POST["filter_nasp"] == $NaspVal["nasp_id"] ? 'selected':'')." >".$NaspVal["nasp_name"]."</option>";
									}
									echo '</select>';
								}
							?>
								<input type="checkbox" name="filter_work" value="1" checked>рабочие<input type="checkbox" name="filter_arhive" value="1" checked>архивные
								<input type="submit" class="buttom" value="фильтр">
							</form>
						</td>
					</tr>
				</thead>
				<tbody>
					<?php
					

					if (is_array($TarifArray))
						foreach ($TarifArray as $TarifKey => $Tarif) {
							$CountAbon=$tinsp->Query("SELECT count('id') as count FROM `billing_users` where billing_users.tarif_id = '".$Tarif["tarif_id"]."'", false);
							$type = $tinsp -> APIGetTypeTarif($Tarif["type"]);
							$type_abon = $tinsp -> APIGetTypeTarifAbon($Tarif["type_abon"]);
							if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
								$TarifGroup=$tinsp->APIGetGroupTarif($Tarif["grtarif_id"]);
								echo '<tr><th colspan=9 class="zagolovok">Группа тарифов: '.$TarifGroup[0]["grtarif_name"].'</th></tr>';
							}
							echo "<tr id='id".$Tarif["tarif_id"]."' ".($Tarif["tarif_arhive"]=="1" ? 'style="text-decoration: line-through; color: rgb(179, 179, 179);"': '').">
								<td id=tarif_name>" .$Tarif["tarif_name"]. "</td>
								<td id=type>" . $type[0]["value"]. "</td>
								<td id=type_abon>" .$type_abon[0]["value"] . "</td>
								<td><span id=abon_cash>" . $Tarif["abon_cash"] . "</span> <span id=valute>" . $Tarif["valute"] . "</span></td>
								<td><span id=tarif_speed_in>" . $Tarif["tarif_speed_in"] . "</span>/<span id=tarif_speed_out>" . $Tarif["tarif_speed_out"] . "</span></td>
								<td id=tarif_prizn>".$Tarif["tarif_prizn"]."</td>
								<td id=tarif_ves>".$Tarif["tarif_ves"]."</td>
								<td><meter value='".$CountAbon[0]["count"]."' max='".$CountBase[0]["count(id)"]."'></meter><b>".$CountAbon[0]["count"]."</b> (".number_format($CountAbon[0]["count"]/$CountBase[0]["count(id)"]*100, 2)."%)</td>
								<td><a onclick='window.open(\"opener.php?opener=edit_tarif&id=" . $Tarif["tarif_id"] . "\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=500\");' href='javascript:void(0);'><img src='img/Setting-icon.png' width='20'></a>
								<a href='javascript:;' onclick='remove_tarif(\"" . $Tarif["tarif_id"] . "\");'><img src='img/delete.png' width='16'></a></td>
								</tr>";
						}
					?>
				</tbody>
			</table>
		</div>
		<div class="box">
			<table class=forms>
				<form action="query/json_query.php" id="APICreatGroupTarif">
					<tr>
						<th colspan=2 class="zagolovok">Группы тарифов</td>
					</tr>
					<tr>
						<th>Название группы тарифов</th><td>
							<input type="text" name="grtarif_name" id="grtarif_name">
						</td>
					</tr>
					<tr>
						<th>Описание группы тарифов</th><td>
							<input type="text" name="grtarif_desc" id="grtarif_desc"  value="">
						</td>
					</tr>
					<tr>
						<td colspan=2>
							<input type="submit" class="button" name="sendBtn" value="Создать" />
						</td>
					</tr>
				</form>
				<tr><td colspan=2>
				<table class="forms" style="width: 100%;">
					<tr><th class="zagolovok">Название группы</th><th class="zagolovok" colspan="2">Описание</th></td>
					<?php
						$GroupTarif=$tinsp->APIGetGroupTarif("");
						if (is_array($GroupTarif)){
							foreach($GroupTarif as $GroupTarifVal){
							echo "<tr id='grtar".$GroupTarifVal["grtarif_id"]."'><td>".$GroupTarifVal["grtarif_name"]."</td>
								<td>".$GroupTarifVal["grtarif_desc"]."</td>
								<td>
									<a onclick='window.open(\"opener.php?opener=edit_tarif_group&id=".$GroupTarifVal["grtarif_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=200\");' href='#'><img src='img/Setting-icon.png' width='20'></a>
									<a href='javascript:;' onclick='remove_tarif_group(\"" . $GroupTarifVal["grtarif_id"] . "\");'><img src='img/delete.png' width='16'></a>
								</td>
								</tr>";
							}
						}
					?>
				</table>
				</td></tr>
			</table>
		</div>
		<!-- тарифы по населенным пунктам-->
		<div class="box">
			<div class="title_info">Фильтр тарифов по населенным пунктам применяется при создании учетных записей и так же для дополнительной фильтрации тарифов в группах по населенным пунктам. 
			</div>
			<table class="forms">
				<form action="query/json_query.php" id="APISetTarifNasp">
					<tr>
						<th colspan=2 class="zagolovok">Использование тарифов</td>
					</tr>
					<?php
					if (is_array($NaspArray)){
						foreach ($NaspArray as $NaspKey => $NaspVal) {
							echo '<tr><th>'.$NaspVal["nasp_name"].'</th>';
							echo '<td><select name="nasp_tarif['.$NaspVal["nasp_id"].'][]" id="nasp_id_'.$NaspVal["nasp_id"].'" multiple="multiple">';
                            if (is_array($TarifArray)){
								foreach ($TarifArray as $TarifKey => $Tarif) {
									if($TarifArray[$TarifKey-1]["grtarif_id"]!=$Tarif["grtarif_id"]){
										echo '<optgroup label="'.$Tarif["grtarif_name"].'">';
									}

									echo "<option value='".$Tarif["tarif_id"]."' "; 
									#Ищем выбранные тарифы
									foreach ($TarifNaspArray[1] as $TarifNaspArrayVal){
										if ($TarifNaspArrayVal["nasp_id"] == $NaspVal["nasp_id"] && $TarifNaspArrayVal["tarif_id"] == $Tarif["tarif_id"]){ 
											echo "selected";
										}
									}
									echo ">".$Tarif["tarif_desc"]." --- [".$Tarif["tarif_name"]."]</option>";
									
									if($TarifArray[$TarifKey+1]["grtarif_id"]!=$Tarif["grtarif_id"]){
										echo '</optgroup>';
									}	
								}
							}
							echo '</td></select></tr>';
						}
					}
					?>
					<tr>
						<td colspan=2>
							<input type="submit" class="button" name="sendBtn" value="Сохранить" />
						</td>
					</tr>
				</form>
			</table>
			
		</div>
		<!--форма регистрации тарифа-->
		<div class="box">
			<table class=forms>
				<form action="query/json_query.php" id="APICreatTarif">
					<tr>
						<td colspan=2 align="center">Тарификация</td>
					</tr>
					<tr>
						<th>Название тарифа</th><td>
						<input type="text" name="tarif_name" id="tarif_name">
						</td>
					</tr>
					<tr>
						<th>Описание тарифа</th><td>
							<input type="text" name="tarif_desc" id="tarif_desc" size="40" value="«» 00 Мбит/с">
						</td>
					</tr>
					<tr>
						<th>Группа тарифов</th>
						<td>
							<select name="grtarif_id" id="grtarif_id">
								<?php 
									$GroupTarif=$tinsp->APIGetGroupTarif();
									if (is_array($GroupTarif)){
										foreach($GroupTarif as $GroupTarifVal){
											echo '<option value="'.$GroupTarifVal["grtarif_id"].'">'.$GroupTarifVal["grtarif_name"].'</option>';
										}
									}
								?>
							</select>
						</td>
					</tr>
					<tr>
						<th>Типа тарификации</th>
						<td>
						<select name="type" id=type>
							<option value="1">Безлимитный</option>
							<option value="2">Лимитный</option>
						</select></td>
					</tr>
					<tr>
						<th>Тип абон. платы</th>
						<td>
						<select name="type_abon" id=type_abon>
							<?php
							foreach ($tinsp->APIGetTypeTarifAbon('') as $TypeAbonCash) {
								if ($TypeAbonCash["key"] == $Tarif_id_options[0]["type_abon"]) {
									echo '<option selected value="' . $TypeAbonCash["key"] . '">'.$TypeAbonCash["value"].'</option>';
								} else {
									echo '<option value="' . $TypeAbonCash["key"] . '">'.$TypeAbonCash["value"].'</option>';
								}
							}
							?>
						</select></td>
					</tr>
					<tr>
						<th>Абон. плата</th><td>
						<input type=text name=abon_cash id=abon_cash value="0">
						</td>
					</tr>
					<tr>
						<th title="Блокировать или нет абонента при достижении баланса < 0">Блокировать <img src="img/question.png" width=12></th>
						<td>
						<select name="tarif_block" id="tarif_block">
							<option value="0">Да</option>
							<option value="1">Нет</option>
						</select></td>
					</tr>
					<tr>
						<th>Валюта</th><td>
						<input type=text name=valute id=valute value="руб">
						</td>
					</tr>
					<tr>
						<th>Вход. скорость/ кбит</th><td>
						<input type=text name=tarif_speed_in id=tarif_speed_in value=0>
						</td>
					</tr>
					<tr>
						<th>Исход. скорость/ кбит</th><td>
						<input type=text name=tarif_speed_out id=tarif_speed_out value=0>
						</td>
					</tr>
					<tr><th>Вес тарифа</th><td>
						<input type=number name=tarif_ves id=tarif_ves value="0" min="0" max="100">
						</td>
					</tr>
					<tr>
						<td colspan=2>
						<input type="submit" class="button" name="sendBtn" value="send" />
						</td>
					</tr>
				</form>
			</table>
			<script>
				$("#APICreatTarif, #APICreatGroupTarif, #APISetTarifNasp").submit(function(event) {
					$("#result").show();
					event.preventDefault();
					/*VARs*/
					var form = $(this);
					var ArrayForm = form.serialize();
					url = form.attr("action");
					funct = form.attr("id");
					/*POST*/
					//alert(form.serialize());
					$.post(url, {
						FUNCTION : funct,
						QUERY : ArrayForm
					}, function(response) {
						//json parse
						//alert(response);
						var data = eval("(" + response + ")");
						if (data.error == '0') {
							$.prompt("Успешно.");
							//window.location.reload(true);
						} else {
							$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
						}
					});
				});
			</script>
		</div>
	</div>

