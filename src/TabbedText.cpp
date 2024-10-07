#include <QWidget>
#include <QTextEdit>
#include <QTabWidget>
#include <QByteArray>
#include <QDebug>
#include <QStringList>
#include "TabbedText.h"
#include "RepresentableTextEdit.h"
#include "DataType.h"
#include "ProjectSettings.h"

TabbedText::TabbedText(QWidget *parent):QTabWidget(parent){
    AsciiText *ascii = new AsciiText(this, data);
    HexText   *hex   = new HexText(this, data);

    ascii_index = addTab(ascii, VIEW_TYPE_ASCII_NAME);
    hex_index = addTab(hex, VIEW_TYPE_HEX_NAME);
    if(ProjectSettings::getDefaultDataType() == VIEW_TYPE::ASCII)
        setCurrentIndex(ascii_index);
    else
        setCurrentIndex(hex_index);
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
