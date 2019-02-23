#pragma once

#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H


#include "mglobal.h"
#include "mpanel.h"


class MPanelDevice : public MPanel
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);

private:
    void initializeLayout();

public:
    void setLibrary(MMediaLibrary *mediaLib);
    void setRaspiLibrary(MMediaLibrary *raspiLib);

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
