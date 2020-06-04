$(document).ready(function () {
	/* Получаем местоположение из адресной строки*/
	var url = new URL(window.location.href);
	var c = url.searchParams.get("location");
	console.log(c);
	console.log(document.getElementById('forecast_table').innerHTML);
	get_indications(c);
});

function get_indications(location) {
	if (!location) {
		location = 'Москва';
	}

	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			console.log(this.responseText);
			var response = JSON.parse(this.responseText);
			if (response.status == 'success') {
				document.getElementById('forecast_table').innerHTML = response.html;
			}
		}
	}

	xhttp.open("GET", "indications.php?location="+location);

	xhttp.send();
}
