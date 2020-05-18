<?php 

include "connection.php";


if (isset($_GET['rfid'])) {
	
	$rfid = $_GET['rfid'];

	$query = "INSERT INTO rid (tag) values('$rfid')";
	if($conn->query($query))
	{
		echo "inserted";
	}

}


?>