function func(form) {
	// form.action="register.php"
	var errorField = document.getElementById("ErrorField");

	var xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function() {
		// 4: request finished and response is ready
		// 200: "OK"
		if (xhttp.readyState == 4 && xhttp.status == 200){
			var response = JSON.parse(this.responseText);
			if (response.status == 'success') {
				setCookie('login', response.login, 30);
				setCookie('uid', response.uid, 30);
				window.location.href = "/"
			}
		}
		errorField.innerText = this.responseText;
	};

	var login = form.login.value;
	var password = form.password.value;

	//инициализируем соединения
	xhttp.open("POST", "register.php", true);
	//устанвливаем заголовок
	xhttp.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=utf-8");
	//посылаем запрос
	xhttp.send("login=" + login + "&password=" + password + "&submit=ok");
}

// Поле ввода пароля
const password_input = document.getElementById('password');
is_valid_password = false;

// Поле ввода логина
const login_input = document.getElementById('login');
is_valid_login = false;

// Поле подтверждения пароля
const password_repeat_input = document.getElementById('password-repeat');
is_valid_password_repeat = false;

// Кнопка подтверждения регстрации
const register_button = document.getElementById('register_button');

password_input.addEventListener('keyup', function(event) {
	is_valid_password = check_password(password_input);
	if (is_valid_login
		&& is_valid_password
		&& is_valid_password_repeat) {
		register_button.disabled = false;
	} else {
		register_button.disabled = true;
	}
})

// Проверяем логин на корректность
login_input.addEventListener('keyup', function(event) {
	is_valid_login = check_login(login_input);

	if (is_valid_login
		&& is_valid_password
		&& is_valid_password_repeat) {
		register_button.disabled = false;
	} else {
		register_button.disabled = true;
	}
})

password_repeat_input.addEventListener('keyup', function(event) {
	is_valid_password_repeat = check_password_repeat(password_input, password_repeat_input);

	if (is_valid_login
		&& is_valid_password
		&& is_valid_password_repeat) {
		register_button.disabled = false;
	} else {
		register_button.disabled = true;
	}
})

// Функция проверки введенного логина на правильность
function check_login(login_input){
	var text = login_input.value;
	var error_field = document.getElementById("login_error");

	error_field.innerHTML = "";

	// латинские буквы верхнего и нижнего регистра, цифра. Длина не менее 3
	var login_pattern = new RegExp(/[a-zA-Z0-9\_]{3,}/);

	if (!login_pattern.test(text)) {
		login_input.setCustomValidity("Invalid field.");
		error_field.innerHTML = "Логин должен быть не менее 4 символов и может содержать в себе цифры, буквы латинского алфавита и нижнее подчеркивание.<br>";
		form.login.setCustomValidity("Invalid field.");
		return false;
	} else {
		form.login.setCustomValidity("");
		return true;
	}
}

function check_password(password_input) {
	var text = password_input.value;
	var error_field = document.getElementById("password_error");

	error_field.innerHTML = "";

	// Буквы латинского алфавита верхнего и нижнего регистра;
	// цифры
	// Спец. символы
	// длина не менее 6 символов
	var password_pattern = new RegExp(/[A-Za-z\d!@.#$%^&*()/*\-\+\=\\,`'":;\[\]\{\}\|\<\>~]{6,}/);

	if (!password_pattern.test(text)){
		password_input.setCustomValidity("Invalid field.");
		error_field.innerHTML = "Пароль должен быть не менее 6 символов и может содержать в себе цифры, буквы латинского алфавита и специальные символы.<br>";
		return false;
	} else {
		password_input.setCustomValidity("");
		return true;
	}
}

function check_password_repeat(password_input, password_repeat_input) {
	var error_field = document.getElementById('password-repeat_error');

	error_field.innerHTML = "";

	if (password_input.value !== password_repeat_input.value) {
		password_repeat_input.setCustomValidity("Пароли не совпадают.");
		error_field.innerHTML = "Пароли не совпадают.<br>";
		return false;
	} else {
		password_repeat_input.setCustomValidity("");
		return true;
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
