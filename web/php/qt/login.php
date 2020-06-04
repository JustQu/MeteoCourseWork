<?php
	/*
	** Вход в приложение.
	** Ответ отсылается в формате json с полями
	** 	в случае ошибки:
	**		status -- со значение 'failure'
	**		code -- код ошбки
	**		msg -- сообщение ошикбки
	** 	в случаеу спеха:
	**		status -- значение 'success'
	**		login -- логин пользователя
	**		group -- название группы пользователя
	**		group_level -- уровень доступа группы пользователя
	*/
	include_once '../mysql_connection.php';

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

	/* Записываем данные для ответа*/
	$response = new class{};
	$response->status = 'success';
	$response->login = $result_row['login'];
	$response->group = $result_row['group_name'];
	$response->group_level = $result_row['group_level'];

	if ($response->group_level > 1) {
		$response->msg = "Недостаточно прав";
	}

	/* отправлям ответ */
	print_r(json_encode($response));
?>
