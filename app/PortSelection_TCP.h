#ifndef PORTSELECTION_TCP_H
#define PORTSELECTION_TCP_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QComboBox>
#include <QKeyEvent>
#include "PortHandler_Base.h"

class PortSelection_TCP: public QWidget{
    Q_OBJECT
    public:
        PortSelection_TCP(QWidget* parent=0);
        void onDnsLookupResults(const QHostInfo& info);
    private slots:
        void onCancel();
        void onOk();
        void onConnected();
        void onErrorOccurred(QAbstractSocket::SocketError);
        void onTypeIndexChanged(int index);
        void onAddressIndexChanged(int index);
    signals:
        void opened(PortHandler_Base*);
    private:
        QGridLayout *layout;
        QComboBox* type;
        QLineEdit* address_line_edit;
        QComboBox* address_cbox;
        QLineEdit* port;
        QPushButton* ok;
        QPushButton* cancel;
        QWidget* parent;
        QTcpSocket *socket;
        QTcpServer* server;
        bool ip_text;

};
#endif // PORTSELECTION_TCP_H
