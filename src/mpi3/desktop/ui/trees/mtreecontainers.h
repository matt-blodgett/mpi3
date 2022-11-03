#pragma once

#ifndef MTREECONTAINERS_H
#define MTREECONTAINERS_H


#include "mpi3/desktop/ui/trees/mtree.h"


class MTreeContainers : public MTree
{
    Q_OBJECT

public:
    explicit MTreeContainers(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
};


#endif
