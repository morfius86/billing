<?php
/*
| Отчеты по событиям 
*/
require_once('class/as-diagrams.php');

function my_url_decode($s){
	$s= strtr ($s, array ("+"=>" ","%3A"=>":","%20"=>" ", "%D0%B0"=>"а", "%D0%90"=>"А", "%D0%B1"=>"б", "%D0%91"=>"Б", "%D0%B2"=>"в", "%D0%92"=>"В", "%D0%B3"=>"г", "%D0%93"=>"Г", "%D0%B4"=>"д", "%D0%94"=>"Д", "%D0%B5"=>"е", "%D0%95"=>"Е", "%D1%91"=>"ё", "%D0%81"=>"Ё", "%D0%B6"=>"ж", "%D0%96"=>"Ж", "%D0%B7"=>"з", "%D0%97"=>"З", "%D0%B8"=>"и", "%D0%98"=>"И", "%D0%B9"=>"й", "%D0%99"=>"Й", "%D0%BA"=>"к", "%D0%9A"=>"К", "%D0%BB"=>"л", "%D0%9B"=>"Л", "%D0%BC"=>"м", "%D0%9C"=>"М", "%D0%BD"=>"н", "%D0%9D"=>"Н", "%D0%BE"=>"о", "%D0%9E"=>"О", "%D0%BF"=>"п", "%D0%9F"=>"П", "%D1%80"=>"р", "%D0%A0"=>"Р", "%D1%81"=>"с", "%D0%A1"=>"С", "%D1%82"=>"т", "%D0%A2"=>"Т", "%D1%83"=>"у", "%D0%A3"=>"У", "%D1%84"=>"ф", "%D0%A4"=>"Ф", "%D1%85"=>"х", "%D0%A5"=>"Х", "%D1%86"=>"ц", "%D0%A6"=>"Ц", "%D1%87"=>"ч", "%D0%A7"=>"Ч", "%D1%88"=>"ш", "%D0%A8"=>"Ш", "%D1%89"=>"щ", "%D0%A9"=>"Щ", "%D1%8A"=>"ъ", "%D0%AA"=>"Ъ", "%D1%8B"=>"ы", "%D0%AB"=>"Ы", "%D1%8C"=>"ь", "%D0%AC"=>"Ь", "%D1%8D"=>"э", "%D0%AD"=>"Э", "%D1%8E"=>"ю", "%D0%AE"=>"Ю", "%D1%8F"=>"я", "%D0%AF"=>"Я"));
	return $s;
} 

?>
<script src="js/fixer.js"></script>
<script src="js/cal.js"></script>
<script type='text/javascript' src='js/jquery.autocomplete.js'></script>
<script> 
$(function() {
	$("#event_fio").autocomplete({
		url: 'query/json_query.php?FUNCTION=BillingGetAjaxUser',
		showResult: function(value, data) {			
				return '<span style="color:gray">' + value + '</span>';		
		},
		onItemSelect: function(item) {

		}
	});
});
$(document).ready(function() 
    { 
        $("#eventtable").tablesorter(); 
			
			$('#form_graphs :radio').click(function(){
			if($('#form_graphs :radio:checked').attr('id')=='type_yearmonth'){
                $('#period').html('Выберите год: <select name=period><option value="2010">2010</option><option value="2011">2011</option><option value="2012">2012</option><option value="2013">2013</option></select>');
            }
            else if($('#form_graphs :radio:checked').attr('id')=='type_day'){
				 $('#period').html('<tr><td>Выберите год: </td><td><select name=period_year><option value="2012">2012</option><option value="2013">2013</option><option value="2014">2014</option><option value="2015">2015</option></select></td></tr>');
                 $('#period').append('<tr><td>Выберите месяц:</td><td> <select name=period_month><option value="01">Январь</option><option value="02">Февраль</option><option value="03">Март</option><option value="04">Апрель</option><option value="05">Мая</option><option value="06">Июнь</option><option value="07">Июль</option><option value="08">Авгут</option><option value="09">Сентябрь</option><option value="10">Октябрь</option><option value="11">Ноябрь</option><option value="12">Декабрь</option></select></td></tr>');
			}
        });
	}	
);

//$('#logtable').columnHover(); 
</script>

