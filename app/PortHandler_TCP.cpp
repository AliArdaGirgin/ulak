#include "PortHandler_TCP.h"
#include "Conf.h"

PortHandler_TCP::PortHandler_TCP(){
    current_socket = nullptr;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(PORT_HANDLER_READ_PERIOD);
}

void PortHandler_TCP::disconnect(){
    if(current_socket){
        current_socket->disconnectFromHost();
        while(current_socket->state() != QAbstractSocket::UnconnectedState){
            ;
        }
        current_socket = nullptr;
        open = false;
    }
}

QString PortHandler_TCP::getPortName(){
    if(current_socket)
        return current_socket->peerAddress().toString();
    else
        return "";
}

bool PortHandler_TCP::setSocket(QTcpSocket* socket){
    if(socket->state() != QAbstractSocket::ConnectedState)
        return false;
    if(current_socket)
        disconnect();
    current_socket = socket;
    open = true;
    return open;
}

void PortHandler_TCP::write(QByteArray data, DATA_TYPE dtype){
    (void)dtype;
    if(current_socket){
        current_socket->write(data);
    }
}

void PortHandler_TCP::run(){
    if(!current_socket)
        return;
    qint64 sz= current_socket->read(read_data, 1024);
    if(sz > 0)
        emit read(QByteArray(read_data, sz), DATA_TYPE::RX);
}
