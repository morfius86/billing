<?php
/*	
–едактор NAS	
*/
if (isset($_GET["id"])){
	$Abonent_id=$_GET["id"];
	if(strlen($Abonent_id)>2){
		$Abonent_id_options=$tinsp->APIGetBillProp(3, $Abonent_id);
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

	<!--форма редактировани¤ абонента-->
		<?php include("edit_abonent_navigation.inc"); ?>
		<table class="forms" style="width: 100%;">

			<form action="query/json_query.php" id="APISetAdvAttr">
			<input type=hidden name=obj size=50 id=obj value="<?=$Abonent_id_options[0]["user_id"]?>">
			<!--“ј–»‘» ј÷»я-->
			<tr><th colspan=2>ј“–»Ѕ”“џ</th></tr>
			<?php
			$AttrValue=array();
			foreach ($tinsp->APIGetAddAttr(2, "") as $Attr) {
				$AttrValueArray=$tinsp->APIGetAdvAttr($_GET["id"], $Attr["attr_id"]);
				if ($AttrValueArray[0]!="0") {
					$AttrValue="";
				}else{
					$AttrValue=$AttrValueArray[1][0]["value"];
				}
				print '<tr><th>'.$Attr["attr_name"].'</th><td><TEXTAREA NAME="'.$Attr["attr_id"].'" COLS="40" ROWS="1">'.$AttrValue.'</TEXTAREA></td></tr>';
			}
			?>			
			<tr><td colspan=2><input type="submit" name="sendBtn" class="button" value="—охранить" /><input type="button" name="sendBtn" class="button" value="«акрыть" onclick="javascript:window.close();"/></td></tr>			
		</form>	


		
	</table>
<script>
//отправка формы
 $("#APISetAdvAttr").submit(function(event) {
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