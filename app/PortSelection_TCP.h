#ifndef PORTSELECTION_TCP_H
#define PORTSELECTION_TCP_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QTcpSocket>
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
    signals:
        void opened(PortHandler_Base*);
    private:
        QGridLayout *layout;
        QLineEdit* address;
        QLineEdit* port;
        QPushButton* ok;
        QPushButton* cancel;
        QWidget* parent;
        QTcpSocket *socket;
        bool ip_text;

};
#endif // PORTSELECTION_TCP_H
