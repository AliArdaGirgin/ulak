#ifndef PORT_HANDLER_H
#define PORT_HANDLER_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>
#include "DataType.h"


class PortHandler:public QObject{
    Q_OBJECT
    public:
        PortHandler(QObject *parent=0);
        static bool commExists();
        bool setPort(QSerialPort *port_in);
        bool removePort();
    signals:
        void read(QByteArray data, DATA_TYPE dtype);
        void portStateChanged(bool state, QString name);
    public slots:
        void write(QByteArray data, DATA_TYPE);

    private slots:
        void run();
    private:
        static QSerialPort *current_port;
        QTimer *timer;
        char read_data[1024];

};
#endif
