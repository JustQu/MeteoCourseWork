<?php

	include_once "../mysql_connection.php";

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"UPDATE IGNORE `users`
		SET `group_id` = (SELECT `group_id`
				FROM `groups`
				WHERE `group_name`=:name
			)
		 WHERE `login`=:login;"
	);

	if ($_POST['role'] == 'Администратор') {
		$name = 'admin';
	} else if ($_POST['role'] == 'Модератор') {
		$name = 'moderator';
	} else if ($_POST['role'] == 'Пользователь') {
		$name = 'user';
	}

	// $name = 'admin'
	$file = fopen("pepa", "w");
	fwrite($file, print_r($_POST, true) . $name);

	$query->bindValue(':name', $name);
	$query->bindValue(':login', $_POST['login']);

	$success = $query->execute();

	if (!$success) {
		fwrite($file, 2);
		echo json_encode($response);
		exit();
	}

	$rowCount = $query->rowCount();

	if ($rowCount == 0) {
		fwrite($file, $rowCount);
		echo json_encode($response);
		exit();
	}

	$response->status = 'success';

	echo json_encode($response);
?>
