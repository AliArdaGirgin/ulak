#ifndef TEST_DEFS_H
#define TEST_DEFS_H

#include <QWidget>

class Test_AddButtonWindow: public QObject{
    Q_OBJECT
private slots:
    void initTestCase();
    void command_create_test();
};
#endif // TEST_DEFS_H
