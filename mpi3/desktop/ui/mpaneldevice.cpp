#include "mpaneldevice.h"
#include "core/mmedialibrary.h"

#include <QGridLayout>
#include <QTreeView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QEvent>


#include <QDebug>


MPanelDevice::MPanelDevice(QWidget *parent) : MPanel(parent){
    initializeLayoutType(true);
    initializeLayout();

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

void MPanelDevice::initializeLayout() {


    m_lblDevices = addLabelTag();

//    m_btnRefreshVolumes = new QPushButton(this);
//    m_btnCreateVolume = new QPushButton(this);
    m_btnRefreshVolumes = addPushButton();
    m_btnCreateVolume = addPushButton();


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
    gridControl()->setMargin(0);



    m_sectionLibrary = addSection();

    m_boxLibName = addLineEditHidden();
    m_lblLibAddedTag = addLabelTag();
    m_lblLibAdded = addLabelValue();
    m_lblSyncMediaTag = addLabelTag();
    m_treeSyncMedia = addTreeView();
    m_lblCurrentDevice = addLabelTag();
    m_frmStorageSpace = new QWidget(this);

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
}

void MPanelDevice::setLibrary(MMediaLibrary *mediaLib){
    m_mediaLibrary = mediaLib;
}
void MPanelDevice::setRaspiLibrary(MMediaLibrary *raspiLib){
    m_deviceLibrary = raspiLib;

    m_lblLibAddedTag->setText("");
    m_lblLibAdded->setText("");
    m_boxLibName->setText("");

    if(m_deviceLibrary){
        m_lblLibAddedTag->setText("Created:");
        m_lblLibAdded->setText(m_deviceLibrary->added());
        m_boxLibName->setText(m_deviceLibrary->name());
    }
}
