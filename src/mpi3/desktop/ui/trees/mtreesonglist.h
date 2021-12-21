#pragma once

#ifndef MTREESONGLIST_H
#define MTREESONGLIST_H


#include "mpi3/desktop/ui/trees/mtree.h"


class MTreeSonglist : public MTree
{
    Q_OBJECT

public:
    explicit MTreeSonglist(QWidget *parent = nullptr);

public:
    void autoFitColumns();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void moveSelected(int row);
};


#endif
