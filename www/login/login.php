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
            //Header("Location http://mrsarver.com/home");
        }
    }
    else
    {
        $msg = "error!";
    }
    $db->close();
}
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>Signin Template for Bootstrap</title>

    <!-- Bootstrap core CSS -->
    <link href="../../resources/library/dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="./../resources/css/login.css" rel="stylesheet">
  </head>

  <body>

    <div class="container">

      <form class="form-signin" role="form" action="" method="post"> 
        <h2 class="form-signin-heading">Please sign in</h2>
        <label for="username" class="sr-only">Username</label>
        <input type="text" name="username" id="username" class="form-control" placeholder="Username" required autofocus>
        <label for="password" class="sr-only">Password</label>
        <input type="password" name="password" id="password" class="form-control" placeholder="Password" required>
        <button class="btn btn-lg btn-primary btn-block" type="submit" name="submitLogin" id="submitLogin">Sign in</button>
      </form>

    </div> <!-- /container -->

    <p>Login? <?php echo $msg; ?></p>
  </body>
</html>

