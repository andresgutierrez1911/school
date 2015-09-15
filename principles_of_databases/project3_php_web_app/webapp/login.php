<html>
<!-- 
*
* Recipe Manager by Katie Schaffer
* Database Systems, Project #3
* December 5, 2014
*
-->
<head>
<title>Recipe Manager</title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
<h1 class="title">Recipe Manager</h1>
<div id="main" class="centered">


<!--  ***** BEGIN CONTENT *****  -->
  
  <?php
  
  // Create session
  session_start();
  //echo session_id();  
  $uid = $_POST['username'];
  $pwd = $_POST['password'];  
  $_SESSION['userID'] = $uid;
  $_SESSION['userPWD'] = $pwd;
  
  // Connect to MySQL
  $link=mysql_connect("localhost", $uid, $pwd) or die (mysql_error());
  mysql_selectdb($uid) or die(mysql_error());

  // Create tables if they do not exist yet  
  $query1 = "create table if not exists Inventory(Ingredient char(100) NOT NULL, Quantity char(100) NOT NULL, PRIMARY KEY (Ingredient) );";
  $query2 = "create table if not exists Recipes(RecipeName char(100) NOT NULL, Ingredient char(100) NOT NULL, Quantity integer NOT NULL, PRIMARY KEY (RecipeName, Ingredient) );";  
  
  //print "<p>Query 1: \"" . $query1 . "\"</p>";
  //print "<p>Query 2: \"" . $query2 . "\"</p>";

  $result1 = mysql_query($query1) or die(mysql_error());
  $result2 = mysql_query($query2) or die(mysql_error());
      
  $name = $_SESSION['userID'];
  print "<h2>Welcome, " . $name . "!</h2>" ;
  
  ?>
  
  <p>Continue to <a href="main.php">the main menu</a>.</p>

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
