<?php
/*
Справочник улиц
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#userstable").tablesorter(); 
    } 	
); 
</script>
<script type="text/javascript">
function remove(id){				
	var txt = '<table class="forms">'+
		'<form action="query/json_query.php" id="APIDelAccess">'+
		'	<input type=hidden name=obj id=obj value="'+id+'">'+
		'	<tr><th>Комментарий</th><td><input type="text" id="comment" name="comment" value="" /></td></tr>'+
		'</form>'+	
		'</table>';
				$.prompt(txt,{ 
					buttons:{Delete:true, Cancel:false},
					callback: function(e,v,m,f){
						if(v){
							var form = m.find("#APIDelAccess");
							var url = form.attr("action");
							var ArrayForm = m.find(':input').serializeArray();
							$.post(url, {FUNCTION: "APIDelAccess", QUERY:ArrayForm},
								function(response){
									var data = eval("("+response+")");
									if (data.error[0] == '0'){
										$('#id'+f.obj).hide('slow', function(){ $(this).remove(); });
									}else{
										$.prompt("Ошибка: "+data.error[0]+" Сообщение: "+data.error[1]+"");
									}
								}
							);
						}
						else{}
						
					}
				});
			}			
		</script>
<p id="tabname">ДОСТУП К БИЛИНГУ<p>
<div id=test></div>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список учетных записей</span></li>
		<li><span>Создать учетную запись</span></li>
	</ul>
	<div class="box visible" >
	<!--список access-->
	<table id="userstable" class="tablesorter">
		<thead>
			<tr><th>Пользователь</th><th>Права доступа</th><th>Комментарий</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php
			if (is_array($tinsp->Query("Select * From billing_access",false)))
			foreach($tinsp->Query("Select * From billing_access",false) as $Access){
				echo "<tr id='id".$Access["access_id"]."'>";
				echo "<td id=nas_name>".$Access["access_name"]."</td>";
				echo "<td id=nas_ip>".str_replace(";",", \n\r",$Access["access"])."</td>";
				echo "<td id=nas_login>".$Access["comment"]."</td>";
				echo "<td><a onclick='window.open(\"opener.php?opener=edit_users&id=".$Access["access_id"]."\",\"contents\",\"Toolbar=no,Location=0,Directories=0,Status=no,Menubar=0,Scrollbars=1,Resizable=1,Width=700,Height=500\");' href='#'><img src='img/Setting-icon.png' width='20'></a><a href='javascript:;' title='delete' class='delete' onclick='remove(\"".$Access["access_id"]."\");'><img src='img/delete.png' width='16'></a></td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
	<!--форма регистрации access-->
	<table class=forms>
		<form action="query/json_query.php" id="APICreatAccess">
			<tr><th>Имя</th><td><input type=text name=access_name id=access_name size=30></td></tr>
			<!-- скрытый text для занесения выбранных насов через разделитель -->
			<tr><th>Пароль</th><td><input type=text name=access_pass id=access_pass size=20></td></tr>
			<tr><th>Комментарий</th><td><input type=text name=access_comment id=access_comment size=20></td></tr>
			<tr><th>Права</th>
			<td>
			<?php
			if (is_array($tinsp->Query("Select * From billing_access_file",false)))
			foreach($tinsp->Query("Select * From billing_access_file order by accessfile_name DESC",false) as $AccessFile){
				echo '<input type=checkbox name="access_access" value="'.$AccessFile["accessfile_name"].'"> '.$AccessFile["accessfile_comment"].'<br>';
			}
				
			?>
			</td>
			</tr>
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>

 $("#result").hide();
 $("#APICreatAccess").submit(function(event) {
 	$("#result").show();
    event.preventDefault();       
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
			if (data.error[0]=="0"){
				$("#result").html("Учетная запись успешно создана");
			}else{				$("#result").html("Ошибка: "+data.error[0]+" Сообщение: "+data.error[1]+"");
			}
			
		}
    );
  });
</script>	
	</div>	
</div>


 