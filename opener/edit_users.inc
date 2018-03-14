<?php
/*	
–едактор прав	
*/
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->Query("Select * from billing_access where `access_id` = '".$Abonent_id."' or `access_name` = '".$Abonent_id."'", false);
		if(!is_array($Abonent_id_options)){
			exit("јбонент с таким id не найден");
		}
	}else{
		exit("ќшибка переданных параметров");
	}
	
}else{
	exit ("Fail exist parametr");
}
#print_r($Abonent_id_options);
?>

	<!--форма редактировани¤ прав-->
		<table class="forms">
			<form action="query/json_query.php" id="APISetAccess">
			<input type=hidden name=obj size=50 id=obj value="<?=$Abonent_id_options[0]["access_id"]?>">
			<!-- -->
			<tr><td colspan=2 align=center>–≈ƒј “»–ќ¬јЌ»≈ ѕќЋ№«ќ¬ј“≈Ћя: <?=$Abonent_id_options[0]["access_name"]?></td></tr>
			<tr><th>Ћогин:</th><td><input type=text name=access_name id=access_name size=20 value="<?=$Abonent_id_options[0]["access_name"]?>"></td></tr>
			<tr><th>ѕароль:</th><td><input type=text name=access_pass id=access_pass size=20 value="<?=$Abonent_id_options[0]["access_pass"]?>"></td></tr>
			<tr><th> омментарий:</th><td><input type=text name=access_comment id=access_comment size=30 value="<?=$Abonent_id_options[0]["comment"]?>"></td></tr>
			<tr><th>ѕрава:</th>
			<td>
			<?php
			$AccessUserArray=array();
			$AccessUserArray=explode(";",$Abonent_id_options[0]["access"]);

			foreach($tinsp->Query("Select * From billing_access_file order by accessfile_name DESC",false) as $AccessFile){
				if (in_array($AccessFile["accessfile_name"], $AccessUserArray)) {
					echo '<input type=checkbox name="access_access" value="'.$AccessFile["accessfile_name"].'" CHECKED> '.$AccessFile["accessfile_comment"].'<br>';
				}else{
					echo '<input type=checkbox name="access_access" value="'.$AccessFile["accessfile_name"].'"> '.$AccessFile["accessfile_comment"].'<br>';
				}
				
			}
			?>
			
			</td>
			</tr>		
			<tr><td colspan=2><input type="submit" name="sendBtn" value="—охранить" /><input type="button" name="sendBtn" value="«акрыть" onclick="javascript:window.close();"/></td></tr>			
		</form>	


		
	</table>
<script>
//отправка формы
 $("#APISetAccess").submit(function(event) {
    event.preventDefault();    
    /*VARs*/
        var form = $(this);
		var ArrayForm = form.serializeArray();
        url = form.attr("action");	
		funct = form.attr("id");		
    /*POST*/
    $.post(url, {FUNCTION: funct, QUERY:ArrayForm},
		function(response){
			//alert(response);
			var data = eval("("+response+")");
			if (data.error[0] == '0'){			
				window.close();
			}else{
				alert("ќшибка: "+data.error+" —ообщение: "+data.message+"");
			}

		}
    );
  });
</script>