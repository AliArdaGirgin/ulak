#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QScrollArea>
#include <QByteArray>
#include <QDebug>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QMessageBox>
#include <QTimer>

#include "MainWindow.h"
#include "PortSelection.h"
#include "CommandArea.h"
#include "DataArea.h"
#include "PortHandler.h"
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
    connect(port_handler, SIGNAL(portStateChanged(bool,QString)), corner_widget, SLOT(setState(bool,QString)));
    connect(port_handler, SIGNAL(portStateChanged(bool,QString)), this, SLOT(setPortState(bool,QString)));
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

void MainWindow::onProjSave(void){

    // Open file dialog, if already not set
    if(save_file_name.isEmpty()){
        save_file_name = QFileDialog::getSaveFileName(this,tr("Open File"),"");
        if(save_file_name.isEmpty()){return;}
    }

    QFile save_file(save_file_name);
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
        obj["dataTab"] = cmd->getDataTab();
        obj["readData"] = QString(cmd->getReadData().toBase64());
        obj["readDataTab"] = cmd->getReadDataTab();
        obj["read_linefeed"] = static_cast<int>(cmd->getReadLineFeed());
        json_arr.push_back(obj);
    }
    QJsonDocument json_doc = QJsonDocument(json_arr);
    save_file.write(json_doc.toJson());
    save_file.close();

}

void MainWindow::onProjOpen(){

    // if already open project close it first
    if(!save_file_name.isEmpty())
        onProjClose();

    save_file_name = QFileDialog::getSaveFileName(this,tr("Open File"),"");
    if(save_file_name.isEmpty()){return;}

    QFile load_file(save_file_name);
    if(!load_file.open(QIODevice::ReadWrite | QIODevice::Text)){
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
            msg->setText("Not a valid ulak settings json object");
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
        // Check if required vairbales exists in json object
        if(!json_obj.contains("name") || !json_obj.contains("type") || !json_obj.contains("delay") ||
           !json_obj.contains(("period")) || !json_obj.contains("linefeed") || !json_obj.contains("data") ||
            !json_obj.contains("readData") || !json_obj.contains("dataTab") || !json_obj.contains("readDataTab"))
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("Not a valid ulak json object");
            msg->exec();
            continue;
        }

        //checks complete, add command to command area
        QString name = json_obj["name"].toString();
        int delay = json_obj["delay"].toInt();
        int period = json_obj["period"].toInt();
        Command::cmd_type cmd_type = static_cast<Command::cmd_type>(json_obj["type"].toInt());
        QByteArray data = QByteArray::fromBase64(json_obj["data"].toString().toLocal8Bit());
        int dataTab = json_obj["dataTab"].toInt();
        LINEFEED_TYPE linefeed = static_cast<LINEFEED_TYPE>(json_obj["linefeed"].toInt());
        QByteArray read_data = QByteArray::fromBase64(json_obj["readData"].toString().toLocal8Bit());
        int readDataTab = json_obj["readDataTab"].toInt();
        LINEFEED_TYPE read_linefeed = static_cast<LINEFEED_TYPE>(json_obj["read_linefeed"].toInt());

        cmd_area->addButton(name, cmd_type, data, dataTab, linefeed, delay, period, read_data, read_linefeed, readDataTab, this);
    }

    proj_close->setEnabled(true);
}

void MainWindow::onProjClose(){
    cmd_area->clearCommands();
    ProjectSettings::setParamatersToDefault();
    data_area->setCurrentTab(VIEW_TYPE::ASCII);
    save_file_name.clear();
    proj_close->setEnabled(false);
}
void MainWindow::onClear(void){
    emit cleared();
}

void MainWindow::drawMenu(void){
    corner_widget = new CommRightCornerWidget(this);
    menuBar()->setCornerWidget(corner_widget, Qt::TopRightCorner);

    QMenu *proj = menuBar()->addMenu("&Project");
    QAction *proj_open = new QAction("&Open");
    QAction *proj_settings  = new QAction("&Settings");
    QAction *proj_save = new QAction("&Save");
    proj_close = new QAction("&Close");
    proj_close->setEnabled(false);
    proj->addAction(proj_open);
    proj->addAction(proj_settings);
    proj->addAction(proj_save);
    proj->addAction(proj_close);

    QMenu *port_settings = menuBar()->addMenu("&Port Settings");
    port_close = new QAction("&Close Port");
    port_close->setEnabled(false);
    QAction *port_selection = new QAction("&Port Selection");
    port_settings->addAction(port_selection);
    port_settings->addAction(port_close);


    QMenu *log = menuBar()->addMenu("&Log");
    QAction *save_data     = new QAction("&Save");
    QAction *clear         = new QAction("&Clear");
    log->addAction(save_data);
    log->addAction(clear);

    connect(proj_settings, SIGNAL(triggered()), this, SLOT(onProjSettings()));
    connect(proj_save, SIGNAL(triggered()), this, SLOT(onProjSave()));
    connect(proj_open, SIGNAL(triggered()), this, SLOT(onProjOpen()));
    connect(proj_close, SIGNAL(triggered()), this, SLOT(onProjClose()));

    connect(port_selection, SIGNAL(triggered()), this, SLOT(portSelect()));
    connect(port_close, SIGNAL(triggered()), this, SLOT(portClose()));

    connect(save_data, SIGNAL(triggered()), this, SLOT(onSaveData()));
    connect(clear, SIGNAL(triggered()), this, SLOT(onClear()));
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

CommRightCornerWidget::CommRightCornerWidget(QWidget* parent_): parent(parent_){
    state = 0;
    layout = new QGridLayout();
    comm_name = new QLabel("None");

    comm_state = new QLabel("");
    icon_active  = new QIcon(":/conn_on.png");
    icon_passive = new QIcon(":/conn_off.png");
    comm_state->setPixmap(icon_passive->pixmap(QSize(16,16)));
    comm_state->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->addWidget(comm_name, 0, 0);
    layout->addWidget(comm_state, 0, 1);
    setLayout(layout);
}

void CommRightCornerWidget::setState(bool state_, QString name){
    state = state_;
    if(state){
        comm_name->setText(name);
        comm_state->setPixmap(icon_active->pixmap(QSize(16,16)));
    }else{
        comm_name->setText("None");
        comm_state->setPixmap(icon_passive->pixmap(QSize(16,16)));
    }

    // resize mainwindow so comm_name and icon fits properly
    // we need to change the size a little for update to occur
    // @TODO: find a better way to do this
    QSize s = parent->size();
    s.rwidth() += 1;
    parent->resize(s);
}

void MainWindow::setPortState(bool state, QString name){
    (void) name;
    if(state)
        port_close->setEnabled(true);
    else
        port_close->setEnabled(false);
}


