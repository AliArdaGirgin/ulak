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

        static TIMESTAMP_FORMAT_TYPE getDefaultTimestampFormat(){ return default_timestamp_format;}
        static void setDefaultTimestampFormat(TIMESTAMP_FORMAT_TYPE t){ default_timestamp_format = t;}

        static void setParamatersToDefault();
    signals:
        void viewTypeUpdated(VIEW_TYPE);
    private slots:
        void onSaved();

    private:
        QGridLayout *layout;
        QPushButton *save;
        QPushButton *cancel;
        static QString project_file_name;

        static VIEW_TYPE default_view_type;
        QComboBox* default_view_type_selection;

        static LINEFEED_TYPE default_linefeed;
        QComboBox* default_linefeed_selection;

        static TIMESTAMP_FORMAT_TYPE default_timestamp_format;
        QComboBox* default_timestamp_selection;
};
#endif // PROJECTSETTINGS_H
