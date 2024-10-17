#include <QCoreApplication>
#include <QtTest>

// add necessary includes here

class AddButtonWindow : public QObject
{
    Q_OBJECT

public:
    AddButtonWindow();
    ~AddButtonWindow();

private slots:
    void test_case1();
};

AddButtonWindow::AddButtonWindow() {}

AddButtonWindow::~AddButtonWindow() {}

void AddButtonWindow::test_case1() {}

QTEST_MAIN(AddButtonWindow)

#include "tst_addbuttonwindow.moc"
