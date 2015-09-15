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

  <?php
  $recipe = $_POST['recipe'];
  print "<h2>Ingredients For Recipe \"" . $recipe . "\" : </h2> ";
  
  // Request all data from Recipe table
  $query_recipes = "select Ingredient, Quantity from Recipes where RecipeName='" . $recipe . "';";
  $recipes = mysql_query($query_recipes) or die(mysql_error());  
  
  // Get number of results
  $numrows = mysql_numrows($recipes);
  
  // If there are results, display them
  if ( $numrows > 0 ) {
    print "<table> <tr> <th>Ingredient</th> <th>Quantity</th> </tr>";
    while($nextRow = mysql_fetch_array($recipes))
    {
       print "<tr>";
       print "<td>" . $nextRow['Ingredient'] . "</td> <td>" . $nextRow['Quantity'] . "</td>";
       print "</tr>\n";
    }  
    print "</table>";
  }
  // Otherwise, display error message
  else {
    print "<p>No results found!</p>";
  }
  
  ?>
  <br />
  <p class="homelink"> << <a href="list.php">Go Back</a></p>
  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
