<?php
	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$location = $_POST['location'];

	$response = new class{};
	$response->status = 'failure';

	$query_add_location = $conn->prepare(
		"INSERT IGNORE INTO `locations`
				(`location_name`)
		 SELECT :location;"
	);

	$query_add_location->bindValue(':location', $location);

	$succes = $query_add_location->execute();

	if (!$succes) {
		echo(json_encode($response));
		exit();
	}

	$rowCount = $query_add_location->rowCount();
	if ($rowCount == 0) {
		echo(json_encode($response));
		exit();
	}

	$response->status = 'success';
	$response->location = $location;
	echo (json_encode($response));
?>
