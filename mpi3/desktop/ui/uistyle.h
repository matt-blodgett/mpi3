#ifndef UISTYLE_H
#define UISTYLE_H

#include <QProxyStyle>


class Mpi3Style: public QProxyStyle
{
    Q_OBJECT

public:
    Mpi3Style(QStyle *style = nullptr);

public:
    enum TV_IndicatorStyle {
        TV_IndicatorStyleMove,
        TV_IndicatorStyleDrop
    };

    TV_IndicatorStyle currentTVIndicatorStyle() const;
    void setTVIndicatorStyle(TV_IndicatorStyle tvIndicatorStyle);

    bool noDraw = false;

private:
    TV_IndicatorStyle m_currentTVIndicatorStyle = Mpi3Style::TV_IndicatorStyleDrop;

public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};

#endif
