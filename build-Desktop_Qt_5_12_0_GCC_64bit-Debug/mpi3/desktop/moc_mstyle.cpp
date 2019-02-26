/****************************************************************************
** Meta object code from reading C++ file 'mstyle.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mpi3/desktop/ui/mstyle.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mstyle.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MProxyStyle_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MProxyStyle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MProxyStyle_t qt_meta_stringdata_MProxyStyle = {
    {
QT_MOC_LITERAL(0, 0, 11) // "MProxyStyle"

    },
    "MProxyStyle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MProxyStyle[] = {

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

void MProxyStyle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MProxyStyle::staticMetaObject = { {
    &QProxyStyle::staticMetaObject,
    qt_meta_stringdata_MProxyStyle.data,
    qt_meta_data_MProxyStyle,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MProxyStyle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MProxyStyle::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MProxyStyle.stringdata0))
        return static_cast<void*>(this);
    return QProxyStyle::qt_metacast(_clname);
}

int MProxyStyle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QProxyStyle::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_MStyle_t {
    QByteArrayData data[28];
    char stringdata0[352];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MStyle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MStyle_t qt_meta_stringdata_MStyle = {
    {
QT_MOC_LITERAL(0, 0, 6), // "MStyle"
QT_MOC_LITERAL(1, 7, 11), // "MLabelStyle"
QT_MOC_LITERAL(2, 19, 10), // "LBL_Normal"
QT_MOC_LITERAL(3, 30, 9), // "LBL_Title"
QT_MOC_LITERAL(4, 40, 10), // "LBL_Header"
QT_MOC_LITERAL(5, 51, 9), // "LBL_Value"
QT_MOC_LITERAL(6, 61, 7), // "LBL_Tag"
QT_MOC_LITERAL(7, 69, 13), // "LBL_SongTitle"
QT_MOC_LITERAL(8, 83, 14), // "LBL_SongArtist"
QT_MOC_LITERAL(9, 98, 16), // "LBL_SongPosition"
QT_MOC_LITERAL(10, 115, 14), // "MLineEditStyle"
QT_MOC_LITERAL(11, 130, 9), // "LE_Normal"
QT_MOC_LITERAL(12, 140, 9), // "LE_Hidden"
QT_MOC_LITERAL(13, 150, 9), // "LE_Search"
QT_MOC_LITERAL(14, 160, 16), // "MPushButtonStyle"
QT_MOC_LITERAL(15, 177, 9), // "PB_Normal"
QT_MOC_LITERAL(16, 187, 11), // "PB_NextSong"
QT_MOC_LITERAL(17, 199, 15), // "PB_PreviousSong"
QT_MOC_LITERAL(18, 215, 11), // "PB_PlaySong"
QT_MOC_LITERAL(19, 227, 7), // "PB_Fade"
QT_MOC_LITERAL(20, 235, 9), // "PB_Search"
QT_MOC_LITERAL(21, 245, 17), // "PB_MinimizeWindow"
QT_MOC_LITERAL(22, 263, 17), // "PB_MaximizeWindow"
QT_MOC_LITERAL(23, 281, 19), // "PB_DeMaximizeWindow"
QT_MOC_LITERAL(24, 301, 14), // "PB_CloseWindow"
QT_MOC_LITERAL(25, 316, 12), // "MSliderStyle"
QT_MOC_LITERAL(26, 329, 10), // "SLD_Volume"
QT_MOC_LITERAL(27, 340, 11) // "SLD_Postion"

    },
    "MStyle\0MLabelStyle\0LBL_Normal\0LBL_Title\0"
    "LBL_Header\0LBL_Value\0LBL_Tag\0LBL_SongTitle\0"
    "LBL_SongArtist\0LBL_SongPosition\0"
    "MLineEditStyle\0LE_Normal\0LE_Hidden\0"
    "LE_Search\0MPushButtonStyle\0PB_Normal\0"
    "PB_NextSong\0PB_PreviousSong\0PB_PlaySong\0"
    "PB_Fade\0PB_Search\0PB_MinimizeWindow\0"
    "PB_MaximizeWindow\0PB_DeMaximizeWindow\0"
    "PB_CloseWindow\0MSliderStyle\0SLD_Volume\0"
    "SLD_Postion"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MStyle[] = {

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
       1,    1, 0x0,    8,   34,
      10,   10, 0x0,    3,   50,
      14,   14, 0x0,   10,   56,
      25,   25, 0x0,    2,   76,

 // enum data: key, value
       2, uint(MStyle::LBL_Normal),
       3, uint(MStyle::LBL_Title),
       4, uint(MStyle::LBL_Header),
       5, uint(MStyle::LBL_Value),
       6, uint(MStyle::LBL_Tag),
       7, uint(MStyle::LBL_SongTitle),
       8, uint(MStyle::LBL_SongArtist),
       9, uint(MStyle::LBL_SongPosition),
      11, uint(MStyle::LE_Normal),
      12, uint(MStyle::LE_Hidden),
      13, uint(MStyle::LE_Search),
      15, uint(MStyle::PB_Normal),
      16, uint(MStyle::PB_NextSong),
      17, uint(MStyle::PB_PreviousSong),
      18, uint(MStyle::PB_PlaySong),
      19, uint(MStyle::PB_Fade),
      20, uint(MStyle::PB_Search),
      21, uint(MStyle::PB_MinimizeWindow),
      22, uint(MStyle::PB_MaximizeWindow),
      23, uint(MStyle::PB_DeMaximizeWindow),
      24, uint(MStyle::PB_CloseWindow),
      26, uint(MStyle::SLD_Volume),
      27, uint(MStyle::SLD_Postion),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject MStyle::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_MStyle.data,
    qt_meta_data_MStyle,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
