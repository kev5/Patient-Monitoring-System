<?php
 include('session.php');

   $conn=mysqli_connect("localhost","root","test456","test_db");
   
   if(!$conn ) {
      die('Could not connect: ' . mysqli_error($conn));
   }

   $sql = "SELECT * FROM patient_db where RFID = '".$_SESSION['rfid']."' ";

   $retval = mysqli_query($conn,$sql);

   if(!$retval ) {
      die('Could not get data: ' . mysqli_error($conn));
   }

   echo '<table id="table1" style="float:center;margin-left:5%;margin-top:20%" border="1">
		<tr>
		<th>Name</th>
		<th>Age</th>
		<th>Gender</th>
		<th>Contact</th>
		<th width=50%>Address</th>
		<th>Blood Group</th>
		<th>Medical Records</th>			
		</tr>';

   while($row = mysqli_fetch_array($retval,MYSQLI_ASSOC)) {

   		echo "<tr>";
		echo "<td width=20%>" . $row['Name'] . "</td>";		
		echo "<td>" . $row['Age'] . "</td>";
		echo "<td>" . $row['Gender'] . "</td>";
		echo "<td width=5%>" . $row['Contact'] . "</td>";
		echo "<td>" . $row['Address'] . "</td>";
		echo "<td>" . $row['Blood_Group'] . "</td>";
		echo "<td width=60%>" . $row['Medical_Records'] . "</td>";
		echo "</tr>";
	}

	echo "</table>";
      /*echo "Name :{$row['Name']}  <br> ".
         "Age : {$row['Age']} <br> ".
         "Gender : {$row['Gender']} <br> ".
         "Contact : {$row['Contact']} <br> ".
         "Address : {$row['Address']} <br> ".
         "Blood Group : {$row['Blood_Group']} <br> ".
         "Medical Records : {$row['Medical_Records']} <br> ";*/

   if(isset($_POST['update'])) {
            
            
      $sql = "SELECT rfid FROM patient_db WHERE rfid = '$rfid'";
      $result = mysqli_query($conn,$sql);
      $row = mysqli_fetch_array($result,MYSQLI_ASSOC);
      //$active = $row['active'];
      
      $count = mysqli_num_rows($result);
      
      // If result matches $rfid, table row must be 1 row
    
      if($count!=0) {
         
         $_SESSION['login_user'] = $rfid;
         
         echo "<script type='text/javascript'> document.location = 'update.php'; </script>";
         exit();
      }
   }

   
?>

<!DOCTYPE html>
<html>
<link href="//db.onlinewebfonts.com/c/a4e256ed67403c6ad5d43937ed48a77b?family=Core+Sans+N+W01+35+Light" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" href="style.css" type="text/css">
<head>
<title>Your Home Page</title>
</head>
<form action="update.php" method="POST">
<body>
	 <!--<h1>Welcome <?php echo $_SESSION['rfid']; ?></h1> -->
	 
	 <p style="float:center;margin-left: 50%">
      <input type="submit" document.location='update.php' value="Update" name="update" class="btn btn-primary" />
      </p>
      <h2><a href = "logout.php"><p style="float:center;margin-left:5%;margin-top:20%" >Sign Out</p></a></h2>

</body>
</form>
</html>
