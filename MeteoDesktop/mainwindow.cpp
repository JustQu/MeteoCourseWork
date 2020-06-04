#include "mainwindow.h"
#include "./ui_mainwindow.h"

const QString defaultUrl = "http://localhost/qt/";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRegExp rx("[0-9,a-z]{3,}");
    QRegExp loginRegExp("[0-9a-zA-Z_]*");

    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    QRegExpValidator *login_validator = new QRegExpValidator(loginRegExp, this);

    /* Проверка ввода в поля */
    ui->login_input->setValidator(login_validator);
    ui->password_input->setValidator(validator);

    /* Сохраняем вкладку для управления перед тем как убрать */
    hiddenTab = ui->controlPanel;
    hiddenTabIndex = ui->tabWidget->indexOf(hiddenTab);
    hiddenTabLabel = ui->tabWidget->tabText(hiddenTabIndex);

    /* Убираем владку управления */
    ui->tabWidget->removeTab(1);

//    ui->selectUserGroupComboBox

    //Получить начальные значения
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this); //new mng
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &MainWindow::get_init_values);//finished function
    const QUrl url = QUrl(defaultUrl + "get_init_values.php"); //set url
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //post params
    QUrlQuery params;
    params.addQueryItem("client_id", "123");
    networkAccessManager->post(request, params.query().toUtf8());
    ui->selectUserGroupComboBox->addItem("Администратор");
    ui->selectUserGroupComboBox->addItem("Модератор");
    ui->selectUserGroupComboBox->addItem("Пользователь");

//    make_plot();

//    wGraphic = new QCustomPlot();
//    QLayout *layout = new QVBoxLayout();
//    layout->addWidget(wGraphic);

//    verticalLine = new QCPCurve(wGraphic->xAxis, wGraphic->yAxis);

//    QVector<double> x(2) , y(2);
//           x[0] = 0;
//           y[0] = -50;
//           x[1] = 0;
//           y[1] = 50;
////    wGraphic->add
////       wGraphic->addPlottable(verticalLine);   // Добавляем линию на полотно
//       verticalLine->setName("Vertical");      // Устанавливаем ей наименование
//       verticalLine->setData(x, y);            // И устанавливаем координаты

//       // создаём вектора для графика
//       QVector<double> x1(5) , y1(5);
//           x1[0] = -45;
//           y1[0] = -43;
//           x1[1] = 46;
//           y1[1] = 42;
//           x1[2] = -25;
//           y1[2] = -24;
//           x1[3] = -12;
//           y1[3] = 10;
//           x1[4] = 25;
//           y1[4] = 26;

//       // Добавляем график на полотно
//       wGraphic->addGraph(wGraphic->xAxis, wGraphic->yAxis);
//       wGraphic->graph(0)->setData(x1,y1);     // Устанавливаем координаты точек графика

//       // Инициализируем трассировщик
//       tracer = new QCPItemTracer(wGraphic);
//       tracer->setGraph(wGraphic->graph(0));   // Трассировщик будет работать с графиком

//       // Подписываем оси координат
//       wGraphic->xAxis->setLabel("x");
//       wGraphic->yAxis->setLabel("y");

//       // Устанавливаем максимальные и минимальные значения координат
//       wGraphic->xAxis->setRange(-50,50);
//       wGraphic->yAxis->setRange(-50,50);

//       // Отрисовываем содержимое полотна
//       wGraphic->replot();
}

