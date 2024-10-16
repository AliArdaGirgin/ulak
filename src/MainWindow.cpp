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
#include "Conf.h"

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
    connect(cmd_area,     SIGNAL(send(QByteArray,DATA_TYPE)),  data_area,    SLOT(write(QByteArray,DATA_TYPE))    );
    connect(cmd_area,     SIGNAL(send(QByteArray,DATA_TYPE)),  port_handler, SLOT(write(QByteArray, DATA_TYPE)));
    connect(port_handler, SIGNAL(read(QByteArray,DATA_TYPE)),  data_area,    SLOT(write(QByteArray,DATA_TYPE))    );
    connect(port_handler, SIGNAL(read(QByteArray,DATA_TYPE)),  cmd_area,     SLOT(dataRead(QByteArray,DATA_TYPE)) );
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

    // view type
    json_sett[PROJ_JSON_SETT_VIEWTYPE_STR] = static_cast<int>( ProjectSettings::getDefaultViewType());

    // linefeed
    json_sett[PROJ_JSON_SETT_LINEFEED_STR] = static_cast<int>( ProjectSettings::getDefaultLinefeed());

    // timestamp format
    json_sett[PROJ_JSON_SETT_TIMESTAMP_STR] = static_cast<int>( ProjectSettings::getDefaultTimestampFormat());

    // add
    json_arr.push_back(json_sett);

    // Save commands
    QVector<Command*> *cmds = cmd_area->getCommands();
    for(auto &cmd : *cmds){
        QJsonObject obj = QJsonObject();
        obj[PROJ_JSON_CMD_NAME_STR] = cmd->getName();
        obj[PROJ_JSON_CMD_CMDTYPE_STR] = static_cast<int>(cmd->getCommandType());
        obj[PROJ_JSON_CMD_DELAY_STR] = cmd->getDelay();
        obj[PROJ_JSON_CMD_PERIOD_STR] = cmd->getPeriod();
        obj[PROJ_JSON_CMD_LINEFEED_STR] = static_cast<int>(cmd->getLineFeed());
        obj[PROJ_JSON_CMD_DATA_STR] = QString(cmd->getData().toBase64());
        obj[PROJ_JSON_CMD_DATATAB_STR] = static_cast<int>(cmd->getDataTab());
        obj[PROJ_JSON_CMD_TRIG_STR] = static_cast<int>(cmd->getTriggerType());
        obj[PROJ_JSON_CMD_READDATA_STR] = QString(cmd->getReadData().toBase64());
        obj[PROJ_JSON_CMD_READDATATAB_STR] = static_cast<int>(cmd->getReadDataTab());
        json_arr.push_back(obj);
    }
    QJsonDocument json_doc = QJsonDocument(json_arr);
    save_file.write(json_doc.toJson());
    save_file.close();

    proj_close->setEnabled(true);
}

