
function get_statistics()
{
	var path = "";
	
	// use an ajax call to use php script
	$.ajax({
		type:	"POST",
		url:	"./get_statistics.php",
		data: 	"path=" + path,
		success: function(data)
		{
			//Update the field html fields with data
			//alert(data);
			// separate data
			var dataArray = data.split("*");
			
			// update corresponding fields
			$("#stats").html(dataArray[0]); 
			$("#hours").html(dataArray[1]);
			$("#months").html(dataArray[2]);
			$("#list404").html(dataArray[3]);
			
			
		}
	});
}