/****************************************************************************
** Meta object code from reading C++ file 'controller_mapping.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/gui/controller_mapping.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controller_mapping.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_XEmuRun__ControllerMapping_t {
    QByteArrayData data[14];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_XEmuRun__ControllerMapping_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_XEmuRun__ControllerMapping_t qt_meta_stringdata_XEmuRun__ControllerMapping = {
    {
QT_MOC_LITERAL(0, 0, 26), // "XEmuRun::ControllerMapping"
QT_MOC_LITERAL(1, 27, 14), // "refreshDevices"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 11), // "saveMapping"
QT_MOC_LITERAL(4, 55, 11), // "loadMapping"
QT_MOC_LITERAL(5, 67, 12), // "startMapping"
QT_MOC_LITERAL(6, 80, 13), // "cancelMapping"
QT_MOC_LITERAL(7, 94, 18), // "controllerSelected"
QT_MOC_LITERAL(8, 113, 5), // "index"
QT_MOC_LITERAL(9, 119, 16), // "platformSelected"
QT_MOC_LITERAL(10, 136, 18), // "mappingItemClicked"
QT_MOC_LITERAL(11, 155, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(12, 173, 4), // "item"
QT_MOC_LITERAL(13, 178, 19) // "pollControllerInput"

    },
    "XEmuRun::ControllerMapping\0refreshDevices\0"
    "\0saveMapping\0loadMapping\0startMapping\0"
    "cancelMapping\0controllerSelected\0index\0"
    "platformSelected\0mappingItemClicked\0"
    "QTableWidgetItem*\0item\0pollControllerInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_XEmuRun__ControllerMapping[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
       9,    1,   67,    2, 0x08 /* Private */,
      10,    1,   70,    2, 0x08 /* Private */,
      13,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,

       0        // eod
};

void XEmuRun::ControllerMapping::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControllerMapping *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->refreshDevices(); break;
        case 1: _t->saveMapping(); break;
        case 2: _t->loadMapping(); break;
        case 3: _t->startMapping(); break;
        case 4: _t->cancelMapping(); break;
        case 5: _t->controllerSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->platformSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->mappingItemClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 8: _t->pollControllerInput(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject XEmuRun::ControllerMapping::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_XEmuRun__ControllerMapping.data,
    qt_meta_data_XEmuRun__ControllerMapping,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *XEmuRun::ControllerMapping::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *XEmuRun::ControllerMapping::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_XEmuRun__ControllerMapping.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int XEmuRun::ControllerMapping::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
