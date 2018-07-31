#ifndef LIBVIEW_H
#define LIBVIEW_H

#include <QProxyStyle>
#include <QTreeView>


class Mpi3TreeViewStyle: public QProxyStyle
{
    Q_OBJECT

public:
    Mpi3TreeViewStyle(QStyle *style = nullptr);

public:
    enum IndicatorStyle {
        IndicatorStyleMove,
        IndicatorStyleDrop
    };

    IndicatorStyle indicatorStyle() const;
    void setIndicatorStyle(Mpi3TreeViewStyle::IndicatorStyle iStyle);

private:
    IndicatorStyle m_indicatorStyle = Mpi3TreeViewStyle::IndicatorStyleDrop;

public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
};


class Mpi3TreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeView(QWidget *parent = nullptr);

protected:
    Mpi3TreeViewStyle *m_drawStyle = nullptr;
};

class Mpi3TreeViewContainers : public Mpi3TreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeViewContainers(QWidget *parent = nullptr);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

class Mpi3TreeViewSonglist : public Mpi3TreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeViewSonglist(QWidget *parent = nullptr);

private:
    void dropEvent(QDropEvent *event);
};


#endif
