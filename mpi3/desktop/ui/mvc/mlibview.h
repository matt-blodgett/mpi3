#ifndef MLIBVIEW_H
#define MLIBVIEW_H

#include <QTreeView>

namespace Mpi3{

class MStyle;

};



class Mpi3TreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeView(QWidget *parent = nullptr);

protected:
    Mpi3::MStyle *m_drawStyle = nullptr;
};


class Mpi3TreeViewContainers : public Mpi3TreeView
{
    Q_OBJECT

public:
    explicit Mpi3TreeViewContainers(QWidget *parent = nullptr);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
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
