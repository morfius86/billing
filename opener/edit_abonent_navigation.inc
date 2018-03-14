<style type="text/css">
    .abon_info{
		clear: both;
		padding: 10px;
		background-color: #f9f2ba;
		border: 1px solid #e9d315;
		text-align: center;
    }
   
</style>

<ul class="pureCssMenu pureCssMenum">
		<li class="pureCssMenui"><a href="?opener=edit_abonent&id=<?=$_GET["id"]?>">Контракт</a></li>
		<li class="pureCssMenui"><a href="?opener=edit_abonent_req&id=<?=$_GET["id"]?>">Реквизиты</a></li>
		<li class="pureCssMenui"><a href="?opener=edit_abonent_service&id=<?=$_GET["id"]?>">Биллинг</a></li>
		<!-- <li class="pureCssMenui"><a href="?opener=edit_abonent_attr&id=<?=$_GET["id"]?>">Атрибуты</a></li> -->
		<li class="pureCssMenui"><a href="?opener=journal_auth&id=<?=$_GET["id"]?>">Сессии</a></li>
		<li class="pureCssMenui"><a href="?opener=journal_bill&id=<?=$_GET["id"]?>">Платежи</a></li>
		<li class="pureCssMenui"><a href="?opener=edit_abonent_lk&id=<?=$_GET["id"]?>">Личный кабинет</a></li>
		<li class="pureCssMenui"><a href="?opener=edit_abonent_switch&id=<?=$_GET["id"]?>">Устройство</a></li>
		<li class="pureCssMenui"><a class="pureCssMenui" href="#"><span>Платёж</span></a>
			<ul class="pureCssMenum">
				<li class="pureCssMenui"><a href="?opener=addcash&id=<?=$_GET["id"]?>">Пополнить счет</a></li>
				<li class="pureCssMenui"><a href="?opener=addcash_per&id=<?=$_GET["id"]?>">Перерасчет</a></li>
				<li class="pureCssMenui"><a href="?opener=addcash_podkl&id=<?=$_GET["id"]?>">Подключение/ремонт</a></li>
				<li class="pureCssMenui"><a href="?opener=addcash_action&id=<?=$_GET["id"]?>">Акция/подарок</a></li>
			</ul>
		</li>
		<li class="pureCssMenui"><a href="?opener=edit_abonent_delete&id=<?=$_GET["id"]?>">Удалить</a></li>
</ul>


<div class="abon_info">
	<b><?=$Abonent_id_options[0]["user_name"]?></b><br>
	Договор: <?=$Abonent_id_options[0]["user_dogovor"]?> |
	Баланс: <?=$Abonent_id_options[0]["balance"]?> руб. |
	Создан: <?=date("Y-m-d", strtotime($Abonent_id_options[0]["creat_add"]))?>
</div>
