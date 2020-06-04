<?php
	session_start();

	$response = new class{};
	$response->status = 'failure';

	if ($_POST['login'] != $_SESSION['login']
			|| $_POST['uid'] != $_SESSION['uid']){
		$_SESSION['login']=NULL;
		$_SESSION['uid']=NULL;
		exit(json_encode($response));
	}
	$response->status = 'success';
	echo(json_encode($response));
?>
