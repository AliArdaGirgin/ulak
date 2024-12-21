#ifndef PORTHANDLER_TCP_SERVER_H
#define PORTHANDLER_TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include "PortHandler_Base.h"

class PortHandler_TCP_Server: public PortHandler_Base{
    Q_OBJECT
public:
    PortHandler_TCP_Server();
    void port_disconnect() override;
    QString getPortName() override;
    bool setServer(QTcpServer*);
public slots:
    void write(QByteArray data, DATA_TYPE dtype) override;
private slots:
    void run();
    void onNewConnection();
    void onSocketDisconnected();
signals:
    void read(QByteArray data, DATA_TYPE dtype) override;
    void closed(QString message) override;
    void updateName(QString name) override;
private:
    QTcpServer* current_server;
    QTimer* timer;
    QTcpSocket* current_socket;
};
#endif // PORTHANDLER_TCP_SERVER_H
