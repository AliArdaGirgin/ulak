#include <QTest>
#include <QWidget>
#include <QSignalSpy>
#include <QApplication>
#include "AddButtonWindow.h"
#include "test_defs.h"
#include "DataType.h"

void Test_AddButtonWindow::initTestCase(){

}
void Test_AddButtonWindow::command_create_test(){
    AddButtonWindow w;

    // Register custom type to be used
    qRegisterMetaType<Command_t>();
    // set signal spy
    QSignalSpy spy(&w, SIGNAL(onButtonAdded(Command_t, QWidget*)));

    // Create addbuttonwindow
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

    // Check command values
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

QTEST_MAIN(Test_AddButtonWindow)
#include "test_addbuttonwindow.moc"

