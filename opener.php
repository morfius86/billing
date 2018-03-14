<?php include_once("auth.php"); ?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Style-Type" content="text/css">
<title>::Simple MT-Billing::</title>
<script type="text/javascript" src="js/jquery.js"></script> 
<script src="js/cal.js"></script>
<script src="js/jquery.multiple.select.js"></script>
<script type="text/javascript" src="js/colortip-1.0.js" ></script>
<script type="text/javascript" src="js/masked.js"></script>
<script type="text/javascript"> 
$(document).ready(function(){
	$('[title]').colorTip({color:'yellow'});
});
</script>

<link href="css/default.css" rel="stylesheet" type="text/css" />
<link href="css/menu.css" rel="stylesheet" type="text/css"/>
<link href="css/cal.css" rel="stylesheet" type="text/css" />
<link href="css/table.css" rel="stylesheet" type="text/css" />
<link href="css/colortip-1.0-jquery.css" rel="stylesheet" type="text/css" />
<link href="css/multiple-select.css" rel="stylesheet" type="text/css" />

<style type=text/css>
body {
	margin: 0; padding: 0;
	font: 12px normal Arial, Helvetica, sans-serif;
	background: #ddd;
}
</style>
</head>
<body>

<?php

#include class mikrotik
#include("class/class.mikrotik.php");


#include file
if(isset($_GET["opener"])){
	if(file_exists("opener/".$_GET["opener"].".inc")){
		if ($_SESSION["access"]=="ALL_ACCESS" or in_array($_GET["opener"], explode(";",$_SESSION["access"]))){
			require("opener/".$_GET["opener"].".inc");
		}else{
			echo '<p id="tabname"><br><img src="img/no_access.png" width=100><p>';
		} 
	}else{
	//error and logs
	//....
	}
}
?>

</body>

</html>

