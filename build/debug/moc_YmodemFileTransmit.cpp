/****************************************************************************
** Meta object code from reading C++ file 'YmodemFileTransmit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/YmodemFileTransmit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'YmodemFileTransmit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_YmodemFileTransmit_t {
    QByteArrayData data[9];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_YmodemFileTransmit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_YmodemFileTransmit_t qt_meta_stringdata_YmodemFileTransmit = {
    {
QT_MOC_LITERAL(0, 0, 18), // "YmodemFileTransmit"
QT_MOC_LITERAL(1, 19, 16), // "transmitProgress"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 8), // "progress"
QT_MOC_LITERAL(4, 46, 14), // "transmitStatus"
QT_MOC_LITERAL(5, 61, 26), // "YmodemFileTransmit::Status"
QT_MOC_LITERAL(6, 88, 6), // "status"
QT_MOC_LITERAL(7, 95, 11), // "readTimeOut"
QT_MOC_LITERAL(8, 107, 12) // "writeTimeOut"

    },
    "YmodemFileTransmit\0transmitProgress\0"
    "\0progress\0transmitStatus\0"
    "YmodemFileTransmit::Status\0status\0"
    "readTimeOut\0writeTimeOut"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_YmodemFileTransmit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   40,    2, 0x08 /* Private */,
       8,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void YmodemFileTransmit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        YmodemFileTransmit *_t = static_cast<YmodemFileTransmit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transmitProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->transmitStatus((*reinterpret_cast< YmodemFileTransmit::Status(*)>(_a[1]))); break;
        case 2: _t->readTimeOut(); break;
        case 3: _t->writeTimeOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (YmodemFileTransmit::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&YmodemFileTransmit::transmitProgress)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (YmodemFileTransmit::*_t)(YmodemFileTransmit::Status );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&YmodemFileTransmit::transmitStatus)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject YmodemFileTransmit::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_YmodemFileTransmit.data,
      qt_meta_data_YmodemFileTransmit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *YmodemFileTransmit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *YmodemFileTransmit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_YmodemFileTransmit.stringdata0))
        return static_cast<void*>(const_cast< YmodemFileTransmit*>(this));
    if (!strcmp(_clname, "Ymodem"))
        return static_cast< Ymodem*>(const_cast< YmodemFileTransmit*>(this));
    return QObject::qt_metacast(_clname);
}

int YmodemFileTransmit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void YmodemFileTransmit::transmitProgress(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void YmodemFileTransmit::transmitStatus(YmodemFileTransmit::Status _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
