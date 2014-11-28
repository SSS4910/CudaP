<?php

include_once("./../resources/config.php");
include_once("./../resources/sessions.php");
//include_once("./../resources/templates/navbar.php");

?>

<!DOCTYPE html>
<html lang="en">
  <head>
    
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="Log Parsing Dashboard">
    <meta name="author" content="Source code refurbished from Bootstrap Sample">
    <link rel="icon" href="../../favicon.ico">

    <title>Dashboard</title>

    <!-- Bootstrap core CSS -->
    <link href="./../resources/library/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="./../resouces/css/core.css" rel="stylesheet">

    <!-- sss4910 Javascript-->
    <script src="./get_statistics.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>
  <body>
    <div class="container-fluid">
        <button type="button" onclick="get_statistics()">Display Statistics</button>

        <div class="row">
            <div id="general_stats" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
        </div>

        <div class="row">

            <div class="span6">
                <div id="hourly_stats"></div><!-- -->
            </div>

            <div class="span6">
                <div id="monthly_stats"></div>
            </div>

        </div>

        <div class="row">
            <div id="topReq_stats" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
        </div>

        <div class="row">
            <div id="404_list" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
        </div>

        <div class="row">
            <div id="inject_list" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
        </div>

    </div>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    <script src="./../resources/library/dist/js/bootstrap.min.js"></script>
    <script src="../../assets/js/docs.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>

    <script src="js/highcharts.js"></script>
    <script src="js/modules/exporting.js"></script>
    
  </body>
</html>
