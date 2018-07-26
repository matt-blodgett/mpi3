#ifndef MTREEVIEW_H
#define MTREEVIEW_H

#include <QTreeView>
#include <QProxyStyle>


class Mpi3TreeView : public QTreeView
{
    Q_OBJECT

public:
    enum IndicatorStyle {
        MoveIndicator,
        DropIndicator
    };

    explicit Mpi3TreeView(QWidget *parent = nullptr);

public:
    IndicatorStyle dropIndicatorStyle() const;
    void setDropIndicatorStyle(IndicatorStyle style);

private:
    IndicatorStyle m_currentStyle = Mpi3TreeView::DropIndicator;

private:
    void paintEvent(QPaintEvent *event);
};


class Mpi3TreeViewStyle: public QProxyStyle
{
    Q_OBJECT

public:
    Mpi3TreeViewStyle(QStyle* style = nullptr);
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};


#endif // MTREEVIEW_H
