<?php
/*
Справочник секторов/vlan
*/
?>
<script type="text/javascript">
$(document).ready(function() 
    { 
        $("#nasptable").tablesorter(); 
    } 
); 


function ulica_show(id) {
	$("#ulica"+id).show();        
}

function ulica_hide(id) {
	$("#ulica"+id).hide();     
}

function remove_nasp(id) {
$.prompt("Уверены в своем желании?", {
	title: "Вы уверены",
	buttons: { "Да, я уверен": true, "Нет, надо подумать": false },
	submit: function(e,v,m,f){
		if (v) {
			$.post("query/json_query.php", {
				FUNCTION : "APIDelNasp",
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
			
	function editnasp(id){
	var nasp_id = $('#id'+id);
	//инфо по сектору
	var arr = {};
	arr['0'] = {};
	arr['0']['value'] = "2";
	arr['1'] = {};
	arr['1']['value'] = id;
	$.post("query/json_query.php", {FUNCTION: "APIGetNasp", QUERY:arr}, function(response){
		var data = eval("("+response+")");
		$.each(data.error, function(key, value){		
		var input = '<form action="query/json_query.php" id="APISetNasp">'+
			'<input type=hidden name=obj id=obj value="'+id+'">'+
			'<tr><th>Название нас.пункта</th><td><input type=text name=edit_nasp_name id=edit_nasp_name size=30 value="'+value.nasp_name+'"></td></tr>'+
			'<tr><th>Комментарий</th><td><input type=text name=edit_nasp_comment id=edit_nasp_comment value="'+value.nasp_comment+'" ></td></tr>'+
			'</form>';	
		$("#editnasp").html(input);		
		});
		
	});
	
	var txt = '<table class="forms" id=editnasp></table>';
				$.prompt(txt,{ 
					buttons:{Change:true, Cancel:false},
					submit: function(e, v,m,f){
						var flag = true;
						if (v) {	
							if (m.find('#edit_nasp_name').val() == '') {
								m.find('#edit_nasp_name').addClass('error');
								flag = false;
							}
							else m.find('#edit_nasp_name').removeClass('error');
														
						}
						return flag;
					},
					close: function(e, v,m,f){
						if(v){							
							var form = m.find("#APISetNasp");
							var ArrayForm = m.find(':input, select').serializeArray();
							//alert(ArrayForm);
							$.post("query/json_query.php", {FUNCTION: "APISetNasp", QUERY:ArrayForm},
								function(response){
									//alert(response);
									var data = eval("("+response+")");
									if (data.error == '0'){
										nasp_id.find('#nasp_name').text(m.find('#edit_nasp_name').val());
										nasp_id.find('#nasp_comment').text(m.find('#edit_nasp_comment').val());
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
<p id="tabname">СПРАВОЧНИК НАСЕЛЕННЫЙ ПУНКТ</p>

<div class="section">
	<ul class="tabs">
		<li class="current"><span>Список населенных пунктов</span></li>
		<li><span>Новый населенный пункт</span></li>
	</ul>
	<div class="box visible" >
	<!--список групп-->
	<table id="nasptable" class="tablesorter">

		<thead>
			<tr><th>Название</th><th>Улицы</th><th>Параметры</th></tr>
		</thead>
		<tbody>

			<?php
			$GetNasp = $tinsp->APIGetNasp(2,"");
			if (is_array($GetNasp)){
				foreach($GetNasp as $Nasp){
				$GetUlica = $tinsp->APIGetUlica(2, NULL, $Nasp["nasp_id"]);
					echo "<tr id='id".$Nasp["nasp_id"]."'>";
					echo "<td id=nasp_name>".$Nasp["nasp_name"]."</td>";
					echo '<td>
							<a href="javascript:;" onclick="ulica_show(\''.$Nasp["nasp_id"].'\');">Показать</a> /
							<a href="javascript:;" onclick="ulica_hide(\''.$Nasp["nasp_id"].'\');">Скрыть</a>
						</td>';
					echo "<td> 
							<a href='javascript:;' title='Edit' class='editnasp' onclick='editnasp(\"".$Nasp["nasp_id"]."\");'><img src='img/Setting-icon.png' width='20'><a href='javascript:;' onclick='remove_nasp(\"".$Nasp["nasp_id"]."\");'><img src='img/delete.png' width='16'></a>
						</td>
					</tr>";
					echo '<tr id="ulica'.$Nasp["nasp_id"].'" style="display: none;"><td colspan=3><table><tr><th>Улица</th><th>Дома</th><th>Параметры</th></tr>';
					if (is_array($GetUlica)){
						foreach ($GetUlica as $Ulica){
							echo '<tr><td>'.$Ulica["ulica_name"].'</td><td></td><td></td></tr>';
						}
						
					}else{
						echo '<tr><td colspan=3>-------</td></tr>';
					}
					echo '</table></td></tr>';
				}
				
			}
			?>
	</tbody>	
	</table>
	</div>
	<div class="box">	
		<!--форма регистрации сектора-->
	<table class=forms>
		<form action="query/json_query.php" id="APICreatNasp">
			<tr><th>Название нас. пункта</th><td><input type=text name=nasp_name id=nasp_name size=30></td></tr>
			<tr><th>Комментарий</th><td><input type=text name=nasp_comment id=nasp_comment size=30></td></tr>			
			<tr><td colspan=2><input type="submit" name="sendBtn" value="создать" /></td></tr>	
		</form>	
	</table>
<div id=result></div>
<script>
 $("#result").hide();
 $("#APICreatNasp").submit(function(event) {
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
 
