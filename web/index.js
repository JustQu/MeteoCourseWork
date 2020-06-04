$(document).ready(function() {
	var xhttp = new XMLHttpRequest();
	load_cities();
	is_loged_in();

	load_indications();
	load_favourite_cities()
	console.log(getCookie("login"));
	is_favourite();
});

$(".fa.fa-star").click(function () {
	console.log("here");
	$(this).toggleClass("checked");

	toggle_favourite_city(document.getElementById('location_name').innerText);
});

//add or remove favourite city from cookies
function toggle_favourite_city(city) {
	if (!cookieExist('cities')) {
		setCookie('cities', city, 365);
		return;
	}
	var cities = getCookie('cities');

	cities = cities.split(',');

	var new_cities = "";

	var flag = false;
	for (var i = 0; i < cities.length; i++) {
		if (cities[i] == city) {
			flag = true;
			continue;
		}
		new_cities += cities[i] + ',';
	}
	if (flag) {
		new_cities = new_cities.slice(0, -1);
		setCookie('cities', new_cities);
		remove_favourite_city(city);
		return;
	}

	add_favourite_city(city);
	new_cities += city;
	setCookie('cities', new_cities);
}

//add city in favourite section
function add_favourite_city(city) {
	var fav = document.getElementById('favourite');
	var html = document.createElement("A");
	html.setAttribute('href', '#');
	html.innerText = city;
	html.classList.add('anodec');
	html.classList.add('favcity');
	html.setAttribute('onclick', 'press_favourite(this);');
	html.setAttribute('id', city);
	fav.appendChild(html);
}

function remove_favourite_city(city) {
	$('#' + city).remove();
}

function load_favourite_cities() {
	if (!cookieExist('cities')) {
		document.getElementById('favourite').innerHTML="";
		return;
	}

	var cities = getCookie('cities');

	cities = cities.split(',');

	for (var i = 0; i < cities.length; i++) {
		add_favourite_city(cities[i]);
	}
}

function is_favourite() {
	if (!cookieExist('cities')) {
		return ;
	}
	var city = document.getElementById('location_name').innerText;

	var star = document.getElementById('star');

	cities = getCookie('cities').split(',');

	for (var i = 0; i < cities.length; i++) {
		console.log("is fav");
		if (city == cities[i]) {
			console.log("fav");
			star.classList.add('checked');
			return ;
		}
	}
	star.classList.remove('checked');
}

function press_favourite(fav) {
	var name = document.getElementById('location_name');
	name.innerText = fav.innerText;
	load_indications();
}

function load_cities() {
	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function () {
		if (xhttp.readyState == 4 && xhttp.status == 200) {
			var response = JSON.parse(this.responseText);
			if (response.status === "success") {
				console.log("2");

				var select = document.getElementById('citychoice');
				for (var i = 0; i < response.num; i++) {
					var option = document.createElement("option");
					option.value = i;
					option.text = response.cities[i];
					select.add(option, null);
				}
			} else {
				console.log("Error");
			}
		}
	}

	xhttp.open("GET", "load_cities.php", true);

	xhttp.send("");
}

//Проверяем правильность авторизованного пользователя пользователя
function is_loged_in() {
	if (!cookieExist('login')
			|| !cookieExist('uid')) {
		console.log("cookies do not exist");
		return ;
	}

	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function () {
		if (xhttp.readyState == 4 && xhttp.status == 200) {
			var response = JSON.parse(this.responseText);
			if (response.status === "success") {
				console.log("1");

				var dropdown = document.getElementById('login_menu');
				dropdown.style.display='inline-block';

				var login_button = document.getElementById('login_button');
				login_button.style.display="none";

				document.getElementById('login').innerText=getCookie('login');
			} else {
				console.log('delete cookies');
				eraseCookie('login');
				eraseCookie('uid');
			}
		}
	}

	var login = getCookie('login');
	var uid = getCookie('uid');

	xhttp.open("POST", "is_loged_in.php", true);

	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

	xhttp.send("login=" + login
		+ "&uid=" + uid);
}

function set_city() {

	document.getElementById('location_name').innerText=document.getElementById('citychoice').selectedOptions[0].innerText;
	is_favourite();
	load_indications();
}

const days = ['Вс', 'Пн', 'Вторник', 'Среда', 'Чт', 'Пт', 'Сб'];
const months = ['Янаваря', 'Февраля', 'Марта', 'Апреля', 'Мая', 'Июня', 'Июля', 'Августа', 'Сентября', 'Октября', 'Ноября'];
function load_indications() {
	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function () {
		if (xhttp.readyState == 4 && xhttp.status == 200) {
			var response = JSON.parse(this.responseText);
			console.log(response);
			if (response.status === "success") {
				console.log("3");

				var d = new Date(response.indications.time)
				document.getElementById('time').innerText = d.getHours() + ":" + d.getMinutes();

				document.getElementById('timeB').innerText = days[d.getDay()] + "," + d.getDay() + " " + months[d.getMonth()];

				document.getElementById('temperature').innerText = (response.indications.temperature>0?"+":"") + response.indications.temperature;
				document.getElementById('temperatureB').innerText = (response.indications.temperature > 0 ? "+" : "") + response.indications.temperature;

				document.getElementById('humidity').innerText = response.indications.humidity;

				document.getElementById('pressure').innerText = response.indications.pressure;

				document.getElementById('wind_speed').innerText = response.indications.wind_speed;

				document.getElementById('wind_direction').innerText = response.indications.wind_direction;

				document.getElementById('precipitations').innerText = response.indications.precipitation;
			} else {
				console.log('err');

				document.getElementById('time').innerText = "--:--";

				document.getElementById('timeB').innerText = "";

				document.getElementById('temperature').innerText = "Нет данных";
				document.getElementById('temperatureB').innerText = "Нет данных";

				document.getElementById('humidity').innerText = "Нет данных";

				document.getElementById('pressure').innerText = "Нет данных";

				document.getElementById('wind_speed').innerText = "Нет данных";

				document.getElementById('wind_direction').innerText = "Нет данных";

				document.getElementById('precipitations').innerText = "Нет данных";
			}
		}
	}

	xhttp.open("POST", "load_indications.php", true);

	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

	console.log(document.getElementById('location_name').textContent);
	xhttp.send("location="
		+ document.getElementById('location_name').textContent);
}

function show_indications() {
	window.location = "/indications?location="+document.getElementById('location_name').innerText;
}

function getCookie(name) {
	var cookies = document.cookie.split(';');

	for (var i = 0; i < cookies.length; i++) {
		var element = cookies[i].trim().split('=');
		if (element[0] == name) {
			return element[1];
		}
	}
}

function setCookie(name, value, days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
		var expires = "; expires=" + date.toGMTString();
	}
	else var expires = "";
	document.cookie = name + "=" + value + expires + "; path=/";
}

function cookieExist(name) {
	var a = getCookie(name);
	if (a == undefined || a === "")
		return false;
	else
		return true;
}

function eraseCookie(name) {
	setCookie(name, "", -100);
}

function logout() {
	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function() {
		if (this.readyState == 4) {
			document.getElementById('login_menu').style.display="none";
			document.getElementById('login_button').style.display="inline-block";
		}
	}

	eraseCookie('login');
	eraseCookie('uid');
	xhttp.open("POST", "logout.php");

	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
	xhttp.send("");
}
