#pragma once

#ifndef MSTYLE_H
#define MSTYLE_H


#include <QProxyStyle>


#include "mutils.h"

#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_UTILS MStyleSheet;
class MPI3_EXPORT_UTILS MStyle;
#endif


class MStyleSheet
{

public:
    explicit MStyleSheet();

public:
    void load(const QString &path = QString());
    void save(const QString &path = QString());

public:
    QString qssName() const;
    QString qssStyle() const;
    QString qssPath() const;

private:
    void setProperty(const QString &line);
    QString removeComments(const QString &text);

private:
    QString m_name;
    QString m_style;
    QString m_filepath;
};


class MStyle: public QProxyStyle
{
    Q_OBJECT

public:
    MStyle(QStyle *style = nullptr);

public:
    void drawPrimitive(
            PrimitiveElement element,
            const QStyleOption *option,
            QPainter *painter,
            const QWidget *widget = nullptr
        ) const;
};


#endif
