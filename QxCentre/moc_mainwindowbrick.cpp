/****************************************************************************
** Meta object code from reading C++ file 'mainwindowbrick.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "mainwindowbrick.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindowbrick.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindowBrick_t {
    QByteArrayData data[15];
    char stringdata0[216];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindowBrick_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindowBrick_t qt_meta_stringdata_MainWindowBrick = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MainWindowBrick"
QT_MOC_LITERAL(1, 16, 12), // "centerWindow"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 24), // "updateCentralWidgetStyle"
QT_MOC_LITERAL(4, 55, 6), // "isDark"
QT_MOC_LITERAL(5, 62, 10), // "handleExit"
QT_MOC_LITERAL(6, 73, 16), // "launchInfoWindow"
QT_MOC_LITERAL(7, 90, 10), // "raiseGroup"
QT_MOC_LITERAL(8, 101, 14), // "activateWindow"
QT_MOC_LITERAL(9, 116, 5), // "index"
QT_MOC_LITERAL(10, 122, 20), // "updateTaskbarWindows"
QT_MOC_LITERAL(11, 143, 24), // "handleAppWindowDestroyed"
QT_MOC_LITERAL(12, 168, 3), // "obj"
QT_MOC_LITERAL(13, 172, 21), // "incrementOpenAppCount"
QT_MOC_LITERAL(14, 194, 21) // "decrementOpenAppCount"

    },
    "MainWindowBrick\0centerWindow\0\0"
    "updateCentralWidgetStyle\0isDark\0"
    "handleExit\0launchInfoWindow\0raiseGroup\0"
    "activateWindow\0index\0updateTaskbarWindows\0"
    "handleAppWindowDestroyed\0obj\0"
    "incrementOpenAppCount\0decrementOpenAppCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindowBrick[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    1,   65,    2, 0x0a /* Public */,
       5,    0,   68,    2, 0x0a /* Public */,
       6,    0,   69,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    1,   71,    2, 0x0a /* Public */,
      10,    0,   74,    2, 0x0a /* Public */,
      11,    1,   75,    2, 0x08 /* Private */,
      13,    0,   78,    2, 0x08 /* Private */,
      14,    0,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QObjectStar,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindowBrick::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindowBrick *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->centerWindow(); break;
        case 1: _t->updateCentralWidgetStyle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->handleExit(); break;
        case 3: _t->launchInfoWindow(); break;
        case 4: _t->raiseGroup(); break;
        case 5: _t->activateWindow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->updateTaskbarWindows(); break;
        case 7: _t->handleAppWindowDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        case 8: _t->incrementOpenAppCount(); break;
        case 9: _t->decrementOpenAppCount(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindowBrick::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindowBrick.data,
    qt_meta_data_MainWindowBrick,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindowBrick::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindowBrick::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindowBrick.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindowBrick::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
