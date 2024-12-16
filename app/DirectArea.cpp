#include <QApplication>
#include <QLabel>
#include <QFont>
#include <QFontMetrics>
#include <QMessageBox>

#include "DirectArea.h"
#include "DataType.h"
#include "ProjectSettings.h"
#include "Command.h"
#include "Conf.h"
#include "PortHandler_Base.h"

DirectArea::DirectArea(QWidget *parent): QWidget{parent}
{
    check_comm = true;
    history_cnt = 0;
    QLabel *name = new QLabel("Enter: ");
    edit = new TabbedText();
    edit->setTextHeight(QFontMetrics(QApplication::font()).height()*3.5);
    send_button = new QPushButton("Send", this);

    QLabel *linefeed_label = new QLabel("Linefeed");
    linefeed_selection = new QComboBox();
    linefeed_selection->addItem(LINEFEED_TYPE_NONE_NAME);
    linefeed_selection->addItem(LINEFEED_TYPE_CR_NAME);
    linefeed_selection->addItem(LINEFEED_TYPE_LF_NAME);
    linefeed_selection->addItem(LINEFEED_TYPE_CR_LF_NAME);
    linefeed_selection->addItem(LINEFEED_TYPE_0_NAME);
    linefeed_selection->setCurrentIndex(
        static_cast<int>( ProjectSettings::getDefaultLinefeed())
    );

    layout = new QHBoxLayout(this);
    layout->addWidget(name);
    layout->addWidget(edit);
    layout->addWidget(linefeed_label);
    layout->addWidget(linefeed_selection);
    layout->addWidget(send_button);

    layout->setStretch(0, 0);
    layout->setStretch(1, 1);
    layout->setStretch(2, 0);
    layout->setStretch(3, 0);
    layout->setStretch(4, 0);

    setLayout(layout);

    connect(send_button, SIGNAL(clicked()), this, SLOT(onSendButton()));
    connect(edit, SIGNAL(onEnterPressed()), this, SLOT(enterPressed()));
    connect(edit, SIGNAL(onUpDownArrowKey(int)), this, SLOT(upDownArrowKeyPressed(int)));
}

void DirectArea::onSendButton(){
    if(check_comm && !PortHandler_Base::isOpen()){
        QMessageBox *msg = new QMessageBox();
        msg->setText("No Connection");
        msg->exec();
        delete msg;
    }else{
        QByteArray *dt = new QByteArray(edit->getData());
        addToHistory(dt);
        Command::setLinefeedData(*dt, static_cast<LINEFEED_TYPE>(linefeed_selection->currentIndex()));
        emit send(*dt, DATA_TYPE::TX);
        edit->clearText();
        edit->setCursor(QTextCursor::Start);
    }
}

void DirectArea::enterPressed(){
    send_button->click();
}

void DirectArea::upDownArrowKeyPressed(int up){
    if(up)
        history_cnt--;
    else
        history_cnt++;

    if(history_cnt < 0)
        history_cnt = 0;
    else if(history_cnt >= static_cast<int>(history.size()))
        history_cnt = history.size();

    edit->clearText();
    if(history_cnt <static_cast<int>(history.size()))
        edit->setData(*history[history_cnt]);
}

void DirectArea::addToHistory(QByteArray* dt){
    history.push_back(dt);
    if(history.size() > DIRECT_AREA_HISTORY_SIZE){
        delete history.front();
        history.pop_front();
    }
    history_cnt = history.size();
}
void DirectArea::clearHistory(){
    history.clear();
    history_cnt = 0;
}
