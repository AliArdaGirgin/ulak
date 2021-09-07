/****************************************************************************
** Meta object code from reading C++ file 'CommandArea.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/CommandArea.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommandArea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CommandArea_t {
    QByteArrayData data[22];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandArea_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandArea_t qt_meta_stringdata_CommandArea = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CommandArea"
QT_MOC_LITERAL(1, 12, 4), // "send"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 11), // "QByteArray&"
QT_MOC_LITERAL(4, 30, 4), // "data"
QT_MOC_LITERAL(5, 35, 8), // "DataType"
QT_MOC_LITERAL(6, 44, 5), // "dtype"
QT_MOC_LITERAL(7, 50, 9), // "addButton"
QT_MOC_LITERAL(8, 60, 4), // "name"
QT_MOC_LITERAL(9, 65, 17), // "Command::cmd_type"
QT_MOC_LITERAL(10, 83, 4), // "type"
QT_MOC_LITERAL(11, 88, 8), // "linefeed"
QT_MOC_LITERAL(12, 97, 5), // "delay"
QT_MOC_LITERAL(13, 103, 6), // "period"
QT_MOC_LITERAL(14, 110, 9), // "read_data"
QT_MOC_LITERAL(15, 120, 8), // "QWidget*"
QT_MOC_LITERAL(16, 129, 6), // "parent"
QT_MOC_LITERAL(17, 136, 8), // "dataRead"
QT_MOC_LITERAL(18, 145, 12), // "deleteButton"
QT_MOC_LITERAL(19, 158, 8), // "Command*"
QT_MOC_LITERAL(20, 167, 3), // "cmd"
QT_MOC_LITERAL(21, 171, 3) // "run"

    },
    "CommandArea\0send\0\0QByteArray&\0data\0"
    "DataType\0dtype\0addButton\0name\0"
    "Command::cmd_type\0type\0linefeed\0delay\0"
    "period\0read_data\0QWidget*\0parent\0"
    "dataRead\0deleteButton\0Command*\0cmd\0"
    "run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandArea[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    8,   44,    2, 0x0a /* Public */,
      17,    2,   61,    2, 0x0a /* Public */,
      18,    1,   66,    2, 0x0a /* Public */,
      21,    0,   69,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9, QMetaType::QByteArray, QMetaType::QByteArray, QMetaType::Int, QMetaType::Int, QMetaType::QByteArray, 0x80000000 | 15,    8,   10,    4,   11,   12,   13,   14,   16,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,

       0        // eod
};

void CommandArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CommandArea *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->send((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< DataType(*)>(_a[2]))); break;
        case 1: _t->addButton((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< Command::cmd_type(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3])),(*reinterpret_cast< QByteArray(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< QByteArray(*)>(_a[7])),(*reinterpret_cast< QWidget*(*)>(_a[8]))); break;
        case 2: _t->dataRead((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< DataType(*)>(_a[2]))); break;
        case 3: _t->deleteButton((*reinterpret_cast< Command*(*)>(_a[1]))); break;
        case 4: _t->run(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 7:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Command* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CommandArea::*)(QByteArray & , DataType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CommandArea::send)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CommandArea::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_CommandArea.data,
    qt_meta_data_CommandArea,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CommandArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CommandArea.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CommandArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CommandArea::send(QByteArray & _t1, DataType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
