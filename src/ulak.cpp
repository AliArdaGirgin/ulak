#include <QApplication>
#include "MainWindow.h"
#include <QDebug>
#include <QFile>
#include <QResource>

int main(int argc, char *argv[]){
    QResource::registerResource("./img/resources.rcc");
    QApplication app(argc, argv);
    QFile file("stylesheet.css");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "FAiled to open stylesheet file";
        return -1;
    }
    app.setStyleSheet(QLatin1String(file.readAll()));
    MainWindow window;
    window.show();
    return app.exec();
}
