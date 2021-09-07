#ifndef REPRESENTABLE_TEXT_EDIT
#define REPRESENTABLE_TEXT_EDIT

#include <QWidget>
#include <QTextEdit>
#include <QObject>
#include <QByteArray>
#include <QChar>
#include <QKeyEvent>

class IRepresantableTextEdit : public QTextEdit{
    public:
        IRepresantableTextEdit(QWidget *parent = 0, bool read_only_in = false);
        virtual void setData(QByteArray &data) = 0;
        virtual QByteArray getData() = 0;
};

class AsciiText : public IRepresantableTextEdit{
    public:
        AsciiText(QWidget *parent = 0, bool read_only_in = false);
        void setData(QByteArray &data);
        QByteArray getData();
};

class HexText : public IRepresantableTextEdit{
    Q_OBJECT
    public:
        HexText(QWidget *parent = 0, bool read_only_in = false);
        void setData(QByteArray &data);
        QByteArray getData();
        void keyPressEvent(QKeyEvent *event);
    private:
        int last_index;
};

#endif