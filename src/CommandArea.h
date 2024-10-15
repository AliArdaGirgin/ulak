#ifndef COMMAND_AREA_H
#define COMMAND_AREA_H

#include <QWidget>
#include <QVector>
#include <QFormLayout>
#include <QTimer>
#include "AddButtonWindow.h"
#include "Command.h"
#include "PortHandler.h"
#include "DataType.h"

class CommandArea:public QWidget{
    Q_OBJECT
    public:
        CommandArea(PortHandler *pHandler, QWidget *parent = 0);
        QVector<Command*>* getCommands();
        int getCommandCount();
        void clearCommands();
    public slots:
        void addButton(Command_t cmd, QWidget *parent);
        void dataRead(QByteArray data, DATA_TYPE dtype);
        void deleteButton(Command *cmd);
    signals:
        void send(QByteArray data, DATA_TYPE dtype);

    private slots:
        void run();
        void onAddButton();
    private:
        QVector<Command*> command_pool;
        QFormLayout *layout;
        QTimer      *timer;
        PortHandler *port_handler;
        QPushButton *add_button;
        AddButtonWindow *add_button_window;

};
#endif
