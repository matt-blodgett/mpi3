/****************************************************************************
** Meta object code from reading C++ file 'maudioengine.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/core/maudioengine.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maudioengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MAudioEngine_t {
    QByteArrayData data[15];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MAudioEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MAudioEngine_t qt_meta_stringdata_MAudioEngine = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MAudioEngine"
QT_MOC_LITERAL(1, 13, 12), // "notifyVolume"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 3), // "vol"
QT_MOC_LITERAL(4, 31, 14), // "notifyPosition"
QT_MOC_LITERAL(5, 46, 3), // "pos"
QT_MOC_LITERAL(6, 50, 17), // "notifyMediaStatus"
QT_MOC_LITERAL(7, 68, 16), // "Mpi3::MediaState"
QT_MOC_LITERAL(8, 85, 5), // "state"
QT_MOC_LITERAL(9, 91, 18), // "notifyEngineStatus"
QT_MOC_LITERAL(10, 110, 17), // "Mpi3::EngineState"
QT_MOC_LITERAL(11, 128, 17), // "notifyErrorStatus"
QT_MOC_LITERAL(12, 146, 16), // "Mpi3::ErrorState"
QT_MOC_LITERAL(13, 163, 19), // "notifyRequestStatus"
QT_MOC_LITERAL(14, 183, 6) // "volume"

    },
    "MAudioEngine\0notifyVolume\0\0vol\0"
    "notifyPosition\0pos\0notifyMediaStatus\0"
    "Mpi3::MediaState\0state\0notifyEngineStatus\0"
    "Mpi3::EngineState\0notifyErrorStatus\0"
    "Mpi3::ErrorState\0notifyRequestStatus\0"
    "volume"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MAudioEngine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       1,   62, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,
       9,    1,   53,    2, 0x06 /* Public */,
      11,    1,   56,    2, 0x06 /* Public */,
      13,    1,   59,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,    8,
    QMetaType::Void, 0x80000000 | 12,    8,
    QMetaType::Void, 0x80000000 | 10,    8,

 // properties: name, type, flags
      14, QMetaType::Float, 0x00095003,

       0        // eod
};

void MAudioEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MAudioEngine *_t = static_cast<MAudioEngine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->notifyVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->notifyPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->notifyMediaStatus((*reinterpret_cast< Mpi3::MediaState(*)>(_a[1]))); break;
        case 3: _t->notifyEngineStatus((*reinterpret_cast< Mpi3::EngineState(*)>(_a[1]))); break;
        case 4: _t->notifyErrorStatus((*reinterpret_cast< Mpi3::ErrorState(*)>(_a[1]))); break;
        case 5: _t->notifyRequestStatus((*reinterpret_cast< Mpi3::EngineState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MAudioEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyVolume)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MAudioEngine::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyPosition)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MAudioEngine::*)(Mpi3::MediaState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyMediaStatus)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MAudioEngine::*)(Mpi3::EngineState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyEngineStatus)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MAudioEngine::*)(Mpi3::ErrorState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyErrorStatus)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MAudioEngine::*)(Mpi3::EngineState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MAudioEngine::notifyRequestStatus)) {
                *result = 5;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        MAudioEngine *_t = static_cast<MAudioEngine *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< float*>(_v) = _t->volume(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        MAudioEngine *_t = static_cast<MAudioEngine *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->gain(*reinterpret_cast< float*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MAudioEngine::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MAudioEngine.data,
    qt_meta_data_MAudioEngine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MAudioEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MAudioEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MAudioEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MAudioEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MAudioEngine::notifyVolume(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MAudioEngine::notifyPosition(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MAudioEngine::notifyMediaStatus(Mpi3::MediaState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MAudioEngine::notifyEngineStatus(Mpi3::EngineState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MAudioEngine::notifyErrorStatus(Mpi3::ErrorState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MAudioEngine::notifyRequestStatus(Mpi3::EngineState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
