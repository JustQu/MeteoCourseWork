#!/usr/bin/php
<?php
    if ($argc != 2) {
        echo "Incorrect Parametres";
        exit;
    }

	if (preg_match("/^([+-]?(([1-9][0-9]*)|(0))([.,][0-9]+)?)[\s]*([\+\-\/\*%])[\s]*([+-]?(([1-9][0-9]*)|(0))([.,][0-9]+)?)$/", $argv[1], $m)) {
		print_r($m);
		eval("echo $argv[1];");
		echo "\n";
    } else {
		print_r($m);
        echo "Syntax Error\n";
    }
?>
