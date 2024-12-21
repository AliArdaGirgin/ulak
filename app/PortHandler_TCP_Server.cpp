#include "PortHandler_TCP_Server.h"

PortHandler_TCP_Server::PortHandler_TCP_Server(){
    current_server = nullptr;
    current_socket = nullptr;
    open = false;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(PORT_HANDLER_READ_PERIOD);
}

void PortHandler_TCP_Server::port_disconnect(){
    if(current_server){
        disconnect(current_server, nullptr, nullptr, nullptr);
        current_server->close();
        delete current_server;
        current_server = nullptr;
        open = false;
    }
}

QString PortHandler_TCP_Server::getPortName(){
    QString name = "";
    if(current_server){
        name += "TCP-Server: ";
        name += current_server->serverAddress().toString();
        name += "(";
        name += QString::number(current_server->serverPort());
        name += ") ";
    }
    return name;
}

bool PortHandler_TCP_Server::setServer(QTcpServer* server){
    if(!server->isListening())
        return false;
    if(current_server)
        port_disconnect();

    current_server = server;

    connect(current_server, SIGNAL(pendingConnectionAvailable()), this, SLOT(onNewConnection()));
    open = false;
    return true;
}

void PortHandler_TCP_Server::onNewConnection(){
    /*if(current_socket){
        return;
    }*/
    current_socket = current_server->nextPendingConnection();
    connect(current_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()) );
    open = true;

    QString name = getPortName();
    name += "Connected";
    emit updateName(name);
}

void PortHandler_TCP_Server::onSocketDisconnected(){
    open = false;
    QString name = getPortName();
    emit updateName(name);
}

void PortHandler_TCP_Server::write(QByteArray data, DATA_TYPE dtype){
    (void)dtype;
    if(current_socket)
        current_socket->write(data);
}

void PortHandler_TCP_Server::run(){
    if(!current_socket)
        return;
    qint64 sz = current_socket->read(read_buffer, 1024);
    if(sz > 0)
        emit read(QByteArray(read_buffer, sz), DATA_TYPE::RX);
}

