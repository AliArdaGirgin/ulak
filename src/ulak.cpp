#include <QApplication>
#include "MainWindow.h"
#include <QDebug>
#include <QFile>
#include <QResource>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QFile stylesheet_file(":/stylesheet.css");
    if(!stylesheet_file.open(QIODevice::ReadOnly)){
        qDebug() << "Failed to open stylesheet file";
        return -1;
    }
    app.setStyleSheet(QLatin1String(stylesheet_file.readAll()));
    MainWindow window;
    window.show();
    return app.exec();
}
