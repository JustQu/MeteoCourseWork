<?php

	include_once "mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status='failure';

	$location = $_GET['location'];

	// print_r($_GET);
	// $location='Москва';

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
		LIMIT 20;"
	);

	$query->bindValue(':location', $location);

	$success = $query->execute();

	if (!$success) {
		exit(json_encode($response));
	}

	$num = $query->rowCount();
	if($num == 0) {
		$response->msg = 'Нет данных';
		exit(json_encode($response));
	}

	$response->num = $num;

	$html = "<tr id=\"title_row\">
				<th>Время</th>
				<th>Температура</th>
				<th>Влажность</th>
				<th>Давление</th>
				<th>Осадки</th>
				<th>Скорость ветра</th>
				<th>Направление</th>
			</tr>";

	foreach($query as $row) {
		// print_r($row);
		$html_table_row =
			"<tr class=\"forecast_row\">\n" .
				"<td>" . $row['time'] . "</td>\n".
				"<td>" . $row['temperature'] . "</td>\n".
				"<td>" . $row['humidity'] . "</td>\n".
				"<td>" . $row['pressure'] . "</td>\n".
				"<td>" . $row['precipitation'] . "</td>\n".
				"<td>" . $row['wind_speed'] . "</td>\n".
				"<td>" . $row['wind_direction'] . "</td>\n".
			"</tr>\n";
		$html .= $html_table_row;
	}

	$response->html = $html;
	$response->status = 'success';
	echo json_encode($response);
?>
