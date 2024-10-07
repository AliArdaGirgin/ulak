#include <QLabel>
#include <QLineEdit>
#include "ProjectSettings.h"
#include "DataType.h"

VIEW_TYPE  ProjectSettings::default_data_type = VIEW_TYPE::ASCII;
QByteArray ProjectSettings::default_linefeed = {};

ProjectSettings::ProjectSettings(){
    setWindowTitle("Project Settings");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);


    // project file
    QLabel* path_name = new QLabel("Projects File");
    QLineEdit* path_to_project_file = new QLineEdit("");
    path_to_project_file->setPlaceholderText("/path/to/file");
    QPushButton *path_select = new QPushButton("Select");

    // default view type
    QLabel *default_data_type_label = new QLabel("Data Type");
    default_data_type_selection = new QComboBox(this);
    default_data_type_selection->addItem(VIEW_TYPE_ASCII_NAME);
    default_data_type_selection->addItem(VIEW_TYPE_HEX_NAME);
    default_data_type_selection->setCurrentIndex(static_cast<int>(default_data_type));

    // line feed selection
    QLabel* linefeed_selection_name = new QLabel("Line feed");
    linefeed_selection = new LineEndSel(this);
    linefeed_selection->setLineEnd( default_linefeed);

    // save and cancel buttons
    save = new QPushButton("Save");
    connect(save, SIGNAL(clicked()), this, SLOT(onSaved()));
    cancel = new QPushButton("Cancel");
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));

    layout = new QGridLayout();
    layout->addWidget(path_name, 0,0);
    layout->addWidget(path_to_project_file, 0,1);
    layout->addWidget(path_select, 0,2);
    layout->addWidget(default_data_type_label, 1, 0);
    layout->addWidget(default_data_type_selection, 1,1);
    layout->addWidget(linefeed_selection_name, 2,0);
    layout->addWidget(linefeed_selection, 2,1);
    layout->addWidget(save, 3, 0);
    layout->addWidget(cancel, 3, 1);
    setLayout(layout);
}

void ProjectSettings::onSaved(){
    default_data_type =static_cast<VIEW_TYPE>(default_data_type_selection->currentIndex());
    default_linefeed = linefeed_selection->getLineEnd();
    emit viewTypeUpdated(default_data_type);
    this->close();
}


