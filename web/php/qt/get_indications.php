<?php

	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$station_id = $_POST['station_id'];
	// $station_id = 2;

	$query = $conn->prepare(
		"SELECT `time`, `temperature`, `humidity`, `pressure`, `wind_speed`
		FROM `indications_history`
		WHERE `station_id`=:station_id
		ORDER BY `time` DESC
		LIMIT 10;"
	);

	$query->bindValue(':station_id', $station_id);

	$success = $query->execute();

	if (!$success) {
		echo json_encode($response);
		exit();
	}

	$n = $query->rowCount();

	if ($n == 0) {
		echo json_encode($response);
		exit();
	}

	$indications = $query->fetchAll(PDO::FETCH_ASSOC);
	$response->status='success';
	$response->indications = $indications;
	$response->num = $n;

	print_r(json_encode($response));
	// print_r($indications);
?>
