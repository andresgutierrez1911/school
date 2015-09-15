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

  <h2>Buy Ingredients From Store</h2>
  <p>Items Ordered:</p>
  <?php  
  // Begin transaction
  $trans = "START TRANSACTION;";
  $result= mysql_query($trans) or die(mysql_error());
  $trans_success = true;
    
  // Get all ingredients and quantities for recipe
  $recipe = $_POST['recipe'];
  $query_recipe = "select Ingredient, Quantity from Recipes where RecipeName=\"" . $recipe . "\";";
  $ingList = mysql_query($query_recipe) or die(mysql_error());  
  $numrows = mysql_numrows($ingList);
  
  // Begin printing receipt
  print "<ul>";
  
  // Find each item in the store
  for($i=0; $i<$numrows; $i++)
    {
      // Current ingredient and amount to attempt buying
      $ingredient = mysql_result($ingList, $i, 0);
      $recipe_quantity = mysql_result($ingList, $i, 1);
      
      // Print ordered item to receipt
      print "<li>".$ingredient." (".$recipe_quantity.")"."</li>";
      
      // Request quantity of this ingredient in the store
      $query_store = "select Quantity from Inventory where Ingredient=\"".$ingredient."\";";
      $store_result = mysql_query($query_store) or die(mysql_error());
      $store_quantity = mysql_result($store_result, 0, 0);
      
      // Check if the store has enough of the ingredient
      if ( $recipe_quantity <= $store_quantity ) {
        // If the purchase can be made, then make changes to database
        $purchase = "update Inventory set Quantity=Quantity-".$recipe_quantity." where ingredient=\"".$ingredient."\";";
        $purchase_request = mysql_query($purchase) or die(mysql_error());
        
        // Check quantity in store again
        $query_store = "select Quantity from Inventory where Ingredient=\"".$ingredient."\";";
        $store_result = mysql_query($query_store) or die(mysql_error());
        $store_quantity = mysql_result($store_result, 0, 0);
        
        // If the current transaction causes a negative value, abort mission
        if ( $store_quantity < 0 ) {
          // Plan on rolling back these changes
          $trans_success = false;
          // Print error message
          print "<span class=\"list-error\">Error: Somebody beat you to buying this item!</span>";
        }
        
      }
      // If not, the transaction needs to be aborted
      else {
        // Plan on rolling these changes back
        $trans_success = false;
        // Show that the store has either an inadequate amound, or none of it
        $alert_quan = $store_quantity;
        if ($store_quantity == null) {
          // If the store doesn't have this item at all, then it has 0
          $alert_quan = 0;
        }
        // Print error message
        print "<span class=\"list-error\">The store only has <b>".$alert_quan."</b> units of <b>".$ingredient."</b>.</span>";
      }      
    } 
    
    // Finish printing receipt
    print "</ul>";
    
  // Was the transaction successful?
  if ( $trans_success ) {
    // If so, commit changes
    $trans = "COMMIT;";
    print "<p>Success! Requested ingredients have been purchased from the store.</p>";
  } 
  else {
    // Otherwise, rollback changes
    $trans = "ROLLBACK;";
    print "<p>Sorry! The purchase could not be made.</p>";
  }   
  // End transaction
  $trans_result = mysql_query($trans) or die(mysql_error());
  
  ?>

  <p class="homelink"> << <a href="main.php">Return to Main Menu</a></p>
  

</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
