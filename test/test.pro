QT += testlib
QT += gui core widgets serialport
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

INCLUDEPATH += ../app

SOURCES +=  \
    main.cpp \
    test_addbuttonwindow.cpp \
    ../app/AddButtonWindow.cpp \
    ../app/Command.cpp \
    ../app/CommandArea.cpp \
    ../app/DataArea.cpp \
    ../app/MainWindow.cpp \
    ../app/PortCommSelection.cpp \
    ../app/PortHandler.cpp \
    ../app/PortSelection.cpp \
    ../app/ProjectSettings.cpp \
    ../app/RepresentableTextEdit.cpp \
    ../app/TabbedText.cpp \
    ../app/DirectArea.cpp \
    test_command.cpp \
    test_dataarea.cpp \
    test_directarea.cpp

HEADERS += \
    test_defs.h \
    ../app/AddButtonWindow.h \
    ../app/Command.h \
    ../app/CommandArea.h \
    ../app/Conf.h \
    ../app/DataArea.h \
    ../app/DataType.h \
    ../app/MainWindow.h \
    ../app/PortCommSelection.h \
    ../app/PortHandler.h \
    ../app/PortSelection.h \
    ../app/ProjectSettings.h \
    ../app/RepresentableTextEdit.h \
    ../app/TabbedText.h \
    ../app/DirectArea.h

RESOURCES = ../resources/resources.qrc
