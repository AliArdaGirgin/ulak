#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "Command.h"
#include "AddButtonWindow.h"
#include <QMessageBox>
#include <QIcon>
#include "PortHandler_Comm.h"
#include "Conf.h"
#include "DataType.h"

Command::Command(Command_t cmd_, QWidget *parent):
    QWidget(parent),cmd(cmd_)
{
    // create linefeed data acoording to LINEFEED_TYPE
    setLinefeedData(linefeed_data, cmd.linefeed);

    current_match = 0;
    trigger_count = 0;
    current_state = COMMAND_STATE::PASSIVE;

    // set run function
    proc_set(cmd.cmd_type, cmd.trig_type);

    // set counter values to match with command area timer resolution
    delay_counter = cmd.delay/COMMAND_AREA_TIMER_RESOLUTION;
    periodic_counter = cmd.period/COMMAND_AREA_TIMER_RESOLUTION;

    // Set up buttons
    start_button = new QPushButton(cmd.name,this);
    start_button->setFixedSize(150,start_button->height());
    start_button->setIcon(QIcon(":/start_inactive.png") );

    stop_button  = new QPushButton("",this);
    stop_button->setIcon(QIcon(":/stop.png"));
    stop_button->setFixedSize(40, stop_button->height());
    stop_button->setEnabled(false);
    
    settings_button = new QPushButton("",this);
    settings_button->setIcon(QIcon(":/settings.png"));
    settings_button->setFixedSize(40, settings_button->height());
    
    del_button = new QPushButton("",this);
    del_button->setIcon(QIcon(":/delete.png"));
    del_button->setFixedSize(40, del_button->height());

    QHBoxLayout *layout = new QHBoxLayout(this);
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

void Command::update(Command_t cmd_, QWidget *parent)
{
    cmd = cmd_;
    // set run function
    proc_set(cmd.cmd_type, cmd.trig_type);

    setLinefeedData(linefeed_data, cmd.linefeed);
    delay_counter = cmd.delay/COMMAND_AREA_TIMER_RESOLUTION;
    periodic_counter = cmd.period/COMMAND_AREA_TIMER_RESOLUTION;
    current_match = 0;
    trigger_count = 0;

    current_state = COMMAND_STATE::PASSIVE;
    start_button->setEnabled(true); // enable start button
    (void)parent;// surpress unused variable warning
    start_button->setText(cmd.name);
}

void Command::activate(){
    // Warning if there is no open connection
    if(!PortHandler_Base::isOpen()){
        QMessageBox *msg = new QMessageBox();
        msg->setText("No Connection");
        msg->exec();
        delete msg;

    }else{
        current_state  = COMMAND_STATE::ACTIVE;
        start_button->setEnabled(false);
        settings_button->setEnabled(false);
        del_button->setEnabled(false);
        stop_button->setEnabled(true);
        periodic_counter = 0; //cmd.period/COMMAND_AREA_TIMER_RESOLUTION;
        delay_counter  = cmd.delay/COMMAND_AREA_TIMER_RESOLUTION;
        current_match = 0;
        trigger_count = 0;
    }
}

void Command::dataRead(QByteArray &data){
    for(int i=0; i<data.length(); i++){
        if(cmd.read_data[current_match] == data[i]){
            current_match++;
            if(current_match == cmd.read_data.length()){
                trigger_count++;
                current_match = 0;
            }
        }else{
            current_match = 0;
        }
    }
}

void Command::stop(){
    current_state = COMMAND_STATE::PASSIVE;
    start_button->setEnabled(true);
    settings_button->setEnabled(true);
    del_button->setEnabled(true);
    stop_button->setEnabled(false);
    current_match = 0;
    trigger_count = 0;
}

void Command::settings(){
    // Call AddButtonWindow with initials values for current command
    AddButtonWindow *addButton = new AddButtonWindow(nullptr, this);
    connect(addButton, SIGNAL(onButtonAdded(Command_t, QWidget*)),
            this,      SLOT(update(Command_t, QWidget*))
    );
    addButton->show();
}

void Command::setLinefeedData(QByteArray &dt, LINEFEED_TYPE ln){
    switch(ln){
        case LINEFEED_TYPE::NONE:
            break;
        case LINEFEED_TYPE::CR:
            dt.append('\r');
            break;
        case LINEFEED_TYPE::LF:
            dt.append('\n');
            break;
        case LINEFEED_TYPE::CR_LF:
            dt.append("\r\n");
            break;
        case LINEFEED_TYPE::NULL_TERMINATOR:
            dt.append('\0');
            break;
        case LINEFEED_TYPE::MAX:
        default:
            break;
    }
}
bool Command::isTriggerType(){
    return (cmd.trig_type == TRIGGER_TYPE::READ_TRIGGER ||
            cmd.trig_type == TRIGGER_TYPE::READ_TRIGGER_CONT);
}

void Command::sendData(int times){
    while(times-- > 0){
        emit send(cmd.data, DATA_TYPE::TX);
        emit send(linefeed_data, DATA_TYPE::TX);
    }
}
void Command::proc_set(COMMAND_TYPE ct, TRIGGER_TYPE tt){
    proc = &Command::proc_dummy;

    if(ct == COMMAND_TYPE::ONE_SHOT && tt == TRIGGER_TYPE::MANUAL){
        proc = &Command::proc_oneshot_manual;

    }else if(ct == COMMAND_TYPE::ONE_SHOT && tt == TRIGGER_TYPE::READ_TRIGGER){
        proc = &Command::proc_oneshot_readtrigger;

    }else if(ct == COMMAND_TYPE::ONE_SHOT && tt == TRIGGER_TYPE::READ_TRIGGER_CONT){
        proc = &Command::proc_oneshot_readtrigger_cont;

    }else if(ct == COMMAND_TYPE::PERIODIC && tt == TRIGGER_TYPE::MANUAL){
        proc = &Command::proc_periodic_manual;

    }else if(ct == COMMAND_TYPE::PERIODIC && (tt == TRIGGER_TYPE::READ_TRIGGER || tt == TRIGGER_TYPE::READ_TRIGGER_CONT)){
        proc = &Command::proc_periodic_readtrigger;
    }
}

void Command::proc_dummy(){
    qDebug() << "Proc dummy called";
}

void Command::proc_oneshot_manual(){
    if(current_state != COMMAND_STATE::ACTIVE)
        return;
    if(--delay_counter <= 0){
        // send data
        delay_counter = 0;
        sendData(1);
        stop();
     }
}
void Command::proc_oneshot_readtrigger(){
    if(current_state != COMMAND_STATE::ACTIVE)
        return;
    if(trigger_count>0 && --delay_counter <= 0){
        // data send
        delay_counter = 0;
        sendData(1);
        stop();
    }
}
void Command::proc_oneshot_readtrigger_cont(){
    if(current_state != COMMAND_STATE::ACTIVE)
        return;
    if(trigger_count > 0 && --delay_counter <= 0){
        delay_counter = 0;
        sendData(trigger_count);
        trigger_count = 0;
    }
}
void Command::proc_periodic_manual(){
    if(current_state != COMMAND_STATE::ACTIVE)
        return;
    if(--delay_counter <= 0 && --periodic_counter <= 0){
        // data send
        sendData(1);
        delay_counter = 0;
        periodic_counter = cmd.period/COMMAND_AREA_TIMER_RESOLUTION;
    }
}
void Command::proc_periodic_readtrigger(){ // also read_periodic_trigger_cont
    if(current_state != COMMAND_STATE::ACTIVE)
        return;
    if(trigger_count > 0 && --delay_counter <= 0 && --periodic_counter <= 0){
        // data send
        sendData(1);
        delay_counter = 0;
        periodic_counter = cmd.period/COMMAND_AREA_TIMER_RESOLUTION;
    }
}

