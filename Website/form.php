<style type="text/css">
.error-box {
    color:#555;
    font-family:Tahoma,Geneva,Arial,sans-serif;font-size:18px;
    padding:10px 10px 10px 36px;
    margin:20px;
    background-color: #ffecec;
    border:1px solid #f5aca6;
    width: 300px;
}
</style>

<?php
    ob_start();
   $conn=mysqli_connect("localhost","root","test456","test_db");
   session_start();
   
   if(isset($_POST['submit'])){ 
      // RFID sent from form 
      
      $rfid= mysqli_real_escape_string($conn,$_POST['rfid']);
      $_SESSION['rfid']=$rfid;
      
      
      $sql = "SELECT rfid FROM patient_db WHERE rfid = '$rfid'";
      $result = mysqli_query($conn,$sql);
      $row = mysqli_fetch_array($result,MYSQLI_ASSOC);
      //$active = $row['active'];
      
      $count = mysqli_num_rows($result);
      
      // If result matches $rfid, table row must be 1 row
    
      if($count == 1) {
         
         $_SESSION['login_user'] = $rfid;
         
         echo "<script type='text/javascript'> document.location = 'profile.php'; </script>";
         exit();
      }else {
         echo "<div class='error-box'>Failed to login. Your RFID is invalid.</div>";
      }
   }
   ob_end_flush();
?>

<link href="//db.onlinewebfonts.com/c/a4e256ed67403c6ad5d43937ed48a77b?family=Core+Sans+N+W01+35+Light" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" href="style.css" type="text/css">
<div class="body-content">
  <div class="module">
    <h1>Welcome to Patient Database</h1>
    <form class="form" action="form.php" method="POST" enctype="multipart/form-data" autocomplete="off">
      <p>
        <label>Enter RFID Number: </label>
        <input type="text" id="rfid" placeholder="RFID Number" name="rfid" required />
      </p>
      <p>
      <input type="submit" value="Submit" name="submit" class="btn btn-block btn-primary" />
      </p>

    </form>
  </div>
</div>