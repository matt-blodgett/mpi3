#ifndef MTREEVIEW_H
#define MTREEVIEW_H

#include <QProxyStyle>
#include <QTreeView>

class Mpi3TreeView;
class Mpi3TreeViewStyle;


class Mpi3TreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeView(QWidget *parent = nullptr);

public:
    enum DisplayStyle{
        DisplayStyleSonglist,
        DisplayStyleContainers
    };

    DisplayStyle displayStyle() const;
    void setDisplayStyle(Mpi3TreeView::DisplayStyle dStyle);

private:
    DisplayStyle m_displayStyle = Mpi3TreeView::DisplayStyleSonglist;
    Mpi3TreeViewStyle *m_drawStyle = nullptr;

private:
    bool eventFilter(QObject *object, QEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};


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


#endif
