<?php
	ini_set('display_errors', 1);
	include_once 'mysql_connection.php';

	session_start();

	$conn = mysql_connect();

	$login = $_POST['login'];
	$password = $_POST['password'];

	/* Пустой объекто для ответа */
	$response = new class{};
	/* По умолчанию статус ошибка*/
	$response->status = "failure";

	/* Достаем из бд, данные пользователя */
	$query_login = $conn->prepare(
		"SELECT `login`, `password`, `group_name`, `group_level`
		FROM `users`
		JOIN `groups` USING(group_id)
		WHERE `login`=:login;");

	/* Привязываем логин к запросу*/
	$query_login->bindValue(':login', $login);

	/* выполняем запрос */
	$succes = $query_login->execute();

	if (!$succes) {
		$response->code = 1;
		$response->msg = "Непредвиденная ошибка на сервере\n";
		print_r(json_encode($response));
		exit();
	}

	/* Если нет рещультатов, значит пользователь не найден*/
	$countRow = $query_login->rowCount();
	if ($countRow == 0) {
		$response->code = 2;
		$response->msg = "Пользователь не найден\n";
		print_r(json_encode($response));
		exit();
	}

	// записываем результат запроса в ассоциативный массив
	$result_row = $query_login->fetch(PDO::FETCH_ASSOC);

	/* проверка пароля */
	if (!password_verify($password, $result_row['password'])) {
		$response->code = 3;
		$response->msg = "Неверный логин или пароль\n";
		print_r(json_encode($response));
		exit();
	}

	$uid = uniqid();

	/* Записываем данные для ответа*/
	$response->status = 'success';
	$response->login = $result_row['login'];
	$response->uid = $uid;

	$_SESSION['login'] = $response->login;
	$_SESSION['uid'] = $uid;

	$file = fopen("login", "w");
	fwrite($file, print_r($_POST, true) . print_r($response, true));
	/* отправлям ответ */
	print_r(json_encode($response));
?>
