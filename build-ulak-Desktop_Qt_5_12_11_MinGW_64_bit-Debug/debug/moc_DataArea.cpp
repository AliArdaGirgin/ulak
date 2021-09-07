/****************************************************************************
** Meta object code from reading C++ file 'DataArea.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/DataArea.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataArea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DataArea_t {
    QByteArrayData data[12];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataArea_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataArea_t qt_meta_stringdata_DataArea = {
    {
QT_MOC_LITERAL(0, 0, 8), // "DataArea"
QT_MOC_LITERAL(1, 9, 5), // "write"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 11), // "QByteArray&"
QT_MOC_LITERAL(4, 28, 7), // "data_in"
QT_MOC_LITERAL(5, 36, 8), // "DataType"
QT_MOC_LITERAL(6, 45, 8), // "dataType"
QT_MOC_LITERAL(7, 54, 5), // "clear"
QT_MOC_LITERAL(8, 60, 4), // "save"
QT_MOC_LITERAL(9, 65, 10), // "tabChanged"
QT_MOC_LITERAL(10, 76, 5), // "index"
QT_MOC_LITERAL(11, 82, 3) // "run"

    },
    "DataArea\0write\0\0QByteArray&\0data_in\0"
    "DataType\0dataType\0clear\0save\0tabChanged\0"
    "index\0run"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataArea[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x0a /* Public */,
       7,    0,   44,    2, 0x0a /* Public */,
       8,    0,   45,    2, 0x0a /* Public */,
       9,    1,   46,    2, 0x0a /* Public */,
      11,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,

       0        // eod
};

void DataArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DataArea *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->write((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< DataType(*)>(_a[2]))); break;
        case 1: _t->clear(); break;
        case 2: _t->save(); break;
        case 3: _t->tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->run(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DataArea::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DataArea.data,
    qt_meta_data_DataArea,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DataArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataArea::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DataArea.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DataArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
