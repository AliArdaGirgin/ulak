#include <QApplication>
#include "MainWindow.h"
#include <QDebug>
#include <QFile>
#include <QResource>
#include "Conf.h"

int main(int argc, char *argv[]){
    QResource::registerResource(RCC_FILE);
    QApplication app(argc, argv);
    QFile file(STYLESHEET_FILE);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug() << "Failed to open stylesheet file";
        return -1;
    }
    app.setStyleSheet(QLatin1String(file.readAll()));
    MainWindow window;
    window.show();
    return app.exec();
}
