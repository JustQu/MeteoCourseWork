<?php
	include_once "mysql_connection.php";

	session_start();

	$conn = mysql_connect();

	$response = new class{};
	$response->status = 'failure';

	$query = $conn->prepare(
		"DELETE FROM `users`
		WHERE login=:login;"
	);

	if ($_POST['login'] != $_SESSION['login']) {
		exit(json_encode($response));
	}

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

	$response->status='success';
	$_SESSION['login']=NULL;
	$_SESSION['uid']=NULL;

	echo json_encode($response);
?>
