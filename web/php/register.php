<?php
	ini_set('display_errors', 1);
	include 'mysql_connection.php';

	session_start();

	/* Подключаемся к базе данных*/
	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	/* проверяем переданные данные*/
	if (!(isset($_POST['login'])
		  && isset($_POST['password'])
		  && isset($_POST['submit']))){
		echo(json_encode($response));
		exit();
	}

	try {
		/* формируем запрос к бд */
		$query_add_user = $conn->prepare(
			"INSERT INTO `users` (`login`, `password`, `group_id`)
				SELECT :login, :password, `group_id`
				FROM `groups` WHERE `group_name` = 'user';");

		$query_add_user->bindParam(':login', $login);
		$query_add_user->bindParam(':password', $hash);

		$login  = $_POST['login'];
		$hash = password_hash($_POST['password'], PASSWORD_DEFAULT);

		/* выполняем запрос */
		$result = $query_add_user->execute();

		/* проверяем результат выполнения запроса */
		if (!$result) {
			echo(json_encode($response));
			exit();
		}
	} catch(PDOException $e) {
		echo(json_encode($response));
		exit();
	}

	$uid = uniqid();

	$_SESSION['login'] = $login;
	$_SESSION['uid'] = $uid;

	$response->status='success';
	$response->login = $login;
	$response->uid = $uid;

	echo(json_encode($response));
?>
