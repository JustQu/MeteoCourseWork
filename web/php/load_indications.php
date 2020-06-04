<?php
	include_once "mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"SELECT h.time,
				h.temperature,
				h.pressure,
				h.humidity,
				h.wind_speed,
				wind_direction,
				precipitation
		FROM indications_history h
			JOIN stations USING(station_id)
			JOIN locations using(location_id)
			JOIN wind_directions w
				on h.wind_direction_id=w.wind_direction_id
			JOIN precipitations p
				on h.precipitation_id=p.precipitation_id
		WHERE location_name=:location
		ORDER by time DESC
		LIMIT 1;"
	);

	$location = $_POST['location'];

	if (!$location)
		$location = 'Москва';

	$query->bindValue(':location', $location);

	$success = $query->execute();

	if (!$success) {
		exit(json_encode($response));
	}

	if($query->rowCount() == 0) {
		$response->msg = 'Нет данных';
		exit(json_encode($response));
	}

	$indications = $query->fetch(PDO::FETCH_ASSOC);

	$response->indications = $indications;
	$response->status = "success";

	echo json_encode($response);
?>
