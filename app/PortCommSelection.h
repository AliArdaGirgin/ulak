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

class PortCommSelection : public QWidget{ 
    Q_OBJECT
    public:
        PortCommSelection(QWidget *parent = 0);
        bool isOpen();
    
    signals:
        void closed();
        void opened(QSerialPort *);
    
    private slots:
        void portSelected();
        void windowClosed();
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
        void ComboSetIndex(QComboBox *combo, QString text);
};

#endif
