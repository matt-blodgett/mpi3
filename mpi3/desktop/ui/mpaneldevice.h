#pragma once

#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H

#include "mglobal.h"
#include "mpanel.h"
class MModelStorageDrives;


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
    void selectionChanged();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MMediaLibrary *m_deviceLibrary = nullptr;
    MModelStorageDrives *m_modelStorageDrives = nullptr;

private:
    MPanelSection *m_sectionDevices = nullptr;
    MPanelSection *m_sectionLibrary = nullptr;

    QLabel *m_lblSelectedDrive = nullptr;
    QLabel *m_lblSelectedLibrary = nullptr;
    QPushButton *m_btnCreateVolume = nullptr;
    QPushButton *m_btnLoadVolume = nullptr;
    QTreeView *m_treeStorageDevices = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAddedTag = nullptr;
    QLabel *m_lblLibAdded = nullptr;
    QLabel *m_lblSyncMediaTag = nullptr;
    QTreeView *m_treeSyncMedia = nullptr;
    QLabel *m_lblCurrentDevice = nullptr;
    QWidget *m_frmStorageSpace = nullptr;

protected:
    void showEvent(QShowEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
