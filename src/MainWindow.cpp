#include <QMenuBar>
#include <QMenu>
#include <QScrollArea>
#include <QByteArray>
#include <QDebug>
#include <QCloseEvent>
#include "MainWindow.h"
#include "PortSelection.h"
#include "CommandArea.h"
#include "DataArea.h"
#include "AddButtonWindow.h"
#include "PortHandler.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include "DataType.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    drawMenu();
    resize(1200,600);
    QWidget *central = new QWidget(this);

    port_handler = new PortHandler(this);

    cmd_area = new CommandArea(port_handler, central);
    cmd_scroll = new QScrollArea(this);
    cmd_scroll->setFixedWidth(350);
    cmd_scroll->setWidgetResizable(true);
    cmd_scroll->setWidget(cmd_area);

    data_area = new DataArea(port_handler, central);
    data_scroll = new QScrollArea(this);
    data_scroll->setWidgetResizable(true);
    data_scroll->setWidget(data_area);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight,central);
    layout->addWidget(cmd_scroll);
    layout->addWidget(data_scroll);

    central->setLayout(layout);
    setCentralWidget(central);

    connect(this, SIGNAL(saved()), data_area, SLOT(save()));
    connect(this, SIGNAL(cleared()), data_area, SLOT(clear()));
    connect(cmd_area,     SIGNAL(send(QByteArray&,DataType)),  data_area, SLOT(write(QByteArray&,DataType))    );
    connect(port_handler, SIGNAL(read(QByteArray&,DataType)),  data_area, SLOT(write(QByteArray&,DataType))    );
    connect(port_handler, SIGNAL(read(QByteArray&,DataType)),  cmd_area,  SLOT(dataRead(QByteArray&,DataType)) );
}

void MainWindow::portSelect(void){
    sel = new PortSelection(port_handler);
    sel->show();
}

void MainWindow::portClose(void){
    if(port_handler->commExists()){
        port_handler->removePort();
    }
}

void MainWindow::onAddButton(void){
    addButton = new AddButtonWindow();
    connect(addButton, SIGNAL(onButtonAdded(QString,Command::cmd_type,QByteArray,QByteArray,int,int,QByteArray,QWidget*)),
            cmd_area,  SLOT(addButton(QString,Command::cmd_type,QByteArray,QByteArray,int,int,QByteArray,QWidget*))    );
    addButton->show();
}

void MainWindow::onSaveData(void){
    emit saved();
}

void MainWindow::onSaveCommands(void){
    QString file_name = QFileDialog::getSaveFileName(this,tr("Open File"),"");
    if(file_name.isEmpty()){return;}

    QFile save_file(file_name);
    if(!save_file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Failed to open file");
        msg->exec();
        return;
    }

    QJsonArray json_arr = QJsonArray();
    QVector<Command*> *cmds = cmd_area->getCommands();
    for(auto &cmd : *cmds){
        QJsonObject obj = QJsonObject();
        obj["name"] = cmd->getName();
        obj["type"] = cmd->getCommandType();
        obj["delay"] = cmd->getDelay();
        obj["period"] = cmd->getPeriod();
        obj["linefeed"] = QString(cmd->getLineFeed().toBase64().toStdString().c_str());
        obj["data"] = QString(cmd->getDataRef().toBase64().toStdString().c_str());
        obj["readData"] = QString(cmd->getReadData().toBase64().toStdString().c_str());
        json_arr.push_back(obj);
    }
    QJsonDocument json_doc = QJsonDocument(json_arr);
    save_file.write(json_doc.toJson());
    save_file.close();
}

void MainWindow::onLoadCommands(){
    QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"),"","Text Files (*.txt)");
    if(file_name.isEmpty()){return;}

    QFile load_file(file_name);
    if(!load_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Failed to open file");
        msg->exec();
        return;
    }

    QJsonDocument json_doc = QJsonDocument::fromJson(load_file.readAll());
    if(json_doc.isNull()){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Not valid json document");
        msg->exec();
        return;
    }else if(!json_doc.isArray()){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Not json array");
        msg->exec();
        return;
    }

    QJsonArray json_arr = json_doc.array();
    for(auto json_value : json_arr){
        if(!json_value.isObject()){
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not json object");
            msg->exec();
            continue;
        }
        QJsonObject json_obj = json_value.toObject();
        if(!json_obj.contains("name") || !json_obj.contains("type") || !json_obj.contains("delay") ||
           !json_obj.contains(("period")) || !json_obj.contains("linefeed") || !json_obj.contains("data") ||
           !json_obj.contains("readData")){
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not a valid ulak json object");
            msg->exec();
            continue;
        }

        QString name = json_obj["name"].toString();
        int delay = json_obj["delay"].toInt();
        int period = json_obj["period"].toInt();
        Command::cmd_type cmd_type = static_cast<Command::cmd_type>(json_obj["type"].toInt());
        QByteArray linefeed = QByteArray::fromBase64(json_obj["linefeed"].toString().toLocal8Bit());
        QByteArray data = QByteArray::fromBase64(json_obj["data"].toString().toLocal8Bit());
        QByteArray read_data = QByteArray::fromBase64(json_obj["readData"].toString().toLocal8Bit());

        cmd_area->addButton(name, cmd_type, data, linefeed, delay, period, read_data, this);
    }
}

void MainWindow::onClear(void){
    emit cleared();
}

void MainWindow::drawMenu(void){
    QMenu *settings = menuBar()->addMenu("&Port Settings");
    QAction *port_close = new QAction("&Close Port");
    QAction *port_selection = new QAction("&Port Selection");
    settings->addAction(port_selection);
    settings->addAction(port_close);

    QAction *addCommand    = menuBar()->addAction("&Add Command");
    QAction *save_commands = menuBar()->addAction("&Save Commands");
    QAction *load_commands = menuBar()->addAction("&Load Commands");
    QAction *save_data     = menuBar()->addAction("&Save Data");
    QAction *clear         = menuBar()->addAction("&Clear");

    connect(port_selection, SIGNAL(triggered()), this, SLOT(portSelect()));
    connect(port_close, SIGNAL(triggered()), this, SLOT(portClose()));
    connect(addCommand, SIGNAL(triggered()), this, SLOT(onAddButton()));
    connect(save_data, SIGNAL(triggered()), this, SLOT(onSaveData()));
    connect(save_commands, SIGNAL(triggered()), this, SLOT(onSaveCommands()));
    connect(load_commands, SIGNAL(triggered()), this, SLOT(onLoadCommands()));
    connect(clear, SIGNAL(triggered()), this, SLOT(onClear()));
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(port_handler->commExists())
        port_handler->removePort();
    event->accept();
}
