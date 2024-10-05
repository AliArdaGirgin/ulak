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
#include <QPushButton>
#include <QIcon>
#include <QPixmap>
#include "CommandArea.h"
#include "DataArea.h"
#include "PortSelection.h"
#include "AddButtonWindow.h"

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
        void onProjSettings();
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
        QPixmap connOn;
        QPixmap connOff;
        QAction *port_close;

        void drawMenu(void);
};

#endif
