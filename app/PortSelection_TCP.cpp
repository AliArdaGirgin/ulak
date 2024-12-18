#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

#include "PortSelection_TCP.h"
#include "PortHandler_TCP.h"

PortSelection_TCP::PortSelection_TCP(QWidget* parent_):
    parent(parent_), ip_text(true){

    setWindowTitle("TCP-Client");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::WindowStaysOnTopHint);


    QLabel *address_label = new QLabel("Address");
    address = new QLineEdit();

    QLabel *port_label = new QLabel("Port");
    port = new QLineEdit();

    // set int validator for port (0 - 50000)
    QIntValidator* validator = new QIntValidator();
    validator->setBottom(0);
    validator->setTop(50000);
    port->setValidator(validator);

    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");

    layout = new QGridLayout(this);
    layout->addWidget(address_label, 0, 0);
    layout->addWidget(address, 0, 1);
    layout->addWidget(port_label, 1,0);
    layout->addWidget(port, 1, 1);
    layout->addWidget(ok, 2, 0);
    layout->addWidget(cancel, 2, 1);
    setLayout(layout);

    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));

    connect(cancel,     SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ok, 	    SIGNAL(clicked()), this, SLOT(onOk()));
}

void PortSelection_TCP::onCancel(){
    parent->close();
}

void PortSelection_TCP::onErrorOccurred(QAbstractSocket::SocketError err){
    (void)err;
    QMessageBox mbox;
    mbox.setText(socket->errorString());
    mbox.exec();
    ok->setEnabled(true);
}

void PortSelection_TCP::onOk(){
    ok->setEnabled(false);
    socket->connectToHost(address->text(), port->text().toUShort(), QIODeviceBase::ReadWrite, QAbstractSocket::AnyIPProtocol);
}

void PortSelection_TCP::onConnected(){
    disconnect(socket, nullptr, nullptr, nullptr);
    PortHandler_TCP* port = new PortHandler_TCP();
    if(port->setSocket(socket))
        emit opened(port);
}
