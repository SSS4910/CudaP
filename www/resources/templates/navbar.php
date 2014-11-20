<?php

include_once "./../config.php" ;
include_once "./../sessions.php";

?>

<!DOCTYPE html>
<html lang="en">
  <body>
    <!-- Navbar -->
    <nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
      <div class="container-fluid">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="./../home">CS4910 - Log Processing Dashboard</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav navbar-right pull-right">
            <li><a href="./../home/" id="homeButton">Home</a></li>
            <li><a href="./../about/" id="aboutButton">About</a></li>
            <?php if (s_check_login() == true) : ?>
            <li><a href="./../dashboard/" id="dashButton">Dashboard</a></li>
            <li class="dropdown" id="menulogout">
            <a class="dropdown-toggle" href="#" data-toggle="dropdown" id="navLogout">Logout</a>
            <div class="dropdown-menu" style="padding:17px;">
              <form class="form" id="formLogout" action="" method="post">
                <button type="submit" name="submitLogout" id="submitLogout" class="btn">Logout</button>
              </form>
            </div>
            </li>
            <?php else : ?>
            <li><a href="./../login/login.php" id="loginButton">Login</a></li>
            <?php endif; ?>
          </ul>
        </div>
      </div>
    </nav>
  </body>
</html>

