#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

#include <QRegExpValidator>
#include <QtNetwork/QNetworkAccessManager> // to make requests
#include <QtNetwork/QNetworkReply> //to handle replies

#include <QUrl>
#include <QUrlQuery>

#include <QJsonValue>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QMessageBox>
#include <QInputDialog>

#include <QSpacerItem>

#include "qcustomplot.h"

#include "station.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void    http_finished(QNetworkReply *reply);

    void    get_init_values(QNetworkReply *reply);

    //Ответ от сервера при входе
    void    log_in_result(QNetworkReply *reply);

    // добавить показания станции
    void    add_indication(QNetworkReply *reply);

    void    on_password_input_returnPressed();

    void    on_login_input_returnPressed();

    void    on_login_input_inputRejected();

    //Переход на страницу с информацией о станции
    void    go_to_station_info();

    //Загружаем и формируем списко станций
    void    add_stations(QNetworkReply *reply);

    void    on_addStationIndicationButton_3_clicked();

    void    on_pushButton_3_clicked();

    void    on_addStationIndicationButton_2_clicked();

    void    on_pushButton_clicked();

    void    station_change_response(QNetworkReply *reply);

    void on_addIndicationsButton_clicked();

    //Добавление нового региона
    void on_addStation_clicked();
    //Добавление нового региона. Ответ от сервера.
    void add_location_response(QNetworkReply *reply);

    void on_addRegionButtom_clicked();

    void on_pushButton_2_clicked();

    void on_acceptStationChangeButton_clicked();

    void on_changeStationButton_clicked();

    void on_pushButton_4_clicked();

    //Добавить новую станцию
    void on_addStatuinButton_clicked();
    //Добавить новую станцию ответ от сервера
    void add_station_response(QNetworkReply *reply);

    void on_stationsPage_currentChanged(int arg1);

    void on_deleteStationButton_clicked();

    void delete_station_response(QNetworkReply *reply);

    void on_deleteEmployee_clicked();
    void delete_user_response(QNetworkReply *reply);

    void on_pushButton_5_clicked();

    void  change_user_response(QNetworkReply *reply);

    void on_pushButton_6_clicked();

    void on_addEmployee_clicked();

    void on_pushButton_7_clicked();

    void make_plot(QNetworkReply *reply = nullptr);

    void on_pushButton_8_clicked();

private:

    void    load_stations();

    Ui::MainWindow  *ui;

    // вкладка управления. Доступна только для пользователей с наивысшим приоритетом
    QWidget                 *hiddenTab;
    int                     hiddenTabIndex;
    QString                 hiddenTabLabel;

    //данные о станциях
    QJsonArray precipitations;
    QJsonArray wind_directions;
    QJsonArray locations;

    QVector<Station *> stations;
    Station *current_station;

    QSpacerItem *spacer;

    User    *user;

    //график
    QCustomPlot *wGraphic;
    QCPCurve    *verticalLine;
    QCPItemTracer *tracer; //трасировщик по точкаи графика
};
#endif // MAINWINDOW_H
