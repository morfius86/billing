  <style type="text/css">
   #menu { /* Левая колонка */
    float: left; /* Обтекание справа */
	#border: 1px solid #333; /* Параметры рамки вокруг */
    padding: 0px; /* Поля вокруг текста */

   }

   #content { /* Правая колонка */
    margin: 10px 5px 20px 195px; /* Значения отступов */
    padding: 5px; /* Поля вокруг текста */
	#border: 1px solid #333; /* Параметры рамки */
	width: 100%
   }

  </style>
<?php
/*
Список заданий
*/
$Week_name_arr=array(1 => "Понедельник", 2 => "Вторник", 3 => "Среда", 4 => "Четверг", 5 => "Пятница", 6 => "Суббота", 0 => "Воскресенье");
$Month_Text=array(1=>"Январь",2=>"Февраль",3=>"Март",4=>"Апрель",5=>"Мая",6=>"Июнь",7=>"Июль",8=>"Август",9=>"Сентябрь",10=> "Октябрь",11=>"Ноябрь",12=>"Декабрь");


function day_week($week_number, $year){
	//получаем дату первого января заданного года
	$dw = date("w", mktime(0,0,0,1,1,$year));
	if (($dw>0)&($dw<5)){
		$week_number--;
	}
	for ($i=1;$i<8;$i++){
		$key=$week_number * 7 * 86400 + mktime(0,0,0,1,1,$year) - date('w', mktime(0,0,0,1,1,$year)) * 86400 + 86400*$i;
		$value=date('Y-m-d', $week_number * 7 * 86400 + mktime(0,0,0,1,1,$year) - date('w', mktime(0,0,0,1,1,$year)) * 86400 + 86400*$i);	
		$day[$key]=$value;
	}
	return $day;
}

// кол-во дней в месяце
function genSet_Stop($month,$year) {
	if(strlen($month)==1) $month = "0".$month;
    if ($month == '12') {
		$month = 1;
		$year++;
    }
    else $month++;
    $stop = date("d", mktime(0,0,0,$month,0,$year));
    return $stop;
}