void    MainWindow::go_to_station_info()
{
    Station *station = (Station *)sender();
    current_station = station;

    if (station->get_temperature() == ""){
        this->ui->stationTemperatureLabel->setText("Нет данных");
    } else {
        this->ui->stationTemperatureLabel->setText(station->get_temperature());
    }

    if (station->get_humidity() == ""){
        this->ui->stationHumidityLabel->setText("Нет данных");
    } else {
        this->ui->stationHumidityLabel->setText(station->get_humidity());
    }

    if (station->get_pressure() == ""){
        this->ui->stationPressureLabel->setText("Нет данных");
    } else {
        this->ui->stationPressureLabel->setText(station->get_pressure());
    }

    if (station->get_wind_speed() == ""){
        this->ui->stationWindSpeedLabel->setText("Нет данных");
    } else {
        this->ui->stationWindSpeedLabel->setText(station->get_wind_speed());
    }

    if (station->get_wind_direction() == ""){
        this->ui->stationWindDirectionLabel->setText("Нет данных");
    } else {
        this->ui->stationWindDirectionLabel->setText(station->get_wind_direction());
    }

    if (station->get_precipitation() == ""){
        this->ui->stationPrecipitationLabel->setText("Нет данных");
    } else {
        this->ui->stationPrecipitationLabel->setText(station->get_precipitation());
    }

    ui->dateTimeEdit->setDateTime(current_station->get_time());

    this->ui->stationLocationLabel->setText(station->get_location());
    this->ui->stationsPage->setCurrentIndex(1);
}

void    MainWindow::http_finished(QNetworkReply *reply)
{
    QByteArray replyData = reply->readAll();
    qDebug() << replyData;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData));
    QJsonObject jsonReply = jsonDoc.object();
    qDebug() << jsonReply["response"].toString();
}

//Получаем начальные значения
void    MainWindow::get_init_values(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    this->precipitations = QJsonArray(response["precipitations"].toArray());
    this->wind_directions = QJsonArray(response["wind_directions"].toArray());
    this->locations = QJsonArray(response["locations"].toArray());

    for (int i = 0; i < precipitations.size(); i++){
        ui->stationPrecipitationAddInput->addItem(precipitations[i].toString());
    }

    for (int i = 0; i < wind_directions.size(); i++) {
        ui->stationWindDirectionAddInput->addItem(wind_directions[i].toString());
    }

    for (int i = 0; i < locations.size(); i++) {
        ui->addStationLocationComboBox->addItem(locations[i].toString());
        ui->changeStationLocationComboBox->addItem(locations[i].toString());
    }

//    ui->stationLocationAddInput->setText();

    qDebug() << precipitations.size();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void    MainWindow::log_in_result(QNetworkReply *reply)
{
    QJsonParseError jsonError;
    QJsonObject response = QJsonDocument::fromJson(reply->readAll(), &jsonError).object();
    if (jsonError.error != QJsonParseError::NoError){
//        qDebug()<<"Error:" << jsonError.errorString();
    }
    qDebug() << response;
    if (response["status"].toString() == "success"
            && response["group_level"].toString().toInt() < 2) {
        ui->loginLabel->setText(response["login"].toString());
        ui->accessLevelLabel->setText(response["group"].toString());
        if (response["group_level"].toInt() == 0){
            ui->tabWidget->insertTab(hiddenTabIndex, hiddenTab, hiddenTabLabel);
        }
        this->setStyleSheet("color: rgb(0, 0, 0);");
        ui->controlPanelPages->setCurrentIndex(0);
        ui->pages->setCurrentIndex(1);
        ui->stationsPage->setCurrentIndex(0);
        this->load_stations();
        user = new User();
        user->set_login(response["login"].toString());
        user->set_level_access(response["group_level"].toString().toInt());
    } else {
        QMessageBox::critical(this, tr("Ошибка"), response["msg"].toString());
    }
}

//Загрузка станций из бд и добавление в список;
void    MainWindow::add_stations(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

    if (response["status"].toString() == "failure") {
        QMessageBox::critical(this, tr("Ошибка"), response["msg"].toString());
        return ;
    }
    qDebug() << response;
    QJsonArray jsonStations = response["stations"].toArray();

    Station *station;
    QLayout *layout = new QVBoxLayout();
    for (int i = 0; i < response["stationCount"].toInt(); i++){
        QJsonObject a = jsonStations[i].toObject();
        station = new Station(a);
        station->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
        stations.append(station);
        connect(station, &Station::clicked, this, &MainWindow::go_to_station_info);
        layout->addWidget(station);
    }
    spacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addItem(spacer);
    this->ui->scrollAreaWidgetContents->setLayout(layout);
    this->ui->scrollArea->setWidgetResizable(true);
    this->ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
}

//Загрузка станций из бд
void    MainWindow::load_stations()
{
        QNetworkAccessManager *mng = new QNetworkAccessManager();
        connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::add_stations);//finished function
        // задаем адресс запроса
        const QUrl url = QUrl(defaultUrl + "load_stations.php"); //set url

        QNetworkRequest request(url);
        //Делает POST запрос
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        //задаем параметры, которые передадим серверу
        QUrlQuery params;
        //Делаем пост запрос
        mng->post(request, params.query().toUtf8());
}

