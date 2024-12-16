#include <QWidget>
#include <QTabWidget>
#include "PortSelection.h"
#include "PortSelection_Comm.h"
#include "PortHandler_Base.h"
#include <QSerialPort>

PortSelection::PortSelection(QWidget* parent):
    QTabWidget(){

    setWindowTitle("Port");

    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);

    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_DeleteOnClose);

    PortCommSelection *commPort = new PortCommSelection(this);
    addTab(commPort,"COMM");

    connect(commPort, SIGNAL(closed()), this, SLOT(cancel()));
    connect(commPort, SIGNAL(opened(PortHandler_Base*)), this, SLOT(open(PortHandler_Base*)));
    connect(commPort, SIGNAL(opened(PortHandler_Base*)), parent, SLOT(setPortState(PortHandler_Base*)));
}

void PortSelection::open(PortHandler_Base *port_in){
    (void)port_in;
    this->close();
}

void PortSelection::cancel(){
    this->close();
}

