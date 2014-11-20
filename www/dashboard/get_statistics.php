<?php

$path = ($_POST['path']);

$file404 = fopen($path."404Data.txt", "r");
$statsFile = fopen($path."stats.txt", "r");


// read in 404 data
$str404 = "";
while(!feof($file404))
{
	//echo fgets($file404);
	$str404 .= fgets($file404)."<br>";
	//echo $str404."<br>";
}

// read in stats data
$totalStats = fgets($statsFile);
$totalHours = fgets($statsFile);
$totalMonths = fgets($statsFile);

// done with files
fclose($file404);
fclose($statsFile);

// break-up data
$statsArray = explode(";", $totalStats);
$hoursArray = explode(";", $totalHours);
$monthsArray = explode(";", $totalMonths);

// Format all strings into one string to be sent back to site.
$outputStr = "";

// Stats data formatting
$outputStr .= "<li>Total 200s: ".$statsArray[0]."</li>";
$outputStr .= "<li>Total 404s: ".$statsArray[1]."</li>";
$outputStr .= "<li>Total Injections: ".$statsArray[2]."</li>";
$outputStr .= "<li>Total Visits: ".$statsArray[3]."</li>";

// add delimiter
$outputStr .= "*";

// hours formatting
for($x = 0; $x < 24; $x++)
{
	$outputStr .= "<li>".($x+1).":00: ".$hoursArray[$x]." visits</li>";
}

// add delimiter
$outputStr .= "*";

// months formatting
for($x = 0; $x < 12; $x++)
{
	$outputStr .= "<li>Month ".($x+1).": ".$monthsArray[$x]." visits</li>";
}

// add delimiter
$outputStr .= "*";

// add 404 data
$outputStr .= $str404;

// return all data
echo $outputStr;
?>