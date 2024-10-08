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
#include "PortHandler.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTimer>
#include <QLabel>
#include "ProjectSettings.h"
#include "DataType.h"

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){

    setWindowTitle("Ulak");
    drawMenu();
    resize(1200,600);
    QWidget *central = new QWidget(this);

    port_handler = new PortHandler(this);

    cmd_area = new CommandArea(port_handler, central);
    cmd_scroll = new QScrollArea(this);
    cmd_scroll->setFixedWidth(350);
    cmd_scroll->setWidgetResizable(true);
    cmd_scroll->setWidget(cmd_area);

    data_area = new DataArea(port_handler,central);
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
    connect(cmd_area,     SIGNAL(send(QByteArray,DataType)),  data_area, SLOT(write(QByteArray,DataType))    );
    connect(port_handler, SIGNAL(read(QByteArray,DataType)),  data_area, SLOT(write(QByteArray,DataType))    );
    connect(port_handler, SIGNAL(read(QByteArray,DataType)),  cmd_area,  SLOT(dataRead(QByteArray,DataType)) );

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timedout()));
    timer->start(100);
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

void MainWindow::onSaveData(void){
    emit saved();
}

void MainWindow::onSaveCommands(void){
    // Open file
    QString file_name = QFileDialog::getSaveFileName(this,tr("Open File"),"");
    if(file_name.isEmpty()){return;}

    QFile save_file(file_name);
    if(!save_file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Failed to open file");
        msg->exec();
        return;
    }

    // Json array to hold saved data
    // [0] -> settings
    // [1..] -> rest is commands
    QJsonArray json_arr = QJsonArray();

    // Save project settings
    QJsonObject json_sett = QJsonObject();
    if(ProjectSettings::getDefaultViewType() == VIEW_TYPE::ASCII)
        json_sett["view_type"] = VIEW_TYPE_ASCII_NAME;
    else
        json_sett["view_type"] = VIEW_TYPE_HEX_NAME;
    json_sett["linefeed"] = static_cast<int>( ProjectSettings::getDefaultLinefeed());
    json_arr.push_back(json_sett);

    // Save commands
    QVector<Command*> *cmds = cmd_area->getCommands();
    for(auto &cmd : *cmds){
        QJsonObject obj = QJsonObject();
        obj["name"] = cmd->getName();
        obj["type"] = cmd->getCommandType();
        obj["delay"] = cmd->getDelay();
        obj["period"] = cmd->getPeriod();
        obj["linefeed"] = static_cast<int>(cmd->getLineFeed());
        obj["data"] = QString(cmd->getData().toBase64());
        obj["readData"] = QString(cmd->getReadData().toBase64());
        obj["read_linefeed"] = static_cast<int>(cmd->getReadLineFeed());
        json_arr.push_back(obj);
    }
    QJsonDocument json_doc = QJsonDocument(json_arr);
    save_file.write(json_doc.toJson());
    save_file.close();
}

void MainWindow::onLoadCommands(){
    QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"),"","All Files(*)");
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
    auto it = json_arr.cbegin();

    // First member in array is settings
    if(it != json_arr.cend()){
        if(!it->isObject()){
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not json object");
            msg->exec();
            return;
        }
        QJsonObject json_obj = it->toObject();
        if(!json_obj.contains("view_type") || !json_obj.contains("linefeed")){
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not a valid ulak json object");
            msg->exec();
            return;
        }
        if(json_obj["view_type"].toString() == VIEW_TYPE_ASCII_NAME){
            ProjectSettings::setDefaultViewType(VIEW_TYPE::ASCII);
            data_area->setCurrentTab(VIEW_TYPE::ASCII);
        }else{ // HEX
            ProjectSettings::setDefaultViewType(VIEW_TYPE::HEX);
            data_area->setCurrentTab(VIEW_TYPE::HEX);
        }
        ProjectSettings::setDefaultLinefeed(
            static_cast<LINEFEED_TYPE>(json_obj["linefeed"].toInt())
        );
    }

    // Rest is commands
    it += 1;
    for(;it!=json_arr.cend(); it++){
        if(!it->isObject()){
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not json object");
            msg->exec();
            continue;
        }
        QJsonObject json_obj = it->toObject();
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
        QByteArray data = QByteArray::fromBase64(json_obj["data"].toString().toLocal8Bit());
        LINEFEED_TYPE linefeed = static_cast<LINEFEED_TYPE>(json_obj["linefeed"].toInt());
        QByteArray read_data = QByteArray::fromBase64(json_obj["readData"].toString().toLocal8Bit());
        LINEFEED_TYPE read_linefeed = static_cast<LINEFEED_TYPE>(json_obj["read_linefeed"].toInt());

        cmd_area->addButton(name, cmd_type, data, 0, linefeed, delay, period, read_data, read_linefeed, 0,this);
    }
}

void MainWindow::onClear(void){
    emit cleared();
}

void MainWindow::drawMenu(void){
    connOn = QIcon("../img/connOn.img").pixmap( QSize(16,16) );
    connOff = QIcon("../img/connOff.img").pixmap( QSize(16,16) );
    connState = new QLabel("");
    connState->setPixmap(connOff);
    connState->setStyleSheet("margin-right: 2px; margin-left: 2px;");
    menuBar()->setCornerWidget(connState, Qt::TopLeftCorner);

    QMenu *port_settings = menuBar()->addMenu("&Port Settings");
    port_close = new QAction("&Close Port");
    port_close->setEnabled(false);
    QAction *port_selection = new QAction("&Port Selection");
    port_settings->addAction(port_selection);
    port_settings->addAction(port_close);

    QMenu *proj = menuBar()->addMenu("&Project");
    QAction *proj_settings  = new QAction("&Settings");
    QAction *proj_save = new QAction("&Save");
    QAction *proj_load = new QAction("&Load");
    proj->addAction(proj_settings);
    proj->addAction(proj_save);
    proj->addAction(proj_load);

    QMenu *log = menuBar()->addMenu("&Log");
    QAction *save_data     = new QAction("&Save");
    QAction *clear         = new QAction("&Clear");
    log->addAction(save_data);
    log->addAction(clear);

    connect(port_selection, SIGNAL(triggered()), this, SLOT(portSelect()));
    connect(port_close, SIGNAL(triggered()), this, SLOT(portClose()));
    connect(proj_settings, SIGNAL(triggered()), this, SLOT(onProjSettings()));
    connect(proj_save, SIGNAL(triggered()), this, SLOT(onSaveCommands()));
    connect(proj_load, SIGNAL(triggered()), this, SLOT(onLoadCommands()));
    connect(save_data, SIGNAL(triggered()), this, SLOT(onSaveData()));
    connect(clear, SIGNAL(triggered()), this, SLOT(onClear()));
}

void MainWindow::timedout(){
    if(PortHandler::commExists()){
        connState->setPixmap(connOn);
        port_close->setEnabled(true);
    }
    else{
        connState->setPixmap(connOff);
        port_close->setEnabled(false);
    }
}

void MainWindow::onProjSettings(){
    ProjectSettings *proj_settings = new ProjectSettings();
    connect(proj_settings, SIGNAL(viewTypeUpdated(VIEW_TYPE)), data_area, SLOT(setCurrentTab(VIEW_TYPE)));
    proj_settings->show();
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(port_handler->commExists())
        port_handler->removePort();
    event->accept();
}
