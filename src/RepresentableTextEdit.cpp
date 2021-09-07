
#include <QWidget>
#include <QTextEdit>
#include <QByteArray>
#include <QObject>
#include <QKeyEvent>
#include <QTextCursor>
#include <QDateTime>
#include "RepresentableTextEdit.h"
#include <cstring>

IRepresantableTextEdit::IRepresantableTextEdit(QWidget *parent, bool read_only_in):QTextEdit(parent){
    setReadOnly(read_only_in);
}

AsciiText::AsciiText(QWidget *parent, bool read_only_in):IRepresantableTextEdit(parent, read_only_in){}

void AsciiText::setData(QByteArray &data){
    QString text = QString();
    QByteArray temp = QByteArray();
    
    for(int i=0; i<data.length(); i++){
        if(data[i] != '\0') // ignore string end delimeter
            temp.append(data[i]);
    }
    temp.append('\0');

    text = QString::fromLatin1(temp);
    setPlainText(text);

    // Set cursor at the end
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

QByteArray AsciiText::getData(){
    QString txt = toPlainText();
    return txt.toLatin1();
}

HexText::HexText(QWidget *parent, bool read_only_in):IRepresantableTextEdit(parent, read_only_in){
    last_index = 0;
}

void HexText::setData(QByteArray &data){
    QString tempQStr = QString();
    QString newText  = QString();
    QTextCursor cursor;
    // convert bytes to hex string
    for(int cnt = 0; cnt<data.length(); cnt++){
        tempQStr.sprintf("%02x", static_cast<unsigned char>(data[cnt]));
        newText.append(tempQStr);
        newText.append(' '); // space between hex bytes
    }

    // remove space if not in 2 char limit
    cursor = textCursor();
    if(!((cursor.position()+1) % 3)){
        newText.remove(newText.length()-1,1);
    }
    setPlainText(newText);

    // Set cursor at the end
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
}

QByteArray HexText::getData(){
    // remove space characters
    QStringList hexBytes = toPlainText().split(' ', QString::SkipEmptyParts);
    QByteArray bytes;
    int cnt = 0;

    // Convert hex string to byte array
    for(QString &str:hexBytes){
        bytes[cnt] = str.toUShort(nullptr,16);
        cnt++;
    }
    return bytes;
}

void HexText::keyPressEvent(QKeyEvent *event){
    int key = event->key();
    int blocked = false;
    QTextCursor cursor;

    // 0,1....9
    if(key >= Qt::Key_0 && key <= Qt::Key_9){
        QTextEdit::keyPressEvent(event); 
    }
    // Aa,Bb......Ff
    else if(key >= Qt::Key_A && key <= Qt::Key_F){
        QTextEdit::keyPressEvent(event); 
    }
    //left,right,up,down
    else if(key >= Qt::Key_Left && key <= Qt::Key_Down){
        blocked = true;
        QTextEdit::keyPressEvent(event);
    }

    else if(key == Qt::Key_Backspace){
        blocked = true;
        QTextEdit::keyPressEvent(event);
    }else{
        // Dont act upon other keys
    }

    // Add space if at every 2 char, if it is not backspace
    cursor = textCursor();
    if((cursor.position()+1) % 3 == 0 && !blocked){
        insertPlainText(" ");
    }
}
