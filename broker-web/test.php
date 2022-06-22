<?php
$sender_id = "1883724643";
$link = mysqli_connect("localhost", "root", "", "iot_2022");
// Check connection
if($link === false){
    die("ERROR: Could not connect. " . mysqli_connect_error());
}
$result = mysqli_query($link,"SELECT kondisi FROM lampu WHERE sender_id='$sender_id' LIMIT 1");
$rowone = mysqli_fetch_assoc($result);
echo $rowone['kondisi'];
?>
