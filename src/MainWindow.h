#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QGroupBox>
#include <QScrollArea>
#include <QString>
#include <QCloseEvent>
#include <QTimer>
#include <QLabel>
#include "CommandArea.h"
#include "DataArea.h"
#include "PortSelection.h"
#include "AddButtonWindow.h"
#include "Command.h"
#include "DataType.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        void closeEvent(QCloseEvent*) override;
    signals:
        void saved();
        void cleared();
    public slots:
        void portSelect();
        void portClose();

    private slots:
        void onAddButton();
        void onSaveData();
        void onSaveCommands();
        void onLoadCommands();
        void onClear();
        void timedout();
    private:
        CommandArea *cmd_area;
        QScrollArea *cmd_scroll;
        DataArea *data_area;
        QScrollArea *data_scroll;
        PortSelection *sel;
        AddButtonWindow *addButton;
        PortHandler *port_handler;
        QTimer *timer;
        QLabel *connState;
        void drawMenu(void);
};

#endif
