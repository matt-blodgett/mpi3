#pragma once

#ifndef MROOT_H
#define MROOT_H


#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE


#include "mpi3/core/mglobal.h"
#include "mpi3/device/mdevice.h"

#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_UI MRootDevice;
#endif


class MStyleSheet;
//class MMediaLibrary;
//class MAudioEngine;


class MRootDevice : public QMainWindow
{
    Q_OBJECT

public:
    MRootDevice(QWidget *parent = nullptr);
    ~MRootDevice();

private:
    QPushButton *m_testButton = nullptr;

public:
    void initialize();

private:
    void initializeObjects();
    void initializeLayout();
    void initializeState();
    void initializeStyle();
    void saveSettings();

private:
    MStyleSheet *m_styleSheet = nullptr;
//    MMediaLibrary *m_mediaLibrary = nullptr;
//    MAudioEngine *m_audioEngine = nullptr;

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
};


#endif
