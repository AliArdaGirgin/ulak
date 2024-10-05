#ifndef COMMAND_H
#define COMMAND_H

#include <QPushButton>
#include <QObject>

class Command:public QWidget{
    Q_OBJECT
    public:
        enum cmd_type{SINGLE, PERIODIC, READ_TRIGGER};
        enum state{ACTIVE, FROZEN, PASSIVE};
        Command(QString name,cmd_type ctype_in,
                QByteArray data_in, int last_tab_,  QByteArray linefeed_in,int delay_in,
                int period_in = 0, QByteArray read_data_in = QByteArray(), int read_last_tab_ =0,
                QWidget *parent = 0);
        QString getName();
        cmd_type getCommandType();
        QByteArray  getData();
        int getDataTab(){ return last_tab;}
        QByteArray& getDataRef();
        QByteArray& getLineFeed();
        int getDelay();
        int getPeriod();
        QByteArray getReadData();
        int getReadDataTab(){ return read_last_tab;}
        state getState();
        void triggered(void);
        bool isTriggered(void);
        int delay_counter;
        int periodic_counter;
    public slots:
        void activate();
        void stop();
        void settings();
        void del();
        void update(QString name,Command::cmd_type ctype_in, 
                QByteArray data_in, int last_tab, QByteArray linefeed_in,int delay_in,
                int period_in = 0, QByteArray read_data_in = QByteArray(), int read_last_tab =0,
                QWidget *parent=0);
        void dataRead(QByteArray &data);

    signals:
        void onDelete(Command*);

    private:
        cmd_type    ctype;
        QByteArray  data;
        QByteArray  linefeed;
        int delay;
        int period;
        QByteArray  read_data;
        state current_state;
        QPushButton *start_button;
        QPushButton *stop_button;
        QPushButton *settings_button;
        QPushButton *del_button;
        int current_match;
        int trigger_count;
        int last_tab;
        int read_last_tab;
};

#endif
