<?php
/*
Справочник домов
*/
?>
<script>
$(document).ready(function() 
    { 
        $("#domtable").tablesorter(); 
    } 	
); 
function remove_dom(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelDom",
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
			
	function editdom(id){
	var dom_id = $('#id'+id);
	//инфо по сектору
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = id;
	
	
	$.post("query/json_query.php", {FUNCTION: "APIGetDom", QUERY:arr}, function(response){
		var data = eval("("+response+")");
		$.each(data.error, function(key, value){			
		//список для редактирования
		var selectulica = '';				
		$.post("query/json_query.php", {FUNCTION: "APIGetUlica", QUERY:Array('2','')}, function(response){
			var data_ulica = eval("("+response+")");
			$.each(data_ulica.error, function(key, ulica_value){
				//select nasp	
				if (value.dom_ulica == ulica_value.ulica_id){
					selectulica+='<option value="'+ulica_value.ulica_id+'" selected>'+ulica_value.nasp_name+' ул. '+ulica_value.ulica_name+'</option>'; 
				}else{
					selectulica+='<option value="'+ulica_value.ulica_id+'">'+ulica_value.nasp_name+' ул. '+ulica_value.ulica_name+'</option>'; 	
				}
			});	
		$("#edit_dom_ulica").html(selectulica);			
		});


		
		var input = '<form action="query/json_query.php" id="APISetDom">'+
			'<input type=hidden name=obj id=obj value="'+id+'">'+
			'<tr><th>Дом</th><td><input type=text name=edit_dom_name id=edit_dom_name size=30 value="'+value.dom_name+'"></td></tr>'+
			'<tr><th>Отдельный объект</th><td><input type=text name=edit_dom_letter id=edit_dom_letter size=5 value="'+value.dom_letter+'"></td></tr>'+
			'<tr><th>Улица</th><td><select id="edit_dom_ulica" name="edit_dom_ulica"></select></td></tr>'+
			'<tr><th>Комментарий</th><td><input type=text name=edit_dom_comment id=edit_dom_comment value="'+value.dom_comment+'" ></td></tr>'+
			'</form>';	
		$("#editdom").html(input);		
		});
		
	});
	
	var txt = '<table class="forms" id=editdom></table>';
				$.prompt(txt,{ 
					buttons:{Change:true, Cancel:false},
					submit: function(e,v,m,f){
						var flag = true;
						if (v) {	
							if (m.find('#edit_dom_name').val() == '') {
								m.find('#edit_dom_name').addClass('error');
								flag = false;
							}
							else m.find('#edit_dom_name').removeClass('error');
														
						}
						return flag;
					},
					close: function(e,v,m,f){
						if(v){							
							var form = m.find("#APISetDom");
							var ArrayForm = m.find(':input, select').serializeArray();
							//alert(ArrayForm);
							$.post('query/json_query.php', {FUNCTION: "APISetDom", QUERY:ArrayForm},
								function(response){
									//alert(response);
									var data = eval("("+response+")");
									if (data.error == '0'){
										dom_id.find('#dom_name').text(m.find('#edit_dom_name').val());
										dom_id.find('#dom_letter').text(m.find('#edit_dom_letter').val());
										dom_id.find('#dom_ulica').text(m.find('#edit_dom_ulica option:selected').html());
										dom_id.find('#dom_comment').text(m.find('#edit_dom_comment').val());
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
<p id="tabname">СПРАВОЧНИК ДОМОВ<p>
<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список домов</span></li>
		<li><span>Новый дом</span></li>
	</ul>
	<div class="box visible" >
		<!--список групп-->
	<table id="domtable" class="tablesorter">
		<thead>
			<tr><th>Населенный пункт</th><th>Улица</th><th>Дом</th><th>Отдельный объект</th><th>Комментарий</th><th width=20>Опции</th></tr>
		</thead>
		<tbody>
			<?php
			$GetDomArr=$tinsp->APIGetDom(2,"");
			if (is_array($GetDomArr))
			foreach($GetDomArr as $Dom){
				echo "<tr id='id".$Dom["dom_id"]."'>";
				echo "<td id=dom_ulica>".$Dom["nasp_name"]."</td>";
				echo "<td id=dom_ulica>".$Dom["ulica_name"]."</td>";
				echo "<td id=dom_name>".$Dom["dom_name"]."</td>";
				echo "<td id=dom_letter>".$Dom["dom_letter"]."</td>";		
				echo "<td id=dom_comment>".$Dom["dom_comment"]."</td>";
				echo "<td><a href='javascript:;' title='Edit' class='editdom' onclick='editdom(\"".$Dom["dom_id"]."\");'><img src='img/Setting-icon.png' width='20'><a href='javascript:;' onclick='remove_dom(\"".$Dom["dom_id"]."\");'><img src='img/delete.png' width='16'></a></td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации улицы-->
	<table class=forms>
		<form action="query/json_query.php" id="APICreatDom">
			<tr><th>Дом</th><td><input type=text name=dom_name id=dom_name size=10></td></tr>
			<tr><th>Объект (<i>Необязателен. Нужен, если объекту в доме нужен отдельный сектор.</i>)</th><td><input type=text name=dom_letter id=dom_letter size=20></td></tr>
			<tr><th>Улица</th>
			<td>
			<select name="select_dom_ulica" id=select_dom_ulica>
			<?php
				if (is_array($tinsp->APIGetUlica(2,"")))
				foreach($tinsp->APIGetUlica(2,"") as $Ulica){
					echo "<option value='".$Ulica["ulica_id"]."'>".$Ulica["nasp_name"]." ул. ".$Ulica["ulica_name"]."</option>";
				}
			?>
			</select>
			</td>
			</tr>
			<tr><th>Комментарий</th><td><input type=text name=dom_comment id=dom_comment size=30></td></tr>			
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>

 $("#result").hide();
 $("#APICreatDom").submit(function(event) {
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


 
