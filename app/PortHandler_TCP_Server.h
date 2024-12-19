#ifndef PORTHANDLER_TCP_SERVER_H
#define PORTHANDLER_TCP_SERVER_H

#include <QTcpServer>
#include <QTimer>

#include "PortHandler_Base.h"

class PortHandler_TCP_Server: public PortHandler_Base{
    Q_OBJECT
public:
    PortHandler_TCP_Server();
    void port_disconnect() override;
    QString getPortName() override;
    bool setSocket(QTcpServer*);
public slots:
    void write(QByteArray data, DATA_TYPE dtype) override;
private slots:
    void run();
signals:
    void read(QByteArray data, DATA_TYPE dtype) override;
private:
    QTcpServer* current_socket;
    QTimer* timer;
};
#endif // PORTHANDLER_TCP_SERVER_H
