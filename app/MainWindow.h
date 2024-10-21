#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QGridLayout>
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
#include "DirectArea.h"

class CommRightCornerWidget: public QWidget{
    Q_OBJECT
    public:
        CommRightCornerWidget(QWidget *parent);

    public slots:
        void setState(bool state_, QString name = "");

    private:
        QWidget *parent;
        QGridLayout *layout;
        QLabel *comm_name;
        QLabel *comm_state;
        QIcon  *icon_active;
        QIcon  *icon_passive;
        bool state;
};

class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        void closeEvent(QCloseEvent*) override;
    signals:
        void saved();
        void cleared();
    public slots:
        void setPortState(bool state, QString);
        void portSelect();
        void portClose();

    private slots:
        void onSaveData();
        void onProjSettings();
        void onProjSave();
        void onProjOpen();
        void onProjClose();
        void onClear();
    private:
        QGridLayout *layout;
        CommandArea *cmd_area;
        QScrollArea *cmd_scroll;
        DataArea *data_area;
        QScrollArea *data_scroll;
        DirectArea * direct_area;
        PortSelection *sel;
        AddButtonWindow *addButton;
        PortHandler *port_handler;
        QAction *port_close;
        QString save_file_name;
        QAction *proj_close;
        CommRightCornerWidget *corner_widget;
        void drawMenu(void);
};

#endif
