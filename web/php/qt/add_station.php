<?php
	include_once '../mysql_connection.php';

	$conn = mysql_connect();

	$location = $_POST['location'];

	// $location = 'Москва';

	$response = new class{};

	$query_add_station = $conn->prepare(
		"INSERT INTO `stations` (`location_id`)
		 SELECT `location_id`
		 FROM `locations`
		 WHERE location_name = :location");

	$query_add_station->bindParam(':location', $location);

	$success = $query_add_station->execute();

	if (!$success) {
		echo json_encode($response);
		exit();
	}

	$id =$conn->lastInsertId();

	$query_get_station = $conn->prepare(
		"SELECT
			`station_id`, `location_name` as `location`, `temperature`, `humidity`, `pressure`, `wind_speed`, `wind_direction_id`, `precipitation_id`
		FROM `stations`
		JOIN `locations` USING(location_id)
		WHERE station_id = :id;");

	$query_get_station->bindValue(':id', $id);

	$query_get_station->execute();

	$station = $query_get_station->fetch(PDO::FETCH_ASSOC);

	$response->station = $station;
	$response->status = "success";

	echo json_encode($response);
?>
