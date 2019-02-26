/****************************************************************************
** Meta object code from reading C++ file 'mframeplayback.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/desktop/ui/frames/mframeplayback.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mframeplayback.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MFramePlayback_t {
    QByteArrayData data[23];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MFramePlayback_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MFramePlayback_t qt_meta_stringdata_MFramePlayback = {
    {
QT_MOC_LITERAL(0, 0, 14), // "MFramePlayback"
QT_MOC_LITERAL(1, 15, 13), // "playRequested"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 13), // "pausRequested"
QT_MOC_LITERAL(4, 44, 13), // "seekRequested"
QT_MOC_LITERAL(5, 58, 8), // "position"
QT_MOC_LITERAL(6, 67, 15), // "volumeRequested"
QT_MOC_LITERAL(7, 83, 6), // "volume"
QT_MOC_LITERAL(8, 90, 13), // "nextRequested"
QT_MOC_LITERAL(9, 104, 13), // "prevRequested"
QT_MOC_LITERAL(10, 118, 11), // "playClicked"
QT_MOC_LITERAL(11, 130, 11), // "nextClicked"
QT_MOC_LITERAL(12, 142, 11), // "prevClicked"
QT_MOC_LITERAL(13, 154, 13), // "volumeChanged"
QT_MOC_LITERAL(14, 168, 9), // "seekBegin"
QT_MOC_LITERAL(15, 178, 7), // "seekEnd"
QT_MOC_LITERAL(16, 186, 15), // "positionChanged"
QT_MOC_LITERAL(17, 202, 15), // "elementModified"
QT_MOC_LITERAL(18, 218, 14), // "MMediaElement*"
QT_MOC_LITERAL(19, 233, 12), // "elemModified"
QT_MOC_LITERAL(20, 246, 13), // "buttonOpacity"
QT_MOC_LITERAL(21, 260, 8), // "iconPlay"
QT_MOC_LITERAL(22, 269, 9) // "iconPause"

    },
    "MFramePlayback\0playRequested\0\0"
    "pausRequested\0seekRequested\0position\0"
    "volumeRequested\0volume\0nextRequested\0"
    "prevRequested\0playClicked\0nextClicked\0"
    "prevClicked\0volumeChanged\0seekBegin\0"
    "seekEnd\0positionChanged\0elementModified\0"
    "MMediaElement*\0elemModified\0buttonOpacity\0"
    "iconPlay\0iconPause"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MFramePlayback[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       3,  106, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    1,   86,    2, 0x06 /* Public */,
       6,    1,   89,    2, 0x06 /* Public */,
       8,    0,   92,    2, 0x06 /* Public */,
       9,    0,   93,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   94,    2, 0x08 /* Private */,
      11,    0,   95,    2, 0x08 /* Private */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    0,   98,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,
      16,    1,  100,    2, 0x08 /* Private */,
      17,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 18,   19,

 // properties: name, type, flags
      20, QMetaType::Double, 0x00095103,
      21, QMetaType::QString, 0x00095003,
      22, QMetaType::QString, 0x00095003,

       0        // eod
};

void MFramePlayback::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MFramePlayback *_t = static_cast<MFramePlayback *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->playRequested(); break;
        case 1: _t->pausRequested(); break;
        case 2: _t->seekRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->volumeRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->nextRequested(); break;
        case 5: _t->prevRequested(); break;
        case 6: _t->playClicked(); break;
        case 7: _t->nextClicked(); break;
        case 8: _t->prevClicked(); break;
        case 9: _t->volumeChanged(); break;
        case 10: _t->seekBegin(); break;
        case 11: _t->seekEnd(); break;
        case 12: _t->positionChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->elementModified((*reinterpret_cast< MMediaElement*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MFramePlayback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::playRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MFramePlayback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::pausRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MFramePlayback::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::seekRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MFramePlayback::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::volumeRequested)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MFramePlayback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::nextRequested)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MFramePlayback::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MFramePlayback::prevRequested)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        MFramePlayback *_t = static_cast<MFramePlayback *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->buttonOpacity(); break;
        case 1: *reinterpret_cast< QString*>(_v) = _t->m_icnPlay; break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->m_icnPause; break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        MFramePlayback *_t = static_cast<MFramePlayback *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setButtonOpacity(*reinterpret_cast< double*>(_v)); break;
        case 1:
            if (_t->m_icnPlay != *reinterpret_cast< QString*>(_v)) {
                _t->m_icnPlay = *reinterpret_cast< QString*>(_v);
            }
            break;
        case 2:
            if (_t->m_icnPause != *reinterpret_cast< QString*>(_v)) {
                _t->m_icnPause = *reinterpret_cast< QString*>(_v);
            }
            break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MFramePlayback::staticMetaObject = { {
    &MFrame::staticMetaObject,
    qt_meta_stringdata_MFramePlayback.data,
    qt_meta_data_MFramePlayback,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MFramePlayback::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MFramePlayback::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MFramePlayback.stringdata0))
        return static_cast<void*>(this);
    return MFrame::qt_metacast(_clname);
}

int MFramePlayback::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MFramePlayback::playRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MFramePlayback::pausRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MFramePlayback::seekRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MFramePlayback::volumeRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MFramePlayback::nextRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MFramePlayback::prevRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
