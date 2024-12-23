#include <QDebug>
#include "PortHandler_TCP_Client.h"
#include "Conf.h"

PortHandler_TCP_Client::PortHandler_TCP_Client(){
    current_socket = nullptr;
    open = false;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(PORT_HANDLER_READ_PERIOD);
}

void PortHandler_TCP_Client::port_disconnect(){
    if(current_socket){
        current_socket->disconnectFromHost();
        while(current_socket->state() != QAbstractSocket::UnconnectedState){
            ;
        }
        disconnect(current_socket, nullptr, nullptr, nullptr);
        current_socket = nullptr;
        open = false;
    }
}

QString PortHandler_TCP_Client::getPortName(){
    QString name = "";
    if(current_socket){
        name += "TCP-Client: ";
        name += current_socket->peerAddress().toString();
        name += "(";
        name += QString::number(current_socket->peerPort());
        name += ")";
    }
    return name;
}

bool PortHandler_TCP_Client::setSocket(QTcpSocket* socket){
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;
    if(current_socket)
        port_disconnect();
    current_socket = socket;

    connect(current_socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(errorOccured(QAbstractSocket::SocketError)));
    open = true;
    return open;
}

void PortHandler_TCP_Client::write(QByteArray data, DATA_TYPE dtype){
    (void)dtype;
    if(current_socket){
        current_socket->write(data);
    }
}

void PortHandler_TCP_Client::run(){
    if(!current_socket)
        return;
    qint64 sz= current_socket->read(read_buffer, 1024);
    if(sz > 0)
        emit read(QByteArray(read_buffer, sz), DATA_TYPE::RX);
}

void PortHandler_TCP_Client::errorOccured(QAbstractSocket::SocketError err){
    (void)err;
    if(current_socket){
        emit closed(current_socket->errorString());
    }
}
