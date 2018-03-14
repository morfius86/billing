<?php
/* 
* Online  DCHP Leases
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

<p id="tabname">DHCP-аренда<p>
<div class="section">
    <table class="forms">
        <form action="" autocomplete="off" method="get">
			<input name="page" type="hidden" value="online_dhcp">
            <tr>
                <th>Имя/Договор</th>
                <th>Сервера</th>
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
                <td><input type="submit" value="Найти"></td>
            </tr>
        </form>
    </table>
	<table id="onlinetable" class="tablesorter">
		<tbody>
<?php
		if (isset($_GET["online_nas"])) {

			$NasTable = $tinsp->APIGetDhcpOnline($_GET["online_nas"]);	// получаем данные с нас-сервера 
			
			$StatusArray = array ("bound" => "арендован", "busy"=>"занят", "offered"=>"не подтвердил аренду", "waiting"=>"ожидание");
			
			if (is_array($NasTable) and $NasTable[0] == "0") {
				foreach ($NasTable[1] as $NasTableValue) {
					// Фильтруем
					if (!empty($_GET["online_fio_dogovor"]) and ($_GET["online_fio_dogovor"] != $NasTableValue["dogovor"] and !preg_match("/".$_GET["online_fio_dogovor"]."/i", $NasTableValue["abonent"]))) {
						continue;
					}				
?>
					<tr>
						<td><?=$NasTableValue["dogovor"]?></td>
						<td>
							<a onclick="window.open('opener.php?opener=edit_abonent&id=<?=$NasTableValue["user_id"]?>','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=550,Height=598');"
							   href="#"><?=$NasTableValue["abonent"]?></a></td>
						<td><?=$NasTableValue["mac-address"]?></td>
						<td><?=$NasTableValue["active-address"]?></td>
						<td><?=$NasTableValue["expires-after"]?></td>
						<td><?=$NasTableValue["last-seen"]?></td>
						<td><?=$NasTableValue["host-name"]?></td>
						<td><?=($NasTableValue["dynamic"] == "true") ? "D": "" ?></td>
						<td><?=$StatusArray[$NasTableValue["status"]]?></td>
					</tr>
<?php
					$NasCountStat["users"]++;	// счетчик абонентов
					
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
			<th>MAC-адрес</th>
			<th>IP-адрес</th>
			<th>Аренда истекает</th>
			<th>Аренда последняя</th>
			<th>Имя хоста</th>  
			<th>Динамический</th>
			<th>Статус</th>
		</tr>
		<tr>
			<td><b>Всего: <?=$NasCountStat["users"]?></b></td>
			<td colspan="7">&nbsp;</td>
			<td></td>
		</tr>
		</thead>
	</table>
</div>

