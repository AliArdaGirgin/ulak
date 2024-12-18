#ifndef PORT_COMM_SELECTION_H
#define PORT_COMM_SELECTION_H

#include <QWidget>
#include <QSerialPort>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QObject>
#include "CommandArea.h"
#include "DataArea.h"
#include "PortHandler_Base.h"

class PortSelection_Comm : public QWidget{
    Q_OBJECT
    public:
        PortSelection_Comm(QWidget *parent = 0);
        bool isOpen();
    
    signals:
        void opened(PortHandler_Base *);
    
    private slots:
        void portSelected();
        void onCancelled();
    private:
        QSerialPort *port;
        QComboBox *cbox_port;
        QComboBox *cbox_baudrate;
        QComboBox *cbox_dataBits;
        QComboBox *cbox_stopBits;
        QComboBox *cbox_parity;
        QComboBox *cbox_flowControl;
        QPushButton *button_ok;
        QPushButton *button_cancel;
        QMessageBox *mbox;
        CommandArea *cmdArea;
        DataArea *dataArea;
        QWidget* parent;
        void ComboSetIndex(QComboBox *combo, QString text);
};

#endif
