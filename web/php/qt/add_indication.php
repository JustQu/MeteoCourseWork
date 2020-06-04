<?php
	/*
	**
	*/

	include_once '../mysql_connection.php';

	//todo: проверить ключи

	/* местоположение станции */
	$station_id = $_POST['station_id'];
	$location = $_POST['location'];
	/* температура */
	$temperature = intval($_POST['temperature']);
	/* влажность */
	$humidity = intval($_POST['humidity']);
	/* атмосферное давление */
	$pressure = intval($_POST['pressure']);
	/* скорость ветра */
	$wind_speed = intval($_POST['wind_speed']);
	/* направление ветра */
	$wind_direction = $_POST['wind_direction'];
	/* Текущие осадки */
	$precipitation = $_POST['precipitation'];
	/* Время показаний */
	$time = $_POST['time'];

	/* подключаемся к бд */
	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	/* зАпрос на добавление показаний станции */
	$query_add = $conn->prepare(
		"INSERT INTO `indications_history`
			(`station_id`,
			`time`,
			`temperature`,
			`humidity`,
			`pressure`,
			`wind_speed`,
			`wind_direction_id`,
			`precipitation_id`)
		SELECT :station_id,
				:time,
				:temperature,
				:humidity,
				:pressure,
				:wind_speed,
				`wind_directions`.`wind_direction_id`,
				`precipitations`.`precipitation_id`
		FROM
			`wind_directions`, `precipitations`
		WHERE wind_direction=:wind_direction
			AND `precipitation`=:precipitation;");


/* 	$station_id = 2;
	$location = 'Москва';
	$temperature = 99;
	$humidity = 20;
	$pressure = 800;
	$wind_speed = 13;
	$wind_direction = 'северный';
	$precipitation = 'град'; */
	$time = date('Y-m-d H:i:s');

	$file = fopen("ind", "w");
	fwrite($file, $station_id . " " . $location . " " . $temperature . " " . $humidity . " " . $pressure . " " . $wind_speed
		. " " . $wind_direction . " " . $precipitation . "\n");

	/* связываем переменные с запросом*/
	$query_add->bindParam(':time', $time);
	$query_add->bindParam(':temperature', $temperature);
	$query_add->bindParam(':humidity', $humidity);
	$query_add->bindParam(':pressure', $pressure);
	$query_add->bindParam(':wind_speed', $wind_speed);
	$query_add->bindParam(':wind_direction', $wind_direction);
	$query_add->bindParam(':precipitation', $precipitation);
	$query_add->bindParam(':station_id', $station_id);

	/* Выполняем запрос */
	$success = $query_add->execute();

	/* проверяем успешность выполнения запроса */
	if (!$success) {
		echo json_encode($response);
		exit();
	}

	/* Сколько строк вставили */
	$result = $query_add->rowCount();
	if ($result == 0) { // если ни одной строки не вставили, значит ошибка в параметрах
		echo json_encode($response);
		exit();
	}

	$id = $conn->lastInsertId();

	fwrite($file, "id: " . $id . "\n");

	/* Получение показаний станции*/
	$query_get_indications = $conn->prepare(
		"SELECT
			 `station_id`, `time`, `temperature`, `humidity`, `pressure`, `wind_speed`, `wind_direction_id`, `precipitation_id`
		FROM `indications_history`
		WHERE indication_id = :id;");

	$query_get_indications->bindValue("id", $id);

	$query_get_indications->execute();

	$indications = $query_get_indications->fetch(PDO::FETCH_ASSOC);

	// fwrite($file, print_r($indications, true));

	/* запись показаний в станцию */
	$query_update_station = $conn->prepare(
		"UPDATE `stations`
		 SET `temperature`=:temperature,
			 `pressure`=:pressure,
			 `humidity`=:humidity,
			 `wind_speed`=:wind_speed,
			 `wind_direction_id`=:wind_direction_id,
			 `precipitation_id`=:precipitation_id
		WHERE
			`station_id` = :station_id");

	$query_update_station->bindValue(":temperature", $indications["temperature"]);
	$query_update_station->bindValue(":pressure", $indications["pressure"]);
	$query_update_station->bindValue(":humidity", $indications["humidity"]);
	$query_update_station->bindValue(":wind_speed", $indications["wind_speed"]);
	$query_update_station->bindValue(":wind_direction_id", $indications["wind_direction_id"]);
	$query_update_station->bindValue(":precipitation_id", $indications["precipitation_id"]);
	$query_update_station->bindValue(":station_id", $indications["station_id"]);

	$query_update_station->execute();

	$query_get_indications = $conn->prepare(
		"SELECT
			`station_id`,
			`location_name` as location,
			`time`,
			i.`temperature`,
			i.`humidity`,
			i.`pressure`,
			i.`wind_speed`,
			`wind_direction`,
			`precipitation`
		FROM `indications_history` i
		JOIN `wind_directions` USING(wind_direction_id)
		JOIN `precipitations` USING(precipitation_id)
		JOIN `stations` USING(station_id)
		JOIN `locations` USING(location_id)
		WHERE indication_id=:id");

	$query_get_indications->bindValue("id", $id);

	$query_get_indications->execute();

	$indications = $query_get_indications->fetch(PDO::FETCH_ASSOC);

	$response->station = $indications;
	$response->status = 'success';

	echo json_encode($response);
?>
