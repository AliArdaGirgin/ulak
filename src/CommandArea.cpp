#include <QWidget>
#include <QFormLayout>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QSerialPort>
#include "AddButtonWindow.h"
#include "CommandArea.h"
#include "Command.h"
#include "PortHandler.h"
#include "DataType.h"
#include "Conf.h"

CommandArea::CommandArea(PortHandler *pHandler, QWidget *parent):QWidget(parent),port_handler(pHandler){
    timer  = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(COMMAND_AREA_TIMER_RESOLUTION);

    add_button = new QPushButton("Add");
    layout = new QFormLayout(this);
    layout->addRow(add_button);
    setLayout(layout);

    connect(add_button, SIGNAL(clicked()), this, SLOT(onAddButton()));
}

void CommandArea::addButton(QString name, Command::cmd_type type,
                QByteArray data, int last_tab, QByteArray linefeed, int delay, int period,
                QByteArray read_data, int read_last_tab, QWidget *parent){
    // fill empty index if exist , appends otherwise
    int empty_index = command_pool.size();
    int cnt = 0;
    for(auto &slot:command_pool){
        if(slot == nullptr){
            empty_index = cnt;
            break;
        }
        cnt++;
    }
    command_pool.insert(empty_index, new Command(name,type, data, last_tab, linefeed, delay, period, read_data, read_last_tab, parent));
    layout->addRow(command_pool.at(empty_index));
    connect(command_pool.at(empty_index), SIGNAL(onDelete(Command*)), this, SLOT(deleteButton(Command*)));
}

void CommandArea::deleteButton(Command *cmd_in){
    int index = 0;
    for(auto &cmd:command_pool){
        if(cmd == cmd_in){
            delete command_pool.takeAt(index);
            break;
        }
        index++;
    }
}

void CommandArea::dataRead(QByteArray data, DataType dtype){
    if(dtype == DataType::TX)
        return;
    for(auto &command:command_pool){
        if(command != nullptr && command->getState()==Command::ACTIVE){
            if(command->getCommandType() == Command::READ_TRIGGER)
                command->dataRead(data);
        }
    }
}

void CommandArea::onAddButton(){
    add_button_window = new AddButtonWindow();
    connect(add_button_window, SIGNAL(onButtonAdded(QString,Command::cmd_type,QByteArray,int,QByteArray,int,int,QByteArray,int,QWidget*)),
            this,  SLOT(addButton(QString,Command::cmd_type,QByteArray,int,QByteArray,int,int,QByteArray,int,QWidget*))    );
    add_button_window->show();
}
void CommandArea::run(){
    QByteArray dt;
    for(auto &command:command_pool){
        // Command not deleted and active
        if(command != nullptr && command->getState()==Command::ACTIVE){
            dt = command->getDataRef();
            dt.append(command->getLineFeed());
            switch(command->getCommandType()){
                case Command::SINGLE:
                    if(command->delay_counter == 0){
                        port_handler->write(command->getDataRef());
                        port_handler->write(command->getLineFeed());
                        emit send(dt, DataType::TX);
                        command->stop();
                    }else{
                        command->delay_counter--;
                    }
                    break;

                case Command::PERIODIC:
                    if(command->delay_counter == 0){
                       if(command->periodic_counter == 0){
                           port_handler->write(command->getDataRef());
                           port_handler->write(command->getLineFeed());
                           emit send(dt, DataType::TX);
                           command->periodic_counter = command->getPeriod()/COMMAND_AREA_TIMER_RESOLUTION;
                       }else{
                           command->periodic_counter--;
                       }
                    }else{
                        command->delay_counter--;
                    }
                    break;
                case Command::READ_TRIGGER:
                    if(command->isTriggered()){
                        if(command->delay_counter == 0){
                            port_handler->write(command->getDataRef());
                            port_handler->write(command->getLineFeed());
                            emit send(dt, DataType::TX);
                            command->triggered();
                        }else{
                            command->delay_counter--;
                        }
                    }
                    break;
                default:
                    //Error
                    break;
            }
        }
    }
}

QVector<Command*>* CommandArea::getCommands(){
    return &command_pool;
}

int CommandArea::getCommandCount(){
    int total = 0;
    // Command might be empty
    for(auto &c:command_pool){
        if(c!=nullptr)
            total++;
    }
    return total;
}
