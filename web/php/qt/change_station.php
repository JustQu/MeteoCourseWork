<?php
	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new Class{};
	$response->status = 'failure';

	$station_id = $_POST['station_id'];
	$new_location = $_POST['new_location'];

	// $station_id = 1;
	// // $new_location = 'Москва';
	// $new_location = 'Омск';

	$query_change_location = $conn->prepare(
		"UPDATE `stations`
		 SET location_id=
		 	(SELECT location_id
			 FROM locations
			 WHERE location_name=:location)
		WHERE station_id=:station_id;");

	$query_change_location->bindValue(':location', $new_location);
	$query_change_location->bindValue(':station_id', $station_id);

	$success = $query_change_location->execute();

	if (!$success) {
		echo 1;
		echo(json_encode($response));
		exit();
	}

	$rowCount = $query_change_location->rowCount();
	if (!$rowCount) {
		echo 2;
		echo(json_encode($response));
		exit();
	}

	$query_get_location = $conn->prepare(
		"SELECT location_name
		FROM locations JOIN stations USING(location_id)
		WHERE station_id=:station_id");

	$query_get_location->bindValue(":station_id", $station_id);

	$succes = $query_get_location->execute();

	if (!$success) {
		echo 3;
		echo(json_encode($response));
		exit();
	}

	$rowCount = $query_get_location->rowCount();
	if (!$rowCount) {
		echo 4;
		echo(json_encode($response));
		exit();
	}

	$location = $query_get_location->fetchAll(PDO::FETCH_COLUMN);
	$response->location = $location[0];
	$response->status = 'success';

	echo(json_encode($response));
?>
