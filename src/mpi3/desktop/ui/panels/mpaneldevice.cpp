#include "mpaneldevice.h"
#include "mmedialibrary.h"
#include "mstyle.h"

#include <QGridLayout>
#include <QTreeView>
#include <QPushButton>
#include <QEvent>
#include <QLineEdit>
#include <QLabel>


#include <QDebug>


MPanelDevice::MPanelDevice(QWidget *parent) : MPanel(parent)
{
    m_lblDevices = new QLabel(this);
    MStyle::setStyle(m_lblDevices, MStyle::LBL_Tag);

    m_btnRefreshVolumes = new QPushButton(this);
    m_btnCreateVolume = new QPushButton(this);

    m_btnRefreshVolumes->setFixedWidth(20);
    m_btnCreateVolume->setFixedWidth(20);

    QGridLayout *gridHeader = new QGridLayout();
    gridHeader->addWidget(m_lblDevices, 0, 0, 1, 1);
    gridHeader->setColumnStretch(1, 1);
    gridHeader->addWidget(m_btnRefreshVolumes, 0, 2, 1, 1);
    gridHeader->addWidget(m_btnCreateVolume, 0, 3, 1, 1);

    gridControl()->addLayout(gridHeader, 0, 0, 1, 1);
    gridControl()->setRowStretch(1, 1);
    gridControl()->setVerticalSpacing(0);
    gridControl()->setHorizontalSpacing(0);
    m_sectionLibrary = addSection();

    m_boxLibName = new QLineEdit(this);
    m_lblLibAddedTag = new QLabel(this);
    m_lblLibAdded = new QLabel(this);
    m_lblSyncMediaTag = new QLabel(this);

    MStyle::setStyle(m_boxLibName, MStyle::LE_Normal);
    MStyle::setStyle(m_lblLibAddedTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_lblLibAdded, MStyle::LBL_Value);
    MStyle::setStyle(m_lblSyncMediaTag, MStyle::LBL_Tag);

    m_treeSyncMedia = new QTreeView(this);
    m_lblCurrentDevice = new QLabel(this);
    m_frmStorageSpace = new QWidget(this);

    MStyle::setStyle(m_lblCurrentDevice, MStyle::LBL_Tag);

    QGridLayout *library_gridWest = m_sectionLibrary->gridWest();
    QGridLayout *library_gridEast = m_sectionLibrary->gridEast();
    QGridLayout *library_gridSouth = m_sectionLibrary->gridSouth();

    library_gridWest->addWidget(m_boxLibName, 0, 0, 1, 3);
    library_gridWest->addWidget(m_lblLibAddedTag, 1, 0, 1, 1);
    library_gridWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    library_gridWest->setColumnStretch(2, 1);
    library_gridWest->setRowStretch(2, 1);

    library_gridEast->addWidget(m_lblSyncMediaTag, 0, 0, 1, 1);
    library_gridEast->addWidget(m_treeSyncMedia, 1, 0, 1, 1);

    library_gridSouth->addWidget(m_lblCurrentDevice, 0, 0, 1, 1);
    library_gridSouth->addWidget(m_frmStorageSpace, 1, 0, 1, 1);
    library_gridSouth->setRowMinimumHeight(1, 40);




    setTitle("Raspberry Pi");
    m_sectionLibrary->setHeader("Device Library");

    m_lblSyncMediaTag->setText("Sync Media");
    m_lblLibAddedTag->setText("");

    m_lblCurrentDevice->setText("RASPI (E:)");
    m_lblLibAdded->setText("");
    m_boxLibName->setText("");


    m_lblDevices->setText("Devices");

    m_lblDevices->setStyleSheet("QLabel {border-bottom: 1px solid #696969;"
                                 "font-size: 14px; padding: 4px 2px 4px 2px;}");

    m_frmStorageSpace->setObjectName("StorageWidget");
}

void MPanelDevice::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
}
void MPanelDevice::setRaspiLibrary(MMediaLibrary *library)
{
    m_deviceLibrary = library;

    m_lblLibAddedTag->setText("");
    m_lblLibAdded->setText("");
    m_boxLibName->setText("");

    if(m_deviceLibrary) {
        m_lblLibAddedTag->setText("Created:");
        m_lblLibAdded->setText(m_deviceLibrary->added());
        m_boxLibName->setText(m_deviceLibrary->name());
    }
}
