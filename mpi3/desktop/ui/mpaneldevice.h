#pragma once

#ifndef MPANELDEVICE_H
#define MPANELDEVICE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QTreeView;
class QSplitter;
class QLineEdit;
class QLabel;

class QAbstractItemModel;
QT_END_NAMESPACE


#include "mglobal.h"
class MModelStorageDrives;


class MPanelDevice : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelDevice(QWidget *parent = nullptr);
    ~MPanelDevice();

    void setModel(QAbstractItemModel *model);


    void setLibrary(MMediaLibrary *mediaLib);
    void setRaspiLibrary(MMediaLibrary *raspiLib);


    void loadVolume();
    void createVolume();

private:
    MMediaLibrary *m_mediaLibrary = nullptr;
    MMediaLibrary *m_deviceLibrary = nullptr;

    void initializeLayout();
    void test();

    void selectionChanged();

private:
    QSplitter *m_frmSplitter = nullptr;

    QTreeView *m_treeStorageDevices = nullptr;
    MModelStorageDrives *m_modelStorageDrives = nullptr;

    QLabel *m_lblLibAddedTag = nullptr;
    QLabel *m_lblSelectedLibrary = nullptr;
    QLabel *m_lblSelectedDrive = nullptr;
    QPushButton *m_btnCreateVolume = nullptr;
    QPushButton *m_btnLoadVolume = nullptr;

    QTreeView *m_treeSyncMedia = nullptr;

    QLineEdit *m_boxLibName = nullptr;
    QLabel *m_lblLibAdded = nullptr;

    QLabel *m_lblCurrentDevice = nullptr;
    QWidget *m_frmStorageSpace = nullptr;

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
