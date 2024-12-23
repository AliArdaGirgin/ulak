#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include "PortSelection_Comm.h"
#include "PortHandler_Comm.h"


PortSelection_Comm::PortSelection_Comm(QWidget *parent_):parent(parent_){
    QGridLayout *grid = new QGridLayout(this);

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    QList<qint32> availableBaudrates = QSerialPortInfo::standardBaudRates();

    button_ok = new QPushButton("Ok");
    button_ok->setEnabled(true);
    button_cancel = new QPushButton("Cancel");

    QLabel *label_port = new QLabel("Comm Port");
    cbox_port = new QComboBox(parent);
    for(auto &port : ports)
        cbox_port->addItem(port.portName());

    QLabel *label_baudrate = new QLabel("Baudrate");
    cbox_baudrate = new QComboBox(parent);
    for(auto &baudrate : availableBaudrates)
       cbox_baudrate->addItem(QString::number(baudrate));
    ComboSetIndex(cbox_baudrate,"115200");

    QLabel *label_dataBits = new QLabel("Data bits");
    cbox_dataBits = new QComboBox(parent);
    cbox_dataBits->addItem("5");
    cbox_dataBits->addItem("6");
    cbox_dataBits->addItem("7");
    cbox_dataBits->addItem("8");
    ComboSetIndex(cbox_dataBits,"8");

    QLabel *label_stopBits = new QLabel("Stop bits");
    cbox_stopBits = new QComboBox(parent);
    cbox_stopBits->addItem("1");
    cbox_stopBits->addItem("1.5");
    cbox_stopBits->addItem("2");
    ComboSetIndex(cbox_stopBits,"1");

    QLabel *label_parity = new QLabel("Parity");
    cbox_parity = new QComboBox(parent);
    cbox_parity->addItem("Even");
    cbox_parity->addItem("Mark");
    cbox_parity->addItem("None");
    cbox_parity->addItem("Odd");
    cbox_parity->addItem("Space");
    ComboSetIndex(cbox_parity,"None");

    QLabel *label_flowControl = new QLabel("Flow control");
    cbox_flowControl = new QComboBox(parent);
    cbox_flowControl->addItem("None");
    cbox_flowControl->addItem("XON/XOFF");
    cbox_flowControl->addItem("RTS/CTS");
    cbox_flowControl->addItem("DSR/DTR");
    ComboSetIndex(cbox_flowControl,"None");

    grid->addWidget(label_port,0,0);
    grid->addWidget(cbox_port,0,1);
    grid->addWidget(label_baudrate,1,0);
    grid->addWidget(cbox_baudrate,1,1);
    grid->addWidget(label_dataBits,2,0);
    grid->addWidget(cbox_dataBits,2,1);
    grid->addWidget(label_stopBits,3,0);
    grid->addWidget(cbox_stopBits,3,1);
    grid->addWidget(label_parity,4,0);
    grid->addWidget(cbox_parity,4,1);
    grid->addWidget(label_flowControl,5,0);
    grid->addWidget(cbox_flowControl,5,1);
    grid->addWidget(button_ok,6,0);
    grid->addWidget(button_cancel,6,1);
    setLayout(grid);

    setWindowTitle("Serial Port Selection");
    resize(300,400);

    connect(button_cancel,SIGNAL(clicked()), this, SLOT(onCancelled()));
    connect(button_ok,    SIGNAL(clicked()), this, SLOT(portSelected()));
}

void PortSelection_Comm::portSelected(void){
    button_ok->setEnabled(false);
    QMessageBox mbox;
    port = new QSerialPort();
    port->setPortName(cbox_port->currentText());
    port->setBaudRate(static_cast<qint32>(cbox_baudrate->currentText().toInt()));
    port->setDataBits(static_cast<QSerialPort::DataBits>(cbox_dataBits->currentText().toInt()));
    port->setStopBits(static_cast<QSerialPort::StopBits>(cbox_stopBits->currentText().toInt()));
    port->setParity(static_cast<QSerialPort::Parity>(cbox_parity->currentText().toInt()));
    port->setFlowControl(static_cast<QSerialPort::FlowControl>(cbox_flowControl->currentText().toInt()));
    
    if(!port->open(QIODevice::ReadWrite)){
        mbox.setText(port->errorString());
        mbox.exec(); 
        button_ok->setEnabled(true);
    }else{
        PortHandler_Comm* comm_port = new PortHandler_Comm();
        comm_port->setPort(port);
        emit opened(comm_port);
    }
}

void PortSelection_Comm::onCancelled(){
    parent->close();
}

void PortSelection_Comm::ComboSetIndex(QComboBox *combo, QString text){
    int index = combo->findText(text);
    if(index != -1)
        combo->setCurrentIndex(index);
}

bool PortSelection_Comm::isOpen(){
    return port->isOpen();
}
