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
#include "ProjectSettings.h"
#include "Conf.h"

AddButtonWindow::AddButtonWindow(QWidget *parent, Command *cmd):QWidget(parent){

    setWindowTitle("Add Command");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);


    // Validators for delay value

    // Common widgets for all command types
    QLabel *name_label = new QLabel("Name",this);
    name_text = new QLineEdit(this);
    name_text->setPlaceholderText("Enter Here");

    QLabel *data_label = new QLabel("Data");
    data_tabbedText    = new TabbedText(this);

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

    QLabel    *command_label = new QLabel("Type");
    command_cbox = new QComboBox(this);
    command_cbox->addItem(COMMAND_TYPE_ONESHOT_NAME);
    command_cbox->addItem(COMMAND_TYPE_PERIODIC_NAME);

    QLabel *delay_label = new QLabel("Delay(ms)");
    delay_text  = new QLineEdit("0");
    delay_valid = new QIntValidator(0,std::numeric_limits<int>::max());
    delay_text->setValidator(delay_valid);

    period_label = new QLabel("Period(ms)");
    period_text = new QLineEdit("100");
    period_valid = new QIntValidator(COMMAND_AREA_TIMER_RESOLUTION,std::numeric_limits<int>::max());
    period_text->setValidator(period_valid);

    trigger_label = new QLabel("Trigger");
    trigger_cbox = new QComboBox();
    trigger_cbox->addItem(TRIGGER_TYPE_MANUAL_NAME);
    trigger_cbox->addItem(TRIGGER_TYPE_READTRIGGER_NAME );

    read_data_label = new QLabel("Read Data");
    read_data_text = new TabbedText(this);

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
    layout->addWidget(linefeed_selection,2,1);
    layout->addWidget(command_label,3,0,Qt::AlignTop);
    layout->addWidget(command_cbox,3,1);
    layout->addWidget(delay_label,4,0,Qt::AlignTop);
    layout->addWidget(delay_text,4,1);
    layout->addWidget(period_label,5,0,Qt::AlignTop);
    layout->addWidget(period_text,5,1);
    layout->addWidget(trigger_label,6,0);
    layout->addWidget(trigger_cbox,6,1);
    layout->addWidget(read_data_label,7,0, Qt::AlignTop);
    layout->addWidget(read_data_text, 7,1);
    layout->addWidget(ok_button,8,0);
    layout->addWidget(cancel_button,8,1);
    setLayout(layout);
    commandTypeChanged(command_cbox->currentIndex());
    triggerTypeChanged(trigger_cbox->currentIndex());

    connect(command_cbox, SIGNAL(currentIndexChanged(int)), this, SLOT(commandTypeChanged(int)));
    connect(trigger_cbox, SIGNAL(currentIndexChanged(int)), this, SLOT(triggerTypeChanged(int)));
    connect(cancel_button, SIGNAL(clicked()),this,SLOT(close()) );
    connect(ok_button, SIGNAL(clicked()), this,SLOT(buttonAdded()) );
}

void AddButtonWindow::buttonAdded(){
    COMMAND_TYPE type = static_cast<COMMAND_TYPE>(command_cbox->currentIndex());

    // Check name and data fileds
    if(data_tabbedText->isDataEmpty() || name_text->text().isEmpty()){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Name and/or data can not be empty");
        msg->exec();

    // Check if delay is compatible with resolution
    }else if(delay_text->text().toInt()%COMMAND_AREA_TIMER_RESOLUTION != 0){
        QString str("Delay must be multiple of ");
        str += QString::number(COMMAND_AREA_TIMER_RESOLUTION);
        str += " ms";
        QMessageBox *msg = new QMessageBox;
        msg->setText(str);
        msg->exec();

    // Check if period is compatible with resolution
    }else if(type == COMMAND_TYPE::PERIODIC &&
               (period_text->text().toInt() == 0 ||
                period_text->text().toInt()%COMMAND_AREA_TIMER_RESOLUTION != 0)){
        QString str("Period must be bigger than 0 and multiple of ");
        str += QString::number(COMMAND_AREA_TIMER_RESOLUTION);
        str += " ms";
        QMessageBox *msg = new QMessageBox;
        msg->setText(str);
        msg->exec();

    // Check read data is empty if command is read trigger
    }else if(trigger_cbox->currentIndex() == static_cast<int>(TRIGGER_TYPE::READ_TRIGGER) &&
            read_data_text->isDataEmpty()){
        QMessageBox *msg = new QMessageBox;
        msg->setText("Read data cant be empty");
        msg->exec();

    }else{
        Command_t cmd;
        cmd.name = name_text->text();
        cmd.cmd_type = type;
        cmd.data = data_tabbedText->getData();
        cmd.last_tab = data_tabbedText->currentIndex();
        cmd.linefeed = static_cast<LINEFEED_TYPE>(linefeed_selection->currentIndex());
        cmd.delay = delay_text->text().toInt();
        cmd.period = period_text->text().toInt();
        cmd.trig_type = static_cast<TRIGGER_TYPE>(trigger_cbox->currentIndex());
        cmd.read_data = read_data_text->getData();
        emit onButtonAdded(cmd);
        this->close();
    }
}

void AddButtonWindow::setInitials(Command *cmd){
    name_text->setText(cmd->getName());

    data_tabbedText->setData(cmd->getData());
    data_tabbedText->setCurrentIndex(cmd->getDataTab());
    data_tabbedText->update();

    linefeed_selection->setCurrentIndex( static_cast<int>(cmd->getLineFeed()));

    command_cbox->setCurrentIndex(static_cast<int>(cmd->getCommandType()));

    delay_text->setText(QString::number(cmd->getDelay()));

    period_text->setText(QString::number(cmd->getPeriod()));

    trigger_cbox->setCurrentIndex( static_cast<int>(cmd->getTriggerType()));

    read_data_text->setData(cmd->getReadData());
    read_data_text->setCurrentIndex( cmd->getReadDataTab());
    read_data_text->update();
}

void AddButtonWindow::commandTypeChanged(int index){
    switch(static_cast<COMMAND_TYPE>(index)){
    case COMMAND_TYPE::ONE_SHOT:
        period_label->setVisible(false);
        period_text->setVisible(false);

        break;
    case COMMAND_TYPE::PERIODIC:
        period_label->setVisible(true);
        period_text->setVisible(true);
        break;
    }
}

void AddButtonWindow::triggerTypeChanged(int index){
    switch(static_cast<TRIGGER_TYPE>(index)){
    case TRIGGER_TYPE::MANUAL:
        read_data_label->setVisible(false);
        read_data_text->setVisible(false);
        break;
    case TRIGGER_TYPE::READ_TRIGGER:
        read_data_label->setVisible(true);
        read_data_text->setVisible(true);
        break;
    }
}
