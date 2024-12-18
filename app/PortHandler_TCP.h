#ifndef PORTHANDLER_TCP_H
#define PORTHANDLER_TCP_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QTimer>
#include "PortHandler_Base.h"
#include "DataType.h"

class PortHandler_TCP:public PortHandler_Base{
    Q_OBJECT
    public:
        PortHandler_TCP();
        void disconnect() override;
        QString getPortName() override;
        bool setSocket(QTcpSocket* socket);
    public slots:
        void write(QByteArray data, DATA_TYPE dtype) override;
    private slots:
        void run();
    signals:
        void read(QByteArray data, DATA_TYPE dtype);
    private:
        QTcpSocket* current_socket;
        QTimer* timer;
        char read_data[1024];
};
#endif // PORTHANDLER_TCP_H
