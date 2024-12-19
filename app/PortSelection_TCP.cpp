#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

#include "PortSelection_TCP.h"
#include "PortHandler_TCP_Client.h"
#include "DataType.h"

PortSelection_TCP::PortSelection_TCP(QWidget* parent_):
    parent(parent_), ip_text(true){

    setWindowTitle("TCP");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QLabel *type_label = new QLabel("Type");
    type = new QComboBox();
    type->addItem("Client");
    type->addItem("Server");
    type->setCurrentIndex(PORT_TCP_CLIENT);

    QLabel *address_label = new QLabel("Address");
    address_cbox = new QComboBox();
    address_line_edit = new QLineEdit();

    QLabel *port_label = new QLabel("Port");
    port = new QLineEdit();

    // set int validator for port (0 - 65535)
    QIntValidator* validator = new QIntValidator();
    validator->setBottom(0);
    validator->setTop(65535);
    port->setValidator(validator);

    ok = new QPushButton("Ok");
    cancel = new QPushButton("Cancel");

    layout = new QGridLayout(this);
    layout->addWidget(type_label, 0,0);
    layout->addWidget(type, 0, 1, 1, 2);
    layout->addWidget(address_label, 1, 0);
    layout->addWidget(address_cbox, 1, 1);
    layout->addWidget(address_line_edit, 1, 2, 1, 2);
    layout->addWidget(port_label, 2,0);
    layout->addWidget(port, 2, 1, 1, 2);
    layout->addWidget(ok, 3, 0);
    layout->addWidget(cancel, 3, 1);
    setLayout(layout);

    // call combobox index chage slots to update to initial values
    onTypeIndexChanged(PORT_TCP_CLIENT);
    onAddressIndexChanged(0);

    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));
    server = new QTcpServer();

    connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeIndexChanged(int)));
    connect(address_cbox, SIGNAL(currentIndexChanged(int)), this, SLOT(onAddressIndexChanged(int)));

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
    if(type->currentIndex() == PORT_TCP_CLIENT){
        socket->connectToHost(address_line_edit->text(), port->text().toUShort(), QIODeviceBase::ReadWrite, QAbstractSocket::AnyIPProtocol);
    }else{ // PORT_TCP_SERVER
        qDebug()<< "Tries to listen";
        if(server->listen(QHostAddress(address_line_edit->text()), port->text().toUShort())){

        }else{
            QMessageBox mbox;
            mbox.setText(server->errorString());
            mbox.exec();
        }
    }
}

void PortSelection_TCP::onConnected(){
    disconnect(socket, nullptr, nullptr, nullptr);
    PortHandler_TCP_Client* port = new PortHandler_TCP_Client();
    if(port->setSocket(socket))
        emit opened(port);
}

void PortSelection_TCP::onTypeIndexChanged(int index){
    if(index == PORT_TCP_CLIENT){
        address_cbox->clear();
        address_cbox->addItem("LocalHost");
        address_cbox->addItem("Enter");
    }else if(index == PORT_TCP_SERVER){
        address_cbox->clear();
        address_cbox->addItem("LocalHost");
        address_cbox->addItem("LocalHost_Ipv6");
        address_cbox->addItem("Broadcast");
        address_cbox->addItem("AnyIPv4");
        address_cbox->addItem("AnyIPv6");
        address_cbox->addItem("Any");
        address_cbox->addItem("Manual");
    }
}

void PortSelection_TCP::onAddressIndexChanged(int index){
    if(type->currentIndex() == PORT_TCP_CLIENT){
        switch(index){
        case PORT_TCP_CLIENT_LOCALHOST:
            address_line_edit->setText("127.0.0.1");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_CLIENT_MANUAL:
            address_line_edit->clear();
            address_line_edit->setEnabled(true);
            break;
        default:
            break;
        }
    }else{ // PORT_TCP_SERVER
        switch(index){
        case PORT_TCP_SERVER_LOCALHOST:
            address_line_edit->setText("127.0.0.1");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_LOCALHOST_IPv6:
            address_line_edit->setText("::1");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_BROADCAST:
            address_line_edit->setText("255.255.255.255");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_ANY_IPv4:
            address_line_edit->setText("0.0.0.0");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_ANY_IPv6:
            address_line_edit->setText("::");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_ANY:
            address_line_edit->setText("Any for IPv4&IPv6");
            address_line_edit->setEnabled(false);
            break;
        case PORT_TCP_SERVER_MANUAL:
            address_line_edit->setText("");
            address_line_edit->setEnabled(true);
            break;

        }
    }
}
