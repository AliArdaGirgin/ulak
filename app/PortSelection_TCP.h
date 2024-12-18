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
        void onDnsLookup();
        void onIpClear();
        void onConnected();
        void onErrorOccurred(QAbstractSocket::SocketError);
    protected:
        void keyPressEvent(QKeyEvent* event) override;
    signals:
        void opened(PortHandler_Base*);
    private:
        QGridLayout *layout;
        QLineEdit* address;
        QPushButton* dns_lookup;
        QLineEdit* port;
        QLineEdit* ip_line_edit;
        QComboBox* ip_cbox;
        QPushButton *ip_clear;
        QPushButton* ok;
        QPushButton* cancel;
        QWidget* parent;
        QTcpSocket *socket;
        bool ip_text;

};
#endif // PORTSELECTION_TCP_H
