#ifndef UISTYLE_H
#define UISTYLE_H

#include <QProxyStyle>


class Mpi3Style: public QProxyStyle
{
    Q_OBJECT

public:
    Mpi3Style(QStyle *style = nullptr);

public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};

#endif // UISTYLE_H
