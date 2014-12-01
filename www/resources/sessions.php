<?php

//config.php

/* FUNCTION: s_start
 * -----------------
 * Initiates session access for current request
 *
 * Return:
 *   true on success
 *   false on failure
 */
function s_start(){
    //session_start();
    return false;
}

/* FUNCTION: s_check_login
 * -----------------------
 * Verifies that a registered user is logged in
 * 
 * Return:
 *   true on success
 *   false on failure
 */
function s_check_login(){
    //if !isSet(username) : return false

    //if time > ttl : return false
    
    //if !cookie == correct : return false

    //return true 
    return false;
}

/* FUNCTION: s_login
 * ------------------
 * Verifies user details and signs specified user in
 *
 * Parameters:
 *   username - provided username to use
 *   password - provided password to use
 *   db - the database connection
 *
 * Return:
 *   true on success
 *   false on failure
 */
function s_login($post_username, $post_password, $db){
    echo "Attempting username: $post_username <br>";
    echo "Attempting password: $post_password <br>";
    //prepare and execute database statement
    if ($stmt = $db->prepare("SELECT username, password, salt FROM members WHERE username=?"))
    {
        $stmt->bind_param("s", $post_username);
        $stmt->execute();
        $stmt->store_result();
        $stmt->bind_result($db_username, $db_password, $db_salt);
        $stmt->fetch();

        //if !response : return
        if (!$stmt->num_rows){ return false; }
        $stmt->close();

        //salt and hash password
        $post_password = hash('sha512', $db_salt.$post_password);
        if ($post_password == $db_password)
        {
            $db_username = preg_replace("/[^a-zA-Z0-9_\-]+/", "", $db_username);
            echo "$db_username <br>";
            //$_SESSION["user"] = $db_username;
            return true;
        }
        return false;
    }
    return false;
}

/* FUNCTION: s_logout
 * ------------------
 * Destroys session and signs current user out
 *
 * Return:
 *   true on success
 *   false on failure
 */
function s_logout(){
    s_start();
    //nullify session array
    //$_SESSION = array();
    //session_destroy();
    //session_destroy()
    return true;
}
?>
