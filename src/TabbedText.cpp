#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
#include "TabbedText.h"
#include "RepresentableTextEdit.h"

TabbedText::TabbedText(QWidget *parent, bool read_only_in):QTabWidget(parent){
    read_only = read_only_in;
    AsciiText *ascii = new AsciiText(this, read_only);
    HexText   *hex   = new HexText(this,read_only);

    index_ascii = addTab(ascii,"ASCII");
    index_hex   = addTab(hex,"HEX");

    prev = currentIndex();

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabbedChanged(int)));
}

void TabbedText::tabbedChanged(int index_in){
    IRepresantableTextEdit *src  = static_cast<IRepresantableTextEdit*>(widget(prev));
    IRepresantableTextEdit *dest = static_cast<IRepresantableTextEdit*>(widget(index_in));

    // Update share byte array
    data = src->getData();
    dest->setData(data);
    prev = index_in;
}

bool TabbedText::isDataEmpty(){
    // Update data when ok pressed
    IRepresantableTextEdit *src  = static_cast<IRepresantableTextEdit*>(widget(prev));
    data = src->getData();
    return data.isEmpty();
}

QByteArray TabbedText::getData(){
    return data;
}

void TabbedText::setData(QByteArray data_in){
    data = data_in;
    IRepresantableTextEdit *dest = static_cast<IRepresantableTextEdit*>(widget(currentIndex()));
    dest->setData(data);
}

void TabbedText::clearText(){
    data.clear();
    IRepresantableTextEdit *dest = static_cast<IRepresantableTextEdit*>(widget(currentIndex()));
    dest->clear();
}

void TabbedText::addData(QByteArray &data_in){
    data.append(data_in);
    IRepresantableTextEdit *dest = static_cast<IRepresantableTextEdit*>(widget(currentIndex()));
    dest->setData(data);
}

