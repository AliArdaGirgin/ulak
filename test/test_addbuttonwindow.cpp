#include <QTest>
#include <QWidget>
#include <QSignalSpy>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include "AddButtonWindow.h"
#include "test_defs.h"
#include "DataType.h"

void Test_AddButtonWindow::initTestCase(){

    // Register custom type to be used
    qRegisterMetaType<Command_t>();

    // Timer to get rid of QMessageBox when testing
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(1);
    connect(timer, SIGNAL(timeout()), this, SLOT(closeMessageBox()));
}
void Test_AddButtonWindow::test_command_create(){
    AddButtonWindow w;

    // set signal spy
    QSignalSpy spy(&w, SIGNAL(onButtonAdded(Command_t, QWidget*)));

    // Create addbuttonwindow and change all parameters from default
    QTest::keyClicks(w.name_text, "Test");
    w.data_tabbedText->setData("Data");
    w.data_tabbedText->setCurrentIndex(static_cast<int>(VIEW_TYPE::HEX));
    w.linefeed_selection->setCurrentIndex(static_cast<int>(LINEFEED_TYPE::CR));
    w.command_cbox->setCurrentIndex(static_cast<int>(COMMAND_TYPE::PERIODIC));
    w.delay_text->setText("12340");
    w.period_text->setText("4560");
    w.trigger_cbox->setCurrentIndex(static_cast<int>(TRIGGER_TYPE::READ_TRIGGER));
    w.read_data_text->setData("ReadData");
    w.read_data_text->setCurrentIndex(static_cast<int>(VIEW_TYPE::HEX));
    // Click Ok
    QTest::mouseClick(w.ok_button, Qt::MouseButton::LeftButton);

    // Check command parameters
    Command_t cmd = qvariant_cast<Command_t>(spy.takeFirst().at(0));
    QCOMPARE(cmd.name, "Test");
    QCOMPARE(cmd.data, "Data");
    QCOMPARE(cmd.last_tab, VIEW_TYPE::HEX);
    QCOMPARE(cmd.cmd_type, COMMAND_TYPE::PERIODIC);
    QCOMPARE(cmd.linefeed, LINEFEED_TYPE::CR);
    QCOMPARE(cmd.delay, 12340);
    QCOMPARE(cmd.period, 4560);
    QCOMPARE(cmd.trig_type, TRIGGER_TYPE::READ_TRIGGER);
    QCOMPARE(cmd.read_data, "ReadData");
    QCOMPARE(cmd.read_last_tab, VIEW_TYPE::HEX);
}

void Test_AddButtonWindow::test_name_field_empty(){
    AddButtonWindow w;

    // set signal spy
    QSignalSpy spy(&w, SIGNAL(onButtonAdded(Command_t, QWidget*)));

    // set name field empty
    QTest::keyClicks(w.name_text,"");

    // Close QMessageBox
    timer->start();

    //Click Ok
    QTest::mouseClick(w.ok_button, Qt::MouseButton::LeftButton);

    // Make sure timer is closed
    while(timer->isActive());

    // Should not emit buttonAdded signel
    QCOMPARE(spy.count(), 0);
}

void Test_AddButtonWindow::test_data_field_empty(){
    AddButtonWindow w;

    // set signal spy
    QSignalSpy spy(&w, SIGNAL(onButtonAdded(Command_t, QWidget*)));

    // name field filled
    QTest::keyClicks(w.name_text,"Start");

    // set data field empty
    w.data_tabbedText->setData(QByteArray());

    // Close QMessageBox
    timer->start();
    //Click Ok
    QTest::mouseClick(w.ok_button, Qt::MouseButton::LeftButton);

    // Make sure timer is closed
    while(timer->isActive());

    // Should not emit buttonAdded signal
    QCOMPARE(spy.count(), 0);
}

void Test_AddButtonWindow::test_read_empty_when_read_trigger(){
    AddButtonWindow w;

    // set signal spy
    QSignalSpy spy(&w, SIGNAL(onButtonAdded(Command_t, QWidget*)));

    // name field filled
    QTest::keyClicks(w.name_text,"Start");

    // data field filled
    w.data_tabbedText->setData("Test");

    // trigger type = read_trigger
    w.trigger_cbox->setCurrentIndex(static_cast<int>(TRIGGER_TYPE::READ_TRIGGER));

    // but read_data empty
    w.read_data_text->setData(QByteArray());

    // Close QMessageBox
    timer->start();

    //Click Ok
    QTest::mouseClick(w.ok_button, Qt::MouseButton::LeftButton);

    // Make sure timer is closed
    while(timer->isActive());

    // Should not emit buttonAdded signal
    QCOMPARE(spy.count(), 0);
}

void Test_AddButtonWindow::closeMessageBox(){
    QWidgetList allToplevelWidgets = QApplication::topLevelWidgets();
    foreach(QWidget *w, allToplevelWidgets){
        if(w->inherits("QMessageBox")){
            w->close();
        }
    }
}

void Test_AddButtonWindow::cleanupTestCase(){
    // make sure timer is closed
    while(timer->isActive());
}

QTEST_MAIN(Test_AddButtonWindow)
#include "test_addbuttonwindow.moc"

