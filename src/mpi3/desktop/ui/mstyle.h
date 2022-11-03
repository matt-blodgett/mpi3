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


// TODO: Use Mpi3::Style
namespace MStyle
{
    Q_NAMESPACE
    void initialize();
};


class MProxyStyle: public QProxyStyle
{
    Q_OBJECT

public:
    MProxyStyle(QStyle *style = nullptr);

public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};

#endif
