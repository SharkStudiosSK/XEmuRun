/****************************************************************************
** Meta object code from reading C++ file 'packager_gui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gui/packager_gui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'packager_gui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_XEmuRun__PackagerGui_t {
    QByteArrayData data[14];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_XEmuRun__PackagerGui_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_XEmuRun__PackagerGui_t qt_meta_stringdata_XEmuRun__PackagerGui = {
    {
QT_MOC_LITERAL(0, 0, 20), // "XEmuRun::PackagerGui"
QT_MOC_LITERAL(1, 21, 14), // "browseGamePath"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 16), // "browseOutputPath"
QT_MOC_LITERAL(4, 54, 16), // "browseExecutable"
QT_MOC_LITERAL(5, 71, 12), // "addConfigRow"
QT_MOC_LITERAL(6, 84, 15), // "removeConfigRow"
QT_MOC_LITERAL(7, 100, 13), // "createPackage"
QT_MOC_LITERAL(8, 114, 10), // "autoDetect"
QT_MOC_LITERAL(9, 125, 23), // "updateCreateButtonState"
QT_MOC_LITERAL(10, 149, 15), // "platformChanged"
QT_MOC_LITERAL(11, 165, 8), // "platform"
QT_MOC_LITERAL(12, 174, 23), // "handlePackagingFinished"
QT_MOC_LITERAL(13, 198, 7) // "success"

    },
    "XEmuRun::PackagerGui\0browseGamePath\0"
    "\0browseOutputPath\0browseExecutable\0"
    "addConfigRow\0removeConfigRow\0createPackage\0"
    "autoDetect\0updateCreateButtonState\0"
    "platformChanged\0platform\0"
    "handlePackagingFinished\0success"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_XEmuRun__PackagerGui[] = {

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
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    1,   72,    2, 0x08 /* Private */,
      12,    1,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Bool,   13,

       0        // eod
};

void XEmuRun::PackagerGui::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PackagerGui *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->browseGamePath(); break;
        case 1: _t->browseOutputPath(); break;
        case 2: _t->browseExecutable(); break;
        case 3: _t->addConfigRow(); break;
        case 4: _t->removeConfigRow(); break;
        case 5: _t->createPackage(); break;
        case 6: _t->autoDetect(); break;
        case 7: _t->updateCreateButtonState(); break;
        case 8: _t->platformChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->handlePackagingFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject XEmuRun::PackagerGui::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_XEmuRun__PackagerGui.data,
    qt_meta_data_XEmuRun__PackagerGui,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *XEmuRun::PackagerGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *XEmuRun::PackagerGui::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_XEmuRun__PackagerGui.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int XEmuRun::PackagerGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_XEmuRun__PackageWorker_t {
    QByteArrayData data[5];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_XEmuRun__PackageWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_XEmuRun__PackageWorker_t qt_meta_stringdata_XEmuRun__PackageWorker = {
    {
QT_MOC_LITERAL(0, 0, 22), // "XEmuRun::PackageWorker"
QT_MOC_LITERAL(1, 23, 8), // "finished"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 7), // "success"
QT_MOC_LITERAL(4, 41, 7) // "process"

    },
    "XEmuRun::PackageWorker\0finished\0\0"
    "success\0process"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_XEmuRun__PackageWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void XEmuRun::PackageWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PackageWorker *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->finished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->process(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PackageWorker::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PackageWorker::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject XEmuRun::PackageWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_XEmuRun__PackageWorker.data,
    qt_meta_data_XEmuRun__PackageWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *XEmuRun::PackageWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *XEmuRun::PackageWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_XEmuRun__PackageWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int XEmuRun::PackageWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void XEmuRun::PackageWorker::finished(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
