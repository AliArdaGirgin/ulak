/****************************************************************************
** Meta object code from reading C++ file 'Command.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/Command.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Command_t {
    QByteArrayData data[19];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Command_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Command_t qt_meta_stringdata_Command = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Command"
QT_MOC_LITERAL(1, 8, 8), // "onDelete"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "Command*"
QT_MOC_LITERAL(4, 27, 8), // "activate"
QT_MOC_LITERAL(5, 36, 4), // "stop"
QT_MOC_LITERAL(6, 41, 8), // "settings"
QT_MOC_LITERAL(7, 50, 3), // "del"
QT_MOC_LITERAL(8, 54, 6), // "update"
QT_MOC_LITERAL(9, 61, 4), // "name"
QT_MOC_LITERAL(10, 66, 17), // "Command::cmd_type"
QT_MOC_LITERAL(11, 84, 8), // "ctype_in"
QT_MOC_LITERAL(12, 93, 7), // "data_in"
QT_MOC_LITERAL(13, 101, 11), // "linefeed_in"
QT_MOC_LITERAL(14, 113, 8), // "delay_in"
QT_MOC_LITERAL(15, 122, 9), // "period_in"
QT_MOC_LITERAL(16, 132, 12), // "read_data_in"
QT_MOC_LITERAL(17, 145, 8), // "QWidget*"
QT_MOC_LITERAL(18, 154, 6) // "parent"

    },
    "Command\0onDelete\0\0Command*\0activate\0"
    "stop\0settings\0del\0update\0name\0"
    "Command::cmd_type\0ctype_in\0data_in\0"
    "linefeed_in\0delay_in\0period_in\0"
    "read_data_in\0QWidget*\0parent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Command[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   62,    2, 0x0a /* Public */,
       5,    0,   63,    2, 0x0a /* Public */,
       6,    0,   64,    2, 0x0a /* Public */,
       7,    0,   65,    2, 0x0a /* Public */,
       8,    8,   66,    2, 0x0a /* Public */,
       8,    7,   83,    2, 0x2a /* Public | MethodCloned */,
       8,    6,   98,    2, 0x2a /* Public | MethodCloned */,
       8,    5,  111,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray, 0x80000000 | 17,    9,   11,   12,   13,   14,   15,   16,   18,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray,    9,   11,   12,   13,   14,   15,   16,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int,    9,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10, QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::Int,    9,   11,   12,   13,   14,

       0        // eod
};

void Command::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Command *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDelete((*reinterpret_cast< Command*(*)>(_a[1]))); break;
        case 1: _t->activate(); break;
        case 2: _t->stop(); break;
        case 3: _t->settings(); break;
        case 4: _t->del(); break;
        case 5: _t->update((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Command::cmd_type(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QByteArray(*)>(_a[7])),(*reinterpret_cast< QWidget*(*)>(_a[8]))); break;
        case 6: _t->update((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Command::cmd_type(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QByteArray(*)>(_a[7]))); break;
        case 7: _t->update((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Command::cmd_type(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6]))); break;
        case 8: _t->update((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Command::cmd_type(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Command* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 7:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Command::*)(Command * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Command::onDelete)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Command::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_Command.data,
    qt_meta_data_Command,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Command::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Command::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Command.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Command::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Command::onDelete(Command * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
