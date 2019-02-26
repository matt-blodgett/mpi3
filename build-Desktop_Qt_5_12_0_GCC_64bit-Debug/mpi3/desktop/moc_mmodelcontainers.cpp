/****************************************************************************
** Meta object code from reading C++ file 'mmodelcontainers.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/desktop/ui/models/mmodelcontainers.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mmodelcontainers.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MModelContainers_t {
    QByteArrayData data[12];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MModelContainers_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MModelContainers_t qt_meta_stringdata_MModelContainers = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MModelContainers"
QT_MOC_LITERAL(1, 17, 15), // "elementModified"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 14), // "MMediaElement*"
QT_MOC_LITERAL(4, 49, 12), // "elemModified"
QT_MOC_LITERAL(5, 62, 15), // "elementInserted"
QT_MOC_LITERAL(6, 78, 12), // "elemInserted"
QT_MOC_LITERAL(7, 91, 10), // "elemParent"
QT_MOC_LITERAL(8, 102, 12), // "elementMoved"
QT_MOC_LITERAL(9, 115, 9), // "elemMoved"
QT_MOC_LITERAL(10, 125, 14), // "elementDeleted"
QT_MOC_LITERAL(11, 140, 11) // "elemDeleted"

    },
    "MModelContainers\0elementModified\0\0"
    "MMediaElement*\0elemModified\0elementInserted\0"
    "elemInserted\0elemParent\0elementMoved\0"
    "elemMoved\0elementDeleted\0elemDeleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MModelContainers[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       5,    2,   37,    2, 0x08 /* Private */,
       8,    2,   42,    2, 0x08 /* Private */,
      10,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    6,    7,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    9,    7,
    QMetaType::Void, 0x80000000 | 3,   11,

       0        // eod
};

void MModelContainers::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MModelContainers *_t = static_cast<MModelContainers *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->elementModified((*reinterpret_cast< MMediaElement*(*)>(_a[1]))); break;
        case 1: _t->elementInserted((*reinterpret_cast< MMediaElement*(*)>(_a[1])),(*reinterpret_cast< MMediaElement*(*)>(_a[2]))); break;
        case 2: _t->elementMoved((*reinterpret_cast< MMediaElement*(*)>(_a[1])),(*reinterpret_cast< MMediaElement*(*)>(_a[2]))); break;
        case 3: _t->elementDeleted((*reinterpret_cast< MMediaElement*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MModelContainers::staticMetaObject = { {
    &QAbstractItemModel::staticMetaObject,
    qt_meta_stringdata_MModelContainers.data,
    qt_meta_data_MModelContainers,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MModelContainers::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MModelContainers::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MModelContainers.stringdata0))
        return static_cast<void*>(this);
    return QAbstractItemModel::qt_metacast(_clname);
}

int MModelContainers::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
