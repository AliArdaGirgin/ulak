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
#endif // TEST_DEFS_H
