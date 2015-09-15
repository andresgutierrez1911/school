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
  
  <?php     
    // Get recipe info from form
    $rec = $_POST['recipe'];
    $ing = $_POST['ingredient'];
    $quan = $_POST['quantity']; 
    
    // See if the recipe/ingredient combo already exists in database
    $search = "select RecipeName from Recipes where RecipeName=\"".$rec."\" and Ingredient=\"".$ing."\";";
    $search_result = mysql_query($search) or die(mysql_error());    
    $numrows = mysql_numrows($search_result);
    
    // If recipe/ingredient combo already exists in database
    if ( $numrows > 0 ) {
      // Update the quantity of the ingrdient in the recipe
      $update = "update Recipes set Quantity=" . $quan . " where RecipeName=\"".$rec."\" and Ingredient=\"".$ing."\";";
    }
    // Otherwise add new recipe ingredient to the database
    else {
      // Add new row to the recipe table   
      $update = "insert into Recipes VALUES('" . $rec . "', '" . $ing . "', " . $quan . ");";
    }    
    
    // Make changes
    $result = mysql_query($update) or die(mysql_error());    
      
  ?>

  <table>
    <tr>
      <th>Ingredient</th><th>Quantity</th>
    </tr>
  <?php
  
  // Request all data from Recipe table
  $query_recipes = "select * from Recipes where RecipeName=\"" . $rec . "\";";
  $recipes = mysql_query($query_recipes) or die(mysql_error());  
  
  while($nextRow = mysql_fetch_array($recipes))
  {
     print "<tr>";
     print "<td>" . $nextRow['Ingredient'] . "</td> <td>" . $nextRow['Quantity'] . "</td>";
     print "</tr>\n";
  }  
  ?>
  </table>
  
  <?php
    print "<p>The recipe \"" . $rec . "\" has been updated.</p>";
  ?> 
  
  <p class="homelink"> << <a href="createadd.php">Make another update.</a></p>
  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
