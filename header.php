<img src="img/logo.png" id="logo" width="150"/>
<div id="header">
<?php
	#INFO
	$CountBase=$tinsp->Query("SELECT count(id) FROM `billing_users`", false);
	$CountBaseUnlock=$tinsp->Query("SELECT count(id) FROM `billing_users` where user_block = 0", false);
	$CountBaseBlock=$tinsp->Query("SELECT count(id) FROM `billing_users` where user_block = 1", false);
	$CountBasePause=$tinsp->Query("SELECT count(id) FROM `billing_users` where user_block = 2", false);
?>    	  
	<div class="disclaimer">Пользователь:
		<?=$_SESSION["login"]?>, ip:
		<?=$_SERVER['REMOTE_ADDR']?> 
		<br>Абонентов:
		<?=$CountBase[0]["count(id)"]?> (Заблокировано:
		<?=$CountBaseBlock[0]["count(id)"]?>, Рабочие:
		<?=$CountBaseUnlock[0]["count(id)"]?>, Пауза:
		<?=$CountBasePause[0]["count(id)"]?>)
	</div>
	<ul class="pureCssMenu pureCssMenum">
		<li class="pureCssMenui"><a href="?page=network_map">Карта сети (Google)</a></li>
		<li class="pureCssMenui"><a href="?page=network_monit">Монитор сети</a></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Online</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li class="pureCssMenui"><a href="?page=online">Авторизация</a></li>
			<li class="pureCssMenui"><a href="?page=online_dhcp">DHCP-аренда</a></li>
		</ul>
		<li class="pureCssMenui"><a href="?page=search">Поиск</a></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Учетные записи</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li class="pureCssMenui"><a href="?page=spr_group">Группы учетных записей</a></li>
			<li class="pureCssMenui"><a href="?page=user_add">Создать</a></li>
		</ul>
		<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Справочники</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li class="pureCssMenui"><a href="?page=spr_tarif">Тарифы</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_attrabon">Атрибуты абонентов</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_sector">Сектор</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_naspunkt">Населенный пункт</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_ulica">Улица</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_dom">Дом</a></li> 
			<li class="pureCssMenui"><a href="?page=spr_switch">Оборудование</a></li>
		</ul>
		<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Отчеты</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Графические отчеты</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
				<ul class="pureCssMenum">
					<li class="pureCssMenui"><a class="pureCssMenui" href="?page=report_graph_pay">Платежи</a></li>
				</ul>
				<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
			<li><a href="?page=report_pay">Платежи</a></li>
			<li><a href="?page=report_credit">Обещанные платежи</a></li>
			<li><a href="?page=report_request_tariff">Заявки на смену тарифа</a> </li>
			<li><a href="?page=report_event">События</a></li>
			<li><a href="?page=report_alfabank_mail">Обработка платежей (Альфа-Банк)</a></li>
		</ul>
		<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Параметры</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li><a href="?page=options_nas">Сервера доступа</a></li> 
			<li><a href="?page=spr_users">Права доступа</a></li>
			<li><a href="?page=sys_options">Системные опции</a></li>
		</ul>
		<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
			<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>HelpDesk/Ticket</span><![if gt IE 6]></a><![endif]><!--[if lte IE 6]><table><tr><td><![endif]-->
		<ul class="pureCssMenum">
			<li><a href="?page=options_nas">Задания</a> </li> 
			<li><a href="?page=spr_users">Обращения</a></li>
		</ul>
		<!--[if lte IE 6]></td></tr></table></a><![endif]--></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="?logout">Выход</a></li>
	</ul>
</div>