<?php

$path = ($_POST['path']);

$file404 = fopen($path."404Data.txt", "r");
$statsFile = fopen($path."stats.txt", "r");
$injectFile = fopen($path."injectFile.txt", "r");
$topReqFile = fopen($path."topRequests.txt", "r");

// read in 404 data
$str404 = "";
while(!feof($file404))
{
	$str404 .= fgets($file404)."<br>";
}

// read in Injection data
$injectStr = "";
while(!feof($injectFile))
{
    $inectStr .= fgets($injectFile)."<br>";
}

// read in Injection data
$topReqStr = "";
while(!feof($topReqFile))
{
    $topReqStr .= fgets($topReqFile)."<br>";
}

// read in stats data
$totalStats = fgets($statsFile);
$totalHours = fgets($statsFile);
$totalMonths = fgets($statsFile);

// done with files
fclose($file404);
fclose($injectFile);
fclose($statsFile);
fclose($topReqFile);

// break-up data
/*$statsArray = explode(";", $totalStats);
$hoursArray = explode(";", $totalHours);
$monthsArray = explode(";", $totalMonths);*/

// Format all strings into one string to be sent back to site.
$outputStr = "";

// add stats data to outputStr (delimiter added at end)
$outputStr .= $totalStats."*";
$outputStr .= $totalHours."*";
$outputStr .= $totalMonths."*";

// add delimiter
/*$outputStr .= "*";

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
*/

// add 404 data
$outputStr .= $str404."*";
$outputStr .= $injectStr"*";
$outputStr .= $topReqStr;

// return all data
echo $outputStr;
?>