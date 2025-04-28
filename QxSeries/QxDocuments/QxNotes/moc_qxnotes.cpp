/****************************************************************************
** Meta object code from reading C++ file 'qxnotes.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "qxnotes.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qxnotes.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QxNotes_t {
    QByteArrayData data[19];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QxNotes_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QxNotes_t qt_meta_stringdata_QxNotes = {
    {
QT_MOC_LITERAL(0, 0, 7), // "QxNotes"
QT_MOC_LITERAL(1, 8, 7), // "newFile"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 8), // "openFile"
QT_MOC_LITERAL(4, 26, 8), // "saveFile"
QT_MOC_LITERAL(5, 35, 10), // "saveAsFile"
QT_MOC_LITERAL(6, 46, 4), // "undo"
QT_MOC_LITERAL(7, 51, 4), // "redo"
QT_MOC_LITERAL(8, 56, 3), // "cut"
QT_MOC_LITERAL(9, 60, 4), // "copy"
QT_MOC_LITERAL(10, 65, 5), // "paste"
QT_MOC_LITERAL(11, 71, 13), // "translateText"
QT_MOC_LITERAL(12, 85, 21), // "toggleMarkdownPreview"
QT_MOC_LITERAL(13, 107, 9), // "speakText"
QT_MOC_LITERAL(14, 117, 9), // "showAbout"
QT_MOC_LITERAL(15, 127, 17), // "currentTabChanged"
QT_MOC_LITERAL(16, 145, 11), // "textChanged"
QT_MOC_LITERAL(17, 157, 8), // "closeTab"
QT_MOC_LITERAL(18, 166, 5) // "index"

    },
    "QxNotes\0newFile\0\0openFile\0saveFile\0"
    "saveAsFile\0undo\0redo\0cut\0copy\0paste\0"
    "translateText\0toggleMarkdownPreview\0"
    "speakText\0showAbout\0currentTabChanged\0"
    "textChanged\0closeTab\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QxNotes[] = {

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
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    0,  101,    2, 0x08 /* Private */,
      10,    0,  102,    2, 0x08 /* Private */,
      11,    0,  103,    2, 0x08 /* Private */,
      12,    0,  104,    2, 0x08 /* Private */,
      13,    0,  105,    2, 0x08 /* Private */,
      14,    0,  106,    2, 0x08 /* Private */,
      15,    0,  107,    2, 0x08 /* Private */,
      16,    0,  108,    2, 0x08 /* Private */,
      17,    1,  109,    2, 0x08 /* Private */,

 // slots: parameters
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void QxNotes::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QxNotes *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newFile(); break;
        case 1: _t->openFile(); break;
        case 2: _t->saveFile(); break;
        case 3: _t->saveAsFile(); break;
        case 4: _t->undo(); break;
        case 5: _t->redo(); break;
        case 6: _t->cut(); break;
        case 7: _t->copy(); break;
        case 8: _t->paste(); break;
        case 9: _t->translateText(); break;
        case 10: _t->toggleMarkdownPreview(); break;
        case 11: _t->speakText(); break;
        case 12: _t->showAbout(); break;
        case 13: _t->currentTabChanged(); break;
        case 14: _t->textChanged(); break;
        case 15: _t->closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QxNotes::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_QxNotes.data,
    qt_meta_data_QxNotes,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QxNotes::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QxNotes::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QxNotes.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QxNotes::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
