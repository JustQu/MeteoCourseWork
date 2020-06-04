<?php
	include 'defines.php';

	function mysql_connect(){
		include 'defines.php';
		try {
			$conn = new PDO("mysql:host=$db_host;
							dbname=$db_name",
							$db_login,
							$db_password);

			// set the PDO error mode to exception
			$conn->setAttribute(PDO::ATTR_ERRMODE, 	PDO::ERRMODE_EXCEPTION);
			// echo "Connected successfully";

			return $conn;
		} catch(PDOException $e) {
			echo "Ошибка подключения: " . $e->getMessage	();
		}
	}
?>
