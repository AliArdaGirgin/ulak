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
        TabbedText(QWidget *parent=nullptr);
        bool isDataEmpty();
        QByteArray getData();
        void setData(QByteArray data_in);
        void addData(QByteArray &data_in);
        void update();
        void setTextHeight(int h);
        void setCursor(QTextCursor::MoveOperation tc);

    signals:
        void onEnterPressed();
        void onUpDownArrowKey(int up);
    public slots:
        void tabbedChanged(int index_in);
        void clearText();

    private:
        QByteArray data;
        int ascii_index;
        int hex_index;
};

#endif
