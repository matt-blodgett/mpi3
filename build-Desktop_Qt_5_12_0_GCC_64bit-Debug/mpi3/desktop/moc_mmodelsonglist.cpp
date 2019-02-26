/****************************************************************************
** Meta object code from reading C++ file 'mmodelsonglist.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/desktop/ui/models/mmodelsonglist.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mmodelsonglist.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MModelSonglist_t {
    QByteArrayData data[13];
    char stringdata0[167];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MModelSonglist_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MModelSonglist_t qt_meta_stringdata_MModelSonglist = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MModelSonglist"
QT_MOC_LITERAL(1, 15, 15), // "elementModified"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 14), // "MMediaElement*"
QT_MOC_LITERAL(4, 47, 12), // "elemModified"
QT_MOC_LITERAL(5, 60, 15), // "elementInserted"
QT_MOC_LITERAL(6, 76, 12), // "elemInserted"
QT_MOC_LITERAL(7, 89, 16), // "MMediaContainer*"
QT_MOC_LITERAL(8, 106, 10), // "elemParent"
QT_MOC_LITERAL(9, 117, 12), // "elementMoved"
QT_MOC_LITERAL(10, 130, 9), // "elemMoved"
QT_MOC_LITERAL(11, 140, 14), // "elementDeleted"
QT_MOC_LITERAL(12, 155, 11) // "elemDeleted"

    },
    "MModelSonglist\0elementModified\0\0"
    "MMediaElement*\0elemModified\0elementInserted\0"
    "elemInserted\0MMediaContainer*\0elemParent\0"
    "elementMoved\0elemMoved\0elementDeleted\0"
    "elemDeleted"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MModelSonglist[] = {

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
       9,    2,   42,    2, 0x08 /* Private */,
      11,    1,   47,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 7,    6,    8,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 7,   10,    8,
    QMetaType::Void, 0x80000000 | 3,   12,

       0        // eod
};

void MModelSonglist::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MModelSonglist *_t = static_cast<MModelSonglist *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->elementModified((*reinterpret_cast< MMediaElement*(*)>(_a[1]))); break;
        case 1: _t->elementInserted((*reinterpret_cast< MMediaElement*(*)>(_a[1])),(*reinterpret_cast< MMediaContainer*(*)>(_a[2]))); break;
        case 2: _t->elementMoved((*reinterpret_cast< MMediaElement*(*)>(_a[1])),(*reinterpret_cast< MMediaContainer*(*)>(_a[2]))); break;
        case 3: _t->elementDeleted((*reinterpret_cast< MMediaElement*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MModelSonglist::staticMetaObject = { {
    &QAbstractItemModel::staticMetaObject,
    qt_meta_stringdata_MModelSonglist.data,
    qt_meta_data_MModelSonglist,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MModelSonglist::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MModelSonglist::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MModelSonglist.stringdata0))
        return static_cast<void*>(this);
    return QAbstractItemModel::qt_metacast(_clname);
}

int MModelSonglist::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_MModelSonglistProxy_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MModelSonglistProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MModelSonglistProxy_t qt_meta_stringdata_MModelSonglistProxy = {
    {
QT_MOC_LITERAL(0, 0, 19) // "MModelSonglistProxy"

    },
    "MModelSonglistProxy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MModelSonglistProxy[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MModelSonglistProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MModelSonglistProxy::staticMetaObject = { {
    &QSortFilterProxyModel::staticMetaObject,
    qt_meta_stringdata_MModelSonglistProxy.data,
    qt_meta_data_MModelSonglistProxy,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MModelSonglistProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MModelSonglistProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MModelSonglistProxy.stringdata0))
        return static_cast<void*>(this);
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int MModelSonglistProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
