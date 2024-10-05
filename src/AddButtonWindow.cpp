#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSpacerItem>
#include <QIntValidator>
#include <QCheckBox>
#include "AddButtonWindow.h"
#include "TabbedText.h"
#include <limits>

AddButtonWindow::AddButtonWindow(QWidget *parent, Command *cmd):QWidget(parent){

    setWindowTitle("Add Command");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // Validators for delay value
    QIntValidator *delay_valid = new QIntValidator(0,std::numeric_limits<int>::max());

    // Different command types require different input widgets
    // use stack widget to change visibility of different parameters
    stacked_widget = new StackedWidget();
    QWidget *stacked = new QWidget(this);
    periodic_widget = new PeriodicWidget(this);
    read_trigger_widget = new ReadTriggerWidget(this);
    stacked_widget->addWidget(stacked);
    stacked_widget->addWidget(periodic_widget);
    stacked_widget->addWidget(read_trigger_widget);

    // Common widgets for all command types
    QLabel *name_label = new QLabel("Name",this);
    name_text = new QLineEdit(this);
    name_text->setPlaceholderText("Enter Here");

    QLabel *data_label = new QLabel("Data");
    data_tabbedText    = new TabbedText(this);

    QLabel *linefeed_label = new QLabel("Line Feed");
    linefeed = new LineEndSel(this);
    QLabel    *command_label = new QLabel("Type");
    command_cbox = new QComboBox(this);
    command_cbox->addItem("Manual");
    command_cbox->addItem("Periodic");
    command_cbox->addItem("Read Trigger");

    QLabel *delay_label = new QLabel("Delay(ms)");
    delay_text  = new QLineEdit("0");
    delay_text->setValidator(delay_valid);

    ok_button = new QPushButton("Ok");
    cancel_button = new QPushButton("Cancel");

    // if this is a settings window set initial state
    if(cmd) setInitials(cmd);

    // Layout, stack widget changes size
    layout = new QGridLayout();
    layout->setSizeConstraint(QLayout::SetFixedSize);
    layout->addWidget(name_label,0,0,Qt::AlignTop);
    layout->addWidget(name_text,0,1);
    layout->addWidget(data_label,1,0,Qt::AlignTop);
    layout->addWidget(data_tabbedText,1,1);
    layout->addWidget(linefeed_label,2,0);
    layout->addWidget(linefeed,2,1);
    layout->addWidget(command_label,3,0,Qt::AlignTop);
    layout->addWidget(command_cbox,3,1);
    layout->addWidget(delay_label,4,0,Qt::AlignTop);
    layout->addWidget(delay_text,4,1);
    layout->addWidget(stacked_widget,5,0,1,2);
    layout->addWidget(ok_button,6,0);
    layout->addWidget(cancel_button,6,1);
    setLayout(layout);

    connect(command_cbox, SIGNAL(currentIndexChanged(int)), stacked_widget, SLOT(setCurrentIndex(int)));
    connect(cancel_button, SIGNAL(clicked()),this,SLOT(close()) );
    connect(ok_button, SIGNAL(clicked()), this,SLOT(buttonAdded()) );
}

void AddButtonWindow::buttonAdded(){
    Command::cmd_type type = static_cast<Command::cmd_type>(command_cbox->currentIndex());
    QMessageBox *msg = new QMessageBox;

    // Check name and data fileds
    if(data_tabbedText->isDataEmpty() || name_text->text().isEmpty()){
        msg->setText("Name and/or data can not be empty");
        msg->exec();
    // Check period if command is periodic
    }else if(type == Command::PERIODIC && periodic_widget->getPeriod() == 0){
        msg->setText("Period min = 5ms");
        msg->exec();
    // Check read data if command is read trigger
    }else if(type == Command::READ_TRIGGER && read_trigger_widget->isReadDataEmpty()){
        msg->setText("Read data cant be empty");
        msg->exec();
    // Checks complete add button to cmd area
    }else{
        QByteArray linefeeds = linefeed->getLineEnd();
        switch(type){
            case Command::SINGLE:
                emit onButtonAdded(name_text->text(), type, data_tabbedText->getData(), data_tabbedText->currentIndex(),
                                   linefeeds, delay_text->text().toInt());
                break;
            case Command::PERIODIC:
                emit onButtonAdded(name_text->text(), type, data_tabbedText->getData(), data_tabbedText->currentIndex(),
                                   linefeeds, delay_text->text().toInt(), periodic_widget->getPeriod());
                break;
            case Command::READ_TRIGGER:
                emit onButtonAdded(name_text->text(), type, data_tabbedText->getData(), data_tabbedText->currentIndex(),
                                   linefeeds, delay_text->text().toInt(), 0,
                                   read_trigger_widget->getReadData(), read_trigger_widget->getLastTab());
                break;
            default:
                //Error
                break;
        }
        this->close();
    }
}

