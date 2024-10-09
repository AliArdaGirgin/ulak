#include <QObject>
#include <QByteArray>
#include "PortHandler.h"
#include <QSerialPort>
#include <QTimer>
#include "DataType.h"

QSerialPort *PortHandler::current_port = nullptr;

PortHandler::PortHandler(QObject *parent):QObject(parent){
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(100);
}

bool PortHandler::commExists(){
    if( !current_port)
        return false;
    return current_port->isOpen();
}

bool PortHandler::removePort(){
    if(current_port){
        current_port->close();
        while(current_port->openMode() != QIODevice::NotOpen){
            ;
        }
        current_port = nullptr;
        emit portStateChanged(false, "");
        return true;
    }else{
        emit portStateChanged(false, "");
        return false;
    }
}

bool PortHandler::setPort(QSerialPort *port_in){
    if(current_port)
        removePort();

    current_port = port_in;
    emit portStateChanged(true, current_port->portName());
    return true;
}

int PortHandler::write(QByteArray &data){

    if(current_port)
        return current_port->write(data);
    else
        return -1;
        
}

void PortHandler::run(){
    if(!current_port)
        return;
    qint64 sz = 0;
    sz = current_port->read(read_data,1024);
    if(sz > 0){
        emit read(QByteArray(read_data, sz), DataType::RX);
    }
}
