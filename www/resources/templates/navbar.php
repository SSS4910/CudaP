<?php

include_once "./../config.php" ;
include_once "./../sessions.php";

?>

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
            <?php if (true) : ?>
            <li><a href="./../dashboard/" id="dashButton">Dashboard</a></li>
            <li><a href="./../logout/" id="logoutButton">Logout</a></li>
            <?php else : ?>
            <li><a href="./../login/login.php" id="loginButton">Login</a></li>
            <?php endif; ?>
          </ul>
        </div>
      </div>
    </nav>

  </body>
</html>

