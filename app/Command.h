#ifndef COMMAND_H
#define COMMAND_H

#include <QPushButton>
#include <QObject>
#include "DataType.h"
#include "test_defs.h"

class Command:public QWidget{
    Q_OBJECT
    friend Test_Command;
    public:
        Command(Command_t cmd_, QWidget *parent = 0);
        QString getName(){ return start_button->text();}
        COMMAND_TYPE getCommandType(){return cmd.cmd_type;}
        QByteArray  getData(){return cmd.data;}
        QByteArray  getDataWithLinefeed(){return cmd.data + linefeed_data;}
        VIEW_TYPE getDataTab(){ return cmd.last_tab;}
        LINEFEED_TYPE getLineFeed(){ return cmd.linefeed;}
        int getDelay(){return cmd.delay;}
        int getPeriod(){return cmd.period;}
        QByteArray getReadData(){ return cmd.read_data;}
        QByteArray getReadDataWithLinefeed(){ return cmd.read_data + read_linefeed_data;}
        VIEW_TYPE getReadDataTab(){ return cmd.read_last_tab;}
        COMMAND_STATE getState(){return current_state;}
        void triggered(void){--trigger_count;}
        bool isTriggered(void){return trigger_count <= 0 ? false : true;}
        TRIGGER_TYPE getTriggerType(){return cmd.trig_type;}
        bool isTriggerType();
        std::function<void(Command*)> proc = &Command::proc_dummy;

    public slots:
        void activate();
        void stop();
        void settings();
        void del(){emit onDelete(this);}
        void update(Command_t cmd, QWidget *parent=0);
        void dataRead(QByteArray &data);

    signals:
        void onDelete(Command*);
        void send(QByteArray, DATA_TYPE);

    private:
        Command_t cmd;
        COMMAND_STATE current_state;
        QByteArray linefeed_data;
        QByteArray read_linefeed_data;
        QPushButton *start_button;
        QPushButton *stop_button;
        QPushButton *settings_button;
        QPushButton *del_button;
        int current_match;
        int trigger_count;
        int delay_counter;
        int periodic_counter;
        void setLinefeedData(QByteArray &dt, LINEFEED_TYPE ln);
        void sendData(int times);
        void proc_set(COMMAND_TYPE ct, TRIGGER_TYPE tt);
        void proc_dummy();
        void proc_oneshot_manual();
        void proc_oneshot_readtrigger();
        void proc_oneshot_readtrigger_cont();
        void proc_periodic_manual();
        void proc_periodic_readtrigger();
};

#endif
