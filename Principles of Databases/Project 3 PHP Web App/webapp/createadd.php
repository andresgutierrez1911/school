<?php
  // Start session
  session_start();
  //echo session_id();
  $uid = $_SESSION['userID'];
  $pwd = $_SESSION['userPWD'];
  // Connect to MySQL
  $link=mysql_connect("localhost", $uid, $pwd) or die (mysql_error());
  mysql_selectdb($uid) or die(mysql_error());  
?>
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
<div id="main">


<!--  ***** BEGIN CONTENT *****  -->

  <h2>Create a Recipe or Add Ingredient to a Recipe</h2>
  <form name="addingredient" action="addIngredient.php" method="post">
    <p>
      Recipe Name : 
      <input type="text", name="recipe"/>   
    </p>
    <p>
      Ingredient : 
      <input type="text", name="ingredient" />
    </p>
    <p>
      Quantity of Ingredient : 
      <input type="text", name="quantity" />
    </p>
    <input type="submit" value="Submit"/>
  </form>
  
  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
