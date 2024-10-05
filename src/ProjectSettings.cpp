#include <QLabel>
#include <QLineEdit>
#include "ProjectSettings.h"

ProjectSettings::ProjectSettings(){
    setWindowTitle("Project Settings");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);


    QLabel *default_data_type_label = new QLabel("Data Type");
    default_data_type = new QComboBox(this);
    default_data_type->addItem("ASCII");
    default_data_type->addItem("Hex");

    QLabel* path_name = new QLabel("Projects File");
    QLineEdit* path_to_project_file = new QLineEdit("");
    path_to_project_file->setPlaceholderText("/path/to/file");
    QPushButton *path_select = new QPushButton("Select");

    QLabel* linefeed_name = new QLabel("Line feed");
    linefeed = new LineEndSel(this);

    layout = new QGridLayout();
    layout->addWidget(path_name, 0,0);
    layout->addWidget(path_to_project_file, 0,1);
    layout->addWidget(path_select, 0,2);
    layout->addWidget(default_data_type_label, 1, 0);
    layout->addWidget(default_data_type, 1,1);
    layout->addWidget(linefeed_name, 2,0);
    layout->addWidget(linefeed, 2,1);
    setLayout(layout);
}

