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

    // ownership of tabs transfer to QTabWidget
    ascii_index = addTab(ascii, VIEW_TYPE_ASCII_NAME);
    hex_index = addTab(hex, VIEW_TYPE_HEX_NAME);

    // set default acoording to project settings
    if(ProjectSettings::getDefaultViewType() == VIEW_TYPE::ASCII)
        setCurrentIndex(ascii_index);
    else
        setCurrentIndex(hex_index);

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(tabbedChanged(int)));
    connect(static_cast<IRepresantableTextEdit*>(widget(ascii_index)), SIGNAL(onEnterPressed()), this, SIGNAL(onEnterPressed()));
    connect(static_cast<IRepresantableTextEdit*>(widget(hex_index)),   SIGNAL(onEnterPressed()), this, SIGNAL(onEnterPressed()));
    connect(static_cast<IRepresantableTextEdit*>(widget(ascii_index)), SIGNAL(onUpDownArrowKey(int)), this, SIGNAL(onUpDownArrowKey(int)));
    connect(static_cast<IRepresantableTextEdit*>(widget(hex_index)),   SIGNAL(onUpDownArrowKey(int)), this, SIGNAL(onUpDownArrowKey(int)));
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

void TabbedText::setTextHeight(int h){
    setMaximumHeight(h);
    setMinimumHeight(h);
}

void TabbedText::setCursor(QTextCursor::MoveOperation tc){
    static_cast<IRepresantableTextEdit*>(currentWidget())->moveCursor(tc);
}
