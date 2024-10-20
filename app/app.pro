QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QMAKE_CXXFLAGS += -Wno-deprecated-copy

SOURCES += \
    AddButtonWindow.cpp \
    Command.cpp \
    CommandArea.cpp \
    DataArea.cpp \
    DirectArea.cpp \
    MainWindow.cpp \
    PortCommSelection.cpp \
    PortHandler.cpp \
    PortSelection.cpp \
    ProjectSettings.cpp \
    RepresentableTextEdit.cpp \
    TabbedText.cpp \
    app.cpp

HEADERS += \
    AddButtonWindow.h \
    Command.h \
    CommandArea.h \
    Conf.h \
    DataArea.h \
    DataType.h \
    DirectArea.h \
    MainWindow.h \
    PortCommSelection.h \
    PortHandler.h \
    PortSelection.h \
    ProjectSettings.h \
    RepresentableTextEdit.h \
    TabbedText.h
RESOURCES = ../resources/resources.qrc

INCLUDEPATH += ../test

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    stylesheet.css
