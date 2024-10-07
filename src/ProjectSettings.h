#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QByteArray>
#include "AddButtonWindow.h"
#include "DataType.h"

class ProjectSettings: public QWidget{
    Q_OBJECT
    public:
        ProjectSettings();
        static VIEW_TYPE getDefaultDataType(){ return default_data_type;}
        static QByteArray getDefaultLineFeed(){ return default_linefeed;}
    signals:
        void viewTypeUpdated(VIEW_TYPE);
    private slots:
        void onSaved();

    private:
        QGridLayout *layout;
        QPushButton *save;
        QPushButton *cancel;
        QComboBox* default_data_type_selection;
        LineEndSel *linefeed_selection;
        static VIEW_TYPE default_data_type;
        static QByteArray default_linefeed;
};
#endif // PROJECTSETTINGS_H
