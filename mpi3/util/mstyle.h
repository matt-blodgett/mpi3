#ifndef MSTYLE_H
#define MSTYLE_H

#include <QProxyStyle>


namespace Mpi3
{

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
    enum TV_IndicatorStyle {
        TV_IndicatorStyleMove,
        TV_IndicatorStyleDrop
    };

    TV_IndicatorStyle currentTVIndicatorStyle() const;
    void setTVIndicatorStyle(TV_IndicatorStyle tvIndicatorStyle);

private:
    TV_IndicatorStyle m_currentTVIndicatorStyle = MStyle::TV_IndicatorStyleDrop;

public:
    void drawPrimitive(
            PrimitiveElement element, const QStyleOption *option,
            QPainter *painter, const QWidget *widget = nullptr) const;
};

};

#endif
