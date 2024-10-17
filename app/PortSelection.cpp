#include <QWidget>
#include <QTabWidget>
#include "PortSelection.h"
#include "PortCommSelection.h"
#include "PortHandler.h"
#include <QSerialPort>

PortSelection::PortSelection(PortHandler *pHandler, QWidget * parent):QTabWidget(parent),port_handler(pHandler){

    setWindowTitle("Port");

    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);

    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_DeleteOnClose);

    PortCommSelection *commPort = new PortCommSelection(this);
    addTab(commPort,"COMM");

    connect(commPort, SIGNAL(closed()), this, SLOT(cancel()));
    connect(commPort, SIGNAL(opened(QSerialPort*)), this, SLOT(open(QSerialPort*)));
}

void PortSelection::open(QSerialPort *port_in){
    if(port_handler->commExists()){
        port_handler->removePort();
    }
    port_handler->setPort(port_in);
    this->close();
}

void PortSelection::cancel(){
    this->close();
}
