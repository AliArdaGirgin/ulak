#include <QString>
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include "Command.h"
#include "AddButtonWindow.h"
#include <QMessageBox>
#include <QIcon>
#include "PortHandler.h"
#include "Conf.h"
#include "DataType.h"

Command::Command(QString name,cmd_type ctype_,
                 QByteArray data_, int last_tab_,
                 LINEFEED_TYPE linefeed_, int delay_, int period_,
                 QByteArray read_data_, LINEFEED_TYPE read_linefeed_,
                 int read_last_tab_, QWidget *parent):
                    QWidget(parent),ctype(ctype_),data(data_), last_tab(last_tab_),
                    linefeed(linefeed_), delay(delay_), period(period_), read_data(read_data_),
                    read_linefeed(read_linefeed_), read_last_tab(read_last_tab_)
{
    setLinefeedData(linefeed_data, linefeed);
    setLinefeedData(read_linefeed_data, read_linefeed);
    current_match = 0;
    trigger_count = 0;
    current_state = Command::PASSIVE;
    delay_counter = COMMAND_AREA_TIMER_RESOLUTION;
    periodic_counter = COMMAND_AREA_TIMER_RESOLUTION;

    QHBoxLayout *layout = new QHBoxLayout(this);

    // Set up buttons
    start_button = new QPushButton(name,this);
    start_button->setFixedSize(150,start_button->height());
    start_button->setIcon(QIcon("../img/start_inactive.img") );

    stop_button  = new QPushButton("",this);
    stop_button->setIcon(QIcon("../img/stop.img"));
    stop_button->setFixedSize(40, stop_button->height());
    stop_button->setEnabled(false);
    
    settings_button = new QPushButton("",this);
    settings_button->setIcon(QIcon("../img/settings.img"));
    settings_button->setFixedSize(40, settings_button->height());
    
    del_button = new QPushButton("",this);
    del_button->setIcon(QIcon("../img/delete.img"));
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

void Command::update(QString name,Command::cmd_type ctype_,
        QByteArray data_, int last_tab_, LINEFEED_TYPE linefeed_,
        int delay_, int period_, QByteArray read_data_,
        LINEFEED_TYPE read_linefeed_,
        int read_last_tab_, QWidget *parent)
{
    ctype = ctype_;
    data = data_;
    last_tab= last_tab_;
    linefeed = linefeed_;
    setLinefeedData(linefeed_data, linefeed);
    delay = delay_;
    delay_counter =COMMAND_AREA_TIMER_RESOLUTION;
    period = period_;
    periodic_counter = 0;
    read_data = read_data_;
    read_linefeed = read_linefeed_;
    setLinefeedData(read_linefeed_data, read_linefeed);
    read_last_tab = read_last_tab_;
    current_state = Command::PASSIVE;
    start_button->setEnabled(true); // enable start button
    (void)parent;// surpress unused variable warning
    start_button->setText(name);
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
        settings_button->setEnabled(false);
        del_button->setEnabled(false);
        stop_button->setEnabled(true);
        periodic_counter = 0;
        delay_counter  = COMMAND_AREA_TIMER_RESOLUTION;
    }
}

void Command::dataRead(QByteArray &data){
    for(int i=0; i<data.length(); i++){
        if(read_data[current_match] == data[i]){
            current_match++;
            if(current_match == read_data.length()){
                trigger_count++;
                current_match = 0;
            }
        }else{
            current_match = 0;
        }
    }
}

void Command::stop(){
    current_state = Command::PASSIVE;
    start_button->setEnabled(true);
    settings_button->setEnabled(true);
    del_button->setEnabled(true);
    stop_button->setEnabled(false);
}

void Command::settings(){
    // Call AddButtonWindow with initials values for current command
    AddButtonWindow *addButton = new AddButtonWindow(nullptr, this);
    connect(addButton, SIGNAL(onButtonAdded(QString, Command::cmd_type, QByteArray, int, LINEFEED_TYPE, int, int, QByteArray, LINEFEED_TYPE, int, QWidget*)),
            this,               SLOT(update(QString, Command::cmd_type, QByteArray, int, LINEFEED_TYPE, int, int, QByteArray, LINEFEED_TYPE, int, QWidget*))
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
        case LINEFEED_TYPE::CR_LF:
            dt.append("\r\n");
        break;
        case LINEFEED_TYPE::NULL_TERMINATOR:
            dt.append('\0');
        break;
    }
}
