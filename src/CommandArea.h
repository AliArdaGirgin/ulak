#ifndef COMMAND_AREA_H
#define COMMAND_AREA_H

#include <QWidget>
#include <QVector>
#include <QFormLayout>
#include <QTimer>
#include "Command.h"
#include "DataArea.h"
#include "PortHandler.h"
#include "DataType.h"

class CommandArea:public QWidget{
    Q_OBJECT
    public:
        static const int TIMER_RES;
        CommandArea(PortHandler *pHandler, QWidget *parent = 0);
        QVector<Command*>* getCommands();
        int getCommandCount();

    public slots:
        void addButton(QString name, Command::cmd_type type,
                QByteArray data, QByteArray linefeed, int delay, int period,
                QByteArray read_data, QWidget *parent);
        void dataRead(QByteArray &data, DataType dtype);
        void deleteButton(Command *cmd);
    signals:
        void send(QByteArray &data, DataType dtype);

    private slots:
        void run();
    private:
        QVector<Command*> command_pool;
        QFormLayout *layout;
        QTimer      *timer;
        PortHandler *port_handler;

};
#endif
