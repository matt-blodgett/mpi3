#pragma once

#ifndef MFRAMEDRIVES_H
#define MFRAMEDRIVES_H


#include "mpi3/desktop/ui/frames/mframe.h"


#include <QStorageInfo>


#include "mpi3/core/mglobal.h"
class MTreeDrives;
class MModelDrives;


class MFrameDrives : public MFrame
{
    Q_OBJECT

public:
    explicit MFrameDrives(QWidget *parent = nullptr);

public:
    void openItemFileLocation();

public:
    MTreeDrives *tree() const;
    MModelDrives *model() const;

private:
    MTreeDrives *m_treeDrives = nullptr;
    MModelDrives *m_modelDrives = nullptr;

private slots:
    void contextMenuTreeview(const QPoint &point);
    void selectionChanged();


signals:
    void selectedDriveChanged(QStorageInfo info);
};


#endif
