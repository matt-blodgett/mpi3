#pragma once

#ifndef MPANEL_H
#define MPANEL_H


#include "mpi3/desktop/ui/frames/mframe.h"


QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE


#include "mpi3/core/mglobal.h"


class MPanel : public MFrame
{
    Q_OBJECT

public:
    explicit MPanel(QWidget *parent = nullptr);
    virtual void setLibrary(MMediaLibrary *library);
    virtual void load(QSettings *settings);
    virtual void save(QSettings *settings);
};


#endif
