
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
			alert(data);
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

			/*alert(generalStats);
			alert(hourlyStats);
			alert(monthlyStats);
			alert(list404);
			alert(listInject);*/

			var intGeneralStats = new Array();
			for(var x = 0; x < 4; x++)
			{
				intGeneralStats[x] = parseInt(generalStats[x]);
			}

			var intHourlyStats = new Array();
			for(x = 0; x < 24; x++)
			{
				intHourlyStats[x] = parseInt(hourlyStats[x]);
			}

			var intMonthlyStats = new Array();
			for(x = 0; x < 12; x++)
			{
				intMonthlyStats[x] = parseInt(monthlyStats[x]);
			}

			$(function () {
				$('#general_stats').highcharts({
				    chart: {
				        type: 'bar'
				    },
				    title: {
				        text: 'Basic statistics'
				    },
				    subtitle: {
				        text: ''
				    },
				    xAxis: {
				        categories: ['200s', '404s', 'Injections', 'total visits'],
				        title: {
				            text: null
				        }
				    },
				    yAxis: {
				        min: 0,
				        title: {
				            text: 'Population (millions)',
				            align: 'high'
				        },
				        labels: {
				            overflow: 'justify'
				        }
				    },
				    tooltip: {
				        valueSuffix: ' millions'
				    },
				    plotOptions: {
				        bar: {
				            dataLabels: {
				                enabled: true
				            }
				        }
				    },
				    legend: {
				        layout: 'horizontal',//'vertical',
				        align: 'right',
				        horizontalAlign: 'top',//verticalAlign: 'top',
				        x: -40,
				        y: 100,
				        floating: true,
				        borderWidth: 1,
				        backgroundColor: ((Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'),
				        shadow: true
				    },
				    credits: {
				        enabled: false
				    },
				    series: [{
				        name: 'Occurrences',
				        data: [intGeneralStats[0], intGeneralStats[1], intGeneralStats[2], intGeneralStats[3]]
				    }]
				});
			});

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
				        data: [intHourlyStats[0], intHourlyStats[1], intHourlyStats[2], intHourlyStats[3], intHourlyStats[4], intHourlyStats[5], intHourlyStats[6], intHourlyStats[7], 
				        		intHourlyStats[8], intHourlyStats[9], intHourlyStats[10], intHourlyStats[11], intHourlyStats[12], intHourlyStats[13], intHourlyStats[14], intHourlyStats[15],
				        		intHourlyStats[16], intHourlyStats[17], intHourlyStats[18], intHourlyStats[19], intHourlyStats[20], intHourlyStats[21], intHourlyStats[22], intHourlyStats[23]]
				    }]
				});
			});

				$(function () {
					$('#monthly_stats').highcharts({
					    title: {
					        text: 'Months of Access',
					        x: -20 //center
					    },
					    subtitle: {
					        text: 'Number of Visits per Month',
					        x: -20
					    },
					    xAxis: {
					        categories: ['January', 'February', 'March', 'April', 'May', 'June', 'July', 
					        			 'August', 'September', 'October', 'November','December']
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
					        data: [intMonthlyStats[0], intMonthlyStats[1], intMonthlyStats[2], intMonthlyStats[3], intMonthlyStats[4], 
					        		intMonthlyStats[5], intMonthlyStats[6], intMonthlyStats[7], intMonthlyStats[8], intMonthlyStats[9], 
					        		intMonthlyStats[10], intMonthlyStats[11]]
					    }]
					});
				});
			
			
		}
	});
}