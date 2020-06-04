<?php

	include_once "mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"SELECT `location_name`
		FROM `locations`;"
	);

	$success = $query->execute();

	if (!$success) {
		exit(json_encode($response));
	}

	$num = $query->rowCount();
	if($num == 0) {
		$response->msg = 'Нет данных';
		exit(json_encode($response));
	}

	$cities = $query->fetchAll(PDO::FETCH_COLUMN);

	$response->cities = $cities;
	$response->num = $num;
	$response->status = 'success';
	echo json_encode($response);
?>