void MainWindow::add_indication(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject station = response["station"].toObject();
    if (response["status"].toString() == "success") {
        current_station->set_humidity(station["humidity"].toString());
        current_station->set_temperature(station["temperature"].toString());
        current_station->set_wind_speed(station["wind_speed"].toString());
        current_station->set_wind_direction(station["wind_direction"].toString());
        current_station->set_precipitation(station["precipitation"].toString());

        ui->stationHumidityLabel->setText(station["humidity"].toString());
        ui->stationPressureLabel->setText(station["pressure"].toString());
        ui->stationWindSpeedLabel->setText(station["wind_speed"].toString());
        ui->stationWindDirectionLabel->setText(station["wind_direction"].toString());
        ui->stationPrecipitationLabel->setText(station["precipitation"].toString());
        ui->stationTemperatureLabel->setText(station["temperature"].toString());

        QMessageBox::information(this, tr("Успешно"), tr("Показания довлены"));
        ui->stationsPage->setCurrentWidget(ui->stationIndicationsPage);
    } else {
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось добавить показания"));
    }
}

void MainWindow::on_password_input_returnPressed()
{
    //Создаем новый менеджер
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::log_in_result);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "login.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("login", this->ui->login_input->text());
    params.addQueryItem("password", this->ui->password_input->text());
    //Делаем пост запрос
    mng->post(request, params.query().toUtf8());
}

void MainWindow::on_login_input_returnPressed()
{
    if (ui->login_input->text().length() == 0) {
        ui->authPageErrorLabel->setText("Введите логин");
    }
    ui->password_input->setFocus();
}

void MainWindow::on_login_input_inputRejected()
{
    qDebug()<< "input rejected\n";
}

void MainWindow::on_addStationIndicationButton_3_clicked()
{
//    ui->stationTimeAddInput->setDate(QDate::currentDate());
    ui->stationLocationAddInput->setText(current_station->get_location());
    ui->stationsPage->setCurrentIndex(2);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stationsPage->setCurrentIndex(0);
}

void MainWindow::on_addStationIndicationButton_2_clicked()
{
    ui->stationsPage->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->pages->setCurrentIndex(0);
}

void MainWindow::on_addIndicationsButton_clicked()
{
    //Создаем новый менеджер
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::add_indication);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "add_indication.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("station_id", this->current_station->get_station_id());
    params.addQueryItem("temperature", this->ui->stationTemperatureAddInput->text());
    params.addQueryItem("humidity", this->ui->stationHumidityAddInput->text());
    params.addQueryItem("pressure", this->ui->stationPressureAddInput->text());
    params.addQueryItem("wind_speed", this->ui->stationWindSpeedAddInput->text());
    params.addQueryItem("precipitation", this->ui->stationPrecipitationAddInput->currentText());
    params.addQueryItem("wind_direction", this->ui->stationWindDirectionAddInput->currentText());
    //Делаем пост запрос
    mng->post(request, params.query().toUtf8());
}

void MainWindow::on_addStation_clicked()
{
    ui->controlPanelPages->setCurrentWidget(ui->addStationPage);
}

