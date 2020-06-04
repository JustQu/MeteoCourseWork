<?php

	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"DELETE FROM `users`
		WHERE login=:login;"
	);

	$query->bindValue(':login', $_POST['login']);

	$status = $query->execute();

	if (!$status) {
		echo json_encode($response);
		exit();
	}

	$num = $query->rowCount();

	if ($num == 0) {
		$response->msg = "Пользователь не найден";
		echo json_encode($response);
		exit();
	}

	$response->login = $_POST['login'];
	$response->status='success';

	echo json_encode($response);
?>
