#include "mainwindow.h"

#include <QApplication>

#include "user.h"

#include <string>

int main(int argc, char *argv[])
{
    User user; // Авторизованный пользователь

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
