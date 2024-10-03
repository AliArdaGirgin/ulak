#ifndef REPRESENTABLE_TEXT_EDIT
#define REPRESENTABLE_TEXT_EDIT

#include <QWidget>
#include <QObject>
#include <QByteArray>
#include <QChar>
#include <QKeyEvent>
#include <QTextEdit>
#include <map>

class IRepresantableTextEdit : public QTextEdit{
    public:
    IRepresantableTextEdit(QWidget *parent, QByteArray& data_):QTextEdit(parent), data(data_){}
        virtual void keyPressEvent(QKeyEvent *event) = 0;
        virtual void update() = 0;
        virtual void clear() = 0;
    protected:
        QByteArray& data;
};

class AsciiText : public IRepresantableTextEdit{
    Q_OBJECT
    public:
        AsciiText(QWidget *parent, QByteArray& data_): IRepresantableTextEdit(parent, data_){}
        void keyPressEvent(QKeyEvent* event) override;
        void update() override;
        void clear() override;
};

class HexText : public IRepresantableTextEdit{
    Q_OBJECT
    public:
        HexText(QWidget *parent, QByteArray& data_): IRepresantableTextEdit(parent, data_){}
        void keyPressEvent(QKeyEvent *event) override;
        void update() override;
        void clear() override;
    private:
        void updateData(const std::string& str);
        const std::map<char, unsigned char> hexCharToByte{
            {'0', 0x0}, {'1', 0x1}, {'2', 0x2}, {'3', 0x3}, {'4', 0x4}, {'5', 0x5},
            {'6', 0x6}, {'7', 0x7}, {'8', 0x8}, {'9', 0x9}, {'A', 0xA}, {'B', 0xB},
            {'C', 0xC}, {'D', 0xD}, {'E', 0xE}, {'F', 0xF}
        };
};

#endif
