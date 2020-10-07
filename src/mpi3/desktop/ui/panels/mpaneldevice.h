#pragma once

#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H


#include "mpi3/desktop/ui/panels/mpanel.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QTreeView;
class QLineEdit;
QT_END_NAMESPACE


class MPanelDevice : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);
    void setLibrary(MMediaLibrary *library) override;

public:
    void setRaspiLibrary(MMediaLibrary *library);
    void loadVolume();
    void createVolume();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MMediaLibrary *m_deviceLibrary = nullptr;

private:
    QLabel *m_lblDevices = nullptr;
    QPushButton *m_btnRefreshVolumes = nullptr;
    QPushButton *m_btnCreateVolume = nullptr;

    MPanelSection *m_sectionLibrary = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAddedTag = nullptr;
    QLabel *m_lblLibAdded = nullptr;
    QLabel *m_lblSyncMediaTag = nullptr;
    QTreeView *m_treeSyncMedia = nullptr;
    QLabel *m_lblCurrentDevice = nullptr;
    QWidget *m_frmStorageSpace = nullptr;

};


#endif
