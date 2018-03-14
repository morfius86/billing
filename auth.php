<?php

/*==================================
CLASS & CONFIG
==================================
*/
include("config.php");
include("class/class.api.php");
define("DIR", dirname($_SERVER['PHP_SELF']));

$tinsp = New API;
$tinsp->encode = "utf8";
$tinsp->config = $config;
$tinsp->ConnSQL($dbhost, $dbuser, $dbpass, $dbname);
if (!$tinsp->SupportBrowser()){
	exit("Браузер не поддерживается");
}
/*==================================
AUTH
==================================*/
function draw_form($bad_login = false)
{
    ?>
<!DOCTYPE HTML>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>Доступ запрещен. Авторизуйтесь</title>
    <link href="css/auth.css" rel="stylesheet" type="text/css">
</head>
<body>
<div class="logo"><img src="img/logo.png" width="70%"/></div>
<div class="loginform">
  <form id="login" method="post" action="">
  <table>
    <tr align="right">
      <td><span>Username:</span></td>
      <td><input name="login" id="login" class="textbox" type="text"></td>
    </tr>
    <tr align="right">        
      <td><span>Password:</span></td>         
      <td><input name="pass" id="pass" class="textbox" type="password"></td>  
    </tr>   
    <tr>        
      <td></td>        
      <td><input name="loginbtn" id="loginbtn" class="loginbtn" value="Вход" type="submit"></td>          
    </tr>
    <tr>        
      <td></td>        
      <td>Powered by <a href="http://www.vash-net.ru">Ваш Интернет</a></td>          
    </tr>
  </table>	  	
</form></div>
<center>
  <span class="error"></span>
</center>
</body>
</html>
<?php
}


function check_login($login, $pass)
{
    global $tinsp;
    $AuthUser = $tinsp->Query("Select * from billing_access where access_name = '" . $login . "' and  access_pass = '" . $pass . "' LIMIT 1", false);
    if (is_array($AuthUser[0])) {
        return $AuthUser[0];
    }
    return false;
}

session_start();

if (isset($_GET['logout'])) {
    session_unset();
    session_destroy();
    header("Location: index.php");
    exit();
}

#???? ?? ??? ??????????????
if (!isset($_SESSION['login'])) {

    $login = $_POST['login'];
    $pass = $_POST['pass'];

    if (count($_POST) <= 0) {
        draw_form();
    } else {
        $CheckLogPas = check_login($login, $pass);
        if (is_array($CheckLogPas)) {
            $_SESSION['access'] = (string)$CheckLogPas["access"];
            $_SESSION['access_id'] = (string)$CheckLogPas["access_id"];
            $_SESSION['login'] = $login;
        } else {
            #?????? ?? ?????????? ??????
            draw_form(true);
        }
    }
} else {

}

isset($_SESSION['login']) or die(); // ????? ???? ??????? ??????? false ?? ??????????? die()
?>