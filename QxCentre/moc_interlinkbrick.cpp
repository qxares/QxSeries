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
    QByteArrayData data[7];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InterlinkBrick_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InterlinkBrick_t qt_meta_stringdata_InterlinkBrick = {
    {
QT_MOC_LITERAL(0, 0, 14), // "InterlinkBrick"
QT_MOC_LITERAL(1, 15, 18), // "windowStateChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 9), // "minimized"
QT_MOC_LITERAL(4, 45, 25), // "handleMainWindowActivated"
QT_MOC_LITERAL(5, 71, 20), // "onAppWindowDestroyed"
QT_MOC_LITERAL(6, 92, 3) // "obj"

    },
    "InterlinkBrick\0windowStateChanged\0\0"
    "minimized\0handleMainWindowActivated\0"
    "onAppWindowDestroyed\0obj"
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
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x08 /* Private */,
       5,    1,   33,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,    6,

       0        // eod
};

void InterlinkBrick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InterlinkBrick *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->windowStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->handleMainWindowActivated(); break;
        case 2: _t->onAppWindowDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InterlinkBrick::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&InterlinkBrick::windowStateChanged)) {
                *result = 0;
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void InterlinkBrick::windowStateChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
