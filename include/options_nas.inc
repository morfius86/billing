<?php
/*
Справочник улиц
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#nastable").tablesorter(); 
    } 	
); 
</script>
<script type="text/javascript">
function remove(id){				
	var txt = '<table class="forms">'+
		'<form action="query/json_query.php" id="APIDelNas">'+
		'	<input type=hidden name=obj id=obj value="'+id+'">'+
		'	<tr><th>Комментарий</th><td><input type="text" id="comment" name="comment" value="" /></td></tr>'+
		'</form>'+	
		'</table>';
				$.prompt(txt,{ 
					buttons:{Delete:true, Cancel:false},
					callback: function(e,v,m,f){
						if(v){
							var form = m.find("#APIDelNas");
							var url = form.attr("action");
							var ArrayForm = m.find(':input').serializeArray();
							$.post(url, {FUNCTION: "APIDelNas", QUERY:ArrayForm},
								function(response){
									var data = eval("("+response+")");
									if (data.error == '0'){
										$('#id'+f.obj).hide('slow', function(){ $(this).remove(); });
									}else{
										$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
									}
								}
							);
						}
						else{}
						
					}
				});
			}			
		</script>
<p id="tabname">NAS/СЕРВЕРА ДОСТУПА<p>
<div id=test></div>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список NAS</span></li>
		<li><span>Создать NAS</span></li>
	</ul>
	<div class="box visible" >
	<!--список nas-->
	<table id="nastable" class="tablesorter">
		<thead>
			<tr><th>Название</th><th>Сектор</th><th>IP</th><th>Login</th><th>Password</th><th>Тип NAS</th><th>Шейпер</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php
			if (is_array($tinsp->APIGetNas(2,"")))
			foreach($tinsp->APIGetNas(2,"") as $Nas){
				echo "<tr id='id".$Nas["nas_id"]."'>";
				echo "<td id=nas_name>".$Nas["nas_name"]."</td>";
				echo "<td id=nas_sector_id>";
					if(eregi(",",$Nas["nas_sector_id"])){
						foreach(explode(",",$Nas["nas_sector_id"]) as $nas_sector_id){
							$nas_sector_id_name=$tinsp->APIGetSector($nas_sector_id);
							print $nas_sector_id_name[0]["sector_name"]."<br>";
						
						}	
					}else{
						$nas_sector_id_name=$tinsp->APIGetSector($Nas["nas_sector_id"]);
						print $nas_sector_id_name[0]["sector_name"]."<br>";
					}
				echo "</td>";
				echo "<td id=nas_ip>".$Nas["nas_ip"]."</td>";
				echo "<td id=nas_login>".$Nas["nas_login"]."</td>";
				echo "<td id=nas_password>".$Nas["nas_password"]."</td>";
				echo "<td id=nas_type>".$Nas["nas_type"]."</td>";
				echo "<td id=nas_shaper>".$Nas["nas_shaper"]."</td>";
				echo "<td><a onclick='window.open(\"opener.php?opener=edit_nas&id=".$Nas["nas_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=390,Height=440\");' href='#'><img src='img/Setting-icon.png' width='20'></a><a href='javascript:;' title='delete' class='delete' onclick='remove(\"".$Nas["nas_id"]."\");'><img src='img/delete.png' width='16'></a></td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации nas-->
	<table class=forms>
		<tr><th>Сектор</th>
			<td>
			<select name="select_nas_sector_val" size=4 multiple id=select_nas_sector_val>
			<?php
				if (is_array($tinsp->APIGetSector()))
				foreach($tinsp->APIGetSector() as $sector){
					echo "<option value='".$sector["sector_id"]."'>".$sector["sector_name"]."</option>";
				}
			?>
			</select>
			</td>
		</tr>
		<form action="query/json_query.php" id="APICreatNas">
			<tr><th>Название</th><td><input type=text name=nas_name id=nas_name size=30></td></tr>
			<!-- скрытый text для занесения выбранных насов через разделитель -->
			<input type=hidden name=select_nas_sector id=select_nas_sector>
			<tr><th>IP</th><td><input type=text name=nas_ip id=nas_ip size=20></td></tr>
			<tr><th>Логин</th><td><input type=text name=nas_login id=nas_login size=20></td></tr>
			<tr><th>Пароль</th><td><input type=text name=nas_password id=nas_password size=20></td></tr>
			<tr><th>Секрет</th><td><input type=text name=nas_secret id=nas_secret size=20></td></tr>
			<tr><th>Тип NAS</th>
				<td>
					<select name="select_nas_type" id=select_nas_type>
					<option value="6">Mikrotik API HotSpot (mac+ip)</option>
					<option value="7">Mikrotik API HotSpot (mac)</option>
					<option value="1">Mikrotik Radius HotSpot (ip+mac)</option>
					<option value="2">Mikrotik Radius HotSpot (ip)</option>
					<option value="3">Mikrotik Radius HotSpot (mac)</option>
					<option value="4">Mikrotik Radius PPTP (log+pass)</option>
					<option value="5">Mikrotik Radius PPTP (log+pass+ip)</option>
					</select>
				</td>
			</tr>
			<tr><th>Шейпер</th>
				<td>
					<select name="select_nas_shaper" id=select_nas_shaper>
					<option value="0">Нет</option>
					<option value="1">Radius</option>
					</select>
				</td>
			</tr>
			<tr><th>Добавлять в Adress-list согласно <br>группе абонента(Mikrotik/Radius)</th>
				<td>
					<select name="select_nas_addresslist" id=select_nas_addresslist>
					<option value="0">Нет</option>
					<option value="1">Да</option>
					</select>
				</td>
			</tr>
			<tr><th>Добавлять IP/MAC в ARP-Static</th>
				<td>
					<select name="select_nas_arp" id=select_nas_arp>
					<option value="0">Нет</option>
					<option value="1">Да</option>
					</select>
				</td>
			</tr>
			<tr><th>DHCP</th>
				<td>
					<select name="select_nas_dhcp" id=select_nas_dhcp>
					<option value="0">Нет</option>
					<option value="1">Да</option>
					</select>
				</td>
			</tr>
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>

 $("#result").hide();
 $("#APICreatNas").submit(function(event) {
 	$("#result").show();
    event.preventDefault();       
		$('#select_nas_sector').val($('#select_nas_sector_val').val());
		var form = $(this);
		var ArrayForm = form.serializeArray();
		//var ArrayForm = form.serialize();
		//alert(ArrayForm);
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
   $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			//alert(response);
			var data = eval("("+response+")");
			$("#result").html("Ошибка: "+data.error+" Сообщение: "+data.message+"");
		}
    );
  });
</script>	
	</div>	
</div>


 
