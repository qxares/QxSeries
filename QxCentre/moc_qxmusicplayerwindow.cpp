/****************************************************************************
** Meta object code from reading C++ file 'qxmusicplayerwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QxAudio/QxAudioPlayer/qxmusicplayerwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxmusicplayerwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QxMusicPlayerWindow_t {
    QByteArrayData data[22];
    char stringdata0[273];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxMusicPlayerWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxMusicPlayerWindow_t qt_meta_stringdata_QxMusicPlayerWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "QxMusicPlayerWindow"
QT_MOC_LITERAL(1, 20, 15), // "toggleDarkTheme"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 23), // "handleWindowStateChange"
QT_MOC_LITERAL(4, 61, 9), // "minimized"
QT_MOC_LITERAL(5, 71, 9), // "playPause"
QT_MOC_LITERAL(6, 81, 4), // "stop"
QT_MOC_LITERAL(7, 86, 4), // "next"
QT_MOC_LITERAL(8, 91, 4), // "prev"
QT_MOC_LITERAL(9, 96, 8), // "openFile"
QT_MOC_LITERAL(10, 105, 7), // "openUrl"
QT_MOC_LITERAL(11, 113, 13), // "openDirectory"
QT_MOC_LITERAL(12, 127, 16), // "handleMediaError"
QT_MOC_LITERAL(13, 144, 24), // "playlistSelectionChanged"
QT_MOC_LITERAL(14, 169, 17), // "playSelectedTrack"
QT_MOC_LITERAL(15, 187, 16), // "toggleVisualizer"
QT_MOC_LITERAL(16, 204, 16), // "updateSeekSlider"
QT_MOC_LITERAL(17, 221, 8), // "position"
QT_MOC_LITERAL(18, 230, 14), // "updateDuration"
QT_MOC_LITERAL(19, 245, 8), // "duration"
QT_MOC_LITERAL(20, 254, 12), // "seekPosition"
QT_MOC_LITERAL(21, 267, 5) // "value"

    },
    "QxMusicPlayerWindow\0toggleDarkTheme\0"
    "\0handleWindowStateChange\0minimized\0"
    "playPause\0stop\0next\0prev\0openFile\0"
    "openUrl\0openDirectory\0handleMediaError\0"
    "playlistSelectionChanged\0playSelectedTrack\0"
    "toggleVisualizer\0updateSeekSlider\0"
    "position\0updateDuration\0duration\0"
    "seekPosition\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxMusicPlayerWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    1,   95,    2, 0x08 /* Private */,
       5,    0,   98,    2, 0x08 /* Private */,
       6,    0,   99,    2, 0x08 /* Private */,
       7,    0,  100,    2, 0x08 /* Private */,
       8,    0,  101,    2, 0x08 /* Private */,
       9,    0,  102,    2, 0x08 /* Private */,
      10,    0,  103,    2, 0x08 /* Private */,
      11,    0,  104,    2, 0x08 /* Private */,
      12,    0,  105,    2, 0x08 /* Private */,
      13,    0,  106,    2, 0x08 /* Private */,
      14,    0,  107,    2, 0x08 /* Private */,
      15,    0,  108,    2, 0x08 /* Private */,
      16,    1,  109,    2, 0x08 /* Private */,
      18,    1,  112,    2, 0x08 /* Private */,
      20,    1,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   17,
    QMetaType::Void, QMetaType::LongLong,   19,
    QMetaType::Void, QMetaType::Int,   21,

       0        // eod
};

void QxMusicPlayerWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QxMusicPlayerWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->toggleDarkTheme(); break;
        case 1: _t->handleWindowStateChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->playPause(); break;
        case 3: _t->stop(); break;
        case 4: _t->next(); break;
        case 5: _t->prev(); break;
        case 6: _t->openFile(); break;
        case 7: _t->openUrl(); break;
        case 8: _t->openDirectory(); break;
        case 9: _t->handleMediaError(); break;
        case 10: _t->playlistSelectionChanged(); break;
        case 11: _t->playSelectedTrack(); break;
        case 12: _t->toggleVisualizer(); break;
        case 13: _t->updateSeekSlider((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 14: _t->updateDuration((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 15: _t->seekPosition((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QxMusicPlayerWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_QxMusicPlayerWindow.data,
    qt_meta_data_QxMusicPlayerWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QxMusicPlayerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxMusicPlayerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QxMusicPlayerWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QxMusicPlayerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
