#ifndef PORTHANDLER_TCP_CLIENT_H
#define PORTHANDLER_TCP_CLIENT_H

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <QTimer>
#include "PortHandler_Base.h"
#include "DataType.h"

class PortHandler_TCP_Client:public PortHandler_Base{
    Q_OBJECT
    public:
        PortHandler_TCP_Client();
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
        void updateName(QString name) override;
    private:
        QTcpSocket* current_socket;
        QTimer* timer;
};
#endif // PORTHANDLER_TCP_CLIENT_H
