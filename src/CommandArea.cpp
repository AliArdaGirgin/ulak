#include <QWidget>
#include <QFormLayout>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QSerialPort>
#include "CommandArea.h"
#include "Command.h"
#include "PortHandler.h"
#include "DataArea.h"
#include "DataType.h"

const int CommandArea::TIMER_RES=100;

CommandArea::CommandArea(PortHandler *pHandler, QWidget *parent):QWidget(parent),port_handler(pHandler){
    timer  = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(run()));
    timer->start(TIMER_RES);
    layout = new QFormLayout(this);
    setLayout(layout);
}

void CommandArea::addButton(QString name, Command::cmd_type type,
                QByteArray data, QByteArray linefeed, int delay, int period,
                QByteArray read_data, QWidget *parent){
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
    command_pool.insert(empty_index, new Command(name,type, data, linefeed, delay, period, read_data, parent));
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

void CommandArea::dataRead(QByteArray &data, DataType dtype){
    if(dtype == DataType::TX)
        return;
    for(auto &command:command_pool){
        if(command != nullptr && command->getState()==Command::ACTIVE){
            if(command->getCommandType() == Command::READ_TRIGGER)
                command->dataRead(data);
        }
    }
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
                           command->periodic_counter = command->getPeriod()/CommandArea::TIMER_RES;
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
