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
  
  <h2>Add To Store</h2>
  
  <?php    
    // Get ingredient and quantity values from form
    $ing = $_POST['ingredient'];
    $quan = $_POST['quantity'];    
    
    // See if ingredient is already in Inventory
    $search = "select Ingredient from Inventory where Ingredient=\"".$ing."\";";
    $search_result = mysql_query($search) or die(mysql_error());    
    $numrows = mysql_numrows($search_result);
    
    // If ingredient already exists in database
    if ( $numrows > 0 ) {
      // Update the quantity of the ingrdient
      $update = "update Inventory set Quantity=Quantity+" . $quan . " where ingredient=\"" . $ing . "\";";
    }
    // Otherwise add the new ingredient to the database
    else {
      // Add new row
      $update = "insert into Inventory VALUES('" . $ing . "', " . $quan . ");";      
    }    
    
    // Make changes
    $result = mysql_query($update) or die(mysql_error());    
  ?>
  
  <p>The inventory has been updated.</p>  
  
  <table>
    <tr>
      <th>Ingredient</th><th>Quantity</th>
    </tr>
  <?php
  
  // Request all data from Recipe table
  $get_inventory = "select Ingredient, Quantity from Inventory;";
  $inventory = mysql_query($get_inventory) or die(mysql_error());  
  
  while($nextRow = mysql_fetch_array($inventory))
  {
     print "<tr>";
     print "<td>" . $nextRow['Ingredient'] . "</td> <td>" . $nextRow['Quantity'] . "</td>";
     print "</tr>\n";
  }  
  ?>
  </table>
  
  <p class="homelink"> << <a href="addtostore.html">Add Another Ingredient</a></p>
  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
