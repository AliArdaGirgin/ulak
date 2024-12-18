#include <QWidget>
#include <QTabWidget>
#include <QSerialPort>

#include "PortSelection.h"
#include "PortSelection_Comm.h"
#include "PortSelection_TCP.h"
#include "PortHandler_Base.h"

PortSelection::PortSelection(QWidget* parent):
    QTabWidget(){

    setWindowTitle("Port");

    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);

    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);

    setAttribute(Qt::WA_DeleteOnClose);

    commPort = new PortSelection_Comm(this);
    tcpPort =  new PortSelection_TCP(this);
    addTab(commPort,"COMM");
    addTab(tcpPort, "TCP-Client");

    // parent = MainWindow
    connect(commPort, SIGNAL(opened(PortHandler_Base*)), this, SLOT(open(PortHandler_Base*)));
    connect(commPort, SIGNAL(opened(PortHandler_Base*)), parent, SLOT(setPortState(PortHandler_Base*)));

    connect(tcpPort, SIGNAL(opened(PortHandler_Base*)), this, SLOT(open(PortHandler_Base*)));
    connect(tcpPort, SIGNAL(opened(PortHandler_Base*)), parent, SLOT(setPortState(PortHandler_Base*)));
}

void PortSelection::open(PortHandler_Base *port_in){
    (void)port_in;
    window_close();
}

void PortSelection::cancel(){
    window_close();
}

void PortSelection::window_close(){
    this->close();
}

