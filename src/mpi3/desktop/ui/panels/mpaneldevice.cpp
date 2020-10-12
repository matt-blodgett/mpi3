#include "mpi3/desktop/ui/panels/mpaneldevice.h"
#include "mpi3/desktop/ui/models/mmodeldrives.h"

#include "mpi3/desktop/ui/frames/mframetreeview.h"

#include "mpi3/desktop/ui/mstyle.h"
#include "mpi3/core/mmedialibrary.h"


#include <QGridLayout>
#include <QTreeView>
#include <QPushButton>
#include <QEvent>
#include <QLineEdit>
#include <QLabel>
#include <QItemSelectionModel>

#include <QSettings>

#include <QDebug>


MPanelDevice::MPanelDevice(QWidget *parent) : MPanel(parent)
{
    m_sectionDrives = addSection();
    m_sectionCurrentDrive = addSection();

    m_lblDrives = new QLabel(this);
    m_lblCurrentDrive = new QLabel(this);

    MStyle::setStyle(m_lblDrives, MStyle::LBL_Tag);
    MStyle::setStyle(m_lblCurrentDrive, MStyle::LBL_Tag);

    QGridLayout *gridHeader = new QGridLayout();
    gridHeader->addWidget(m_lblDrives, 0, 0, 1, 1);
    gridHeader->setColumnStretch(1, 1);

    gridControl()->addLayout(gridHeader, 0, 0, 1, 1);
    gridControl()->setRowStretch(1, 1);
    gridControl()->setContentsMargins(0, 0, 0, 0);


    m_frmDrives = new MFrameDrives(this);
    QGridLayout *devices_gridNorth = m_sectionDrives->gridNorth();
    devices_gridNorth->addWidget(m_frmDrives, 0, 0, 1, 1);


    setTitle("Raspberry Pi");
    m_sectionDrives->setHeader("Available Removable Storage Devices");

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

