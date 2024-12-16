#ifndef PORTHANDLER_COMM_H
#define PORTHANDLER_COMM_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QTimer>
#include "DataType.h"
#include "PortHandler_Base.h"


class PortHandler_Comm:public PortHandler_Base{
    Q_OBJECT
    public:
        PortHandler_Comm();
        void disconnect() override;
        bool setPort(QSerialPort *port_in);
        QString getPortName() override;
    signals:
        void read(QByteArray data, DATA_TYPE dtype);
    public slots:
        void write(QByteArray data, DATA_TYPE) override;

    private slots:
        void run();
    private:
        QSerialPort *current_port;
        QTimer *timer;
        char read_data[1024];

};
#endif
