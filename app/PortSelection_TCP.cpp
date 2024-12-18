#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

#include "PortSelection_TCP.h"
#include "PortHandler_TCP.h"

PortSelection_TCP::PortSelection_TCP(QWidget* parent_):
    parent(parent_), ip_text(true){

    setWindowTitle("TCP");
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::WindowStaysOnTopHint);


    QLabel *address_label = new QLabel("Address");
    address = new QLineEdit();

    dns_lookup = new QPushButton("DNS lookup");

    QLabel *ip_label = new QLabel("IP");
    ip_line_edit = new QLineEdit(); // for single and manuel enter ip addresses
    ip_cbox = new QComboBox(); // when multiple ip addresses return from dns lookup
    ip_clear = new QPushButton("Clear");

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
    layout->addWidget(dns_lookup, 0, 2);
    layout->addWidget(ip_label, 1, 0);
    layout->addWidget(ip_line_edit, 1, 1);
    layout->addWidget(ip_clear, 1, 2);
    layout->addWidget(port_label, 2,0);
    layout->addWidget(port, 2, 1);
    layout->addWidget(ok, 3, 0);
    layout->addWidget(cancel, 3, 1);
    setLayout(layout);

    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(onErrorOccurred(QAbstractSocket::SocketError)));

    connect(dns_lookup, SIGNAL(clicked()), this, SLOT(onDnsLookup()));
    connect(ip_clear,   SIGNAL(clicked()), this, SLOT(onIpClear()));
    connect(cancel,     SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ok, 	    SIGNAL(clicked()), this, SLOT(onOk()));
}

void PortSelection_TCP::onCancel(){
    parent->close();
}

void PortSelection_TCP::onIpClear(){
    ip_cbox->clear();
    ip_line_edit->clear();
    if(!ip_text){ // if multiple ip addresses exit
        layout->removeWidget(ip_cbox);
        ip_cbox->setParent(nullptr);
        layout->addWidget(ip_line_edit, 1, 1);
    }
    ip_text = true;
}
void PortSelection_TCP::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Return && address->hasFocus()){
        onDnsLookup();
    }
}
void PortSelection_TCP::onDnsLookup(){
    QString host = address->text();
    if(!host.isEmpty()){
        // disable buttons
        address->setEnabled(false);
        dns_lookup->setEnabled(false);
        ip_line_edit->setEnabled(false);
        ip_cbox->setEnabled(false);
        // send dns request
        QHostInfo::lookupHost(host, this, &PortSelection_TCP::onDnsLookupResults);
    }
}
void PortSelection_TCP::onDnsLookupResults(const QHostInfo& info){

    // On Error
    if(info.error() != QHostInfo::NoError){
        ip_line_edit->setText(info.errorString());
        address->setEnabled(true);
        dns_lookup->setEnabled(true);
        ip_line_edit->setEnabled(true);
        ip_cbox->setEnabled(true);
        return;
    }
    auto ip_addr = info.addresses();
    if(ip_addr.length() == 1){ // if only one address, line edit
        ip_line_edit->setText(ip_addr.back().toString());
        if(!ip_text){ // field is combobox, replace with line_edit
            layout->removeWidget(ip_cbox);
            ip_cbox->setParent(nullptr);
            layout->addWidget(ip_line_edit, 1, 1);
        }
        ip_text = true;

    }else{ // multiple ip addresses
        // if ip field line_edit, replace with combobox
        if(ip_text){
            layout->removeWidget(ip_line_edit);
            ip_line_edit->setParent(nullptr);
            layout->addWidget(ip_cbox, 1,1);
        }else{ // otherwise clear combobox
            ip_cbox->clear();
        }
        // add ip addressses to combo box
        for(auto addr: ip_addr){
            ip_cbox->addItem(addr.toString());
        }
        ip_text= false;
    }
    address->setEnabled(true);
    dns_lookup->setEnabled(true);
    ip_line_edit->setEnabled(true);
    ip_cbox->setEnabled(true);
}

void PortSelection_TCP::onErrorOccurred(QAbstractSocket::SocketError err){
    QMessageBox mbox;
    QString message("QAbstractSocket::SocketError= ");
    message += QString::number((int)err);
    mbox.setText(message);
    mbox.exec();
}

void PortSelection_TCP::onOk(){
    ok->setEnabled(false);
    QString host;
    if(ip_text){
        host = ip_line_edit->text();
    }else{
        host = ip_cbox->currentText();
    }
    socket->connectToHost(host, port->text().toUShort(), QIODeviceBase::ReadWrite, QAbstractSocket::AnyIPProtocol);
}

void PortSelection_TCP::onConnected(){
    PortHandler_TCP* port = new PortHandler_TCP();
    if(port->setSocket(socket))
        emit opened(port);
}
