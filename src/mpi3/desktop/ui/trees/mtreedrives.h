#pragma once

#ifndef MTREEDRIVES_H
#define MTREEDRIVES_H


#include <QTreeView>


class MProxyStyle;


class MTreeDrives : public QTreeView
{
    Q_OBJECT

public:
    explicit MTreeDrives(QWidget *parent = nullptr);

protected:
    MProxyStyle *m_drawStyle = nullptr;
};


#endif
