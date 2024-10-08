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


#endif // DATATYPE_H
