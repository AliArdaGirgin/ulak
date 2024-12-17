#ifndef PORTHANDLER_TCP_H
#define PORTHANDLER_TCP_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include "PortHandler_Base.h"
#include "DataType.h"

class PortHandler_TCP:public PortHandler_Base{
    Q_OBJECT
    public:
        PortHandler_TCP();
        void disconnect() override;
        QString getPortName() override;
        void setSocket(QTcpSocket* socket);
    public slots:
        void write(QByteArray data, DATA_TYPE dtype) override;
};
#endif // PORTHANDLER_TCP_H
