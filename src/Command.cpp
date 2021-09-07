#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "Command.h"
#include "AddButtonWindow.h"
#include <QMessageBox>
#include "PortHandler.h"

Command::Command(QString name,cmd_type ctype_in, 
                 QByteArray data_in, QByteArray linefeed_in,int delay_in, int period_in,
                 QByteArray read_data_in, QWidget *parent):
                    QWidget(parent),ctype(ctype_in),data(data_in),linefeed(linefeed_in),
                    delay(delay_in), period(period_in), read_data(read_data_in)
{
    current_match = 0;
    trigger_count = 0;
    current_state = Command::PASSIVE;
    delay_counter = delay/CommandArea::TIMER_RES;
    periodic_counter = period/CommandArea::TIMER_RES;

    QHBoxLayout *layout = new QHBoxLayout(this);

    // Set up buttons
    start_button = new QPushButton(name,this);
    start_button->setFixedSize(150,start_button->height());

    stop_button  = new QPushButton("Stop",this);
    stop_button->setFixedSize(40, stop_button->height());
    
    settings_button = new QPushButton("Settings",this);
    settings_button->setFixedSize(40, settings_button->height());
    
    del_button = new QPushButton("Delete",this);
    del_button->setFixedSize(40, del_button->height());

    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(start_button);
    layout->addWidget(stop_button);
    layout->addWidget(settings_button);
    layout->addWidget(del_button);
    setLayout(layout);

    connect(start_button,    SIGNAL(clicked()),  this, SLOT(activate()));
    connect(stop_button,     SIGNAL(clicked()),  this, SLOT(stop()));
    connect(settings_button, SIGNAL(clicked()),  this, SLOT(settings()));
    connect(del_button,      SIGNAL(clicked()),  this, SLOT(del()));
}

void Command::update(QString name,Command::cmd_type ctype_in, 
        QByteArray data_in, QByteArray linefeed_in,
        int delay_in, int period_in, QByteArray read_data_in, QWidget *parent)
{
    ctype = ctype_in;
    data = data_in;
    linefeed = linefeed_in;
    delay = delay_in;
    delay_counter = delay/CommandArea::TIMER_RES;
    period = period_in;
    periodic_counter = 0;
    read_data = read_data_in;
    current_state = Command::PASSIVE;
    start_button->setEnabled(true); // enable start button
    (void)parent;// surpress unused variable warning
    start_button->setText(name);
}

void Command::triggered(void){
    trigger_count--;
}

bool Command::isTriggered(void){
    return trigger_count <= 0 ? false : true;
}

void Command::activate(){
    // Warning if there is no open connection
    if(!PortHandler::commExists()){
        QMessageBox *msg = new QMessageBox();
        msg->setText("No Connection");
        msg->exec();
    }else{
        current_state  = Command::ACTIVE;
        start_button->setEnabled(false);
        periodic_counter = 0;
        delay_counter  = delay/CommandArea::TIMER_RES;
    }
}

void Command::dataRead(QByteArray &data){
    int min = 0;
    int remaining = read_data.length()-current_match; 
    if(remaining <= data.length())
        min = remaining;
    else
        min = data.length();
    
    for(int i=0; i<min; i++){
        if(read_data[current_match] == data[i]){
            current_match++;
            if(current_match == read_data.length()){
                trigger_count++;
                current_match = 0;
            }
        }else{
            current_match = 0;
            break;
        }
    }
}

void Command::stop(){
    current_state = Command::PASSIVE;
    start_button->setEnabled(true);
}

void Command::settings(){
    // Call AddButtonWindow with initials values for current command
    AddButtonWindow *addButton = new AddButtonWindow(this);
    connect(addButton, SIGNAL(onButtonAdded(QString,Command::cmd_type,QByteArray,QByteArray,int,int,QByteArray,QWidget*)),
            this,      SLOT(update(QString,Command::cmd_type,QByteArray,QByteArray,int,int,QByteArray,QWidget*))
    );
    addButton->show();
}

void Command::del(){
    // Delete signal, Command must be deleted from CommandArea
    emit onDelete(this);
}

// Getters
QString Command::getName(){
    return start_button->text();
}

QByteArray Command::getData(){
    return data;
}

int Command::getDelay(){
    return delay;
}

int Command::getPeriod(){
    return period;
}

QByteArray Command::getReadData(){
    return read_data;
}

Command::cmd_type Command::getCommandType(){
    return ctype;
}

Command::state Command::getState(){
    return current_state;
}

QByteArray& Command::getDataRef(){
    return data;
}

QByteArray& Command::getLineFeed(){
    return linefeed;
}
