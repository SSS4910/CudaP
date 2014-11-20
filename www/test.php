<?php
include_once "./resources/config.php" ;
include_once "./resources/sessions.php";

$dev_s_start = "";
$dev_login = "";
$dev_logout = "";
$dev_uname = "";
$dev_upass = "";
$nameerror = $passerror = $required = $msg = "";
$username = $password = "";
$error = false;

s_start();

if (isset($_POST["btnLogout"]))
{
    s_logout();
}

if (isset($_POST["btnLogin"]))
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
        else
        {
            $msg = "failure!";
        }
        //$db->close();
		//header("Location: http://localhost/nerd.html");
    }
    $db->close();
}

function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}
?>

<html>
<head>
  <meta content="text/html;charset=utf-8" http-equiv="Content-Type">
  <meta content="utf-8" http-equiv="encoding">
</head>
<body>
  <?php if (s_check_login() == true) : ?>
  <p>Welcome: <?php echo $_SESSION["username"]; ?></p>
  <form class="form" id="formLogout" action="" method="post">
    <button type="submit" name="btnLogout" id="btnLogout" class="btn">Logout</button>
  </form>
  <?php else : ?>
  <form class="form" id="formLogin" action="" method="post">
    <span class= "error">* Required field</span><br>
    <input name="username" id="username" placeholder="Username" type="text"><span>*</span><br>
    <span class="error"><?php echo $nameerror; ?></span><br>
    <input name="password" id="password" placeholder="Password" type="password"><span>*</span><br>
    <span class="error"><?php echo $passerror; ?></span><br>
    <button type="submit" name="btnLogin" id="btnLogin" class="btn">Login</button>
  </form>
  <?php endif; ?>
Logging in as... <?php echo $username; ?><br>
Your password is <?php echo $password; ?><br>
<span class="error">errors: <?php echo $error; ?></span><br>
<span class="error">SQL QUERY USERNAME: <?php echo $dev_uname; ?></span><br>
<span class="error">SQL QUERY PASSWORD: <?php echo $dev_upass; ?></span><br>
<span class="error">msg: <?php echo $msg; ?></span><br>
<span class="error">session started?: <?php echo $dev_s_start; ?></span><br>
<span class="error">login?: <?php echo $dev_login; ?></span><br>
<span class="error">session logout?: <?php echo $dev_logout; ?></span>
</body>
</html>
