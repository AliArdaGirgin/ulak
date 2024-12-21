#ifndef PORTHANDLER_BASE_H
#define PORTHANDLER_BASE_H

#include <QObject>
#include <QByteArray>
#include "DataType.h"
#include "Conf.h"

class PortHandler_Base: public QObject{
public:
    PortHandler_Base(){open = false;}
    static bool isOpen(){ return open; }
    virtual void port_disconnect() = 0;
    virtual QString getPortName() = 0;
public slots:
    virtual void write(QByteArray data, DATA_TYPE dtype) = 0;
signals:
    virtual void read(QByteArray data, DATA_TYPE dtype) = 0;
    virtual void closed(QString message) = 0;
    virtual void updateName(QString name){(void)name;}
protected:
    static bool open;
    char read_buffer[BUFFER_SIZE];
    char write_buffer[BUFFER_SIZE];
};
#endif // PORTHANDLER_BASE_H
