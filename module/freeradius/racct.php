#!/usr/bin/php -q
<?php
 $message = "";
   $fd = fopen("php://stdin", "r");
     while (!feof($fd)){
         $message .= fread($fd, 1024);
     }
    traplog($message);
    
    function traplog($str){
	$file=fopen("/var/www/billing/module/freeradius/".date("Y-m-d")."_acct.log", "a");
	fwrite($file,$str."\n");
	fclose($file);
    }
?>