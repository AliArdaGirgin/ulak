#include <QTest>
#include <QWidget>
#include <QSignalSpy>
#include <QApplication>
#include <QMessageBox>
#include <QTimer>

#include "test_defs.h"
#include "DataType.h"
#include "Command.h"
#include "Conf.h"

void Test_Command::initTestCase(){

}
void Test_Command::test_oneshot_cmd(){
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "OneShotManual";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);

    QList<QVariant> args = spy.takeFirst();
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "OneShotManual");
}

void Test_Command::test_delay_cmd(){
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Delay";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 10*COMMAND_AREA_TIMER_RESOLUTION;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should not emit the signals
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);

    // Run 9 or more times should emit
    for(int i=0; i<10; i++)
        c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeFirst();
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "Delay");
}

void Test_Command::test_readtrigger_cmd(){
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "OneShotReadTrigger";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::READ_TRIGGER;
    cmd.read_data = "Trigger";
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;

    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should not emit the signals
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);

    // Random data Should not emit signal
    QByteArray arr = "asdasdasdasdasdasdasd";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);

    // Should emit signal
    arr = "Trigger";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeFirst();
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "OneShotReadTrigger");
    spy.takeFirst(); // clear spy

    // Correct data but trig type is not continous command passive, should not emit
    arr = "Trigger";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
}
void Test_Command::test_periodic_cmd(){
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "PeriodicManual";
    cmd.cmd_type = COMMAND_TYPE::PERIODIC;
    cmd.delay = 0;
    cmd.period = 10*COMMAND_AREA_TIMER_RESOLUTION;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;

    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should not emit the signals
    c.run();
    QCOMPARE(spy.count(), 0);

    // Run 9 or more times should emit the signals
    for(int i=0; i<10; i++)
        c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE); // Still active
    QList<QVariant> args = spy.takeFirst();
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "PeriodicManual");
    // Clear
    spy.takeFirst();

    // Run 9 or more times should emit the signals
    for(int i=0; i<10; i++)
        c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE); // Still active
    args = spy.takeFirst();
    dt = args.at(0).toByteArray();
    t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "PeriodicManual");
}

void Test_Command::test_readtrigger_cont(){
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "OneShotReadTrigger";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::READ_TRIGGER_CONT;
    cmd.read_data = "Trigger";
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;

    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should not emit the signals
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);

    // Random data Should not emit signal
    QByteArray arr = "asdasdasdasdasdasdasd";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 0);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);

    // Should emit signal
    arr = "Trigger";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);
    QList<QVariant> args = spy.takeFirst();
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "OneShotReadTrigger");
    spy.takeFirst(); // clear spy

    // Still active, Should emit the signal again
    arr = "Trigger";
    c.dataRead(arr);
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::ACTIVE);
    args = spy.takeFirst();
    dt = args.at(0).toByteArray();
    t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, "OneShotReadTrigger");
    spy.takeFirst();
}
void Test_Command::test_linefeed_types(){

    { // TEST LINEFEED NONE
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Linefeed";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    cmd.linefeed = LINEFEED_TYPE::NONE;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeAt(1);
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QCOMPARE(dt, QByteArray());
    spy.takeFirst();
    }

    { // TEST LINEFEED <CR>
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Linefeed";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    cmd.linefeed = LINEFEED_TYPE::CR;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeAt(1);
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QByteArray ba(1,13);
    QCOMPARE(dt, ba);
    spy.takeFirst();
    }

    { // TEST LINEFEED <LF>
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Linefeed";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    cmd.linefeed = LINEFEED_TYPE::LF;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeAt(1);
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QByteArray ba(1,10);
    QCOMPARE(dt, ba);
    spy.takeFirst();
    }

    { // TEST LINEFEED <CR><LF>
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Linefeed";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    cmd.linefeed = LINEFEED_TYPE::CR_LF;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeAt(1);
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QByteArray ba;
    ba.append(13);
    ba.append(10);
    QCOMPARE(dt, ba);
    spy.takeFirst();
    }

    { // TEST LINEFEED <\0>
    // Create command
    Command_t cmd;
    cmd.name = "Test";
    cmd.data = "Linefeed";
    cmd.cmd_type = COMMAND_TYPE::ONE_SHOT;
    cmd.delay = 0;
    cmd.trig_type = TRIGGER_TYPE::MANUAL;
    cmd.linefeed = LINEFEED_TYPE::NULL_TERMINATOR;
    Command c(cmd);
    c.current_state = COMMAND_STATE::ACTIVE;
    // set signal spy
    QSignalSpy spy(&c, SIGNAL(send(QByteArray, DATA_TYPE)));

    // run once should emit the signals
    c.run();
    QCOMPARE(spy.count(), 2);
    QCOMPARE(c.current_state, COMMAND_STATE::PASSIVE);
    QList<QVariant> args = spy.takeAt(1);
    QByteArray dt = args.at(0).toByteArray();
    DATA_TYPE  t = qvariant_cast<DATA_TYPE>(args.at(1));
    QCOMPARE(t, DATA_TYPE::TX);
    QByteArray ba(1,0);
    QCOMPARE(dt, ba);
    spy.takeFirst();
    }
}

void Test_Command::cleanupTestCase(){

}

