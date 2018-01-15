<?php
 include('session.php');

   if(isset($_POST['submit'])){

   	$conn=mysqli_connect("localhost","root","test456","test_db");
   
   if(!$conn ) {
      die('Could not connect: ' . mysqli_error($conn));
   }

    $sql = "SELECT * FROM patient_db where RFID = '".$_SESSION['rfid']."' ";

	$res = mysqli_query($conn,$sql);
    $row= mysqli_fetch_array($res,MYSQLI_ASSOC);

        $newage = $_POST['newage'];
        $newcontact = $_POST['newcontact'];
        $newaddress = $_POST['newaddress'];
        $newmedrec = $_POST['newmedrec'];

      		if(($newage=="") && ($newcontact=="") && ($newaddress==""))
      		{
        	$newsql = "UPDATE patient_db SET Medical_Records = CONCAT(ifnull(Medical_Records,''),'<br/>$newmedrec') WHERE RFID = '".$_SESSION['rfid']."' ";
        	}
        	else
        	{
        		$newsql = "UPDATE patient_db SET Age = '$newage', Contact = '$newcontact', Address = '$newaddress', Medical_Records = CONCAT(ifnull(Medical_Records,''),'<br/><br/>$newmedrec') WHERE RFID = '".$_SESSION['rfid']."' ";
        	}

            $retval = mysqli_query($conn,$newsql);
            
            if(! $retval ) {
               die('Could not update data: ' . mysqli_error($conn));
            }
            echo "Updated data successfully\n";
            
            mysqli_close($conn);

       // echo "<meta http-equiv='refresh' content='0';url=profile.php>";
    	
	}

?>

<!DOCTYPE html>
<html>
<link href="//db.onlinewebfonts.com/c/a4e256ed67403c6ad5d43937ed48a77b?family=Core+Sans+N+W01+35+Light" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" href="style.css" type="text/css">
<head>
<title>Update</title>
</head>
<form action="update.php" method="POST">

<body>
<div class="body-content">
  <div class="module">
<h1>Update Your Records -</h1>
	  <p>
        <label>Enter New Age: </label>
        <input type="text" id="newage" placeholder="New Age" name="newage"/>
      </p><br/>
      <p>
        <label>Enter New Contact: </label>
        <input type="text" id="newcontact" placeholder="New Contact" name="newcontact"/>
      </p><br/>
      <p>
        <label>Enter New Address: </label>
        <input type="text" id="newaddress" placeholder="New Address" name="newaddress"/>
      </p><br/>
       <p>
        <label>Update Medical Records: </label>
        <input type="text" id="newmedrec" placeholder="Update Medical Records" name="newmedrec"/>
      </p><br/>
<p style="float:center;margin-left: 50%">
      <input type="submit" value="Submit" name="submit" id="submit" class="btn btn-primary" />
      </p>
      <h2><a href = "logout.php"><p style="float:center;margin-left:5%;margin-top:20%" >Sign Out</p></a></h2>
      </div>
      </div>
</body>

</form>
</html>