$(document).ready(function() {


})

is_loged_in();

//Проверяем правильность авторизованного пользователя пользователя
function is_loged_in() {
	if (!cookieExist('login')
		|| !cookieExist('uid')) {
		console.log("cookies do not exist");
		return;
	}

	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function () {
		if (xhttp.readyState == 4 && xhttp.status == 200) {
			var response = JSON.parse(this.responseText);
			if (response.status === "success") {
				console.log("1");

				document.getElementById('user_login').innerText = getCookie('login');
			} else {
				console.log('delete cookies');
				eraseCookie('login');
				eraseCookie('uid');
				window.location = "/sign_in";
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

function delete_account() {
	if (!confirm("Вы уверены?")) {
		return ;
	}

	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200) {
			var response = JSON.parse(this.responseText);
			console.log(response);
			if (response.status === 'success') {
				eraseCookie('login');
				eraseCookie('uid');
				window.location ="/";
			} else {
				alert("Произошла ошибка");
			}
		}
	}

	xhttp.open("POST", "delete_user.php");

	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

	xhttp.send("login="+getCookie('login') +
				"&uid="+getCookie('uid'));
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
