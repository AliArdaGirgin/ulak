#ifndef PORTHANDLER_BASE_H
#define PORTHANDLER_BASE_H

#include <QObject>
#include <QByteArray>
#include "DataType.h"

class PortHandler_Base: public QObject{
public:
    PortHandler_Base(){open = false;}
    static bool isOpen(){ return open; }
    virtual void disconnect() = 0;
    virtual void write(QByteArray data, DATA_TYPE dtype) = 0;
    virtual QString getPortName() = 0;
signals:
    void read(QByteArray data, DATA_TYPE dtype);
protected:
    static bool open;
};
#endif // PORTHANDLER_BASE_H
