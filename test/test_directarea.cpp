#include <QTest>
#include <QSignalSpy>

#include "DirectArea.h"
#include "test_defs.h"
#include "DataType.h"
#include "Conf.h"

void Test_DirectArea::test_ascii_send(){
    DirectArea da;
    da.check_comm = false;
    da.edit->setCurrentIndex(static_cast<int>(VIEW_TYPE::ASCII));

    QSignalSpy spy(&da, SIGNAL(send(QByteArray, DATA_TYPE)));

    // Test send button
    da.linefeed_selection->setCurrentIndex(static_cast<int>(LINEFEED_TYPE::NONE));
    QTest::keyClicks(da.edit->currentWidget(), "asciidata");
    QTest::mouseClick(da.send_button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    QByteArray dt = spy.takeFirst().at(0).toByteArray();
    QCOMPARE(dt, "asciidata");
    QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned

    // Test Enter
    QTest::keyClicks(da.edit->currentWidget(), "asciidata");
    QTest::keyClick(da.edit->currentWidget(), Qt::Key_Return);

    QCOMPARE(spy.count(), 1);
    dt = spy.takeFirst().at(0).toByteArray();
    QCOMPARE(dt, "asciidata");
    QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned
}

void Test_DirectArea::test_hex_send(){
    DirectArea da;
    da.check_comm = false;
    da.edit->setCurrentIndex(static_cast<int>(VIEW_TYPE::HEX));

    QSignalSpy spy(&da, SIGNAL(send(QByteArray, DATA_TYPE)));

    // Test send button
    da.linefeed_selection->setCurrentIndex(static_cast<int>(LINEFEED_TYPE::NONE));
    QTest::keyClicks(da.edit->currentWidget(), "31 32 33 34");
    QTest::mouseClick(da.send_button, Qt::LeftButton);

    QCOMPARE(spy.count(), 1);
    QByteArray dt = spy.takeFirst().at(0).toByteArray();
    QByteArray c;
    c.append(0x31);
    c.append(0x32);
    c.append(0x33);
    c.append(0x34);
    QCOMPARE(dt, c);
    QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned

    // Test Enter
    QTest::keyClicks(da.edit->currentWidget(), "31 32 33 34");
    QTest::keyClick(da.edit->currentWidget(), Qt::Key_Return);

    QCOMPARE(spy.count(), 1);
    dt = spy.takeFirst().at(0).toByteArray();
    QCOMPARE(dt, c);
    QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned
}

void Test_DirectArea::test_history(){
    DirectArea da;
    da.check_comm = false;
    da.edit->setCurrentIndex(static_cast<int>(VIEW_TYPE::ASCII));
    da.linefeed_selection->setCurrentIndex(static_cast<int>(LINEFEED_TYPE::NONE));

    // Single Test ----------------------------------------------
    {
        // Command send must be added to history
        QTest::keyClicks(da.edit->currentWidget(), "First");
        QTest::keyClick(da.edit->currentWidget(), Qt::Key_Return);
        QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned

        // Check history
        QTest::keyClick(da.edit->currentWidget(), Qt::Key_Up); // up
        QCOMPARE(da.edit->getData(), "First");
        QTest::keyClick(da.edit->currentWidget(), Qt::Key_Up); // up again, shouldnot change if on last
        QCOMPARE(da.edit->getData(), "First");

        // return to normal enter
        QTest::keyClick(da.edit->currentWidget(), Qt::Key_Down);
        QVERIFY(da.edit->isDataEmpty());
    }

    da.clearHistory();

    // Multiple Commands Test ------------------------------------
    {
        // Command send must be added to history
        for(int i=0; i<10; i++){
            QTest::keyClicks(da.edit->currentWidget(), QString::number(i));
            QTest::keyClick(da.edit->currentWidget(), Qt::Key_Return);
        }
        // Check history
        QVERIFY(da.edit->isDataEmpty()); // make sure text area cleaned

        // check history
        for(int i=9; i>=0; i--){
            QTest::keyClick(da.edit->currentWidget(), Qt::Key_Up);
            QCOMPARE(da.edit->getData(), QString::number(i));
        }
    }

    da.clearHistory();

    // Check DIRECT_AREA_HISTORY_SIZE limit
    {
        // Add to history more than limit
        for(int i=0; i<DIRECT_AREA_HISTORY_SIZE+10; i++){
            QTest::keyClicks(da.edit->currentWidget(), QString::number(i));
            QTest::keyClick(da.edit->currentWidget(), Qt::Key_Return);
        }
        QCOMPARE(da.history.size(), DIRECT_AREA_HISTORY_SIZE);

        for(int i=DIRECT_AREA_HISTORY_SIZE-1; i>=0; i--){
            QTest::keyClick(da.edit->currentWidget(), Qt::Key_Up);
            QCOMPARE(da.edit->getData(), QString::number(i+10));
        }

    }
}
