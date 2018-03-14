<?php
/*
Справочник атрибутов абоненты
*/
?>

<script>
$(document).ready(function() 
    { 
        $("#abonattrtable").tablesorter();	
		 
    } 
); 
</script>

<script type="text/javascript">
function removeUser(id){
			//	var txt = '<input type="hidden" id="userid" name="userid" value="'+ id +'" />';
							
	var txt = '<table class="forms">'+
		'<form action="query/json_query.php" id="DelAttr">'+
		'	<input type=hidden name=type id=type value=2>'+
		'	<input type=hidden name=obj id=obj value="'+id+'">'+
		'	<tr><th>Комментарий</th><td><input type="text" id="del_attr_comment" name="del_attr_comment" value="" /></td></tr>'+
		'</form>'+	
		'</table>';
				$.prompt(txt,{ 
					buttons:{Delete:true, Cancel:false},
					callback: function(e,v,m,f){
						if(v){
							var form = m.find("#DelAttr");
							var url = form.attr("action");
							var ArrayForm = m.find(':input').serializeArray();
							$.post(url, {FUNCTION: "APIDelAttr", QUERY:ArrayForm},
								function(response){
									var data = eval("("+response+")");
									if (data.error == '0'){
										//$.prompt(""+v.obj+" Сообщение: "+data.message+"");
										$('#userid'+f.obj).hide('slow', function(){ $(this).remove(); });
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
			
	function editUser(id){
				var user = $('#userid'+id);
				var attr_name = user.find('.attr_name').text();
				var attr_comment = user.find('.attr_comment').text();				
				//alert(''+attr_name+' '+attr_comment+'');
				
				
	var txt = '<table class="forms">'+
		'<form action="query/json_query.php" id="EditAddAttr">'+
		'	<input type=hidden name=type id=type value=2>'+
		'	<input type=hidden name=obj id=obj value="'+attr_name+'">'+
		'	<tr><th>Название атрибута</th><td><input type="text" id="edit_attr_name" name="editfname" value="'+attr_name+'" /></td></tr>'+
		'	<tr><th>Комментарий</th><td><input type="text" id="edit_attr_comment" name="editlname" value="'+attr_comment +'" /></td></tr>'+
		'</form>'+	
		'</table>';
				$.prompt(txt,{ 
					buttons:{Change:true, Cancel:false},
					submit: function(v,m,f){
						//this is simple pre submit validation, the submit function
						//return true to proceed to the callback, or false to take 
						//no further action, the prompt will stay open.
						var flag = true;
						if (v) {	
							if (m.find('#edit_attr_name').val() == '') {
								m.find('#edit_attr_name').addClass('error');
								flag = false;
							}
							else m.find('#edit_attr_name').removeClass('error');
														
						}
						return flag;
					},
					callback: function(v,m,f){
						if(v){							
							var form = m.find("#EditAddAttr");
							var url = form.attr("action");
							var ArrayForm = m.find(':input').serializeArray();
							//alert(ArrayForm);
							$.post(url, {FUNCTION: "APISetAddAttr", QUERY:ArrayForm},
								function(response){
									var data = eval("("+response+")");
									if (data.error == '0'){
										user.find('.attr_name').text(m.find('#edit_attr_name').val());
										user.find('.attr_comment').text(m.find('#edit_attr_comment').val());
									}else{
										$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
									}
								}
							);
						}else{}
						
					}
				});
			}
			
		</script>
<p id="tabname">СПРАВОЧНИК АТРИБУТОВ АБОНЕНТОВ<p>
<div class="section">
	<ul class="tabs">
		<li class='current'><span>Список атрибутов</span></li>
		<li><span>Новый атрибут</span></li>
	</ul>
	<div class='box visible'>
		<!--список атрибутов-->
	<table id="abonattrtable" class="tablesorter">
		<thead> 
			<tr><th>Название атрибута</th><th>Комментарий</th><th width=50>Опции</th></tr>
		</thead> 
		<tbody> 
			<?php
			if (is_array($tinsp->APIGetAddAttr(2,"")))
			foreach($tinsp->APIGetAddAttr(2,"") as $AttrAbon){
				echo "<tr id='userid".$AttrAbon["attr_id"]."'>
					<td class=attr_name>".$AttrAbon["attr_name"]."</td>
					<td class=attr_comment>".$AttrAbon["comment"]."</td>
					<td><a href='javascript:;' title='Edit User' class='edituser' onclick='editUser(\"".$AttrAbon["attr_id"]."\");'><img src='img/Setting-icon.png' width='20'><a href='javascript:;' title='delete' class='delete' onclick='removeUser(\"".$AttrAbon["attr_id"]."\");'><img src='img/delete.png' width='16'></a>
					</td>
					</tr>";	
			}
			
			?>
		</tbody> 
	</table>
	</div>
	<div class="box">	
		<!--ФОРМА РЕГИСТРАЦИИ ГРУППЫ-->
	<table class="forms">
		<form action="query/json_query.php" id="APISetAddAttr">
			<input type=hidden name=type id=type value=2>
			<input type=hidden name=obj id=obj>
			<tr><th>Название атрибута</th><td><input type=text name=atr_name id=attr_name></td></tr>
			<tr><th>Комментарий</th><td><input type=text name=atr_comment id=atr_comment></td></tr>
			<tr><td colspan=2><input type="submit" name="sendBtn" value="send" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>
 $("#result").hide();
 $("#APISetAddAttr").submit(function(event) {
	$("#result").show();
    event.preventDefault();       
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        var url = form.attr("action");	
		var funct = form.attr("id");		
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//json parse
			//alert(response);
			var data = eval("("+response+")");
			//$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
			$("#result").html("Ошибка: "+data.error+" Сообщение: "+data.message+"");
		}
    );
  });
</script>

</div>


