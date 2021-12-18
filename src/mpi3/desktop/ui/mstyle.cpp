#include "mpi3/desktop/ui/mstyle.h"

#include <QFontDatabase>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>


// TODO: Use namespace Mpi3::Style {...}


void MStyle::initialize()
{
    QFontDatabase::addApplicationFont(":/fonts/open-sans/OpenSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/open-sans/OpenSans-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/open-sans/OpenSans-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/open-sans/OpenSans-Regular.ttf");
}

void MStyle::setStyle(QLabel *widget, MLabelStyle style)
{
    switch(style) {
        case MStyle::LBL_Normal: {
            widget->setObjectName("Normal");
            break;
        }
        case MStyle::LBL_Title: {
            widget->setObjectName("Title");
            break;
        }
        case MStyle::LBL_Header: {
            widget->setObjectName("Header");
            break;
        }
        case MStyle::LBL_Value: {
            widget->setObjectName("Value");
            break;
        }
        case MStyle::LBL_Tag: {
            widget->setObjectName("Tag");
            break;
        }
        case MStyle::LBL_SongTitle: {
            widget->setObjectName("SongTitle");
            break;
        }
        case MStyle::LBL_SongArtist: {
            widget->setObjectName("SongArtist");
            break;
        }
        case MStyle::LBL_SongPosition: {
            widget->setObjectName("SongPosition");
            break;
        }
    }
}
void MStyle::setStyle(QLineEdit *widget, MLineEditStyle style)
{
    switch(style) {
        case MStyle::LE_Normal: {
            widget->setObjectName("Normal");
            break;
        }
        case MStyle::LE_Hidden: {
            widget->setObjectName("Hidden");
            break;
        }
        case MStyle::LE_Search: {
            widget->setObjectName("Search");
            break;
        }
    }
}
void MStyle::setStyle(QPushButton *widget, MPushButtonStyle style)
{
    switch(style) {
        case MStyle::PB_Normal: {
            widget->setObjectName("Normal");
            break;
        }
        case MStyle::PB_NextSong: {
            widget->setObjectName("NextSong");
            break;
        }
        case MStyle::PB_PreviousSong: {
            widget->setObjectName("PreviousSong");
            break;
        }
        case MStyle::PB_PlaySong: {
            widget->setObjectName("PlaySong");
            break;
        }
        case MStyle::PB_Fade: {
            widget->setObjectName("Fade");
            break;
        }
        case MStyle::PB_Search: {
            widget->setObjectName("Search");
            break;
        }
        case MStyle::PB_MinimizeWindow: {
            widget->setObjectName("MinimizeWindow");
            break;
        }
        case MStyle::PB_MaximizeWindow: {
            widget->setObjectName("MaximizeWindow");
            break;
        }
        case MStyle::PB_DeMaximizeWindow: {
            widget->setObjectName("DeMaximizeWindow");
            break;
        }
        case MStyle::PB_CloseWindow: {
            widget->setObjectName("CloseWindow");
            break;
        }
    }
}
void MStyle::setStyle(QSlider *widget, MSliderStyle style)
{
    switch(style) {
        case MStyle::SLD_Volume: {
            widget->setObjectName("Volume");
            break;
        }
        case MStyle::SLD_Postion: {
            widget->setObjectName("Position");
            break;
        }
    }
}


MProxyStyle::MProxyStyle(QStyle *style) : QProxyStyle(style)
{

}

void MProxyStyle::drawPrimitive(
        PrimitiveElement element, const QStyleOption *option,
        QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::PE_IndicatorRadioButton) {
        return;
    }
    else if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()) {
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
