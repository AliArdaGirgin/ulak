#include <QTest>
#include <QObject>
#include "DataArea.h"
#include "test_defs.h"

void Test_DataArea::initTestCase(){

}

void Test_DataArea::test_timestamp_update(){
    DataArea da(nullptr);
    da.timer->stop();

    // Timestamp hasnot changed
    da.write("Datadatadatadata", DATA_TYPE::TX);
    QCOMPARE(da.data.size(), 1);
    da.write("Datadatadatadata", DATA_TYPE::TX);
    QCOMPARE(da.data.size(), 1);
    QCOMPARE(da.data.back().type, DATA_TYPE::TX);

    // New data type is different, new row added
    da.write("Datadatadatadata", DATA_TYPE::RX);
    QCOMPARE(da.data.size(), 2);
    QCOMPARE(da.data.back().type, DATA_TYPE::RX);

    // Event type is same timestamp changed
    da.timestampChanged = false;
    //simulate timer timeout
    da.timer->start(0);
    QApplication::processEvents();
    da.timer->stop();
    // Check
    da.write("Datadatadatadata", DATA_TYPE::RX);
    QCOMPARE(da.data.size(), 3);
    QCOMPARE(da.data.back().type, DATA_TYPE::RX);
}

void Test_DataArea::test_timestamp_formats(){
    DataArea da(nullptr);
    // Date
    verify_date(da.getTimestamp(TIMESTAMP_FORMAT_TYPE::DATE));

    // Time
    verify_time(da.getTimestamp(TIMESTAMP_FORMAT_TYPE::TIME));

    // Date Time
    QStringList sl = da.getTimestamp(TIMESTAMP_FORMAT_TYPE::DATE_TIME).split(' ');
    QCOMPARE(sl.size(), 2);
    verify_date(sl.at(0));
    verify_time(sl.at(1));

    // Time,ms
    sl = da.getTimestamp(TIMESTAMP_FORMAT_TYPE::TIME_MS).split(',');
    QCOMPARE(sl.size(), 2);
    verify_time(sl.at(0)); // time
    QVERIFY(sl.at(1).toInt() >= 0); // ms
    QVERIFY(sl.at(1).toInt() < 1000);

    // Date Time,ms
    sl = da.getTimestamp(TIMESTAMP_FORMAT_TYPE::DATE_TIME_MS).split(',');
    QCOMPARE(sl.size(), 2);
    QVERIFY(sl.at(1).toInt() >= 0);    // ms
    QVERIFY(sl.at(1).toInt() < 1000);
    QStringList sl2 = sl.at(0).split(' ');
    QCOMPARE(sl2.size(), 2);
    verify_date(sl2.at(0)); // date
    verify_time(sl2.at(1)); // time

    // Milliseconds from start
    QString str = da.getTimestamp(TIMESTAMP_FORMAT_TYPE::MS_FROM_START);
    QVERIFY(str.toLong() >= 0);

    // Milliseconds from start
    str = da.getTimestamp(TIMESTAMP_FORMAT_TYPE::SEC_FROM_START);
    QVERIFY(str.toLong() >= 0);
}

void Test_DataArea::verify_date(QString str){
    QStringList str_list = str.split('-');
    QCOMPARE(str_list.size(), 3);
    int n = str_list.at(0).toInt(); // year
    QVERIFY( n > 2000);
    QVERIFY( n <= 2100);
    n = str_list.at(1).toInt(); // month
    QVERIFY( n > 0);
    QVERIFY( n < 13);
    n = str_list.at(2).toInt(); // day
    QVERIFY(n > 0);
    QVERIFY(n < 32);
}

void Test_DataArea::verify_time(QString str){
    QStringList str_list = str.split(':');
    QCOMPARE(str_list.size(), 3);
    int n = str_list.at(0).toInt(); // hour
    QVERIFY(n >= 0);
    QVERIFY(n < 24);
    n = str_list.at(1).toInt(); // minute
    QVERIFY(n >= 0);
    QVERIFY(n < 60);
    n = str_list.at(2).toInt(); // secs
    QVERIFY(n >= 0);
    QVERIFY(n < 60);
}

void Test_DataArea::test_tab_switch(){
    DataArea da(nullptr);
    QByteArray ba;
    ba.append(0x30); // 0
    ba.append(0x31); // 1
    ba.append(0x32); // 2
    ba.append(0x33); // 3

    // check ascii view
    da.write(ba, DATA_TYPE::TX);
    da.setCurrentTab(VIEW_TYPE::ASCII);
    QStringList sl = da.ascii->toPlainText().split('\n');
    QVERIFY(sl.at(0).startsWith("[TX]"));
    QVERIFY(sl.at(1).startsWith("0123"));
    da.clear();

    // check hex view
    da.write(ba, DATA_TYPE::RX);
    da.setCurrentTab(VIEW_TYPE::HEX);
    sl = da.hex->toPlainText().split('\n');
    QVERIFY(sl.at(0).startsWith("[RX]"));
    QVERIFY(sl.at(1).startsWith("30 31 32 33"));
}

void Test_DataArea::cleanupTestCase(){

}
