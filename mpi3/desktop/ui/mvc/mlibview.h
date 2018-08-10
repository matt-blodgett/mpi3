#pragma once

#ifndef MLIBVIEW_H
#define MLIBVIEW_H

#include <QTreeView>

class MStyle;


class MTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MTreeView(QWidget *parent = nullptr);

protected:
    MStyle *m_drawStyle = nullptr;
};


class MTreeContainers : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeContainers(QWidget *parent = nullptr);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};


class MTreeSonglist : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeSonglist(QWidget *parent = nullptr);

private:
    void dropEvent(QDropEvent *event);
};


#endif
