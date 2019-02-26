#pragma once

#ifndef MSTYLE_H
#define MSTYLE_H


#include <QProxyStyle>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class QSlider;
QT_END_NAMESPACE


namespace MStyle
{
    Q_NAMESPACE

    void initialize();

    enum MLabelStyle {
        LBL_Normal,
        LBL_Title,
        LBL_Header,
        LBL_Value,
        LBL_Tag,
        LBL_SongTitle,
        LBL_SongArtist,
        LBL_SongPosition
    }; Q_ENUM_NS(MLabelStyle)

    enum MLineEditStyle {
        LE_Normal,
        LE_Hidden,
        LE_Search
    }; Q_ENUM_NS(MLineEditStyle)

    enum MPushButtonStyle {
        PB_Normal,
        PB_NextSong,
        PB_PreviousSong,
        PB_PlaySong,
        PB_Fade,
        PB_Search,
        PB_MinimizeWindow,
        PB_MaximizeWindow,
        PB_DeMaximizeWindow,
        PB_CloseWindow
    }; Q_ENUM_NS(MPushButtonStyle)

    enum MSliderStyle {
        SLD_Volume,
        SLD_Postion
    }; Q_ENUM_NS(MSliderStyle)

    void setStyle(QLabel *widget, MLabelStyle style);
    void setStyle(QLineEdit *widget, MLineEditStyle style);
    void setStyle(QPushButton *widget, MPushButtonStyle style);
    void setStyle(QSlider *widget, MSliderStyle style);
};


class MProxyStyle: public QProxyStyle
{
    Q_OBJECT

public:
    MProxyStyle(QStyle *style = nullptr);

public:
    void drawPrimitive(
        PrimitiveElement element, const QStyleOption *option,
        QPainter *painter, const QWidget *widget = nullptr) const;
};


#endif
