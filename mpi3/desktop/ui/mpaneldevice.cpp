#include "mpaneldevice.h"
#include "core/mmedialibrary.h"
#include "mvc/mdrivemodel.h"

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
    m_sectionDevices->setHeader("Storage Devices");
    m_sectionLibrary->setHeader("Device Library");

    m_btnCreateVolume->setText("Create");
    m_btnLoadVolume->setText("Load");

    m_lblSyncMediaTag->setText("Sync Media");
    m_lblLibAddedTag->setText("");

    m_lblCurrentDevice->setText("RASPI (E:)");
    m_lblLibAdded->setText("");
    m_boxLibName->setText("");

    m_frmStorageSpace->setObjectName("StorageWidget");

    m_modelStorageDrives = new MModelStorageDrives(this);
    m_treeStorageDevices->setModel(m_modelStorageDrives);
    m_treeStorageDevices->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_treeStorageDevices->setSelectionMode(QAbstractItemView::SingleSelection);
    m_treeStorageDevices->setFocusPolicy(Qt::NoFocus);
    m_treeStorageDevices->setIndentation(0);
    m_treeStorageDevices->setItemsExpandable(false);
    m_treeStorageDevices->setSortingEnabled(true);
    m_treeStorageDevices->setAlternatingRowColors(true);
    m_treeStorageDevices->viewport()->installEventFilter(this);
    m_treeStorageDevices->setFixedHeight(200);

    connect(m_treeStorageDevices->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, [this](){selectionChanged();});

    connect(m_btnCreateVolume, &QPushButton::released, this, &MPanelDevice::createVolume);
    connect(m_btnLoadVolume, &QPushButton::released, this, &MPanelDevice::loadVolume);

    selectionChanged();
}

void MPanelDevice::initializeLayout() {

    m_sectionDevices = addSection();
    m_sectionLibrary = addSection();

    m_lblSelectedDrive = addLabelTag();
    m_lblSelectedLibrary = addLabelValue();
    m_btnCreateVolume = addPushButton();
    m_btnLoadVolume = addPushButton();
    m_treeStorageDevices = addTreeView();

    m_boxLibName = addLineEditHidden();
    m_lblLibAddedTag = addLabelTag();
    m_lblLibAdded = addLabelValue();
    m_lblSyncMediaTag = addLabelTag();
    m_treeSyncMedia = addTreeView();
    m_lblCurrentDevice = addLabelTag();
    m_frmStorageSpace = new QWidget(this);

    QGridLayout *device_gridNorth = m_sectionDevices->gridNorth();
    device_gridNorth->addWidget(m_lblSelectedDrive, 0, 0, 1, 1);
    device_gridNorth->addWidget(m_lblSelectedLibrary, 1, 0, 1, 1);
    device_gridNorth->addWidget(m_btnCreateVolume, 3, 0, 1, 1);
    device_gridNorth->addWidget(m_btnLoadVolume, 3, 0, 1, 1);
    device_gridNorth->addWidget(m_treeStorageDevices, 0, 2, 4, 1);
    device_gridNorth->setColumnMinimumWidth(0, m_btnCreateVolume->width());
    device_gridNorth->setColumnMinimumWidth(1, 10);
    device_gridNorth->setRowStretch(2, 1);

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

void MPanelDevice::loadVolume(){
    QModelIndex idx = m_treeStorageDevices->selectionModel()->currentIndex();
    MMediaLibrary *raspiLib = m_modelStorageDrives->raspiLibraryAt(idx);
    setRaspiLibrary(raspiLib);
}
void MPanelDevice::createVolume(){
    QModelIndex idx = m_treeStorageDevices->selectionModel()->currentIndex();
    QString rootPath = m_modelStorageDrives->rootPathAt(idx);
    MMediaLibrary *raspiLib = m_mediaLibrary->createRaspiVolume(rootPath);
    qDebug() << raspiLib->name();
}

void MPanelDevice::selectionChanged(){

    bool deviceSelected = m_treeStorageDevices->selectionModel()->selectedRows().size() == 1;

    if(deviceSelected){
        QModelIndex idx = m_treeStorageDevices->selectionModel()->currentIndex();
        QModelIndex idx_driveName = m_modelStorageDrives->index(idx.row(), 0);
        QModelIndex idx_libName = m_modelStorageDrives->index(idx.row(), 1);

        QString str_driveName = m_modelStorageDrives->data(idx_driveName).toString();
        QString str_libName = m_modelStorageDrives->data(idx_libName).toString();

        bool nolibrary = str_libName == "";

        m_lblSelectedDrive->setText(str_driveName);
        m_lblSelectedLibrary->setText(str_libName);

        m_btnCreateVolume->setVisible(nolibrary);
        m_btnLoadVolume->setVisible(!nolibrary);
    }
    else {
        m_lblSelectedDrive->setText("");
        m_lblSelectedLibrary->setText("");

        m_btnCreateVolume->setVisible(false);
        m_btnLoadVolume->setVisible(false);
    }
}

void MPanelDevice::showEvent(QShowEvent *event){

    int totalWidth = 0;
    QList<int> colWidths = {0, 240, 85, 85, 85};

    for(int i = 0; i < colWidths.size(); i++){
        m_treeStorageDevices->setColumnWidth(i, colWidths[i]);
        totalWidth += colWidths[i];
    }

    int treeWidth = m_treeStorageDevices->width();
    m_treeStorageDevices->setColumnWidth(0, treeWidth - totalWidth - 2);

    MPanel::showEvent(event);
}
bool MPanelDevice::eventFilter(QObject *obj, QEvent *event){

    if(obj == m_treeStorageDevices->viewport()){
        if(event->type() == QEvent::MouseButtonPress){
            m_treeStorageDevices->selectionModel()->clear();

            m_modelStorageDrives->refresh();

        }
    }

    return QWidget::eventFilter(obj, event);
}
















