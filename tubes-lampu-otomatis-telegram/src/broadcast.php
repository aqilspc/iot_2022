<?php
date_default_timezone_set("Asia/Jakarta");
$time = date('Y-m-d H:i:s');
$username = $_GET['username'];
$cast = $_GET['cast'];
$sender_id = $_GET['sender_id'];

$kata = "";
if($cast == 'urip'){
  $kata = "Lampu rumah bagian depan sudah di hidupkan \n oleh ".$username."";
}elseif ($cast == 'mati') {
  $kata = "Lampu rumah bagian depan sudah di matikan \n oleh ".$username."";
}elseif ($cast == 'otomatis') {
  $kata = "Saklar lampu otomatis di aktifkan! \n oleh ".$username."";
}else{
  $kata = "Saklar lampu otomatis di non aktifkan! \n oleh ".$username."";
}
$rpl = str_replace(array("\n"), "\xA" , $kata);

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

$sqlget = "SELECT sender_id FROM lampu";
$result = $conn->query($sqlget);
//echo $result;
  while($row = $result->fetch_assoc()) {
     if($sender_id != $row['sender_id']){
        $apiToken = "5425621140:AAFeRpdh4OibtGpjQDYYZ4_jdVw053Jpqxs";
        $data =[
              'chat_id' => $row["sender_id"],
              'text' => $rpl,
            ];
          $response = file_get_contents("https://api.telegram.org/bot$apiToken/sendMessage?".http_build_query($data));
        }
      }
mysqli_close($conn);
?>
