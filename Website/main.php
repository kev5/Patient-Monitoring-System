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

      $username=$_POST['username'];
      $password=$_POST['password'];
      
      $sql = "SELECT * FROM doc_login where username='$username' and password='$password'";
      $result = mysqli_query($conn,$sql);
      $row = mysqli_fetch_array($result,MYSQLI_ASSOC);
      
      if($row['username']==$username && $row['password']==$password) {
         
         echo "<script type='text/javascript'> document.location = 'form.php'; </script>";
         exit();
      }else {
         echo "<div class='error-box'>Failed to login. Please enter correct username and password.</div>";
      }
   }
   ob_end_flush();
?>

<link href="//db.onlinewebfonts.com/c/a4e256ed67403c6ad5d43937ed48a77b?family=Core+Sans+N+W01+35+Light" rel="stylesheet" type="text/css"/>
<link rel="stylesheet" href="style.css" type="text/css">
<div class="body-content">
  <div class="module">
    <h1>Login</h1>
    <form class="form" action="main.php" method="POST" enctype="multipart/form-data" autocomplete="off">
      <p>
        <label>Enter Username: </label>
        <input type="text" id="username" placeholder="Username" name="username" required />
      </p>
      <p>
        <label>Enter Password: </label>
        <input type="password" id="password" placeholder="Password" name="password" required />
      </p>
      <p>
      <input type="submit" value="Submit" name="submit" class="btn btn-block btn-primary" />
      </p>

    </form>
  </div>
</div>