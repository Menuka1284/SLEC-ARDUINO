<?php

$conn = mysqli_connect("localhost", "root", "", "sensors");

if (!$conn) {
	die ("Connection error");
}
if (isset($_GET["temp"]) && isset($_GET["hum"]) && isset($_GET["smoke"]) && isset($_GET["fire"])) {
	$temp = $_GET["temp"];
	$hum = $_GET["hum"];
	$smoke = $_GET["smoke"];
	$fire = $_GET["fire"];

	$query = "INSERT INTO sensor (temperature, humidity, smoke, fire) VALUES('$temp', '$hum', '$smoke', '$fire')";
echo $query;
	if (mysqli_query($conn, $query)) {
		echo "Inserted";
	} else {
		echo "fail to insert";
	}
}

?>