//Добавление нового региона в бд
void MainWindow::on_addRegionButtom_clicked()
{
    bool    ok;
    //диалоговое окно с вводом название нового региона
    QString text = QInputDialog::getText(this, tr("Введите наименование региона"),
                                         tr("Навзание региона"), QLineEdit::Normal,
                                         "", &ok);
    QRegExp rx("[а-яА-Я]{2,}");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    int a= 0;
    if (validator->validate(text, a) == QValidator::Invalid) {
        QMessageBox::warning(this, tr("Ошибка ввода"), tr("Неверный формат ввода данных"));
        return;
    }

    if (ok && !text.isEmpty()) {
        //Создаем новый менеджер
        QNetworkAccessManager *mng = new QNetworkAccessManager(this);
        //привязываем функцию
        connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::add_location_response);//finished function
        // задаем адресс запроса
        const QUrl url = QUrl(defaultUrl + "add_location.php"); //set url

        QNetworkRequest request(url);
        //Делает POST запрос
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        //задаем параметры, которые передадим серверу
        QUrlQuery params;
        params.addQueryItem("location", text);
        mng->post(request, params.query().toUtf8());
    }
}

void MainWindow::add_location_response(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

    if (response["status"] == "success") {
        QMessageBox::information(this, tr("Успешно"), "Регион добавлен");
        this->locations.append(QJsonValue(response["location"].toString()));
        ui->changeStationLocationComboBox->addItem(response["location"].toString());
        ui->addStationLocationComboBox->addItem(response["location"].toString());
    } else {
        QMessageBox::critical(this, tr("Ошибка"), "Не удалось добавить регион");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stationsPage->setCurrentWidget(ui->stationIndicationsPage);
}

//Изменение местоположения станции в бд.
void MainWindow::on_acceptStationChangeButton_clicked()
{
    //Создаем новый менеджер
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::station_change_response);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "change_station.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("station_id", current_station->get_station_id());
    params.addQueryItem("new_location", ui->changeStationLocationComboBox->currentText());
    mng->post(request, params.query().toUtf8());
}

//Изменение местоположения станции в бд. ОТвет от сервера
void    MainWindow::station_change_response(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    if (response["status"] == "success") {
        current_station->set_location(response["location"].toString());
        ui->stationLocationLabel->setText(response["location"].toString());
        ui->currentLocationField->setText(response["location"].toString());
        ui->stationLocationAddInput->setText(response["location"].toString());
        QMessageBox::information(this, "Успешно", "Местоположение изменено");
        ui->stationsPage->setCurrentWidget(ui->stationIndicationsPage);
    } else {
        QMessageBox::critical(this, tr("Ошибка"), "Не удалось изменить регион");
    }
}

//Переход к странице изменения станции
void MainWindow::on_changeStationButton_clicked()
{
    if (user->get_level_access() != 0) {
        QMessageBox::information(this, "Ошибка", "У вас недостаточно прав для данного действия\n");
        return ;
    }
    ui->currentLocationField->setText(current_station->get_location());
    ui->stationsPage->setCurrentWidget(ui->changeStationPage_2);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->controlPanelPages->setCurrentWidget(ui->controlPanelPage);
}

//Добавить новую станцию
void MainWindow::on_addStatuinButton_clicked()
{
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::add_station_response);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "add_station.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("location", ui->addStationLocationComboBox->currentText());
    mng->post(request, params.query().toUtf8());
}

//Добавить новую станцию ответ от сервера
void MainWindow::add_station_response(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject jsonStation = response["station"].toObject();
    if (response["status"] == "success") {
        Station *station = new Station(jsonStation);
        connect(station, &Station::clicked, this, &MainWindow::go_to_station_info);
        station->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);
        stations.append(station);
        ui->scrollAreaWidgetContents->layout()->removeItem(spacer);
        ui->scrollAreaWidgetContents->layout()->addWidget(station);
        ui->scrollAreaWidgetContents->layout()->addItem(spacer);
        QMessageBox::information(this, "Успешно", "Станция добавлена");
        ui->controlPanelPages->setCurrentWidget(ui->controlPanelPage);
    } else {
        QMessageBox::critical(this, tr("Ошибка"), "Не удалось добавить станцию");
    }
}

