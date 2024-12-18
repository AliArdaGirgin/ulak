QT += testlib
QT += gui core widgets serialport
QT += network
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
    ../app/PortSelection.cpp \
    ../app/PortSelection_Comm.cpp \
    ../app/PortSelection_TCP.cpp \
    ../app/PortHandler_Base.cpp \
    ../app/PortHandler_Comm.cpp \
    ../app/PortHandler_TCP.cpp \
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
    ../app/PortSelection_Comm.h \
    ../app/PortSelection_TCP.h \
    ../app/PortHandler_Base.h \
    ../app/PortHandler_Comm.h \
    ../app/PortHandler_TCP.h \
    ../app/PortSelection.h \
    ../app/ProjectSettings.h \
    ../app/RepresentableTextEdit.h \
    ../app/TabbedText.h \
    ../app/DirectArea.h

RESOURCES = ../resources/resources.qrc
