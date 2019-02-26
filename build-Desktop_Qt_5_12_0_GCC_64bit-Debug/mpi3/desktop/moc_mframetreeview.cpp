/****************************************************************************
** Meta object code from reading C++ file 'mframetreeview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/desktop/ui/frames/mframetreeview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mframetreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MFrameTreeView_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MFrameTreeView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MFrameTreeView_t qt_meta_stringdata_MFrameTreeView = {
    {
QT_MOC_LITERAL(0, 0, 14) // "MFrameTreeView"

    },
    "MFrameTreeView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MFrameTreeView[] = {

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

void MFrameTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MFrameTreeView::staticMetaObject = { {
    &MFrame::staticMetaObject,
    qt_meta_stringdata_MFrameTreeView.data,
    qt_meta_data_MFrameTreeView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MFrameTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MFrameTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MFrameTreeView.stringdata0))
        return static_cast<void*>(this);
    return MFrame::qt_metacast(_clname);
}

int MFrameTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MFrame::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_MFrameContainers_t {
    QByteArrayData data[9];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MFrameContainers_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MFrameContainers_t qt_meta_stringdata_MFrameContainers = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MFrameContainers"
QT_MOC_LITERAL(1, 17, 17), // "containerSelected"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "MMediaContainer*"
QT_MOC_LITERAL(4, 53, 9), // "container"
QT_MOC_LITERAL(5, 63, 17), // "iconFolderChanged"
QT_MOC_LITERAL(6, 81, 19), // "iconPlaylistChanged"
QT_MOC_LITERAL(7, 101, 10), // "iconFolder"
QT_MOC_LITERAL(8, 112, 12) // "iconPlaylist"

    },
    "MFrameContainers\0containerSelected\0\0"
    "MMediaContainer*\0container\0iconFolderChanged\0"
    "iconPlaylistChanged\0iconFolder\0"
    "iconPlaylist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MFrameContainers[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       2,   22, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // properties: name, type, flags
       7, QMetaType::QString, 0x00495003,
       8, QMetaType::QString, 0x00495003,

 // properties: notify_signal_id
    1879048197,
    1879048198,

       0        // eod
};

void MFrameContainers::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MFrameContainers *_t = static_cast<MFrameContainers *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->containerSelected((*reinterpret_cast< MMediaContainer*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MFrameContainers::*)(MMediaContainer * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFrameContainers::containerSelected)) {
                *result = 0;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        MFrameContainers *_t = static_cast<MFrameContainers *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->m_icnFolder; break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->m_icnPlaylist; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        MFrameContainers *_t = static_cast<MFrameContainers *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_icnFolder != *reinterpret_cast< QString*>(_v)) {
                _t->m_icnFolder = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->iconFolderChanged();
            }
            break;
        case 1:
            if (_t->m_icnPlaylist != *reinterpret_cast< QString*>(_v)) {
                _t->m_icnPlaylist = *reinterpret_cast< QString*>(_v);
                Q_EMIT _t->iconPlaylistChanged();
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MFrameContainers::staticMetaObject = { {
    &MFrameTreeView::staticMetaObject,
    qt_meta_stringdata_MFrameContainers.data,
    qt_meta_data_MFrameContainers,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MFrameContainers::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MFrameContainers::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MFrameContainers.stringdata0))
        return static_cast<void*>(this);
    return MFrameTreeView::qt_metacast(_clname);
}

int MFrameContainers::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MFrameTreeView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MFrameContainers::containerSelected(MMediaContainer * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
// If you get a compile error in this function it can be because either
//     a) You are using a NOTIFY signal that does not exist. Fix it.
//     b) You are using a NOTIFY signal that does exist (in a parent class) but has a non-empty parameter list. This is a moc limitation.
Q_DECL_UNUSED static void checkNotifySignalValidity_MFrameContainers(MFrameContainers *t) {
    t->iconFolderChanged();
    t->iconPlaylistChanged();
}
struct qt_meta_stringdata_MFrameSonglist_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MFrameSonglist_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MFrameSonglist_t qt_meta_stringdata_MFrameSonglist = {
    {
QT_MOC_LITERAL(0, 0, 14) // "MFrameSonglist"

    },
    "MFrameSonglist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MFrameSonglist[] = {

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

void MFrameSonglist::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MFrameSonglist::staticMetaObject = { {
    &MFrameTreeView::staticMetaObject,
    qt_meta_stringdata_MFrameSonglist.data,
    qt_meta_data_MFrameSonglist,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MFrameSonglist::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MFrameSonglist::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MFrameSonglist.stringdata0))
        return static_cast<void*>(this);
    return MFrameTreeView::qt_metacast(_clname);
}

int MFrameSonglist::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MFrameTreeView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
