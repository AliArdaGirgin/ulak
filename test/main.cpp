#include <QTest>
#include <QApplication>
#include <QDebug>
#include "test_defs.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    int status = 0;
    status |= QTest::qExec(new Test_AddButtonWindow, argc, argv);
    status |= QTest::qExec(new Test_Command, argc, argv);
    status |= QTest::qExec(new Test_DataArea, argc, argv);

    if(status){
        qDebug() << "Some tests failed";
    }else{
        qDebug() << "All tests passed";
    }
    return 0;
}
