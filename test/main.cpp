#include <QTest>
#include <QApplication>
#include "test_defs.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QTest::qExec(new Test_AddButtonWindow, argc, argv);
    QTest::qExec(new Test_Command, argc, argv);

    return 0;
}
