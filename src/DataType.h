#ifndef DATATYPE_H
#define DATATYPE_H

enum class DataType{TX,RX};

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


#endif // DATATYPE_H
