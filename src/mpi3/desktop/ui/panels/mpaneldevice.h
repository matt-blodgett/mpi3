#pragma once

#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H


#include "mpi3/desktop/ui/panels/mpanel.h"


QT_BEGIN_NAMESPACE
class QStorageInfo;
class QLabel;
QT_END_NAMESPACE


//class MModelDrives;
class MFrameDrives;


class MPanelDevice : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;
    void load(QSettings *settings) override;
    void save(QSettings *settings) override;


private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MMediaLibrary *m_deviceLibrary = nullptr;

    void setCurrentDrive(QStorageInfo info);

private:
    MFrameDrives *m_frmDrives = nullptr;
    QLabel *m_lblCurrentDrive = nullptr;

};


#endif