void AddButtonWindow::setInitials(Command *cmd){
    name_text->setText(cmd->getName());
    data_tabbedText->setData(cmd->getData());
    data_tabbedText->setCurrentIndex(cmd->getDataTab());
    data_tabbedText->update();
    linefeed->setLineEnd(cmd->getLineFeed());
    delay_text->setText(QString::number(cmd->getDelay()));
    switch(cmd->getCommandType()){
        case Command::SINGLE:
            command_cbox->setCurrentText("Manual");
            stacked_widget->setCurrentIndex(0);
            break;
        case Command::PERIODIC:
            command_cbox->setCurrentText("Periodic");
            periodic_widget->setPeriod(cmd->getPeriod());
            stacked_widget->setCurrentIndex(1);
            break;
        case Command::READ_TRIGGER:
            command_cbox->setCurrentText("Read Trigger");
            read_trigger_widget->setReadData(cmd->getReadData());
            read_trigger_widget->setLastTab(cmd->getReadDataTab());
            stacked_widget->setCurrentIndex(2);
            break;
        default:
            // ERROR
            break;
    }


}

PeriodicWidget::PeriodicWidget(QWidget *parent):QWidget(parent){
    period_valid = new QIntValidator(100,std::numeric_limits<int>::max());
    period_valid->setBottom(5);
    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(0);
    name = new QLabel("Period(ms)          ");
    text = new QLineEdit("100");
    text->setValidator(period_valid);

    layout->addWidget(name,0,0,Qt::AlignTop);
    layout->addWidget(text,0,1);
    setLayout(layout);
}

int PeriodicWidget::getPeriod(){
    return text->text().toInt();
}

void PeriodicWidget::setPeriod(int delay_in){
    text->setText(QString::number(delay_in));
}

ReadTriggerWidget::ReadTriggerWidget(QWidget *parent):QWidget(parent){
    QGridLayout *layout = new QGridLayout();
    layout->setSpacing(0);
    name = new QLabel("Read Data          ");
    text = new TabbedText(this);
    layout->addWidget(name,0,0,Qt::AlignTop);
    layout->addWidget(text,0,1);
    setLayout(layout);
}

bool ReadTriggerWidget::isReadDataEmpty(){
    return text->isDataEmpty();
}

QByteArray ReadTriggerWidget::getReadData(){
    return text->getData();
}

void ReadTriggerWidget::setReadData(QByteArray read_data_in){
    text->setData(read_data_in);
}

LineEndSel::LineEndSel(QWidget *parent):QWidget(parent){
    layout = new QGridLayout();
    CR_label = new QLabel("CR");
    CR_check = new QCheckBox(this);
    LF_label = new QLabel("LF");
    LF_check = new QCheckBox(this);

    layout->addWidget(CR_label, 0,0,Qt::AlignRight);
    layout->addWidget(CR_check, 0,1);
    layout->addWidget(LF_label, 0,2,Qt::AlignRight);
    layout->addWidget(LF_check, 0,3);
    setLayout(layout);
}

QByteArray LineEndSel::getLineEnd(){
    QByteArray lineend;
    if(CR_check->isChecked())
        lineend.append('\r');
    if(LF_check->isChecked())
        lineend.append('\n');
    return lineend;
}

void LineEndSel::setLineEnd(QByteArray linefeed_in){
    if(linefeed_in[0] == '\r')
        CR_check->setCheckState(Qt::CheckState::Checked);
    if(linefeed_in[1] == '\n')
        LF_check->setCheckState(Qt::CheckState::Checked);
}

