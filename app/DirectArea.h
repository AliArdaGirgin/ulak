#ifndef DIRECT_AREA_H
#define DIRECT_AREA_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <deque>

#include "TabbedText.h"
#include "DataType.h"

class DirectArea : public QWidget
{
    Q_OBJECT
public:
    DirectArea(QWidget *parent);
public slots:
    void enterPressed();
    void upDownArrowKeyPressed(int up);
signals:
    void send(QByteArray dt, DATA_TYPE t);

private slots:
    void onSendButton();

private:
    TabbedText   *edit;
    QPushButton *send_button;
    QComboBox   *linefeed_selection;
    QHBoxLayout *layout;
    std::deque<QByteArray*> history;
    int history_cnt;
    void addToHistory(QByteArray* dt);
};

#endif // DIRECT_H
