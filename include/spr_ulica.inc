<?php
/*
Справочник улиц
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#ulicatable").tablesorter(); 
    } 	
); 


function remove_ulica(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelUlica",
				QUERY : id
			}, function(response) {
				alert(response);
				var data = eval("("+response+")");
				if (data.error == '0') {
					$('#id'+id).hide('slow', function() {$(this).remove();});
					$.prompt("Успешно удален!");
				} else {
					$.prompt("Ошибка: " + data.error + " Сообщение: " + data.message + "");
			}});
		}}
	});
}
			
function editulica(id){
	var ulica_id = $('#id'+id);
	//инфо по сектору
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = id;
	
	
	$.post("query/json_query.php", {FUNCTION: "APIGetUlica", QUERY:arr}, function(response){
		var data = eval("("+response+")");
		$.each(data.error, function(key, value){			
		//список для редактирования
		var selectnasp = '';				
		$.post("query/json_query.php", {FUNCTION: "APIGetNasp", QUERY:Array('2','')}, function(response){
			var data_nasp = eval("("+response+")");
			$.each(data_nasp.error, function(key, nasp_value){
				//select nasp	
				if (value.ulica_nasp == nasp_value.nasp_id){
					selectnasp+='<option value="'+nasp_value.nasp_id+'" selected>'+nasp_value.nasp_name+'</option>'; 
				}else{
					selectnasp+='<option value="'+nasp_value.nasp_id+'">'+nasp_value.nasp_name+'</option>'; 	
				}
			});	
		$("#edit_ulica_nasp").html(selectnasp);			
		});


		
		var input = '<form action="query/json_query.php" id="APISetUlica">'+
			'<input type=hidden name=obj id=obj value="'+id+'">'+
			'<tr><th>Название улицы</th><td><input type=text name=edit_ulica_name id=edit_ulica_name size=30 value="'+value.ulica_name+'"></td></tr>'+
			'<tr><th>Нас.пункт</th><td><select id="edit_ulica_nasp" name="edit_ulica_nasp"></select></td></tr>'+
			'<tr><th>Комментарий</th><td><input type=text name=edit_ulica_comment id=edit_ulica_comment value="'+value.ulica_comment+'" ></td></tr>'+
			'</form>';	
		$("#editulica").html(input);		
		});
		
	});
	
	var txt = '<table class="forms" id=editulica></table>';
				$.prompt(txt,{ 
					buttons:{Change:true, Cancel:false},
					submit: function(e,v,m,f){
						var flag = true;
						if (v) {	
							if (m.find('#edit_ulica_name').val() == '') {
								m.find('#edit_ulica_name').addClass('error');
								flag = false;
							}
							else m.find('#edit_ulica_name').removeClass('error');
														
						}
						return flag;
					},
					close: function(e,v,m,f){
						if(v){							
							var form = m.find("#APISetUlica");
							var ArrayForm = m.find(':input, select').serializeArray();
							$.post("query/json_query.php", {FUNCTION: "APISetUlica", QUERY:ArrayForm},
								function(response){
									var data = eval("("+response+")");
									if (data.error == '0'){
										ulica_id.find('#ulica_name').text(m.find('#edit_ulica_name').val());
										ulica_id.find('#ulica_nasp').text(m.find('#edit_ulica_nasp option:selected').html());
										ulica_id.find('#ulica_comment').text(m.find('#edit_ulica_comment').val());
									}else{
										$.prompt("Ошибка: "+data.error+" Сообщение: "+data.message+"");
									}
								}
							);
						}else{

						}
						
					}
				});
			}

			
		</script>
<p id="tabname">СПРАВОЧНИК УЛИЦЫ<p>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список улиц</span></li>
		<li><span>Новая улица</span></li>
	</ul>
	<div class="box visible" >
		<!--список групп-->
	<table id="ulicatable" class="tablesorter">
		<thead>
			<tr><th>Нас.пункт</th><th>Улица</th><th>Комментарий</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php
			
			if (is_array($tinsp->APIGetUlica(2,"")))
			foreach($tinsp->APIGetUlica(2,"") as $Ulica){
				echo "<tr id='id".$Ulica["ulica_id"]."'>";
				echo "<td id=ulica_nasp>".$Ulica["nasp_name"]."</td>";
				echo "<td id=ulica_name>".$Ulica["ulica_name"]."</td>";
				echo "<td id=ulica_comment>".$Ulica["ulica_comment"]."</td>";
				echo "<td><a href='javascript:;' title='Edit' class='editulica' onclick='editulica(\"".$Ulica["ulica_id"]."\");'><img src='img/Setting-icon.png' width='20'><a href='javascript:;' onclick='remove_ulica(\"".$Ulica["ulica_id"]."\");'><img src='img/delete.png' width='16'></a></td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации улицы-->
	<table class=forms>
		<form action="query/json_query.php" id="APICreatUlica">
			<tr><th>Название улицы</th><td><input type=text name=ulica_name id=ulica_name size=30></td></tr>
			<tr><th>Нас. пункт</th>
			<td>
			<select name="select_ulica_nasp" id=select_ulica_nasp>
			<option value="0"> </option>
			<?php
				if (is_array($tinsp->APIGetNasp(2,"")))
				foreach($tinsp->APIGetNasp(2,"") as $Nasp){
					echo "<option value='".$Nasp["nasp_id"]."'>".$Nasp["nasp_name"]."</option>";
				}
			?>
			</select>
			</td>
			</tr>
			<tr><th>Комментарий</th><td><input type=text name=ulica_comment id=ulica_comment size=30></td></tr>			
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>

 $("#result").hide();
 $("#APICreatUlica").submit(function(event) {
 	$("#result").show();
    event.preventDefault();       
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
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


 
