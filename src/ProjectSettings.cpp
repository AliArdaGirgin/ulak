#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include "ProjectSettings.h"
#include "DataType.h"

VIEW_TYPE  ProjectSettings::default_view_type = VIEW_TYPE::ASCII;
LINEFEED_TYPE ProjectSettings::default_linefeed = {};
QString    ProjectSettings::project_file_name = {};

ProjectSettings::ProjectSettings(){
    setWindowTitle("Project Settings");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);

    // default view type
    QLabel *default_data_type_label = new QLabel("Data Type");
    default_view_type_selection = new QComboBox(this);
    default_view_type_selection->addItem(VIEW_TYPE_ASCII_NAME);
    default_view_type_selection->addItem(VIEW_TYPE_HEX_NAME);
    default_view_type_selection->setCurrentIndex(static_cast<int>(default_view_type));

    // line feed selection
    QLabel* default_linefeed_selection_name = new QLabel("Linefeed");
    default_linefeed_selection = new QComboBox();
    default_linefeed_selection->addItem(LINEFEED_TYPE_CR_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_CR_LF_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_0_NAME);
    default_linefeed_selection->setCurrentIndex(static_cast<int>(default_linefeed));


    // save and cancel buttons
    save = new QPushButton("Ok");
    connect(save, SIGNAL(clicked()), this, SLOT(onSaved()));
    cancel = new QPushButton("Cancel");
    connect(cancel, SIGNAL(clicked()), this, SLOT(close()));

    layout = new QGridLayout();
    layout->addWidget(default_data_type_label, 0, 0);
    layout->addWidget(default_view_type_selection, 0, 1);
    layout->addWidget(default_linefeed_selection_name, 1,0);
    layout->addWidget(default_linefeed_selection, 1,1);
    layout->addWidget(save, 2, 0);
    layout->addWidget(cancel, 2, 1);
    setLayout(layout);
}

void ProjectSettings::onSaved(){

    // view type
    default_view_type =static_cast<VIEW_TYPE>(default_view_type_selection->currentIndex());
    emit viewTypeUpdated(default_view_type);

    // linefeed
    default_linefeed = static_cast<LINEFEED_TYPE>(default_linefeed_selection->currentIndex());
    this->close();
}


