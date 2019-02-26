/****************************************************************************
** Meta object code from reading C++ file 'mglobal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/core/mglobal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mglobal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Mpi3_t {
    QByteArrayData data[22];
    char stringdata0[278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mpi3_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mpi3_t qt_meta_stringdata_Mpi3 = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Mpi3"
QT_MOC_LITERAL(1, 5, 11), // "ElementType"
QT_MOC_LITERAL(2, 17, 11), // "BaseElement"
QT_MOC_LITERAL(3, 29, 11), // "SongElement"
QT_MOC_LITERAL(4, 41, 16), // "ContainerElement"
QT_MOC_LITERAL(5, 58, 15), // "PlaylistElement"
QT_MOC_LITERAL(6, 74, 13), // "FolderElement"
QT_MOC_LITERAL(7, 88, 14), // "LibraryElement"
QT_MOC_LITERAL(8, 103, 10), // "MediaState"
QT_MOC_LITERAL(9, 114, 10), // "MediaEmpty"
QT_MOC_LITERAL(10, 125, 10), // "MediaReady"
QT_MOC_LITERAL(11, 136, 9), // "MediaBusy"
QT_MOC_LITERAL(12, 146, 11), // "EngineState"
QT_MOC_LITERAL(13, 158, 13), // "EngineStopped"
QT_MOC_LITERAL(14, 172, 12), // "EngineActive"
QT_MOC_LITERAL(15, 185, 10), // "EngineIdle"
QT_MOC_LITERAL(16, 196, 10), // "ErrorState"
QT_MOC_LITERAL(17, 207, 7), // "NoError"
QT_MOC_LITERAL(18, 215, 13), // "ResourceError"
QT_MOC_LITERAL(19, 229, 15), // "FileFormatError"
QT_MOC_LITERAL(20, 245, 15), // "FileAccessError"
QT_MOC_LITERAL(21, 261, 16) // "FileInvalidError"

    },
    "Mpi3\0ElementType\0BaseElement\0SongElement\0"
    "ContainerElement\0PlaylistElement\0"
    "FolderElement\0LibraryElement\0MediaState\0"
    "MediaEmpty\0MediaReady\0MediaBusy\0"
    "EngineState\0EngineStopped\0EngineActive\0"
    "EngineIdle\0ErrorState\0NoError\0"
    "ResourceError\0FileFormatError\0"
    "FileAccessError\0FileInvalidError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mpi3[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       4,   14, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    6,   34,
       8,    8, 0x0,    3,   46,
      12,   12, 0x0,    3,   52,
      16,   16, 0x0,    5,   58,

 // enum data: key, value
       2, uint(Mpi3::BaseElement),
       3, uint(Mpi3::SongElement),
       4, uint(Mpi3::ContainerElement),
       5, uint(Mpi3::PlaylistElement),
       6, uint(Mpi3::FolderElement),
       7, uint(Mpi3::LibraryElement),
       9, uint(Mpi3::MediaEmpty),
      10, uint(Mpi3::MediaReady),
      11, uint(Mpi3::MediaBusy),
      13, uint(Mpi3::EngineStopped),
      14, uint(Mpi3::EngineActive),
      15, uint(Mpi3::EngineIdle),
      17, uint(Mpi3::NoError),
      18, uint(Mpi3::ResourceError),
      19, uint(Mpi3::FileFormatError),
      20, uint(Mpi3::FileAccessError),
      21, uint(Mpi3::FileInvalidError),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject Mpi3::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_Mpi3.data,
    qt_meta_data_Mpi3,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
