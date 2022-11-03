#pragma once

#ifndef MTREE_H
#define MTREE_H


#include <QTreeView>


class MProxyStyle;


class MTree : public QTreeView
{
    Q_OBJECT

public:
    explicit MTree(QWidget *parent = nullptr);

protected:
    MProxyStyle *m_drawStyle = nullptr;
};


#endif
