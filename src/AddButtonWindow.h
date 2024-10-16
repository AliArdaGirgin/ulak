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
#include "DataType.h"

class AddButtonWindow:public QWidget{
    Q_OBJECT
    public:
        AddButtonWindow(QWidget *parent=0, Command *cmd =0);
    signals:
        void onButtonAdded(Command_t cmd, QWidget *parent=0);

    private slots:
        void buttonAdded();
        void commandTypeChanged(int);
        void triggerTypeChanged(int);
    private:
        QGridLayout *layout;
        QLineEdit   *name_text;
        TabbedText  *data_tabbedText;
        QComboBox   *linefeed_selection;
        QComboBox   *command_cbox;
        QLineEdit   *delay_text;
        QLabel		*period_label;
        QLineEdit   *period_text;
        QLabel      *trigger_label;
        QComboBox   *trigger_cbox;
        QLabel      *read_data_label;
        TabbedText  *read_data_text;
        QPushButton *ok_button;
        QPushButton *cancel_button;
        void setInitials(Command *cmd);
};

#endif
