#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
#include "TabbedText.h"
#include "RepresentableTextEdit.h"

TabbedText::TabbedText(QWidget *parent):QTabWidget(parent){
    AsciiText *ascii = new AsciiText(this, data);
    HexText   *hex   = new HexText(this, data);

    addTab(ascii,"ASCII");
    addTab(hex,"HEX");

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabbedChanged(int)));
}

void TabbedText::tabbedChanged(int index_in){
    static_cast<IRepresantableTextEdit*>(widget(index_in))->update();
}

bool TabbedText::isDataEmpty(){
    return data.isEmpty();
}

QByteArray TabbedText::getData(){
    return data;
}

void TabbedText::setData(QByteArray data_in){
    data = data_in;
    static_cast<IRepresantableTextEdit*>(currentWidget())->update();
}

void TabbedText::clearText(){
    data.clear();
    static_cast<IRepresantableTextEdit*>(currentWidget())->update();
}

void TabbedText::addData(QByteArray &data_in){
    data.append(data_in);
}

void TabbedText::update(){
    static_cast<IRepresantableTextEdit*>(currentWidget())->update();
}
