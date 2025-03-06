/****************************************************************************
** Meta object code from reading C++ file 'launcher_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gui/launcher_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'launcher_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_XEmuRun__LauncherGui_t {
    QByteArrayData data[17];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_XEmuRun__LauncherGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_XEmuRun__LauncherGui_t qt_meta_stringdata_XEmuRun__LauncherGui = {
    {
QT_MOC_LITERAL(0, 0, 20), // "XEmuRun::LauncherGui"
QT_MOC_LITERAL(1, 21, 10), // "importGame"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "launchGame"
QT_MOC_LITERAL(4, 44, 14), // "refreshLibrary"
QT_MOC_LITERAL(5, 59, 12), // "gameSelected"
QT_MOC_LITERAL(6, 72, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(7, 89, 4), // "item"
QT_MOC_LITERAL(8, 94, 15), // "showGameDetails"
QT_MOC_LITERAL(9, 110, 8), // "GameInfo"
QT_MOC_LITERAL(10, 119, 4), // "game"
QT_MOC_LITERAL(11, 124, 13), // "applySettings"
QT_MOC_LITERAL(12, 138, 15), // "showAboutDialog"
QT_MOC_LITERAL(13, 154, 12), // "openSettings"
QT_MOC_LITERAL(14, 167, 12), // "scanForGames"
QT_MOC_LITERAL(15, 180, 10), // "applyTheme"
QT_MOC_LITERAL(16, 191, 5) // "theme"

    },
    "XEmuRun::LauncherGui\0importGame\0\0"
    "launchGame\0refreshLibrary\0gameSelected\0"
    "QListWidgetItem*\0item\0showGameDetails\0"
    "GameInfo\0game\0applySettings\0showAboutDialog\0"
    "openSettings\0scanForGames\0applyTheme\0"
    "theme"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_XEmuRun__LauncherGui[] = {

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
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    1,   67,    2, 0x08 /* Private */,
       8,    1,   70,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,
      12,    0,   74,    2, 0x08 /* Private */,
      13,    0,   75,    2, 0x08 /* Private */,
      14,    0,   76,    2, 0x08 /* Private */,
      15,    1,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,

       0        // eod
};

void XEmuRun::LauncherGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LauncherGui *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->importGame(); break;
        case 1: _t->launchGame(); break;
        case 2: _t->refreshLibrary(); break;
        case 3: _t->gameSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->showGameDetails((*reinterpret_cast< const GameInfo(*)>(_a[1]))); break;
        case 5: _t->applySettings(); break;
        case 6: _t->showAboutDialog(); break;
        case 7: _t->openSettings(); break;
        case 8: _t->scanForGames(); break;
        case 9: _t->applyTheme((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject XEmuRun::LauncherGui::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_XEmuRun__LauncherGui.data,
    qt_meta_data_XEmuRun__LauncherGui,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *XEmuRun::LauncherGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *XEmuRun::LauncherGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_XEmuRun__LauncherGui.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int XEmuRun::LauncherGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
