
#include <QWidget>
#include <QTextEdit>
#include <QByteArray>
#include <QObject>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDateTime>
#include "RepresentableTextEdit.h"
#include <cstring>
#include <sstream>
#include <QDebug>


void AsciiText::keyPressEvent(QKeyEvent* event){
    QTextEdit::keyPressEvent(event);
    data.append(event->text().toLocal8Bit());
}

void AsciiText::update(){
    setPlainText(data);
}

void AsciiText::clear(){
    QTextEdit::clear();
    data.clear();
}

void HexText::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    int blocked = false;
    int propagate = false;

    // 0,1....9
    if(key >= Qt::Key_0 && key <= Qt::Key_9){
        propagate = true;
    }
    // Aa,Bb......Ff
    else if(key >= Qt::Key_A && key <= Qt::Key_F){
        propagate = true;
    }
    //left,right,up,down
    else if(key >= Qt::Key_Left && key <= Qt::Key_Down){
        blocked = true;
        propagate = true;
    }

    else if(key == Qt::Key_Backspace){
        blocked = true;
        propagate = true;
    }else{
        // Dont act upon other keys
        return;
    }

    if(propagate)
        QTextEdit::keyPressEvent(event);

    // Add space if at every 2 char, if it is not backspace
    if((textCursor().position()+1) % 3 == 0 && !blocked){
        insertPlainText(" ");
    }

    updateData(toPlainText().toStdString());
}
void HexText::update(){
    std::stringstream ss;
    for(auto& c:data)
        ss << std::hex << (int)c << " ";
    setPlainText(QString::fromStdString(ss.str()));
}
void HexText::clear(){
    QTextEdit::clear();
    data.clear();
}

void HexText::updateData(const std::string& str){
    data.clear();
    unsigned char temp = 0;
    for(auto c:str){
        if(c == ' '){
            data.append(temp);
            temp = 0;
        }else{
            temp = temp << 4;
            c = std::toupper(c);
            temp |= (hexCharToByte.at(c) & 0x0f);
        }
    }
}