void MainWindow::on_stationsPage_currentChanged(int arg1)
{

}

void MainWindow::on_deleteStationButton_clicked()
{
    if (user->get_level_access() != 0) {
        QMessageBox::information(this, "Ошибка", "У вас недостаточно прав для данного действия\n");
        return ;
    }
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Удаление", "Вы уверены, что хотите удалить станцию?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QNetworkAccessManager *mng = new QNetworkAccessManager(this);
        //привязываем функцию
        connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::delete_station_response);//finished function
        // задаем адресс запроса
        const QUrl url = QUrl(defaultUrl + "delete_station.php"); //set url

        QNetworkRequest request(url);
        //Делает POST запрос
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        //задаем параметры, которые передадим серверу
        QUrlQuery params;
        params.addQueryItem("station_id", current_station->get_station_id());
        mng->post(request, params.query().toUtf8());
    }
}

void MainWindow::delete_station_response(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject jsonStation = response["station"].toObject();
    if (response["status"] == "success") {
        QMessageBox::information(this, "Успех", "Станция удалена");
        ui->scrollAreaWidgetContents->layout()->removeWidget(current_station);
        stations.removeOne(current_station);
        ui->stationsPage->setCurrentWidget(ui->stationListPage);
    } else {
        QMessageBox::critical(this, "Ошибка", "Произошла непредвиденная ошибка");
    }
}

void MainWindow::on_deleteEmployee_clicked()
{
    bool    ok;
    //диалоговое окно с вводом название нового региона
    QString text = QInputDialog::getText(this, tr("Введите имя пользователя для удаления"),
                                         tr("Имя пользователя"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && text == user->get_login()){
        QMessageBox::warning(this, "Ошибка", "Вы не можете удалить собственный профиль");
        return ;
    }
    if (ok && !text.isEmpty()){
        QNetworkAccessManager *mng = new QNetworkAccessManager(this);
        //привязываем функцию
        connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::delete_user_response);//finished function
        // задаем адресс запроса
        const QUrl url = QUrl(defaultUrl + "delete_user.php"); //set url

        QNetworkRequest request(url);
        //Делает POST запрос
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        //задаем параметры, которые передадим серверу
        QUrlQuery params;
        params.addQueryItem("login", text);
        mng->post(request, params.query().toUtf8());
    }
}

void MainWindow::delete_user_response(QNetworkReply * reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject jsonStation = response["station"].toObject();
    if (response["status"] == "success") {
        QMessageBox::information(this, "Успех", "Пользователь удален");
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось удалить пользователя " + response["login"].toString());
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString login = ui->lineEdit->text();
    if (login == user->get_login()) {
        QMessageBox::warning(this, "Ошибка", "Вы не можете изменять собственный профиль");
        return ;
    }
    QString role = ui->selectUserGroupComboBox->currentText();
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::change_user_response);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "change_role.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("login", login);
    params.addQueryItem("role", role);
    mng->post(request, params.query().toUtf8());
}

void MainWindow::change_user_response(QNetworkReply *reply)
{
     QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
     QJsonObject jsonStation = response["station"].toObject();
     if (response["status"] == "success") {
         QMessageBox::information(this, "Успех", "Пользователь изменен");
     } else {
         QMessageBox::critical(this, "Ошибка", "Не удалось изменить пользователя " + response["login"].toString());
     }
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->controlPanelPages->setCurrentIndex(0);
}

void MainWindow::on_addEmployee_clicked()
{
    ui->controlPanelPages->setCurrentWidget(ui->changeUserPage);
}

void MainWindow::on_pushButton_7_clicked()
{
    QNetworkAccessManager *mng = new QNetworkAccessManager(this);
    //привязываем функцию
    connect(mng, &QNetworkAccessManager::finished, this, &MainWindow::make_plot);//finished function
    // задаем адресс запроса
    const QUrl url = QUrl(defaultUrl + "get_indications.php"); //set url

    QNetworkRequest request(url);
    //Делает POST запрос
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //задаем параметры, которые передадим серверу
    QUrlQuery params;
    params.addQueryItem("station_id", current_station->get_station_id());
    mng->post(request, params.query().toUtf8());

    ui->stationsPage->setCurrentWidget(ui->plotsPage);
}

