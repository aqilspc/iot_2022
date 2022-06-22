<?php
$sender_id = $_POST['sender_id'];
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

$sqlcek = "SELECT * FROM lampu WHERE sender_id=".$sender_id."";
$result = $conn->query($sqlcek);
$bisa = "bisa";
if ($result->num_rows <= 0) {

        $sql = "INSERT INTO lampu (sender_id)
        VALUES ('".$sender_id."')";

        if ($conn->query($sql) === TRUE) {
                echo "New record created successfully";
        } else {
                echo "Error: " . $sql . "<br>" . $conn->error;
        }
        $conn->close();
}else{
$bisa = "exist";
mysqli_close($conn);
}
echo json_encode($bisa);
?>