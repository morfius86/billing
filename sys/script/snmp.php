<?php
/*
Works with switches via SNMP
*/
set_time_limit(90);
error_reporting (E_ERROR | E_WARNING | E_PARSE | E_NOTICE);
define ("COUNT_STREAM", 4);	// задаем кол-во потоков
$cycle_time = time();		// метка цикла для сбора мак-адресов. Если в одном цикле есть одинаковые маки, значит есть подмена мака

$i = 0;
$pid_arr = array();
while ($i <= intval(COUNT_STREAM)){
	$pid = pcntl_fork(); // вот он, - знакомый до боли fork()
	if ($pid == -1){
		die('could not fork');
	}else{
		if ($pid){
			$pid_arr[$i] = $pid;
		}else{
			system('php /var/www/billing/sys/script/snmp_stream.php '.COUNT_STREAM.' '.$i.' '.@$argv[1].' '.$cycle_time.''); 
			// делаем выход или процессы будут продолжать плодиться
			exit(0);
		}
	}
	$i++;
}
foreach ($pid_arr as $pid){
	pcntl_waitpid($pid, $status);
}






?>