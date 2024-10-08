#ifndef COMMAND_H
#define COMMAND_H

#include <QPushButton>
#include <QObject>
#include "DataType.h"

class Command:public QWidget{
    Q_OBJECT
    public:
        enum cmd_type{SINGLE, PERIODIC, READ_TRIGGER};
        enum state{ACTIVE, FROZEN, PASSIVE};
        Command(QString name,cmd_type ctype_in,
                QByteArray data_in, int last_tab_,
                LINEFEED_TYPE linefeed_in, int delay_in,
                int period_in = 0, QByteArray read_data_in = QByteArray(),
                LINEFEED_TYPE read_linefeed_in = LINEFEED_TYPE::NONE,
                int read_last_tab_ =0, QWidget *parent = 0);
        QString getName(){ return start_button->text();}
        cmd_type getCommandType(){return ctype;}
        QByteArray  getData(){return data;}
        QByteArray  getDataWithLinefeed(){return data + linefeed_data;}
        int getDataTab(){ return last_tab;}
        LINEFEED_TYPE getLineFeed(){ return linefeed;}
        int getDelay(){return delay;}
        int getPeriod(){return period;}
        QByteArray getReadData(){ return read_data;}
        QByteArray getReadDataWithLinefeed(){ return read_data + read_linefeed_data;}
        int getReadDataTab(){ return read_last_tab;}
        LINEFEED_TYPE getReadLineFeed(){return read_linefeed;}
        state getState(){return current_state;}
        void triggered(void){--trigger_count;}
        bool isTriggered(void){return trigger_count <= 0 ? false : true;}
        int delay_counter;
        int periodic_counter;

    public slots:
        void activate();
        void stop();
        void settings();
        void del(){emit onDelete(this);}
        void update(QString name,Command::cmd_type ctype_in, 
                QByteArray data_in, int last_tab,
                LINEFEED_TYPE read_linefeedi_in, int delay_in,
                int period_in = 0, QByteArray read_data_in = QByteArray(),
                    LINEFEED_TYPE read_linefeed_in = LINEFEED_TYPE::NONE,
                int read_last_tab =0, QWidget *parent=0);
        void dataRead(QByteArray &data);

    signals:
        void onDelete(Command*);

    private:
        cmd_type    ctype;
        QByteArray  data;
        LINEFEED_TYPE  linefeed;
        QByteArray linefeed_data;
        int delay;
        int period;
        QByteArray  read_data;
        LINEFEED_TYPE read_linefeed;
        QByteArray read_linefeed_data;
        state current_state;
        QPushButton *start_button;
        QPushButton *stop_button;
        QPushButton *settings_button;
        QPushButton *del_button;
        int current_match;
        int trigger_count;
        int last_tab;
        int read_last_tab;
        void setLinefeedData(QByteArray &dt, LINEFEED_TYPE ln);
};

#endif
