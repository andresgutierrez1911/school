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

  <h2>List Ingredients</h2>
  <form name="listingredients" action="inglist.php" method="post">
    <p>
      Recipe : 
      <?php  
      // Request recipes from database
      $query_recipes = "select distinct RecipeName from Recipes;";
      $recipes = mysql_query($query_recipes) or die(mysql_error());  
      
      // Generate select box of recipes
      print "<select name=\"recipe\">";
      while($nextRow = mysql_fetch_array($recipes)) {
         print "<option value=\"".$nextRow['RecipeName']."\">";
         print $nextRow['RecipeName'];
         print "</option>\n";
      } 
      print "</select>";

      ?>
      <!-- <input type="text", name="recipe"/> -->
      <input type="submit" value="Submit"/>
    </p>
  </form>
  <br />
  <br />
  <br />
  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  
  
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
