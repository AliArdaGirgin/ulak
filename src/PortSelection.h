#ifndef PORT_SELECTION_H
#define PORT_SELECTION_H
#include <QWidget>
#include <QTabWidget>
#include <QSerialPort>
#include "PortHandler.h"

class PortSelection:public QTabWidget{
    Q_OBJECT
    public:
        PortSelection(PortHandler *pHandler,QWidget *parent = 0);
        
    public slots:
        void cancel();
        void open(QSerialPort *port_in);
    private:
        PortHandler *port_handler;
};
#endif
