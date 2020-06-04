<?php
	/*
	** Запрашиваем значения, нужные для десктопного приложения,
	** и отправляем, упоковав в json
	*/
	include_once '../mysql_connection.php';

	$response = new class{};
	$response->status = 'failure';

	$conn = mysql_connect();

	/* Запришиваем все регионы в бд */
	$query_locations = $conn->prepare("SELECT location_name FROM `locations`;");
	if (!($query_locations->execute())){
		echo "Error\n";
	}
	$locations = $query_locations->fetchAll(PDO::FETCH_COLUMN);
	$response->locations = $locations;
	// $response->locations = json_encode($locations, JSON_UNESCAPED_UNICODE);

	// print_r($response->locations);

	/* Запрашиваем все направления ветров в бд */
	$query_wind_directions = $conn->prepare("SELECT wind_direction FROM `wind_directions`;");
	if (!($query_wind_directions->execute())) {
		echo "Error\n";
	}
	$wind_directions = $query_wind_directions->fetchAll(PDO::FETCH_COLUMN);
	$response->wind_directions = $wind_directions;
	// $wind_directions_json = json_encode($wind_directions, JSON_UNESCAPED_UNICODE);

	// print_r($wind_directions_json);

	/* запрашиваем все виды атмосфрных осадков */
	$query_precipitation = $conn->prepare('SELECT precipitation FROM  `precipitations`;');
	if (!($query_precipitation->execute())) {
		echo "Error\n";
	}
	$precipitations = $query_precipitation->fetchAll(PDO::FETCH_COLUMN);
	$response->precipitations = $precipitations;
	// $precepitations_json = json_encode($precepitations, JSON_UNESCAPED_UNICODE);

	// print_r($precepitations_json);
	$response->status = 'success';
	print_r(json_encode($response, JSON_UNESCAPED_UNICODE));
?>
