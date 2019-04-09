#pragma once

#ifndef MMODELSONGLISTPROXY_H
#define MMODELSONGLISTPROXY_H


#include <QSortFilterProxyModel>


class MModelSonglistProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit MModelSonglistProxy(QObject *parent = nullptr);
};


#endif
