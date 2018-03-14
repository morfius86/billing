<?php
/* 
* Online 
*/
?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='text/javascript' src='js/jquery.autocomplete.js'></script>
<script>
    $('#logtable').columnHover();
    $(document).ready(function () {
		$("#onlinetable").tablesorter({
			sortList: [[5,1]] 
			});
	});
</script>

<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css"/>
<style type="text/css"></style>

<p id="tabname">ONLINE<p>
<div class="section">
    <table class="forms">
        <form action="" autocomplete="off" method="get">
			<input name="page" type="hidden" value="online">
            <tr>
                <th>Имя/Договор</th>
                <th>Сервера</th>
                <th>Автообновление</th>
				<th>&nbsp;</th>
            </tr>
            <tr>
                <td><input type="text" name="online_fio_dogovor" id="online_fio_dogovor" value="<?=$_GET["online_fio_dogovor"]?>"></td>
                <td>
                    <select name="online_nas" id="online_nas">
                        <?php foreach ($tinsp->APIGetNas(2, "") as $nas) {
                            echo '<option value="' . $nas["nas_id"] . '" '.(($_GET["online_nas"] == $nas["nas_id"])? 'selected' : '').'>' . $nas["nas_name"] . '</option>';
                        } ?>
                    </select>
                </td>
				<td>
					<input type="checkbox" name="online_auto" <?php print (($_GET["online_auto"])? 'checked':'')?>>
					<input type="number" name="online_auto_sec" style="width: 2.5em" value="<?php print ((!empty($_GET["online_auto_sec"])) ? $_GET["online_auto_sec"] : 5)?>" min="2" max="100"> сек<br>
					<input type="checkbox" name="online_auto_graph" <?php print (($_GET["online_auto_graph"])? 'checked':'')?>> График
				</td>
                <td><input type="submit" value="Найти"></td>
            </tr>
        </form>
    </table>
	<table id="onlinetable" class="tablesorter">
		<tbody>
<?php
		if (isset($_GET["online_nas"])) {
			
			// включен autorefresh
			if (isset($_GET["online_auto"])) {
				echo '<meta http-equiv="refresh" content="'.$_GET["online_auto_sec"].';'.$_SERVER["REQUEST_URI"].'">';
			}else{
				unset($_SESSION["NAS_ONLINE_GRAPH"]);	// удалим данные по графику
				unset($_SESSION["NAS_ONLINE"]);			// удалим данные по скорости
			}

			$NasTable = $tinsp->APIGetNasOnline($_GET["online_nas"]);	// получаем данные с нас-сервера 
			$NasCountStat = array('user'=> 1, 'tx'=> 0, 'rx' => 0);		// собираем статистику
			
			if (is_array($NasTable) and $NasTable[0] == "0") {
				foreach ($NasTable[1] as $NasTableValue) {
					// Фильтруем
					if (!empty($_GET["online_fio_dogovor"]) and ($_GET["online_fio_dogovor"] != $NasTableValue["dogovor"] and !preg_match("/".$_GET["online_fio_dogovor"]."/i", $NasTableValue["abonent"]))) {
						continue;
					}
					// Есть данные в SESSION, рассчитываем дельту скорости tx/rx
					if (is_array($_SESSION["NAS_ONLINE"])){
						$NasTableValue["tx"] = number_format(((($NasTableValue["bytes-in"]) - ($_SESSION["NAS_ONLINE"][$NasTableValue[".id"]]["in"])) / ($NasTableValue["time"] - $_SESSION["NAS_ONLINE"][$NasTableValue[".id"]]["time"])) / 131072, 2);
						$NasTableValue["rx"] = number_format(((($NasTableValue["bytes-out"])- ($_SESSION["NAS_ONLINE"][$NasTableValue[".id"]]["out"]))/ ($NasTableValue["time"] - $_SESSION["NAS_ONLINE"][$NasTableValue[".id"]]["time"])) / 131072, 2);
					}				
?>
					<tr>
						<td><?=$NasTableValue["dogovor"]?></td>
						<td>
							<a onclick="window.open('opener.php?opener=edit_abonent&id=<?=$NasTableValue["user_id"]?>','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598');"
							   href="#"><?=$NasTableValue["abonent"]?></a></td>
						<td><?=$NasTableValue["user"]?></td>
						<td><?=$NasTableValue["address"]?></td>
						<td><?=str_replace(array('h','m','s'), array(' час ',' мин ',' сек'), $NasTableValue["uptime"])?></td>
						<td><?=$NasTableValue["rx"]?></td>
						<td><?=$NasTableValue["tx"]?></td>
						<td>
<?php 						// Включено построение графиков
							if (isset ($_GET["online_auto_graph"]) and is_array($_SESSION["NAS_ONLINE_GRAPH"])) {
								echo '<img src="sys/phplot/index.php?data='.base64_encode(serialize($_SESSION["NAS_ONLINE_GRAPH"][$NasTableValue[".id"]])).'">';
							}
?>
						</td>
					</tr>
<?php
					$NasCountStat["users"]++;	// счетчик абонентов
					
					// включен autorefresh
					if (isset($_GET["online_auto"])) {
						$_SESSION["NAS_ONLINE"][$NasTableValue[".id"]] = array("in" => $NasTableValue["bytes-in"], "out" => $NasTableValue["bytes-out"], "time" => $NasTableValue["time"]);	// запишем полученные данные скорости
						$_SESSION["NAS_ONLINE_GRAPH"][$NasTableValue[".id"]][] = array(date("i:s"), $NasTableValue["rx"], $NasTableValue["tx"]);	// запишем данные для построения графиков
						// максимум 20 точек на графике на абонента
						if (count($_SESSION["NAS_ONLINE_GRAPH"][$NasTableValue[".id"]]) > 20) {
							array_shift($_SESSION["NAS_ONLINE_GRAPH"][$NasTableValue[".id"]]);
						}
						
						$NasCountStat["tx"]=$NasCountStat["tx"]+$NasTableValue["tx"];	// подсчитаем  скорость tx
						$NasCountStat["rx"]=$NasCountStat["rx"]+$NasTableValue["rx"];	// подсчитаем  скорость rx
					}
				}	
			}
		} else {
			echo '<tr><td colspan="9" align="center">Нажмите "Найти", чтобы показать список.</td></tr>';
		}
?>       
		</tbody>
		<thead>
		<tr>
			<th>Договор</th>
			<th>Пользователь</th>
			<th>User/MAC-адрес</th>
			<th>IP-адрес</th>
			<th>Время авторизации</th>
			<th>Download / Мбит</th>
			<th>Upload / Мбит</th>  
			<th>График</th>
		</tr>
		<tr>
			<td><b>Всего: <?=$NasCountStat["users"]?></b></td>
			<td colspan="4">&nbsp;</td>
			<td><b><?=$NasCountStat["rx"]?> Мбит</b></td>
			<td><b><?=$NasCountStat["tx"]?> Мбит</b></td>
			<td></td>
		</tr>
		</thead>
	</table>
</div>

