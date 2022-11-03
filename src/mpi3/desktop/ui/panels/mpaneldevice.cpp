#include "mpi3/desktop/ui/panels/mpaneldevice.h"
//#include "mpi3/desktop/ui/models/mmodeldrives.h"
#include "mpi3/desktop/ui/frames/mframetreeview.h"
#include "mpi3/core/mmedialibrary.h"


#include <QGridLayout>
#include <QTreeView>
#include <QPushButton>
#include <QEvent>
#include <QLineEdit>
#include <QLabel>
#include <QItemSelectionModel>

#include <QScrollArea>

#include <QSettings>

#include <QDebug>


MPanelDevice::MPanelDevice(QWidget *parent) : MPanel(parent)
{
    m_frmDrives = new MFrameDrives(this);



    QGridLayout *m_gridPanel = new QGridLayout();
    QWidget *m_frmPanel = new QWidget(this);
    m_frmPanel->setLayout(m_gridPanel);

    // Change styling for this

    QLabel *m_lblPanelTitle = new QLabel(this);
    m_lblPanelTitle->setObjectName("Title");

    QScrollArea *m_scrollArea = new QScrollArea(this);
    QWidget *m_frmScrollArea = new QWidget(this);
    m_scrollArea->setWidget(m_frmScrollArea);
    m_scrollArea->setWidgetResizable(true);

    m_gridPanel->addWidget(m_lblPanelTitle, 0, 0, 1, 1);
    m_gridPanel->addWidget(m_scrollArea, 1, 0, 1, 1);
    m_gridPanel->setAlignment(m_lblPanelTitle, Qt::AlignCenter);
    m_gridPanel->setContentsMargins(0, 10, 0, 0);
    m_gridPanel->setRowStretch(1, 1);
    m_gridPanel->setColumnStretch(0, 1);

    QGridLayout *m_gridMain = new QGridLayout();
    m_gridMain->addWidget(m_frmPanel);
    m_gridMain->setContentsMargins(0, 0, 0, 0);
    m_gridMain->setColumnStretch(0, 1);
    m_gridMain->setRowStretch(0, 1);
    setLayout(m_gridMain);

    QGridLayout *m_gridScrollArea = new QGridLayout();
    m_frmScrollArea->setLayout(m_gridScrollArea);

    QGridLayout *m_grid0 = new QGridLayout();
    m_grid0->addWidget(m_frmDrives, 0, 0, 1, 1);
    m_grid0->setContentsMargins(0, 0, 0, 0);
    m_grid0->setRowStretch(0, 1);
    m_grid0->setColumnStretch(0, 1);

    m_gridScrollArea->addLayout(m_grid0, 0, 1, 1, 1);
    m_gridScrollArea->setContentsMargins(0, 0, 0, 0);
    m_gridScrollArea->setColumnMinimumWidth(0, 50);
//    m_gridScrollArea->setColumnStretch(0, 0);
    m_gridScrollArea->setColumnStretch(1, 1);
//    m_gridScrollArea->setColumnStretch(2, 0);
    m_gridScrollArea->setColumnMinimumWidth(2, 50);

    m_frmPanel->setObjectName("PanelScrollArea");
    m_frmScrollArea->setObjectName("PanelScrollArea");


    m_lblPanelTitle->setText("Raspberry Pi");


    m_lblCurrentDrive = new QLabel(this);
    m_lblCurrentDrive->setObjectName("Tag");
//    m_sectionDrives->setHeader("Available Removable Storage Devices");

    connect(m_frmDrives, &MFrameDrives::selectedDriveChanged, this, &MPanelDevice::setCurrentDrive);
}


void MPanelDevice::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
}
void MPanelDevice::load(QSettings *settings)
{
    settings->beginGroup("RaspberryPi");
    settings->endGroup();
}
void MPanelDevice::save(QSettings *settings)
{
    settings->beginGroup("RaspberryPi");
    settings->endGroup();
}

void MPanelDevice::setCurrentDrive(QStorageInfo info)
{
    m_lblCurrentDrive->setText(info.rootPath());
}

