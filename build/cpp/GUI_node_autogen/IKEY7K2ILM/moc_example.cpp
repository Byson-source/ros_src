/****************************************************************************
** Meta object code from reading C++ file 'example.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/cpp/src/gui/include/example.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'example.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_overloaded_window_t {
    QByteArrayData data[9];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_overloaded_window_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_overloaded_window_t qt_meta_stringdata_overloaded_window = {
    {
QT_MOC_LITERAL(0, 0, 17), // "overloaded_window"
QT_MOC_LITERAL(1, 18, 12), // "valueChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 7), // "signal0"
QT_MOC_LITERAL(4, 40, 13), // "open_database"
QT_MOC_LITERAL(5, 54, 22), // "rtabmap::ParametersMap"
QT_MOC_LITERAL(6, 77, 19), // "overridedParameters"
QT_MOC_LITERAL(7, 97, 14), // "close_database"
QT_MOC_LITERAL(8, 112, 14) // "status_checker"

    },
    "overloaded_window\0valueChanged\0\0signal0\0"
    "open_database\0rtabmap::ParametersMap\0"
    "overridedParameters\0close_database\0"
    "status_checker"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_overloaded_window[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   43,    2, 0x0a /* Public */,
       7,    0,   46,    2, 0x0a /* Public */,
       8,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void overloaded_window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<overloaded_window *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->signal0(); break;
        case 2: _t->open_database((*reinterpret_cast< const rtabmap::ParametersMap(*)>(_a[1]))); break;
        case 3: _t->close_database(); break;
        case 4: _t->status_checker(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (overloaded_window::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&overloaded_window::valueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (overloaded_window::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&overloaded_window::signal0)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject overloaded_window::staticMetaObject = { {
    &rtabmap::MainWindow::staticMetaObject,
    qt_meta_stringdata_overloaded_window.data,
    qt_meta_data_overloaded_window,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *overloaded_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *overloaded_window::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_overloaded_window.stringdata0))
        return static_cast<void*>(this);
    return rtabmap::MainWindow::qt_metacast(_clname);
}

int overloaded_window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = rtabmap::MainWindow::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void overloaded_window::valueChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void overloaded_window::signal0()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
