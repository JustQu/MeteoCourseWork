function setCookie(name, value,days) {
	if (days) {
		var date = new Date();
		date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
		var expires = "; expires=" + date.toGMTString();
	}
	else var expires = "";
	document.cookie = name + "=" + value + expires + "; path=/";
}

function	sign_in(form) {
	var xhttp = new XMLHttpRequest();

	xhttp.onreadystatechange = function () {
		console.log(this.responseText);

		if (xhttp.readyState == 4 && xhttp.status == 200) {
				var response = JSON.parse(this.responseText);
				if (response.status == 'success'){
					setCookie("login", response.login);
					setCookie("uid", response.uid);
					// console.log("tut");
					window.location.href="/";
				} else {
					if (response.status == 'failure'){
						alert("Ошибка");
					} else {
						alert("Непредвиденная ошибка");
					}
				}
			// } catch(err) {
			// 	alert("Непредвиденная ошибка.");
			// }
		}
	}

	var login = form.login.value;
	var password = form.password.value;

	xhttp.open("POST", "login.php");

	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");

	xhttp.send("login=" + login
				+"&password=" + password);
}

const password_input = document.getElementById('password');
is_valid_password = false;

password_input.addEventListener('keyup', function (event) {
	is_valid_password = check_password(password_input);
	if (is_valid_password) {
		register_button.disabled = false;
	} else {
		console.log("here");
		register_button.disabled = true;
	}
});

function check_password(password_input) {
	var text = password_input.value;
	var error_field = document.getElementById("password_error");

	error_field.innerHTML = "";

	// Буквы латинского алфавита верхнего и нижнего регистра;
	// цифры
	// Спец. символы
	// длина не менее 6 символов
	var password_pattern = new RegExp(/[A-Za-z\d!@.#$%^&*()/*\-\+\=\\,`'":;\[\]\{\}\|\<\>~]{5,}/);

	if (!password_pattern.test(text)) {
		password_input.setCustomValidity("Invalid field.");
		error_field.innerHTML = "Пароль должен быть не менее 6 символов и может содержать в себе цифры, буквы латинского алфавита и специальные символы.<br>";
		return false;
	} else {
		password_input.setCustomValidity("");
		return true;
	}
}
