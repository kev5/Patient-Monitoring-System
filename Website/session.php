<?php

$conn=mysqli_connect("localhost","root","test456","test_db");
session_start();

$user_check=$_SESSION['login_user'];

$ses_sql=mysqli_query($conn,"select rfid from patient_db where rfid='$user_check'");
$row = mysqli_fetch_array($ses_sql,MYSQLI_ASSOC);
   
   $login_session = $row['rfid'];
   
   if(!isset($_SESSION['login_user'])){
      header("location:form.php");
      exit();
}
?>