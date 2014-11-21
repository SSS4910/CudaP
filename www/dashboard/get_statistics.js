
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
			/*$("#stats").html(dataArray[0]); 
			$("#hours").html(dataArray[1]);
			$("#months").html(dataArray[2]);
			$("#list404").html(dataArray[3]);*/

			var generalStats = dataArray[0].split(";");
			var hourlyStats = dataArray[1].split(";");
			var monthlyStats = dataArray[2].split(";");
			var list404 = dataArray[3];
			var listInject = dataArray[4];

			$(function () {
				$('#hourly_stats').highcharts({
				    title: {
				        text: 'Hours of Access',
				        x: -20 //center
				    },
				    subtitle: {
				        text: 'Number of Visits per Hour',
				        x: -20
				    },
				    xAxis: {
				        categories: ['12:00am', '1:00am', '2:00am', '3:00am', '4:00am', '5:00am', '6:00am', '7:00am', '8:00am', '9:00am', '10:00am',
				        				'11:00am', '12:00pm', '1:00pm', '2:00pm', '3:00pm', '4:00pm', '5:00pm', '6:00pm', '7:00pm', '8:00pm', '9:00pm', '10:00pm',
				        				'11:00pm']
				    },
				    yAxis: {
				        title: {
				            text: 'Number of Visits'
				        },
				        plotLines: [{
				            value: 0,
				            width: 1,
				            color: '#808080'
				        }]
				    },
				    tooltip: {
				        valueSuffix: 'Visits'
				    },
				    legend: {
				        layout: 'vertical',
				        align: 'right',
				        verticalAlign: 'middle',
				        borderWidth: 0
				    },
				    series: [{
				        name: 'Visits',
				        data: [hourlyStats[0], hourlyStats[1], hourlyStats[2], hourlyStats[3], hourlyStats[4], hourlyStats[5], hourlyStats[6], hourlyStats[7], 
				        		hourlyStats[8], hourlyStats[9], hourlyStats[10], hourlyStats[11], hourlyStats[12], hourlyStats[13], hourlyStats[14], hourlyStats[15],
				        		hourlyStats[16], hourlyStats[17], hourlyStats[18], hourlyStats[19], hourlyStats[20], hourlyStats[21], hourlyStats[22], hourlyStats[23]]
				    }]
				});
				});
			
			
		}
	});
}