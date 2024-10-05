#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include "AddButtonWindow.h"

class ProjectSettings: public QWidget{
    public:
        ProjectSettings();
    private:
        QGridLayout *layout;
        QComboBox* default_data_type;
        LineEndSel *linefeed;
};
#endif // PROJECTSETTINGS_H
