#ifndef DATATYPE_H
#define DATATYPE_H

#include <QString>
#include <QByteArray>

enum class DATA_TYPE{TX,RX};

enum class COMMAND_TYPE{ONE_SHOT, PERIODIC, MAX};
#define COMMAND_TYPE_ONESHOT_NAME "One-Shot"
#define COMMAND_TYPE_PERIODIC_NAME "Periodic"

enum class TRIGGER_TYPE{MANUAL,READ_TRIGGER, READ_TRIGGER_CONT, MAX};
#define TRIGGER_TYPE_MANUAL_NAME "Manual"
#define TRIGGER_TYPE_READTRIGGER_NAME "Read Trigger Single"
#define TRIGGER_TYPE_READTRIGGER_CONT_NAME "Read Trigger Continuous"

enum class COMMAND_STATE{ACTIVE, FROZEN, PASSIVE};

enum class VIEW_TYPE{ ASCII, HEX, MAX};
#define VIEW_TYPE_ASCII_NAME "ASCII"
#define VIEW_TYPE_HEX_NAME  "Hex"

enum class LINEFEED_TYPE {NONE, CR, LF, CR_LF, NULL_TERMINATOR, MAX};
#define LINEFEED_TYPE_NONE_NAME "None"
#define LINEFEED_TYPE_CR_NAME "<CR>"
#define LINEFEED_TYPE_LF_NAME "<LF>"
#define LINEFEED_TYPE_CR_LF_NAME "<CR><LF>"
#define LINEFEED_TYPE_0_NAME "</0>"

enum class TIMESTAMP_FORMAT_TYPE {DATE, TIME, DATE_TIME,
                                  TIME_MS, DATE_TIME_MS,
                                  MS_FROM_START, SEC_FROM_START,
                                  MAX};
#define TIMESTAMP_DATE_NAME "Date "
#define TIMESTAMP_TIME_NAME "Time "
#define TIMESTAMP_DATE_TIME_NAME (TIMESTAMP_DATE_NAME TIMESTAMP_TIME_NAME)
#define TIMESTAMP_TIME_MS_NAME (TIMESTAMP_TIME_NAME "Milliseconds")
#define TIMESTAMP_DATE_TIME_MS_NAME \
    (TIMESTAMP_DATE_NAME TIMESTAMP_TIME_NAME "Milliseconds")
#define TIMESTAMP_MS_FROM_START_NAME "Millisecs from program started"
#define TIMESTAMP_SEC_FROM_START_NAME "Seconds from program started"

#define PROJ_JSON_SETT_VIEWTYPE_STR  "view_type"
#define PROJ_JSON_SETT_LINEFEED_STR  "linefeed"
#define PROJ_JSON_SETT_TIMESTAMP_STR "timestamp_format"
#define PROJ_JSON_CMD_NAME_STR       "name"
#define PROJ_JSON_CMD_CMDTYPE_STR    "cmd_type"
#define PROJ_JSON_CMD_DELAY_STR      "delay"
#define PROJ_JSON_CMD_PERIOD_STR     "period"
#define PROJ_JSON_CMD_LINEFEED_STR   "linefeed"
#define PROJ_JSON_CMD_DATA_STR       "data"
#define PROJ_JSON_CMD_DATATAB_STR    "data_tab"
#define PROJ_JSON_CMD_TRIG_STR       "trigger_type"
#define PROJ_JSON_CMD_READDATA_STR   "read_data"
#define PROJ_JSON_CMD_READDATATAB_STR "read_data_tab"

#define PORT_TCP_CLIENT 0
#define PORT_TCP_SERVER 1

#define PORT_TCP_CLIENT_LOCALHOST 0
#define PORT_TCP_CLIENT_MANUAL    1

#define PORT_TCP_SERVER_LOCALHOST      0
#define PORT_TCP_SERVER_LOCALHOST_IPv6 1
#define PORT_TCP_SERVER_BROADCAST      2
#define PORT_TCP_SERVER_ANY_IPv4       3
#define PORT_TCP_SERVER_ANY_IPv6       4
#define PORT_TCP_SERVER_ANY            5
#define PORT_TCP_SERVER_MANUAL         6


struct Command_t{
    QString name;
    COMMAND_TYPE cmd_type;
    TRIGGER_TYPE trig_type;
    QByteArray data;
    VIEW_TYPE last_tab;
    LINEFEED_TYPE linefeed;
    int delay;
    int period;
    QByteArray read_data;
    VIEW_TYPE read_last_tab;
};
#endif // DATATYPE_H
