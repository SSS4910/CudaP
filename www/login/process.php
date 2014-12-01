<?php

include_once "./../resources/config.php";
include_once "./../resources/sessions.php";

$db = new mysqli(HOST, USER, PASSWORD, DATABASE);
if($db->connect_errno > 0){
    die('Unable to connect to database [' . $db->connect_error . ']');
}

if (isset($_POST["submitLogin"]))
{
    $error = false;

    //server side input validation
    empty($_POST["username"]) ? $error = true : $post_username = strip_input($_POST["username"]);
    empty($_POST["password"]) ? $error = true : $post_password = strip_input($_POST["password"]);

    if (!preg_match("/^[a-zA-Z0-9_]{5,32}$/", $post_username))
    {
        $error = true;
    }
    if (!preg_match("/^[a-zA-Z0-9_]{5,64}$/", $post_password))
    {
        $error = true;
    }
    if (!$error)
    {
        //if !s_login : header(login):
        if (!s_login($post_username, $post_password, $db))
        {
            echo "Login failed!<br>";
            header("Location: http://127.0.0.1/login/");
        }
        else
        {
            echo "Hello Admin!<br>";
            //sleep(5);
        }
    }
    else{ header("Location: http://127.0.0.1/login/"); }
}
else
{
    header("Location: http://127.0.0.1/login/");
}

$db->close();

function strip_input($data) {
	$data = trim($data);
	$data = stripslashes($data);
	$data = htmlspecialchars($data);
	return $data;
}
?>
