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
        void port_disconnect() override;
        QString getPortName() override;
        bool setSocket(QTcpSocket* socket);
    public slots:
        void write(QByteArray data, DATA_TYPE dtype) override;
        void errorOccured(QAbstractSocket::SocketError);
    private slots:
        void run();
    signals:
        void read(QByteArray data, DATA_TYPE dtype) override;
        void closed(QString message) override;
    private:
        QTcpSocket* current_socket;
        QTimer* timer;
};
#endif // PORTHANDLER_TCP_H
