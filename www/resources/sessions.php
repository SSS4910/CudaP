<?php

function s_start(){
    session_start();
}

function s_login($username, $password, $db){
    if ($stmt = $db->prepare("SELECT username, password FROM members WHERE username=?"))
    {
        $stmt->bind_param("s", $username);
        $stmt->execute();
        $stmt->bind_result($db_username, $db_password);
        $stmt->fetch();

        $stmt->close();
        if ($password == $db_password){
            s_start();
            session_regenerate_id(true);
            $username = preg_replace("/[^a-zA-Z0-9_]+/", "", $username);
            $_SESSION["username"] = $username;
            return true;
        }
        return false;
    }
    return false;
}

function s_check_login(){
    if (isset($_SESSION["username"]))
    {
        return true;
    }
    return false;
}

function s_check_timeout(){
    if (!isset($_SESSION["timeout"]))
    {
        $_SESSION["timeout"] = time();
    }
    elseif(time() - $_SESSION["timeout"] > 10 * 60)
    {
        return true;
    }
    else
    {
        //reset session timeout
        $_SESSION["timeout"] = time();
    }
    return false;
}

function s_logout(){
    $_SESSION = array();
    session_destroy();
}

?>
