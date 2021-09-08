#ifndef DATAAREA_H
#define DATAAREA_H

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
#include "PortHandler.h"
#include "DataType.h"

struct TimestampedData{
    DataType type;
    QString timestamp;
    QByteArray dt;
};

class DataArea : public QWidget{
    Q_OBJECT
    public:
        static const int TIMER_RES;
        DataArea(PortHandler *pHandler,QWidget *parent = 0);
    public slots:
        void write(QByteArray &data_in, DataType dataType);
        void clear();
        void save();
        void tabChanged(int index);
    private slots:
        void run();
    private:
        QByteArray  time_stamp;
        QGridLayout *layout;
        PortHandler *port_handler;
        QTabWidget  *tabbed;
        QTimer      *timer;
        QString     lastTimestamp;
        QString     prevTimestamp;
        QTextEdit   *ascii;
        int         ascii_index;
        QTextEdit   *hex;
        int         hex_index;
        int         current_index;
        QVector<TimestampedData> data;
        TimestampedData currentData;
        bool timestampChanged;

};



#endif
