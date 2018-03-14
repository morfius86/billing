<?php
/*	
Редактор абонентов: Личный кабинет	
*/
#change pass settings
$change_pass="";
if (count($_POST)>2) {
	$Query=$tinsp->Query("UPDATE billing_users SET user_perspass = '".$_POST["user_perspass"]."', user_perstrans = '".$_POST["user_perstrans"]."', user_persinterpass = '".$_POST["user_persinterpass"]."' WHERE user_id = '".$_POST["obj"]."'");
	if($Query!="0"){
		$change_pass="<tr><td colspan=2><div id=result>Ошибка сохранения. Обратитесь к администратору.</div></td></tr>";
	}else{
		$qqq=$tinsp->APICreatEvent("billing", $_POST["obj"], "", $_SESSION["access_id"], "", "Edit abonent: user_perspass = ".$_POST["user_perspass"].", user_pershelp = ".$_POST["user_pershelp"].", user_persinterpass = ".$_POST["user_persinterpass"]." ");;
		$change_pass="<tr><td colspan=2><div id=result>Успешно сохранен.</div></td></tr>";
	}
}
#valid guid
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
		if(!is_array($Abonent_id_options)){
			exit("Абонент с таким id не найден");
		}
	}else{
		exit("Ошибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
//print_r($Abonent_id_options);
?>
<script type='text/javascript'>

function PassGens (lens)
{
  var PassGen = (function(){
    var Chars = '0123456789',
        Length = Chars.length,
        RndChar = function() {
            return Chars.charAt(Math.floor(Math.random() * Length));
        };
    return function(len) {
        var s = '', i;
        for (i = 0; i < len; i++) s += RndChar();
        return s;
    };
})();

  $('#user_perspass').val(PassGen(lens));

}
//проверка вводимого пароля
function checkPassword(pass)
{
	if (pass.value == '123' || pass.value  == '1234' || pass.value  == '12345' || pass.value  == '123456') {
		pass.setCustomValidity('Ребят, слишком просто. Генерируйте с помощью случайного пароля!');
	} else {
		pass.setCustomValidity('');
	}
}

</script>

	<!--форма редактирования параметров личного кабинета абонента -->
		<?php include("edit_abonent_navigation.inc"); ?>
		<table class="forms" style="width: 100%;">
			
			<form action="<?=$_SERVER["REQUEST_URI"]?>" method=post>
			<input type=hidden name=obj size=50 id=obj value="<?=$Abonent_id_options[0]["user_id"]?>">
			
			<!-- ПАРОЛЬ, -->
			<tr><th colspan=2>ЛИЧНЫЙ КАБИНЕТ</th></tr>
			<tr><th>Пароль <button type="button"  onclick='PassGens("6");'>сгенерировать</button></i></a></th><td><input onfocus="checkPassword(this);" oninput="checkPassword(this);" type=text name=user_perspass id=user_perspass value="<?=$Abonent_id_options[0]["user_perspass"]?>" ></td></tr>
			<tr><th>Разрешить переводы</th>
				<td>
					<INPUT TYPE=RADIO NAME=user_perstrans VALUE="1" <?php if ($Abonent_id_options[0]["user_perstrans"] == "1") echo "CHECKED"; ?> > Да
					<INPUT TYPE=RADIO NAME=user_perstrans VALUE="0" <?php if ($Abonent_id_options[0]["user_perstrans"] == "0") echo "CHECKED"; ?> > Нет 
				</td>
			</tr>

			<tr><td colspan=2><input type="submit" name="sendBtn" class="button" value="Сохранить" /><input type="button" name="sendBtn" class="button" value="Закрыть" onclick="javascript:window.close();"/></td></tr>			
		</form>	
		<?=$change_pass?>		
	</table>