void MainWindow::make_plot(QNetworkReply *reply)
{
    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonObject jsonStation = response["station"].toObject();
    qDebug() << response;
    if (response["status"] != "success") {
        QMessageBox::critical(this, "Ошибка", "Нет данных");
        ui->stationsPage->setCurrentWidget(ui->stationIndicationsPage);
    }
    QJsonArray indications = response["indications"].toArray();

    int num = response["num"].toInt();
    qDebug() << num;
    QVector<QCPGraphData> timeData(num);
    for (int i = 0; i < num; i++) {
        timeData[i].key = QDateTime::fromString(indications[i].toObject()["time"].toString(),
                                "yyyy-MM-dd hh:mm:ss").toTime_t();
    }

    ui->customPlotHumidity->xAxis->setLabel("Время");
    ui->customPlotHumidity->yAxis->setLabel("Влажность");

    ui->customPlotPressure->xAxis->setLabel("Время");
    ui->customPlotPressure->yAxis->setLabel("Давление");

    ui->customPlotTemperature->xAxis->setLabel("Время");
    ui->customPlotTemperature->yAxis->setLabel("Температура");

    // set locale to english, so we get english month names:
    ui->customPlotHumidity->setLocale(QLocale(QLocale::Russian, QLocale::Russia));
    ui->customPlotPressure->setLocale(QLocale(QLocale::Russian, QLocale::Russia));
    ui->customPlotTemperature->setLocale(QLocale(QLocale::Russian, QLocale::Russia));

    //humidity
    // seconds of current time, we'll use it as starting point in time for data:
    ui->customPlotHumidity->addGraph();
    QColor color(20+200/4.0*1,70*(1.6-1/4.0), 150, 150);
    ui->customPlotHumidity->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlotHumidity->graph()->setPen(QPen(color.darker(200)));
    ui->customPlotHumidity->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//    ui->customPlotHumidity->graph()->setBrush(QBrush(color));

    for (int i = 0; i < num; i++) {
        timeData[i].value = indications[i].toObject()["humidity"].toString().toInt();
        qDebug() << timeData[i].key;
        qDebug() << timeData[i].value;
    }
    ui->customPlotHumidity->graph()->data()->set(timeData);

    //pressure
    ui->customPlotPressure->addGraph();
    color = QColor(20+200/4.0*4,70*(1.6-4/4.0), 150, 150);
    ui->customPlotPressure->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlotPressure->graph()->setPen(QPen(color.darker(200)));
    ui->customPlotPressure->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    ui->customPlotPressure->graph()->setBrush(QBrush(color));

    for (int i = 0; i < num; i++) {
        timeData[i].value = indications[i].toObject()["pressure"].toString().toInt();
        qDebug() << timeData[i].key;
        qDebug() << timeData[i].value;
    }
    ui->customPlotPressure->graph()->data()->set(timeData);

    //temperature
    ui->customPlotTemperature->addGraph();
    color = QColor(20+200/4.0*3,70*(1.6-3/4.0), 150, 150);
    ui->customPlotTemperature->graph()->setLineStyle(QCPGraph::lsLine);
    ui->customPlotTemperature->graph()->setPen(QPen(color.darker(200)));
    ui->customPlotTemperature->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
//    ui->customPlotHumidity->graph()->setBrush(QBrush(color));

    for (int i = 0; i < num; i++) {
        timeData[i].value = indications[i].toObject()["temperature"].toString().toInt();
        qDebug() << timeData[i].key;
        qDebug() << timeData[i].value;
    }
    ui->customPlotTemperature->graph()->data()->set(timeData);

    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("hh:mm\nd. MMMM\nyyyy");
    ui->customPlotHumidity->xAxis->setTicker(dateTicker);
    ui->customPlotPressure->xAxis->setTicker(dateTicker);
    ui->customPlotTemperature->xAxis->setTicker(dateTicker);
    // configure left axis text labels:
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTick(10, "10%");
    textTicker->addTick(90, "90%");
    ui->customPlotHumidity->yAxis->setTicker(textTicker);

    QSharedPointer<QCPAxisTickerText> textTicker2(new QCPAxisTickerText);
    textTicker2->addTick(680, "680");
    textTicker2->addTick(820, "820");
    ui->customPlotPressure->yAxis->setTicker(textTicker2);

    QSharedPointer<QCPAxisTickerText> textTicker3(new QCPAxisTickerText);
    textTicker3->addTick(-40, "-40");
    textTicker3->addTick(+40, "+40");
    ui->customPlotTemperature->yAxis->setTicker(textTicker3);

    // set a more compact font size for bottom and left axis tick labels:
    ui->customPlotHumidity->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlotHumidity->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlotTemperature->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlotTemperature->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlotPressure->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
    ui->customPlotPressure->yAxis->setTickLabelFont(QFont(QFont().family(), 8));
    // set axis labels:
    ui->customPlotHumidity->xAxis->setLabel("Дата");
    ui->customPlotHumidity->yAxis->setLabel("Относительная влажность");
    ui->customPlotTemperature->xAxis->setLabel("Дата");
    ui->customPlotTemperature->yAxis->setLabel("Температура, С");
    ui->customPlotPressure->xAxis->setLabel("Дата");
    ui->customPlotPressure->yAxis->setLabel("Давление, мм рт. ст.");
    // make top and right axes visible but without ticks and labels:

    ui->customPlotHumidity->xAxis2->setVisible(true);
    ui->customPlotHumidity->yAxis2->setVisible(true);
    ui->customPlotHumidity->xAxis2->setTicks(false);
    ui->customPlotHumidity->yAxis2->setTicks(false);
    ui->customPlotHumidity->xAxis2->setTickLabels(false);
    ui->customPlotHumidity->yAxis2->setTickLabels(false);

    ui->customPlotTemperature->xAxis2->setVisible(true);
    ui->customPlotTemperature->yAxis2->setVisible(true);
    ui->customPlotTemperature->xAxis2->setTicks(false);
    ui->customPlotTemperature->yAxis2->setTicks(false);
    ui->customPlotTemperature->xAxis2->setTickLabels(false);
    ui->customPlotTemperature->yAxis2->setTickLabels(false);

    ui->customPlotPressure->xAxis2->setVisible(true);
    ui->customPlotPressure->yAxis2->setVisible(true);
    ui->customPlotPressure->xAxis2->setTicks(false);
    ui->customPlotPressure->yAxis2->setTicks(false);
    ui->customPlotPressure->xAxis2->setTickLabels(false);
    ui->customPlotPressure->yAxis2->setTickLabels(false);
    // set axis ranges to show all data:
    ui->customPlotHumidity->xAxis->setRange(timeData[0].key, timeData[num-1].key);
    ui->customPlotHumidity->yAxis->setRange(0, 100);

    ui->customPlotTemperature->xAxis->setRange(timeData[0].key, timeData[num-1].key);
    ui->customPlotTemperature->yAxis->setRange(0, 30);

    ui->customPlotPressure->xAxis->setRange(timeData[0].key, timeData[num-1].key);
    ui->customPlotPressure->yAxis->setRange(600, 900);
    // show legend with slightly transparent background brush:
//    ui->customPlotHumidity->legend->setVisible(true);
//    ui->customPlotHumidity->legend->setBrush(QColor(255, 255, 255, 150));
    ui->customPlotHumidity->replot();
    ui->customPlotTemperature->replot();
    ui->customPlotPressure->replot();
}


void MainWindow::on_pushButton_8_clicked()
{
    ui->stationsPage->setCurrentWidget(ui->stationIndicationsPage);
}