void MainWindow::onProjOpen(){

    // if already open project close it first
    if(!save_file_name.isEmpty())
        onProjClose();

    save_file_name = QFileDialog::getSaveFileName(this, tr("Open File"), "");
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

        // view type
        if(json_obj.contains(PROJ_JSON_SETT_VIEWTYPE_STR)){
            int t = json_obj[PROJ_JSON_SETT_VIEWTYPE_STR].toInt();
            if(t < 0 || t >= static_cast<int>(VIEW_TYPE::MAX)){
                ProjectSettings::setDefaultViewType(VIEW_TYPE::ASCII);
                data_area->setCurrentTab(VIEW_TYPE::ASCII);
            }else{
                ProjectSettings::setDefaultViewType(static_cast<VIEW_TYPE>(t));
                data_area->setCurrentTab(static_cast<VIEW_TYPE>(t));
            }
        }else{
            ProjectSettings::setDefaultViewType(VIEW_TYPE::ASCII);
            data_area->setCurrentTab(VIEW_TYPE::ASCII);
        }

        // linefeed
        if(json_obj.contains(PROJ_JSON_SETT_LINEFEED_STR)){
            int t = json_obj[PROJ_JSON_SETT_LINEFEED_STR].toInt();
            if(t < 0 || t >=static_cast<int>(LINEFEED_TYPE::MAX)){
                ProjectSettings::setDefaultLinefeed( LINEFEED_TYPE::NONE);
            }else{
                ProjectSettings::setDefaultLinefeed( static_cast<LINEFEED_TYPE>(t));
            }
        }else{
            ProjectSettings::setDefaultLinefeed( LINEFEED_TYPE::NONE);
        }


        // timestamp_format
        if(json_obj.contains(PROJ_JSON_SETT_TIMESTAMP_STR)){
            int t = json_obj[PROJ_JSON_SETT_TIMESTAMP_STR].toInt();
            if(t < 0 || t >= static_cast<int>(TIMESTAMP_FORMAT_TYPE::MAX)){
                ProjectSettings::setDefaultTimestampFormat(TIMESTAMP_FORMAT_TYPE::TIME_MS);
            }else{
                ProjectSettings::setDefaultTimestampFormat( static_cast<TIMESTAMP_FORMAT_TYPE>(t));
            }
        }else{
            ProjectSettings::setDefaultTimestampFormat(TIMESTAMP_FORMAT_TYPE::TIME_MS);
        }
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

        Command_t cmd;

        // name
        if(json_obj.contains(PROJ_JSON_CMD_NAME_STR))
            cmd.name = json_obj[PROJ_JSON_CMD_NAME_STR].toString();
        else{
            continue; // must have a name
        }

        // delay
        if(json_obj.contains(PROJ_JSON_CMD_DELAY_STR)){
            cmd.delay = json_obj[PROJ_JSON_CMD_DELAY_STR].toInt();
            if(cmd.delay < 0)
                cmd.delay = 0;
        }else{
            cmd.delay = 0;
        }

        // period
        if(json_obj.contains(PROJ_JSON_CMD_PERIOD_STR)){
            cmd.period = json_obj["period"].toInt();
            if(cmd.period < COMMAND_AREA_TIMER_RESOLUTION)
                cmd.period = COMMAND_AREA_TIMER_RESOLUTION;
        }else{
            cmd.period = COMMAND_AREA_TIMER_RESOLUTION;
        }

        // command type
        if(json_obj.contains(PROJ_JSON_CMD_CMDTYPE_STR)){
            int t = json_obj[PROJ_JSON_CMD_CMDTYPE_STR].toInt();
            if( t<0 || t >= static_cast<int>(COMMAND_TYPE::MAX))
                continue; // out of range
            else
                cmd.cmd_type = static_cast<COMMAND_TYPE>(t);
        }else{
            continue; // command type must exist
        }

        // data
        if(json_obj.contains(PROJ_JSON_CMD_DATA_STR)){
            cmd.data = QByteArray::fromBase64(json_obj[PROJ_JSON_CMD_DATA_STR].toString().toLocal8Bit());
            if(cmd.data.isEmpty())
                continue; // can't be empty
        }else{
            continue; // data must exist
        }

        // data_tab
        if(json_obj.contains(PROJ_JSON_CMD_DATATAB_STR)){
            int t = json_obj[PROJ_JSON_CMD_DATATAB_STR].toInt();
            if(t < 0 || t >= static_cast<int>(VIEW_TYPE::MAX))
                cmd.last_tab = ProjectSettings::getDefaultViewType();
            else
                cmd.last_tab = static_cast<VIEW_TYPE>(t);
        }else{
            cmd.last_tab = ProjectSettings::getDefaultViewType();
        }

        //linefeed
        if(json_obj.contains(PROJ_JSON_CMD_LINEFEED_STR)){
            int t = json_obj[PROJ_JSON_CMD_LINEFEED_STR].toInt();
            if( t < 0 || t >= static_cast<int>(LINEFEED_TYPE::MAX))
                cmd.linefeed = ProjectSettings::getDefaultLinefeed();
            else
                cmd.linefeed = static_cast<LINEFEED_TYPE>(t);
        }else{
            cmd.linefeed = ProjectSettings::getDefaultLinefeed();
        }

        // trigger type
        if(json_obj.contains(PROJ_JSON_CMD_TRIG_STR)){
            int t = static_cast<int>(json_obj[PROJ_JSON_CMD_TRIG_STR].toInt());
            if( t < 0 || t >= static_cast<int>(TRIGGER_TYPE::MAX)){
                cmd.trig_type = TRIGGER_TYPE::MANUAL;
            }else{
                cmd.trig_type = static_cast<TRIGGER_TYPE>(t);
            }
        }else{
            cmd.trig_type = TRIGGER_TYPE::MANUAL;
        }

        // read data
        if(json_obj.contains(PROJ_JSON_CMD_READDATA_STR)){
            cmd.read_data = QByteArray::fromBase64(json_obj[PROJ_JSON_CMD_READDATA_STR].toString().toLocal8Bit());
            if(cmd.read_data.isEmpty()){
                if(cmd.trig_type == TRIGGER_TYPE::READ_TRIGGER){
                    continue; // for read trigger, read data cant be empty
                }
            }
        }else{
            if(cmd.trig_type == TRIGGER_TYPE::READ_TRIGGER)
                continue; // for read trigger, read data cant be empty
            else
                cmd.read_data = QByteArray();
        }

        // read_data_tab
        if(json_obj.contains(PROJ_JSON_CMD_READDATATAB_STR)){
            int t = json_obj[PROJ_JSON_CMD_READDATATAB_STR].toInt();
            if(t < 0 || t >= static_cast<int>(VIEW_TYPE::MAX))
                cmd.read_last_tab = ProjectSettings::getDefaultViewType();
            else
                cmd.read_last_tab = static_cast<VIEW_TYPE>(t);
        }else{
            cmd.read_last_tab = ProjectSettings::getDefaultViewType();
        }

        cmd_area->addButton(cmd, this);
    }

    load_file.close();
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


