<?php

	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"DELETE FROM `stations`
		WHERE station_id=:station_id;"
	);

	$query->bindValue(':station_id', $_POST['station_id']);

	$success = $query->execute();

	if (!$success) {
		echo json_encode($response);
		exit();
	}

	$response->status = 'success';
	echo json_encode($response);
?>
