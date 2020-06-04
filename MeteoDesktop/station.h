#ifndef STATION_H
#define STATION_H

#include <QWidget>

#include <QJsonArray>
#include <QJsonObject>

#include <QMouseEvent>

#include <QDateTime>

namespace Ui {
class Station;
}

class Station : public QWidget
{
    Q_OBJECT

public:
    explicit Station(QWidget *parent = nullptr);
    Station(QString name, QString property);
    Station(int station_id, QString location,
            int temperature, int pressure, int humidity,
            int wind_speed, QString wind_direction, QString precipitation);
    Station(QJsonObject &jsonStation);
    ~Station();

    QString get_info();

    QString get_station_id() const;
    QString get_location() const;
    QString get_wind_direction()const;
    QString get_precipitation() const;
    QString get_temperature() const;
    QString get_pressure() const;
    QString get_humidity() const;
    QString get_wind_speed() const;
    QDateTime get_time() const;

    void set_location(const QString location);
    void set_station_id(const QString);
//    void set_location(const QString);
    void set_wind_direction(const QString);
    void set_precipitation(const QString);
    void set_temperature(const QString);
    void set_pressure(const QString);
    void set_humidity(const QString);
    void set_wind_speed(const QString);

    void set_time(const QString);

signals:
    void    clicked();
    void    mouse_in();

private:
    Ui::Station *ui;

    QString station_id;
    QString location;
    QString wind_direction;
    QString precipitation;
    QString temperature;
    QString pressure;
    QString humidity;
    QString wind_speed;

    QDateTime time;

private slots:
    void    mouse_in_highlight();

protected:
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);
};

#endif // STATION_H
