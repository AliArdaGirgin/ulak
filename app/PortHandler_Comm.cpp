#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include <QTimer>
#include <QDebug>

#include "DataType.h"
#include "Conf.h"
#include "PortHandler_Comm.h"

PortHandler_Comm::PortHandler_Comm(){
    current_port = nullptr;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(PORT_HANDLER_READ_PERIOD);
}


void PortHandler_Comm::port_disconnect(){
    if(current_port){
        current_port->close();
        while(current_port->openMode() != QIODevice::NotOpen){
            ;
        }
        current_port = nullptr;
        open = false;
    }
}

bool PortHandler_Comm::setPort(QSerialPort *port_in){
    if(!port_in->isOpen())
        return false;

    if(current_port)
        port_disconnect();

    current_port = port_in;
    open = true;
    return true;
}

QString PortHandler_Comm::getPortName(){
    QString name = "";
    if(current_port){
        name += "COMM: ";
        name += current_port->portName();
    }
    return name;
}
void PortHandler_Comm::write(QByteArray data, DATA_TYPE t){
    (void)t;
    if(current_port)
        if(current_port->write(data) < 0){
            qDebug() << "failed to write to port, closing";
        }
}

void PortHandler_Comm::run(){
    if(!current_port)
        return;
    qint64 sz = current_port->read(read_buffer,1024);
    if(sz > 0)
        emit read(QByteArray(read_buffer, sz), DATA_TYPE::RX);
}
