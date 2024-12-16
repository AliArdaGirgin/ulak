#ifndef PORT_SELECTION_H
#define PORT_SELECTION_H
#include <QWidget>
#include <QTabWidget>
#include <QSerialPort>
#include "PortHandler_Base.h"

class PortSelection:public QTabWidget{
    Q_OBJECT
    public:
        PortSelection(QWidget *parent = 0);

    signals:
        void portStateChanged(PortHandler_Base *port);

    public slots:
        void cancel();
        void open(PortHandler_Base *port_in);
};
#endif
