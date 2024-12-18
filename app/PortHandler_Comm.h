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
        void port_disconnect() override;
        bool setPort(QSerialPort *port_in);
        QString getPortName() override;
    public slots:
        void write(QByteArray data, DATA_TYPE) override;
    private slots:
        void run();
    signals:
        void read(QByteArray data, DATA_TYPE dtype) override;
        void closed(QString message) override;
    private:
        QSerialPort *current_port;
        QTimer *timer;

};
#endif
