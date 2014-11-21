<?php

include_once("./../resources/config.php");
include_once("./../resources/sessions.php");
include_once("./../resources/templates/navbar.php");

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

    <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
      <h1 class="page-header">Contact Us</h1>
      <h2 class="sub-header">University</h2>
      <p class="lead">Lorem ipsum dolor sit amet, nec cetero euismod ea, eos at suas labitur detraxit. Cum ubique oportere ex. Te quo purto accumsan cotidieque, iusto euripidis cu vel, pro alii veri nostro an. Eam graeco meliore cu, diam scaevola perfecto usu at. Ne soluta petentium tincidunt his.</p>
      <h2 class="sub-header">Email</h2>
      <p>Qui te odio case definitionem, eos purto vivendum similique ei. Putant appetere interpretaris mea ei, ei magna dicunt dignissim sit. Officiis intellegat te duo. Meis tamquam interesset qui eu, vix ne labitur apeirian dissentiet. His et movet affert invidunt, at quod stet sea. Cum ut dolorum propriae dissentiunt, ne duo agam aeterno vivendo. Sit an sint solet denique.</p>
      <p>Cu mel saepe offendit, ne has eros tamquam. Nec nibh vocent aliquam an. Est congue dissentias et, delicata theophrastus vel eu. Sit ea amet meis eloquentiam, quo ne eros porro, ad vis denique definiebas. Eu mei causae aeterno mentitum. Nam et facer deserunt concludaturque, has ei illud affert.</p>
      <p>An natum causae sed, ut summo mundi salutatus ius. Mea cu clita essent legimus, usu quod veri atqui ei. Usu cu minimum recteque, assum utamur pri no. Sit id suas tincidunt, nam nibh natum consectetuer no.</p>
      <p>Vide sale eam no. Mea mazim hendrerit eu, est alia lucilius ei. Pro percipitur signiferumque at. Ne vix sumo persius, sit ut vidit definiebas vituperatoribus. Usu ex dolorem omittam suscipit, dico quando pro ex. Dicat convenire voluptatum ad vis, sed te tota minim.</p>
    </div>
    <button type="button" onclick="get_statistics()">Do shit</button>
    <div id="general_stats" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
    <div id="hourly_stats" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
    <div id="monthly_stats" style="min-width: 310px; height: 400px; margin: 0 auto"></div>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    <script src="./../resources/library/dist/js/bootstrap.min.js"></script>
    <script src="../../assets/js/docs.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>



  </body>
</html>
