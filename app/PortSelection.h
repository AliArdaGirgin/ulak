#ifndef PORT_SELECTION_H
#define PORT_SELECTION_H
#include <QWidget>
#include <QTabWidget>
#include <QSerialPort>
#include "PortHandler_Base.h"
#include "PortSelection_Comm.h"
#include "PortSelection_TCP.h"

class PortSelection:public QTabWidget{
    Q_OBJECT
    public:
        PortSelection(QWidget *parent = 0);
        void window_close();
    public slots:
        void cancel();
        void open(PortHandler_Base *port_in);
    signals:
        void portStateChanged(PortHandler_Base *port);
    private:
        PortSelection_Comm* commPort;
        PortSelection_TCP*  tcpPort;
};
#endif
