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


MProxyStyle::MProxyStyle(QStyle *style) : QProxyStyle(style)
{

}

void MProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if(element == QStyle::PE_IndicatorRadioButton) {
        return;
    }
    else if(element == QStyle::PE_IndicatorItemViewItemDrop && !option->rect.isNull()) {
        return;
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
