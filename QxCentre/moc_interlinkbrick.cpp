/****************************************************************************
** Meta object code from reading C++ file 'interlinkbrick.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "interlinkbrick.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'interlinkbrick.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_InterlinkBrick_t {
    QByteArrayData data[9];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InterlinkBrick_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InterlinkBrick_t qt_meta_stringdata_InterlinkBrick = {
    {
QT_MOC_LITERAL(0, 0, 14), // "InterlinkBrick"
QT_MOC_LITERAL(1, 15, 14), // "taskbarUpdated"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "windowTitles"
QT_MOC_LITERAL(4, 44, 15), // "appCountChanged"
QT_MOC_LITERAL(5, 60, 5), // "count"
QT_MOC_LITERAL(6, 66, 18), // "handleWindowClosed"
QT_MOC_LITERAL(7, 85, 12), // "WindowBrick*"
QT_MOC_LITERAL(8, 98, 6) // "window"

    },
    "InterlinkBrick\0taskbarUpdated\0\0"
    "windowTitles\0appCountChanged\0count\0"
    "handleWindowClosed\0WindowBrick*\0window"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InterlinkBrick[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   35,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Int,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void InterlinkBrick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InterlinkBrick *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->taskbarUpdated((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->appCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->handleWindowClosed((*reinterpret_cast< WindowBrick*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< WindowBrick* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InterlinkBrick::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InterlinkBrick::taskbarUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (InterlinkBrick::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InterlinkBrick::appCountChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject InterlinkBrick::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_InterlinkBrick.data,
    qt_meta_data_InterlinkBrick,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *InterlinkBrick::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InterlinkBrick::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_InterlinkBrick.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int InterlinkBrick::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void InterlinkBrick::taskbarUpdated(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void InterlinkBrick::appCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