// генерация календаря
function genCalendar_Month($month,$year,$stop) {
	global $Month_Text;
	$month=intval($month);

	if ($month == 12) {
		$prev_month = $month - 1;
		$prev_year = $year;
		$next_month = 1;
		$next_year = $year + 1;
	}elseif ($month == 1) {
		$prev_month = 12;
		$prev_year = $year - 1;
		$next_month = $month + 1;
		$next_year = $year;
	}else {
		$prev_month = $month - 1;
		$prev_year = $year;
		$next_month = $month + 1;
		$next_year = $year;
	}
	
	$string = '<tr><td align="center" colspan=7>';
	$string.= '<form action="?page=ticket_task" method=post name="submit_calendar"><select name=select_month>';
		foreach ($Month_Text as $Month_Key => $Month_Val) {
			if ($Month_Key==(int)$month){
				$string.="<option selected value='".$Month_Key."'>".$Month_Val."</option>";
			}else{
				$string.="<option value='".$Month_Key."'>".$Month_Val."</option>";
			}
			
		}
	$string.= '</select>';
	$string.= '<select name=select_year>';
		for ($i=2012; $i<=2020; $i++) {
			if ($i==$year){
				$string.="<option selected value='".$i."'>".$i."</option>";
			}else{
				$string.="<option value='".$i."'>".$i."</option>";
			}
		}
	$string.= '</select><button type="submit" value="ok">></button></form></td>' .
	'</tr>' .
       '<tr><td  align="center"><font color="#666666"><b>Пн</b></font></td>'."\n" .
       '<td align="center"><font color="#666666"><b>Вт</b></font></td>'."\n" .
       '<td align="center"><font color="#666666"><b>Ср</b></font></td>'."\n" .
       '<td align="center"><font color="#666666"><b>Чт</b></font></td>'."\n" .
       '<td align="center"><font color="#666666"><b>Пт</b></font></td>'."\n" .
       '<td align="center"><font color="#2D83E9"><b>Сб</b></font></td>'."\n" .
       '<td align="center"><font color="#2D83E9"><b>Вс</b></font></td></tr>'."\n" .
       '<tr class=ex3>';
    $start = date("w",mktime(0,0,0,$month,1,$year)) - 1;
    if ($start == -1) $start = 6;
    for ($i=0; $i<$start; $i++)   $string .= '<td>&nbsp;</td>';

    $frame = $start - 1;

    for ($i=1; $i<=$stop; $i++) {
        $day = mktime(0,0,0,date("m"),$i,date("Y"));
        $frame++;
        if($frame > 6) {
            $string .= "</tr>\n";
            if($i < $stop) $string .= '<tr class=ex3>';
            $frame = 0;
        }

        if($month == date("m", $day) && $year == date("Y", $day) && date("d") == date("d", $day) ) {
            $string .= "<td align=\"center\"><a class=cal_day2 href=\"?page=ticket_task&select_day=true&day=$i&month=$month&year=$year\">$i</a></td>";
            continue;
        }

        if ($frame == 5 || $frame == 6) {
            $string .= "<td align=\"center\"><a class=cal_day3 href=\"?page=ticket_task&select_day=true&day=$i&month=$month&year=$year\">$i</a></td>";
        }else {
            $string .= "<td align=\"center\"><a class=cal_day1 href=\"?page=ticket_task&select_day=true&day=$i&month=$month&year=$year\">$i</a></td>";
        }
    }

    for ($i=1; $frame < 6; $frame++) $string .= "<td>&nbsp;</td>";

    if ($frame < 6) $string .= "</tr>";
    return $string;
}
?>

	<!--КАЛЕНДАРЬ-->
	<p id="tabname">Календарь заданий<p>	
	<div id="menu">
	  <table width=100%>
			<tr>
				<td>
				<table class="tableticket">
					<?php
					#ВЫБРАН КАЛЕНДАРЬ
					if (isset($_POST["select_month"])) {
						$month = $_SESSION['selected_calendar_month'] = $_POST["select_month"];
						$year = $_SESSION['selected_calendar_year'] = $_POST["select_year"];
					}else{
						if (!isset($_SESSION['selected_calendar_month'])){
							$month = date("m");
							$year = date("Y");
						}else{
							$month = $_SESSION['selected_calendar_month'];
							$year = $_SESSION['selected_calendar_year'];
						}				
					}
					$day_number = genSet_Stop($month, $year);
					print $mid_html = genCalendar_Month($month, $year, $day_number);
					?>
				</table>
				</td>
			</tr>
			<tr>
				<td>
				<table class="tableticket">
					<tr><th>Статус</th></tr>
					<tr>
						<td>
						<input type="checkbox" id="status0" value="0" name="ticket_status"><label for="status0">Активные</label><br>
						<input type="checkbox" id="status1" value="1" name="ticket_status"><label for="status1">Просроченные</label><br>
						<input type="checkbox" id="status2" value="2" name="ticket_status"><label for="status2">Завершенные</label>
						</td>
					</tr>
					<tr><th>Категории</th></tr>
					<tr>
						<td>
							<?php
								foreach($tinsp->Query("Select * From billing_ticket_category",false) as $Category){
									echo "<input type=\"checkbox\" name=ticket_cat value='".$Category["id"]."'>".$Category["cat_name"]."<br>";
								}
								//print_r($_SESSION);
							?>
							
						</td>
					</tr>
					<tr><th>Автор задания</th></tr>
					<tr>
						<td>
							<select name="ticket_autor">
							<?php
								if ($_SESSION["access"] == "ALL_ACCESS" or eregi("ticket_autors", $_SESSION["access"])) {
									foreach($tinsp->Query("Select * From billing_access",false) as $Access){
										if($_SESSION['access_id'] == $Access["access_id"]) {
											echo "<option selected value='".$Access["access_id"]."'>".$Access["access_name"]."</option>";
										}else{
											echo "<option value='".$Access["access_id"]."'>".$Access["access_name"]."</option>";
										}	
									}
								}else{
									echo "<option value='".$_SESSION['access_id']."'>".$_SESSION['login']."</option>";
								}
								
							?>
							</select>
						</td>
					</tr>
					<tr><th>Исполнитель</th></tr>
					<tr>
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
				</table>
				</td>
			</tr>
		</table>
	</div>
	<div id="content">
	<table id="task" class="tableticket">
		<thead>
			<tr>
				<td colspan=8> <button onclick="window.open('opener.php?opener=ticket_task_add','','Toolbar=0,Location=0,Directories=0,Status=0,Menubar=0,Scrollbars=1,Resizable=1,Width=450,Height=400');"><img width=30 src="img/creat_task.png" alt="Зонтик" style="vertical-align: middle"> Добавить</button></td>
			</tr>
			<tr>
				<th>Исполнитель</th>
			<?php
				#Выбран день в календаре
				if ($_GET["select_day"]=="true"){
					$Week_days_arr=day_week(date("W", mktime(0,0,0,$_GET["month"], $_GET["day"], $_GET["year"])), $_GET["year"]);
				}else{
					$Week_days_arr=day_week(date("W"), date("Y"));
				}
				
				foreach($Week_days_arr as $day_week_time => $day_week_date){
					echo "<th>".date("d", $day_week_time)." ".$Month_Text[(int)date("m", $day_week_time)]."<br>".$Week_name_arr[date("w", $day_week_time)]."</th>";
				}
			?>
			</tr>
		</thead>
		<tbody>
			<?php
			foreach($tinsp->Query("Select * From billing_access",false) as $Access){
				echo "<tr id='id".$Access["access_id"]."'>";
				echo "<td>".$Access["access_name"]."</td>";
				foreach($Week_days_arr as $key=>$day_week_date){
					$sql_task=$tinsp->Query("
					Select billing_ticket_task.*,billing_ticket_category.id as c_id, billing_ticket_category.cat_name
					From billing_ticket_task 
					LEFT JOIN billing_ticket_category ON billing_ticket_category.id = billing_ticket_task.cat
					where DATE_FORMAT(billing_ticket_task.start_date, '%Y-%m-%d') = '".$day_week_date."' 
					and billing_ticket_task.to_access_id = '".$Access["access_id"]."' ",false);	
					#подсвечиваем текущий день недели
					if($day_week_date==date("Y-m-d")) {
						echo "<td class=today>";
					}else{
						echo "<td> ";
					}
					#Задания для абонента на эту дату не найдены
					if(is_array($sql_task)){
						echo "<table>";
						foreach($sql_task as $task_val){
							echo "<tr><td class='prior".$task_val["prior"]."'>".$task_val["id"]."</td><td>".$task_val["cat_name"]."</td><td><img src='img/ticket/status".$task_val["status"].".gif' width=16></td></tr>";
						}
						echo "</table>";
					}else{
						echo "&nbsp;";
					}

					echo "</td>";
				}
				echo "</tr>";
			}
					?>
		</tbody>
	</table>
</div>

	







 
