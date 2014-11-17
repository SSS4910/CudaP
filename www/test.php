<?php
include_once "core.php";

$error = "";
$username = $password = "";

$db = new mysqli(HOST, USER, PASSWORD, DATABASE);
if($db->connect_errno > 0){
    die('Unable to connect to database [' . $db->connect_error . ']');
}

if (isset($_POST["btnLogin"]))
{
	empty($_POST["username"]) ? $error = "Username is required" : $username = test_input($_POST["username"]);
	empty($_POST["password"]) ? $error = "Password is required" : $password = test_input($_POST["password"]);

	//uname, pass rules?
	if (!preg_match("/^[a-zA-Z0-9_]{5,32}$/", $username))
		$error = "Invalid username: must contain letters, numbers or an underscore";
	if (!preg_match("/^[a-zA-Z0-9_]{5,32}$/", $password))
		$error = "Invalid password: must contain letters, numbers or an underscore";

	$stmt = "select * from users;";
	$result = $db->query($stmt);

	while($row = $result->fetch_assoc())
	{
    	$stuff = $row['username'];
    	$morestuff = $row['password'];
	}

	//we've succeeded in logging in!!!
	if (empty($error))
	{
		//header("Location: http://localhost/nerd.html");
	}
}

function test_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}

?>

<html>
<body>
  <form class="form" id="formLogin" action="" method="post">
    <input name="username" id="username" placeholder="Username" type="text"><br>
    <input name="password" id="password" placeholder="Password" type="password"><br>
    <button type="submit" name="btnLogin" id="btnLogin" class="btn">Login</button>
  </form>
Logging in as... <?php echo $username; ?><br>
Your password is <?php echo $_POST["password"]; ?><br>
<span class="error">* <?php echo $error; ?></span>
<span class="error">stuff <?php echo $stuff; ?></span>
<span class="error">more stuff <?php echo $morestuff; ?></span>
</body>
</html>