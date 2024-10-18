#ifndef TEST_DEFS_H
#define TEST_DEFS_H

#include <QWidget>

class Test_AddButtonWindow: public QObject{
    Q_OBJECT
public slots:
    void closeMessageBox();

private slots:
    void initTestCase();
    void test_command_create();
    void test_name_field_empty();
    void test_data_field_empty();
    void test_read_empty_when_read_trigger();
    void cleanupTestCase();
private:
    QTimer *timer;
};

class Test_Command: public QObject{
    Q_OBJECT
private slots:
    void initTestCase();
    void test_oneshot_cmd();
    void test_delay_cmd();
    void test_periodic_cmd();
    void test_readtrigger_cmd();
    void test_linefeed_types();
    void cleanupTestCase();
};
#endif // TEST_DEFS_H
