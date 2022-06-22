<?php
date_default_timezone_set("Asia/Jakarta");
$time = date('Y-m-d H:i:s');
$kondisi = $_GET['kondisi'];
$otomatis = $_GET['otomatis'];
$ldr = $_GET['ldr'];

$servername = "localhost";
$username = "aqil";
$password = "password";
$dbname = "iot_2022";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sqlget = "SELECT sender_id,kondisi FROM lampu";
$result = $conn->query($sqlget);
//echo $result;
  while($row = $result->fetch_assoc()) {
    $sender_id = $row["sender_id"];
    $link = mysqli_connect("localhost", "aqil", "password", "iot_2022");
    // Check connection
    if($link === false){
        die("ERROR: Could not connect. " . mysqli_connect_error());
    }
    $resultone = mysqli_query($link,"SELECT kondisi FROM lampu WHERE sender_id='$sender_id' LIMIT 1");
    $rowone = mysqli_fetch_assoc($resultone);
   //echo $rowone['kondisi'];
     if($kondisi != $rowone['kondisi']){
          $sql = "UPDATE lampu SET kondisi='$kondisi',otomatis='$otomatis',sensor_cahaya='$ldr',created_at='$time' WHERE sender_id='$sender_id'";
          if(mysqli_query($link, $sql)){
              $apiToken = "5425621140:AAFeRpdh4OibtGpjQDYYZ4_jdVw053Jpqxs";
              $kata = "Lampu rumah bagian depan sudah ".$kondisi."\n pada :".$time." \n Tingkat intensitas cahaya :".$ldr." silahkan balas nonotomatis jika lampu akan dinyalakan secara manual atau sesuai perintah anda untuk info perintah lain silahkan balas info";
              $rpl = str_replace(array("\n"), "\xA" , $kata);
              $data = [
                'chat_id' => $row["sender_id"],
                'text' => $rpl,
              ];
              $response = file_get_contents("https://api.telegram.org/bot$apiToken/sendMessage?".http_build_query($data));
          }
           mysqli_close($link);
        }
      }
// Close connection
mysqli_close($conn);
?>
