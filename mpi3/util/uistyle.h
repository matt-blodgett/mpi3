#ifndef UISTYLE_H
#define UISTYLE_H

#include <QString>
#include <QProxyStyle>


class Mpi3StyleSheet
{

public:
    Mpi3StyleSheet();
    ~Mpi3StyleSheet();

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
