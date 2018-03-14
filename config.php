<?php
// db config
$dbtype = 'mysql';
$dbhost = '127.0.0.1';
$dbport = '';
$dbname = 'radius';
$dbuser = 'root';
$dbpass = '123';

// settings
$config["dir_log"] = "/var/www/billing/log/";

#php error log
@ini_set ("error_log", "/var/www/billing/log/php.error_log-" . $_SERVER ["HTTP_HOST"] . "-" . $_SERVER ["REMOTE_ADDR"] . "-" . $_SERVER ["REQUEST_METHOD"] . "-" . str_replace ("/", "|", $_SERVER ["REQUEST_URI"])); 


?>