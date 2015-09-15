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

  <h2>Main Menu</h2>
  <p><a href="createadd.php">Create a Recipe or Add Ingredient to a Recipe</a></p>
  <p><a href="list.php">List a Recipe's Ingredients</a></p>
  <p><a href="buy.php">Buy All Recipe Ingredients From The Store</a></p>
  <p><a href="addtostore.html">Add Ingredients To The Store</a></p>
  <br />
  <?php
  
  session_start();
  //echo session_id();

  $uid = $_SESSION['userID'];
  $pwd = $_SESSION['userPWD'];
    
  print "<h3 class=\"note\">You are logged in as \"" . $uid . "\".</h3>" ;
  
  ?>
  
  <p class="homelink"> << <a href="index.html">Return to login</a></p>
  
  
</div>
<p class="footer">K Schaffer - Database Systems, Project #3</p>
</body>
</html>
