<?php

$host	= 'localhost';
$base	= 'radius';
$user	= 'root';
$pass	= '123';

//Вызван ли скрипт другим таким же скриптом
$remote = false;

if(isset($_GET['remote'])) $remote = true;

if(!$link = mysql_connect($host, $user, $pass)) die('Ошибка подключения к MySQL');
mysql_select_db($base, $link);

$result = mysql_query('SHOW TABLES', $link);
while($i = mysql_fetch_array($result)) {
	$result_columns = mysql_query('SHOW COLUMNS FROM `'.$i[0].'`', $link);
	while($j = mysql_fetch_array($result_columns)) {
		$current_base[$i[0]][$j['Field']] = array(
			'field'		=> $j['Field'],
			'type'		=> $j['Type'],
			'null'		=> $j['Null'],
			'key'		=> $j['Key'],
			'default'	=> $j['Default'],
			'extra'		=> $j['Extra']
		);
	}
}

//Если скрипт вызван удаленно, просто отдаем получившийся массив 
if($remote) {
	echo serialize($current_base);
	exit();
}

//Забираем удаленные данные
if(isset($_GET['remote_script'])) {
	$remote_data = file_get_contents($_GET['remote_script'].'?remote');
	$remote_base = unserialize($remote_data);
}

$ret = '';
foreach ($current_base as $table=>$data) {
	$ret .= '<h1>'.$table.'</h1>';
	$ret .= '<table width="100%" border="1" cellspacing="0">';
	$ret .= '<tr><th colspan="6">Current</th><th colspan="6">Remote</th></tr>';
	$ret .= '<tr><th>Field</th><th>Type</th><th>Null</th><th>Key</th><th>Default</th><th>Extra</th><th>Field</th><th>Type</th><th>Null</th><th>Key</th><th>Default</th><th>Extra</th></tr>';
	foreach ($data as $field=>$fdata) {
		if($current_base[$table][$field] != $remote_base[$table][$field]) $color = 'yellow';
		else $color = '';
		if(isset($current_base[$table][$field]) && isset($remote_base[$table][$field])) {
			$ret .= '<tr style="background-color:'.$color.'">
						<td>'.$current_base[$table][$field]['field'].'</td><td>'.$current_base[$table][$field]['type'].'</td><td>'.$current_base[$table][$field]['null'].'</td><td>'.$current_base[$table][$field]['key'].'</td><td>'.$current_base[$table][$field]['default'].'</td><td>'.$current_base[$table][$field]['extra'].'</td>
						<td>'.$remote_base[$table][$field]['field'].'</td><td>'.$remote_base[$table][$field]['type'].'</td><td>'.$remote_base[$table][$field]['null'].'</td><td>'.$remote_base[$table][$field]['key'].'</td><td>'.$remote_base[$table][$field]['default'].'</td><td>'.$remote_base[$table][$field]['extra'].'</td>
					</tr>';
		} elseif (isset($current_base[$table][$field]) && !isset($remote_base[$table][$field])) {
			$color = 'green';
			$ret .= '<tr style="background-color:'.$color.'">
						<td>'.$current_base[$table][$field]['field'].'</td><td>'.$current_base[$table][$field]['type'].'</td><td>'.$current_base[$table][$field]['null'].'</td><td>'.$current_base[$table][$field]['key'].'</td><td>'.$current_base[$table][$field]['default'].'</td><td>'.$current_base[$table][$field]['extra'].'</td>
						<td>'.$remote_base[$table][$field]['field'].'</td><td>'.$remote_base[$table][$field]['type'].'</td><td>'.$remote_base[$table][$field]['null'].'</td><td>'.$remote_base[$table][$field]['key'].'</td><td>'.$remote_base[$table][$field]['default'].'</td><td>'.$remote_base[$table][$field]['extra'].'</td>
					</tr>';
		}
		$exist[] = $current_base[$table][$field]['field'];
	}
	
	$color = 'blue';
	foreach ($remote_base[$table] as $field=>$fdata) {
		if(!in_array($remote_base[$table][$field]['field'], $exist)) {
			$ret .= '<tr style="background-color:'.$color.'">
						<td>'.$current_base[$table][$field]['field'].'</td><td>'.$current_base[$table][$field]['type'].'</td><td>'.$current_base[$table][$field]['null'].'</td><td>'.$current_base[$table][$field]['key'].'</td><td>'.$current_base[$table][$field]['default'].'</td><td>'.$current_base[$table][$field]['extra'].'</td>
						<td>'.$remote_base[$table][$field]['field'].'</td><td>'.$remote_base[$table][$field]['type'].'</td><td>'.$remote_base[$table][$field]['null'].'</td><td>'.$remote_base[$table][$field]['key'].'</td><td>'.$remote_base[$table][$field]['default'].'</td><td>'.$remote_base[$table][$field]['extra'].'</td>
					</tr>';
		}
	}
	//echo '<pre>'.print_r(array_diff_key($current_base[$table], $remote_base[$table])).'</pre>';
	$ret .= '</table>';
}


echo $ret;


?>