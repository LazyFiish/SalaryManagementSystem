#include "mainwindow.h"
#include <QApplication>
#include "logindialog.h"
#include "helper.h"
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile qss("stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        app.setStyleSheet(qss.readAll());
        qss.close();
    }

    Helper::connectDatabase();

    LoginDialog w;
    w.show();
    return app.exec();
}
