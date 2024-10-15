#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>
#include <QByteArray>

enum class DATA_TYPE{TX,RX};

enum class COMMAND_TYPE{ONE_SHOT, PERIODIC, READ_TRIGGER};
#define COMMAND_TYPE_ONESHOT_NAME "One-Shot"
#define COMMAND_TYPE_PERIODIC_NAME "Periodic"
#define COMMAND_TYPE_READTRIGGER_NAME "Read-Trigger"

enum class COMMAND_STATE{ACTIVE, FROZEN, PASSIVE};

enum class VIEW_TYPE{ ASCII, HEX};
#define VIEW_TYPE_ASCII_NAME "ASCII"
#define VIEW_TYPE_HEX_NAME  "Hex"

enum class LINEFEED_TYPE {NONE, CR, LF, CR_LF, NULL_TERMINATOR};
#define LINEFEED_TYPE_NONE_NAME "None"
#define LINEFEED_TYPE_CR_NAME "<CR>"
#define LINEFEED_TYPE_LF_NAME "<LF>"
#define LINEFEED_TYPE_CR_LF_NAME "<CR><LF>"
#define LINEFEED_TYPE_0_NAME "</0>"

enum class TIMESTAMP_FORMAT_TYPE {DATE, TIME, DATE_TIME,
                                  TIME_MS, DATE_TIME_MS,
                                  MS_FROM_START, SEC_FROM_START};
#define TIMESTAMP_DATE_NAME "Date "
#define TIMESTAMP_TIME_NAME "Time "
#define TIMESTAMP_DATE_TIME_NAME (TIMESTAMP_DATE_NAME TIMESTAMP_TIME_NAME)
#define TIMESTAMP_TIME_MS_NAME (TIMESTAMP_TIME_NAME "Milliseconds")
#define TIMESTAMP_DATE_TIME_MS_NAME \
    (TIMESTAMP_DATE_NAME TIMESTAMP_TIME_NAME "Milliseconds")
#define TIMESTAMP_MS_FROM_START_NAME "Millisecs from program started"
#define TIMESTAMP_SEC_FROM_START_NAME "Seconds from program started"

struct Command_t{
    QString name;
    COMMAND_TYPE cmd_type;
    QByteArray data;
    int last_tab;
    LINEFEED_TYPE linefeed;
    int delay;
    int period;
    QByteArray read_data;
    LINEFEED_TYPE read_linefeed;
    int read_last_tab;
};
#endif // DATATYPE_H
