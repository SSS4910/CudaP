<?php

include_once "./../resources/config.php";
include_once "./../resources/sessions.php";

s_start();

$msg = "";
$username = $password = "";
$error = false;

if (isset($_POST["submitLogout"]))
{
    s_logout();
}

if (isset($_POST["submitLogin"]))
{
    $db = new mysqli(HOST, USER, PASSWORD, DATABASE);
    if($db->connect_errno > 0){
        die('Unable to connect to database [' . $db->connect_error . ']');
    }

	empty($_POST["username"]) ? $error = true : $username = test_input($_POST["username"]);
	empty($_POST["password"]) ? $error = true : $password = test_input($_POST["password"]);

	//uname, pass rules?
    if (!preg_match("/^[a-zA-Z0-9_]{5,32}$/", $username))
    {
		$nameerror = "Invalid username: must contain letters, numbers or an underscore";
        $error = true;
    }
	if (!preg_match("/^[a-zA-Z0-9_]{5,64}$/", $password))
    {
		$passerror = "Invalid password: must contain letters, numbers or an underscore";
        $error = true;
    }
	//sql login, session
	if (!$error)
    {
        if (s_login($username, $password, $db))
        {
            $msg = "success!";
        }
    }
    else
    {
        $msg = "error!";
    }
    $db->close();
}

include "./../resources/templates/navbar.php";

function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}
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

    <title>CS4910 - Log Parsing Dashboard</title>

    <!-- Bootstrap core CSS -->
    <link href="./../resources/library/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="./../resources/css/core.css" rel="stylesheet">
  </head>

  <body>
    <span class="error">Login? <?php echo $msg; ?></span>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.1/jquery.min.js"></script>
    <script src="./../resources/library/dist/js/bootstrap.min.js"></script>
    <script type="text/javascript">
      $(document).ready(function(){
        $('#content').load('home.html');
        $('#homeButton').on('click', function() {
          $('#content').load('home.html');
        });
        $('#aboutButton').on('click', function() {
          $('#content').load('about.html');
        });
      });
    </script>
  </body>
</html>
