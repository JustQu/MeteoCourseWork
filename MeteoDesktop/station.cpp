#include "station.h"
#include "ui_station.h"

#include <QtDebug>

Station::Station(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Station)
{
    ui->setupUi(this);
}

Station::Station(QString name, QString property):
    ui(new Ui::Station)
{
    ui->setupUi(this);
}

Station::Station(QJsonObject &jsonStation) :
    ui(new Ui::Station)
{
    ui->setupUi(this);
    this->humidity = jsonStation["humidity"].toString();
    this->location = jsonStation["location"].toString();
    this->precipitation = jsonStation["precipitation"].toString();
    this->wind_direction = jsonStation["wind_direction"].toString();
    this->pressure = jsonStation["pressure"].toString();
    this->wind_speed = jsonStation["wind_speed"].toString();
    this->temperature = jsonStation["temperature"].toString();
    this->station_id = jsonStation["station_id"].toString();
    this->time = QDateTime::fromString(jsonStation["time"].toString(), "yyyy-MM-dd hh:mm:ss");

    ui->stationLocationLabel->setText(this->location);
    ui->temperature->display(temperature);
}

void    Station::mouse_in_highlight()
{
    qDebug() << "here\n";
    this->setStyleSheet("border: 1px solid red");
}

void Station::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}

void    Station::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if (pos.x() <= this->size().width() && pos.x() >= 0
            && pos.y() <= this->size().height() && pos.y() >= 0)
    {
        emit mouse_in();
    }
}

Station::~Station()
{
    delete ui;
}

QString Station::get_station_id() const { return station_id; }
QString Station::get_location() const { return location; }
QString Station::get_wind_direction() const { return wind_direction; }
QString Station::get_precipitation() const { return precipitation; }
QString Station::get_temperature() const { return temperature; }
QString Station::get_pressure() const { return pressure; }
QString Station::get_humidity() const { return humidity; }
QString Station::get_wind_speed() const { return wind_speed; }
QDateTime Station::get_time() const {return time;}

void    Station::set_location(const QString location)
{
    this->ui->stationLocationLabel->setText(location);
    this->location = location;
}

void Station::set_station_id(const QString station_id) { this->station_id = station_id; }
void Station::set_wind_direction(const QString dir) {  this->wind_direction = dir; }
void Station::set_precipitation(const QString precipitation) {  this->precipitation = precipitation; }
void Station::set_temperature(const QString temperature)
{
    this->temperature = temperature;
    ui->temperature->display(temperature);
}
void Station::set_pressure(const QString pressure) {  this->pressure = pressure; }
void Station::set_humidity(const QString humidity) {  this->humidity = humidity; }
void Station::set_wind_speed(const QString wind_speed) {  this->wind_speed = wind_speed; }
void Station::set_time(const QString time) {this->time=QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");}





















