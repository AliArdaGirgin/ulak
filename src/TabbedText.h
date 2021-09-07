#ifndef TABBED_TEXT_H
#define TABBED_TEXT_H

#include <QWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QByteArray>
#include <QObject>
#include <QPushButton>
#include <QChar>

class TabbedText : public QTabWidget{
    Q_OBJECT
    public:
        TabbedText(QWidget *parent = 0, bool read_only_in=false);
        bool isDataEmpty();
        QByteArray getData();
        void setData(QByteArray data_in);
        void addData(QByteArray &data_in);
    public slots:
        void tabbedChanged(int index_in);
        void clearText();

    private:
        QByteArray data;
        int prev;
        int index_hex;
        int index_ascii;
        bool read_only;
};

#endif