<link rel="stylesheet" type="text/css" href="css/jquery.autocomplete.css" />
<style type="text/css">
#stat td.hover, #logtable tbody tr:hover
{
	font-weight: bold;
	color: black;
	background-color: LightCyan;
}
div.scrollTableContainer {
	height: 400px;
	overflow: auto;
	width: 1100px;
	margin: 15px 0 0 0;
	position: relative;
}
div.scrollTableContainer thead tr {
	position:relative;
	top: expression(offsetParent.scrollTop); /*IE5+ only*/
	left: 0px;
}
div.scrollTableContainer td:last-child {padding-right: 20px;}
</style>
<p id="tabname">Отчет по событиям<p>
<div class="section">
	<!--Отчеты-->
	<form action="?page=report_event" autocomplete="off" method=post>
		<table class=forms >
		<tr><th>ФИО/Договор</th><th>Тип события</th><th>Автор</th></tr>
		<tr>
			<td>
				<input type=text name="event_fio" id="event_fio">
			</td>
			<td>
				<select name="event_type" id=event_type>
				<option value="">Все</option>
				<?php	
				if (is_array($tinsp->ArrTypeEvent))
				foreach($tinsp->ArrTypeEvent as $EventKey => $EventType){
					echo '<option value="'.$EventKey.'">'.$EventType.'</option>';
				}
				?>
				</select>
			</td>
			<td>
				<select name="event_autor" id=event_autor>
				<option value="">Все</option>
				<?php	
				if (is_array($tinsp->Query("Select * From billing_access",false)))
				foreach($tinsp->Query("Select * From billing_access",false) as $Access){
					echo '<option value="'.$Access["access_id"].'">'.$Access["access_name"].'</option>';
				}
				?>
				</select>
			</td>
		</tr>
		<tr><th>Дата начала</th><th>Дата окончания</th><th></th></tr>
		<tr>
			<td><input type=text name=period1 id=period1 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()); ?>" ></td>
			<td><input type=text name=period2 id=period2 class="w8em format-y-m-d divider-dash highlight-days-12 " value="<?php print date("Y-m-d",time()+(86400*1)); ?>" ></td>
			<td></td>
		</tr>
		<tr>
			<td colspan=3 align=center><input type=submit value="Найти" ></td>
		</tr>
		</table>
	</form>	
	
			<?php
			/* ОТЧЕТ СПИСОК */
			if (count($_POST)>1){
				$i=1;
				#заголовок
				echo '<table id="eventtable" class="tablesorter">
					<thead class="fixedHeader">
						<tr><th width=20>#</th><th>Тип события</th><th>Абонент</th><th>Оператор</th><th>Номер ошибки</th><th>Комментарий</th><th>Дата</th></tr>
					</thead>
					<tbody id=result>';
				#ищем абонента по фио и договору
				if(strlen($_POST["event_fio"])>2){
					$User=$tinsp->APISearchAbonent("!".$_POST["event_fio"], "", "",  "",  "",  "");
					if(is_array($User[1])){
						$_POST["event_fio"]=$User[1][0]["user_id"];
					}
				}else{
					$_POST["event_fio"]="";
				}
				#формируем скуль запрос
				$EventQuery=$tinsp->Query("SELECT billing_log.*, billing_users.user_name as user_name_real 
				FROM billing_log 
				LEFT JOIN billing_users ON billing_users.user_id = billing_log.user_id	
				WHERE 
				(billing_log.log_date >= '".$_POST["period1"]."' and billing_log.log_date < '".$_POST["period2"]."')
				AND billing_log.log_type LIKE '%".$_POST["event_type"]."%'
				AND billing_log.user_id like '%".$_POST["event_fio"]."%'
				AND billing_log.access_id like '%".$_POST["event_autor"]."'ORDER BY billing_log.id", false);
				#print_r($EventQuery);
				#имя автора
				if(is_array($EventQuery)){
					foreach ($EventQuery as $EventVal) {
						if (strlen($EventVal["access_id"])>=1){
							if (is_array($tinsp->Query("Select access_name From `billing_access` where access_id = '".$EventVal["access_id"]."'",false))) {
								$AccessQuery=$tinsp->Query("Select access_name From `billing_access` where access_id = '".$EventVal["access_id"]."'",false);
							}else{
								$AccessQuery["0"]["access_name"]="";
							}
						}else{
							$AccessQuery["0"]["access_name"]="";
						}
					echo '<tr id="id'.$EventVal["id"].'"><td>'.$i++.'</td><td>'.$EventVal["log_type"].'</td><td>'.$EventVal["user_name_real"].'</td><td>'.$AccessQuery["0"]["access_name"].'</td><td>'.$EventVal["log_errnum"].'</td><td>'.my_url_decode($EventVal["log_comment"]).'</td><td>'.$EventVal["log_date"].' '.$EventVal["log_time"].'</td></tr>';
					}
				}else{
					echo '<tr><td colspan=8 align=center>Нет данных</td></tr>';

				}
				echo '</tbody></table>';
			/* ОТЧЕТ ГРАФИК */
			}elseif(count($_POST)>1 && $_GET["table"]==2){
				
			}
		?>

	


</div>

