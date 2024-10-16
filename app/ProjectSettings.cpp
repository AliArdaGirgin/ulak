#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include "ProjectSettings.h"
#include "DataType.h"

QString    ProjectSettings::project_file_name = {};

VIEW_TYPE  ProjectSettings::default_view_type = VIEW_TYPE::ASCII;
LINEFEED_TYPE ProjectSettings::default_linefeed = LINEFEED_TYPE::NONE;
TIMESTAMP_FORMAT_TYPE ProjectSettings::default_timestamp_format = TIMESTAMP_FORMAT_TYPE::TIME_MS;

ProjectSettings::ProjectSettings(){
    setWindowTitle("Project Settings");
    // Block input to other windows
    setWindowModality(Qt::ApplicationModal);
    // Always on top of main window
    setWindowFlags(Qt::WindowStaysOnTopHint);
    // Delete on close
    setAttribute(Qt::WA_DeleteOnClose);

    // default view type
    QLabel *default_data_type_label = new QLabel("Data Type");
    default_view_type_selection = new QComboBox(this);
    default_view_type_selection->addItem(VIEW_TYPE_ASCII_NAME);
    default_view_type_selection->addItem(VIEW_TYPE_HEX_NAME);
    default_view_type_selection->setCurrentIndex(static_cast<int>(default_view_type));

    // line feed selection
    QLabel* default_linefeed_selection_name = new QLabel("Linefeed");
    default_linefeed_selection = new QComboBox();
    default_linefeed_selection->addItem(LINEFEED_TYPE_NONE_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_CR_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_LF_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_CR_LF_NAME);
    default_linefeed_selection->addItem(LINEFEED_TYPE_0_NAME);
    default_linefeed_selection->setCurrentIndex(static_cast<int>(default_linefeed));

    QLabel* default_timestamp_format_name = new QLabel("Timestamp Format");
    default_timestamp_selection = new QComboBox(this);
    default_timestamp_selection->addItem(TIMESTAMP_DATE_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_TIME_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_DATE_TIME_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_TIME_MS_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_DATE_TIME_MS_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_MS_FROM_START_NAME);
    default_timestamp_selection->addItem(TIMESTAMP_SEC_FROM_START_NAME);
    default_timestamp_selection->setCurrentIndex(static_cast<int>(default_timestamp_format));

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
    layout->addWidget(default_timestamp_format_name, 2, 0);
    layout->addWidget(default_timestamp_selection, 2, 1);
    layout->addWidget(save, 3, 0);
    layout->addWidget(cancel, 3, 1);
    setLayout(layout);
}

void ProjectSettings::onSaved(){

    // view type
    default_view_type =static_cast<VIEW_TYPE>(default_view_type_selection->currentIndex());
    emit viewTypeUpdated(default_view_type);

    // linefeed
    default_linefeed = static_cast<LINEFEED_TYPE>(default_linefeed_selection->currentIndex());

    // timestam format
    default_timestamp_format = static_cast<TIMESTAMP_FORMAT_TYPE>( default_timestamp_selection->currentIndex());

    this->close();
}

void ProjectSettings::setParamatersToDefault(){
    default_view_type = VIEW_TYPE::ASCII;
    default_linefeed = LINEFEED_TYPE::NONE;
    default_timestamp_format = TIMESTAMP_FORMAT_TYPE::TIME_MS;
}

