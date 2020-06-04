<?php

	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query_get_stations = $conn->prepare(
		"SELECT
			`station_id`, `location_id`, `temperature`, `humidity`, `pressure`, `wind_speed`, `wind_direction_id`, `precipitation_id`
		FROM `stations`;");

	$query_get_wind_directions = $conn->prepare(
		"SELECT `wind_direction_id`, `wind_direction`
		 FROM `wind_directions`;");

	$query_get_locations = $conn->prepare(
		"SELECT `location_id`, `location_name`
		 FROM `locations`;");

	$query_get_precipitations = $conn->prepare(
		"SELECT `precipitation_id`, `precipitation`
		FROM `precipitations`;");

	$success = $query_get_stations->execute();
	$query_get_locations->execute();
	$query_get_precipitations->execute();
	$query_get_wind_directions->execute();

	$locations = $query_get_locations->fetchAll(PDO::FETCH_KEY_PAIR);
	$precipitations = $query_get_precipitations->fetchAll(PDO::FETCH_KEY_PAIR);
	$wind_directions = $query_get_wind_directions->fetchAll(PDO::FETCH_KEY_PAIR);

	if (!$success) {
		$response->code = 1;
		$response->msg = "Непредвиденная ошибка на сервере. Не удалось получить список станций.\n";
		print_r(json_encode($response));
		exit();
	}

	$stationCount = $query_get_stations->rowCount();
	if ($stationCount == 0) {
		$response->code = 2;
		$response->msg = "Не удалось получить список станций\n";
		print_r(json_encode($response));
		exit();
	}

	$q = $conn->prepare(
		"select  stations.station_id, max(time)
		from indications_history
		join stations using(station_id)
		group by station_id");
	$q->execute();

	$times = $q->fetchAll(PDO::FETCH_KEY_PAIR);

	$stations = array();
	for ($i = 0; $i < $stationCount; $i++) {
		$row = $query_get_stations->fetch(PDO::FETCH_ASSOC);
		$station = $row;

		if (is_null($station['precipitation_id'])) {
			$station['precipitation'] = NULL;
		} else {
			$station['precipitation'] = $precipitations[$station['precipitation_id']];
		}

		if (is_null($station['wind_direction_id'])) {
			$station['wind_direction'] = NULL;
		} else {
			$station['wind_direction'] = $wind_directions[$station['wind_direction_id']];
		}

		if (isset($times[$i + 1])) {
			$station['time'] = $times[$i + 1];
		}

		$station['location'] = $locations[$station['location_id']];

		// print_r($station);
		$stations[] = $station;
	}

	// $query = $conn->prepare(
	// 	"SELECT `time`
	// 	FROM indications_history
	// 	join stations
	// 	where stations.station_id = :id
	// 	ORDER BY `time` DESC limit 1;");

	// $query->bindValue(':id',

	$response->stationCount = $stationCount;
	$response->stations = $stations;
	$response->status = 'success';

	print_r(json_encode($response, JSON_UNESCAPED_UNICODE));
?>
