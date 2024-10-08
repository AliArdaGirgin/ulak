#ifndef ADDBUTTONWINDOW_H
#define ADDBUTTONWINDOW_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QStackedWidget>
#include <QCheckBox>
#include "TabbedText.h"
#include "Command.h"

// Needed, QStackedWidget always gets max size
// We need a dynamically sized StackWidget
class StackedWidget:public QStackedWidget{
    QSize sizeHint() const override{
        return currentWidget()->sizeHint();
    }
    QSize minimumSizeHint() const override{
        return currentWidget()->minimumSize();
    }
};


class PeriodicWidget:public QWidget{
    Q_OBJECT
    public:
        PeriodicWidget(QWidget *parent=0);
        int getPeriod(){return text->text().toInt();}
        void setPeriod(int delay_in){text->setText(QString::number(delay_in));}
    private:
        QIntValidator *period_valid;
        QLabel *name;
        QLineEdit *text;
};

class ReadTriggerWidget:public QWidget{
    Q_OBJECT
    public:
        ReadTriggerWidget(QWidget *parent=0);
        QByteArray getReadData(){return text->getData();}
        void setReadData(QByteArray read_data_in){text->setData(read_data_in);}
        bool isReadDataEmpty(){ return text->isDataEmpty();}
        int getLastTab(){ return text->currentIndex();}
        void setLastTab(int tab_){ text->setCurrentIndex(tab_);}
        LINEFEED_TYPE getLinefeed(){
            return static_cast<LINEFEED_TYPE>(read_linefeed_selection->currentIndex());
        }
        void setLinefeed( LINEFEED_TYPE t){
            read_linefeed_selection->setCurrentIndex(static_cast<int>(t));}
    private:
        QLabel *name;
        TabbedText *text;
        QComboBox* read_linefeed_selection;
};

class AddButtonWindow:public QWidget{
    Q_OBJECT
    public:
        AddButtonWindow(QWidget *parent=0, Command *cmd =0);
    signals:
        void onButtonAdded(QString name, Command::cmd_type cmd,
                           QByteArray data, int last_tab, LINEFEED_TYPE linefeed, int delay, int period=0,
                           QByteArray read_data=QByteArray(), LINEFEED_TYPE read_linefeed = LINEFEED_TYPE::NONE,
                           int read_last_tab=0,  QWidget *parent=0);

    private slots:
        void buttonAdded();
    private:
        QGridLayout *layout;
        QLineEdit   *name_text;
        TabbedText  *data_tabbedText;
        QComboBox   *linefeed_selection;
        QLabel      *read_trigger_name;
        TabbedText  *read_trigger_tabbedText;
        QLabel      *period_label;
        QLineEdit   *period_text;
        QPushButton *ok_button;
        QPushButton *cancel_button;
        QComboBox   *command_cbox;
        QLineEdit   *delay_text;
        StackedWidget     *stacked_widget;
        PeriodicWidget    *periodic_widget;
        ReadTriggerWidget *read_trigger_widget;
        void setInitials(Command *cmd);
};

#endif
