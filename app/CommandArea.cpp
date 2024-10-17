#include <QWidget>
#include <QFormLayout>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QSerialPort>
#include <QIcon>

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

    add_button = new QPushButton();
    add_button->setIconSize(QSize(100,30));
    add_button->setIcon(QIcon(":/add_command.png"));
    layout = new QFormLayout(this);
    layout->addRow(add_button);
    setLayout(layout);

    connect(add_button, SIGNAL(clicked()), this, SLOT(onAddButton()));
}

void CommandArea::addButton(Command_t cmd, QWidget *parent){
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
    command_pool.insert(empty_index, new Command(cmd, parent));
    layout->addRow(command_pool.at(empty_index));
    connect(command_pool.at(empty_index), SIGNAL(onDelete(Command*)), this, SLOT(deleteButton(Command*)));
    connect(command_pool.at(empty_index), SIGNAL(send(QByteArray,DATA_TYPE)), this, SIGNAL(send(QByteArray,DATA_TYPE)));
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

void CommandArea::dataRead(QByteArray data, DATA_TYPE dtype){
    if(dtype == DATA_TYPE::TX)
        return;
    for(auto &command:command_pool){
        if(command != nullptr && command->getState()==COMMAND_STATE::ACTIVE){
            if(command->getTriggerType() == TRIGGER_TYPE::READ_TRIGGER)
                command->dataRead(data);
        }
    }
}

void CommandArea::onAddButton(){
    add_button_window = new AddButtonWindow();
    connect(
        add_button_window, SIGNAL(onButtonAdded(Command_t, QWidget*)),
            this, SLOT(addButton(Command_t, QWidget*))
    );
    add_button_window->show();
}
void CommandArea::run(){
    QByteArray dt;
    for(auto &command:command_pool){
        // Command not deleted
        if(command != nullptr){
            command->run();
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

void CommandArea::clearCommands(){
    while(!command_pool.isEmpty())
        delete command_pool.takeLast();
}
