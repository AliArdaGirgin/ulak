#ifndef DATAAREA_H
#define DATAAREA_H

#include <chrono>
#include <QWidget>
#include <QTextEdit>
#include <QObject>
#include <QGridLayout>
#include <QSerialPort>
#include <QTabWidget>
#include <QByteArray>
#include <QString>
#include <QVector>
#include <QTime>
#include <QTimer>
#include <functional>
#include "DataType.h"
#include "test_defs.h"

struct TimestampedData{
    DATA_TYPE type;
    QString timestamp;
    QByteArray dt;
};

class DataArea : public QWidget{
    Q_OBJECT
    friend Test_DataArea;
    public:
        DataArea(QWidget *parent = 0);
    public slots:
        void write(QByteArray data_in, DATA_TYPE dataType);
        void clear();
        void save();
        void tabChanged(int);
        void setCurrentTab(VIEW_TYPE);
    private slots:
        void run();
    private:
        QByteArray  time_stamp;
        QGridLayout *layout;
        QTabWidget  *tabbed;
        QTimer      *timer;
        QString     lastTimestamp;
        QTextEdit   *ascii;
        int         ascii_index;
        QTextEdit   *hex;
        int         hex_index;
        int         current_index;
        QVector<TimestampedData> data;
        TimestampedData currentData;
        bool timestampChanged;
        std::chrono::time_point<std::chrono::system_clock> time_prog_start;
        void textFieldUpdate(QTextEdit*, std::function<QString(QByteArray&)>);
        QString getTimestamp(TIMESTAMP_FORMAT_TYPE);
};




#endif
