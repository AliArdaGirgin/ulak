#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QByteArray>
#include "DataType.h"

class ProjectSettings: public QWidget{
    Q_OBJECT
    public:
        ProjectSettings();
        static VIEW_TYPE getDefaultViewType(){ return default_view_type;}
        static void setDefaultViewType(VIEW_TYPE t){ default_view_type = t;}
        static LINEFEED_TYPE getDefaultLinefeed(){ return default_linefeed;}
        static void setDefaultLinefeed(LINEFEED_TYPE t){ default_linefeed = t;}
        static void setParamatersToDefault();
    signals:
        void viewTypeUpdated(VIEW_TYPE);
    private slots:
        void onSaved();

    private:
        QGridLayout *layout;
        QPushButton *save;
        QPushButton *cancel;
        QComboBox* default_view_type_selection;
        QComboBox* default_linefeed_selection;
        static QString project_file_name;
        static VIEW_TYPE default_view_type;
        static LINEFEED_TYPE default_linefeed;
};
#endif // PROJECTSETTINGS_H
