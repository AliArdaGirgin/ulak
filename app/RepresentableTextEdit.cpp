
#include <QWidget>
#include <QTextEdit>
#include <QByteArray>
#include <QObject>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDateTime>
#include <QMessageBox>
#include "RepresentableTextEdit.h"
#include <cstring>
#include <QDebug>
#include <climits>
#include <cstdio>

void AsciiText::keyPressEvent(QKeyEvent* event){


    // if non-ascii data exist in underlying bytearray do not allow
    // modification from ascii view
    if(!isDataAscii()){
        QMessageBox *msg = new QMessageBox();
        msg->setText("Modifiying binary data from ascii edit is not allowed");
        msg->exec();
        delete msg;
        return;
    }
    QTextEdit::keyPressEvent(event);

    // emit these, after event processed
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        emit onEnterPressed();
    if(event->key() == Qt::Key_Up)
        emit onUpDownArrowKey(1);
    if(event->key() == Qt::Key_Down)
        emit onUpDownArrowKey(0);

    data = toPlainText().toLocal8Bit();
}

void AsciiText::update(){
    setPlainText(data);
    moveCursor(QTextCursor::End);
}

void AsciiText::clear(){
    QTextEdit::clear();
    data.clear();
}

bool AsciiText::isDataAscii() const{
    for(auto& c: data){
#if CHAR_MIN < 0 // for signed char
    if(c<0)
        return false;
#else
    if(c > 127) // for unsigned char
        return false;
#endif
    }
    return true;
}



void HexText::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    int blocked = false;
    int propagate = false;

    if(key == Qt::Key_Return || key == Qt::Key_Enter)
        emit onEnterPressed();
    if(key == Qt::Key_Up)
        emit onUpDownArrowKey(1);
    if(key == Qt::Key_Down)
        emit onUpDownArrowKey(0);

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
    std::string temp;
    for(auto& c:data){
        char *s = new char[4]; // "ff \0" -> size= 4
        std::sprintf(s, "%02x ", (unsigned char)c);
        temp.append(s);
        delete[] s;
    }
    setPlainText(QString::fromStdString(temp));
    moveCursor(QTextCursor::End);
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
