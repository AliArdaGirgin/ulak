#include "PortHandler_TCP_Server.h"

PortHandler_TCP_Server::PortHandler_TCP_Server(){
    current_socket = nullptr;
}

void PortHandler_TCP_Server::port_disconnect(){

}

QString PortHandler_TCP_Server::getPortName(){
    return "";
}

bool PortHandler_TCP_Server::setSocket(QTcpServer* socket){
    (void)socket;
    return true;
}

void PortHandler_TCP_Server::write(QByteArray data, DATA_TYPE dtype){
    (void)data;
    (void)dtype;
}

void PortHandler_TCP_Server::run(){

}

