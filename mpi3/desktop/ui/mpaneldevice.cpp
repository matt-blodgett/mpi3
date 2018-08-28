#include "mpaneldevice.h"
#include "mvc/mdrivemodel.h"
#include "core/mmedialibrary.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>

#include <QHeaderView>
#include <QScrollBar>
#include <QTreeView>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include <QEvent>



#include <QDebug>



MPanelDevice::MPanelDevice(QWidget *parent) : QWidget(parent){
    initializeLayout();


    m_lblCurrentDevice->setText("RASPI (E:)");


    m_lblLibAdded->setText("");
    m_boxLibName->setText("");
    m_lblLibAddedTag->setText("");


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

    connect(m_treeStorageDevices->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this](){selectionChanged();});

    connect(m_btnCreateVolume, &QPushButton::released, this, &MPanelDevice::createVolume);
    connect(m_btnLoadVolume, &QPushButton::released, this, &MPanelDevice::loadVolume);

    selectionChanged();

}
MPanelDevice::~MPanelDevice(){}


void MPanelDevice::setModel(QAbstractItemModel *model){
    m_treeSyncMedia->setModel(model);
}

void MPanelDevice::test(){

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


bool MPanelDevice::eventFilter(QObject *obj, QEvent *event){

    if(obj == m_treeStorageDevices->viewport()){
        if(event->type() == QEvent::MouseButtonPress){
            m_treeStorageDevices->selectionModel()->clear();
        }
    }

    return QWidget::eventFilter(obj, event);
}

void MPanelDevice::initializeLayout(){

    int w_split = 400;
    int w_separator = 10;
    int w_section = (w_split * 2) + w_separator;

    // -------------------------------------------------- CONTROL PANEl

    QWidget *frmControl = new QWidget(this);

    QGridLayout *layoutControl = new QGridLayout(this);
    frmControl->setLayout(layoutControl);
    frmControl->setMinimumWidth(120);
    frmControl->setMaximumWidth(400);

    // -------------------------------------------------- REMOVEABLE STORAGE

    QWidget *frmStorageSection = new QWidget(this);

    m_lblSelectedLibrary = new QLabel(this);
    m_lblSelectedDrive = new QLabel(this);
    m_btnCreateVolume = new QPushButton(this);
    m_btnLoadVolume = new QPushButton(this);
    m_treeStorageDevices = new QTreeView(this);

    m_btnCreateVolume->setFixedWidth(120);
    m_btnLoadVolume->setFixedWidth(120);

    QGridLayout *layoutStorageNorth = new QGridLayout();
    layoutStorageNorth->addWidget(m_lblSelectedDrive, 0, 0, 1, 1);
    layoutStorageNorth->addWidget(m_lblSelectedLibrary, 1, 0, 1, 1);
    layoutStorageNorth->addWidget(m_btnCreateVolume, 3, 0, 1, 1);
    layoutStorageNorth->addWidget(m_btnLoadVolume, 3, 0, 1, 1);
    layoutStorageNorth->addWidget(m_treeStorageDevices, 0, 2, 4, 1);
    layoutStorageNorth->setColumnMinimumWidth(0, m_btnCreateVolume->width());
    layoutStorageNorth->setColumnMinimumWidth(1, w_separator);
    layoutStorageNorth->setRowStretch(2, 1);

    QGridLayout *layoutStorageWest = new QGridLayout();
    QGridLayout *layoutStorageEast = new QGridLayout();

    QGridLayout *layoutStorage = new QGridLayout(this);
    layoutStorage->addLayout(layoutStorageNorth, 0, 0, 1, 3);
    layoutStorage->addLayout(layoutStorageWest, 1, 0, 1, 1);
    layoutStorage->addLayout(layoutStorageEast, 1, 2, 1, 1);
    layoutStorage->setColumnMinimumWidth(0, w_split);
    layoutStorage->setColumnMinimumWidth(1, w_separator);
    layoutStorage->setColumnMinimumWidth(2, w_split);
    layoutStorage->setMargin(16);
    frmStorageSection->setLayout(layoutStorage);
    frmStorageSection->setMaximumWidth(w_section);

    // -------------------------------------------------- LIBRARY

    QWidget *frmLibrarySection = new QWidget(this);

    m_lblLibAddedTag = new QLabel(this);

    m_boxLibName = new QLineEdit(this);
    m_lblLibAdded = new QLabel(this);

    m_lblCurrentDevice = new QLabel(this);
    m_frmStorageSpace = new QWidget(this);

    QLabel *lblSyncMediaTag = new QLabel(this);
    m_treeSyncMedia = new QTreeView(this);

    QGridLayout *layoutLibraryWest = new QGridLayout();
    layoutLibraryWest->addWidget(m_lblLibAddedTag, 1, 0, 1, 1);
    layoutLibraryWest->addWidget(m_boxLibName, 0, 0, 1, 3);
    layoutLibraryWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    layoutLibraryWest->setRowStretch(2, 1);
    layoutLibraryWest->setColumnStretch(2, 1);

    QGridLayout *layoutLibraryEast = new QGridLayout();
    layoutLibraryEast->addWidget(lblSyncMediaTag, 0, 0, 1, 1);
    layoutLibraryEast->addWidget(m_treeSyncMedia, 1, 0, 1, 1);

    QGridLayout *layoutLibrarySouth = new QGridLayout();
    layoutLibrarySouth->addWidget(m_lblCurrentDevice, 0, 0, 1, 1);
    layoutLibrarySouth->addWidget(m_frmStorageSpace, 1, 0, 1, 1);
    layoutLibrarySouth->setRowMinimumHeight(1, 40);

    QGridLayout *layoutLibrary = new QGridLayout(this);
    layoutLibrary->addLayout(layoutLibraryWest, 0, 0, 1, 1);
    layoutLibrary->addLayout(layoutLibraryEast, 0, 2, 1, 1);
    layoutLibrary->addLayout(layoutLibrarySouth, 1, 0, 1, 3);
    layoutLibrary->setColumnMinimumWidth(0, w_split);
    layoutLibrary->setColumnMinimumWidth(1, w_separator);
    layoutLibrary->setColumnMinimumWidth(2, w_split);
    layoutLibrary->setMargin(16);
    frmLibrarySection->setLayout(layoutLibrary);
    frmLibrarySection->setMaximumWidth(w_section);

    // -------------------------------------------------- DISPLAY PANEl

    QWidget *frmDisplay = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QLabel *lblPanelTitle = new QLabel(this);

    QLabel *lblStorageHeader = new QLabel(this);
    QLabel *lblLibraryHeader = new QLabel(this);

    QGridLayout *layoutDisplay = new QGridLayout(this);
    layoutDisplay->addWidget(lblPanelTitle, 0, 0, 1, 1);
    layoutDisplay->addWidget(lblStorageHeader, 1, 0, 1, 1);
    layoutDisplay->addWidget(frmStorageSection, 2, 0, 1, 1);
    layoutDisplay->addWidget(lblLibraryHeader, 3, 0, 1, 1);
    layoutDisplay->addWidget(frmLibrarySection, 4, 0, 1, 1);
    layoutDisplay->setColumnMinimumWidth(0, w_section);
    layoutDisplay->setRowMinimumHeight(5, 40);
    layoutDisplay->setColumnStretch(0, 1);
    layoutDisplay->setRowStretch(6, 1);
    layoutDisplay->setMargin(20);
    frmDisplay->setLayout(layoutDisplay);

    scrollArea->setWidget(frmDisplay);
    scrollArea->setWidgetResizable(true);

    // -------------------------------------------------- MAIN PANEl

    m_frmSplitter = new QSplitter(this);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmSplitter);
    layoutMain->setMargin(0);
    setLayout(layoutMain);

    m_frmSplitter->addWidget(frmControl);
    m_frmSplitter->addWidget(scrollArea);
    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);
    m_frmSplitter->setStretchFactor(1, 1);

    // -------------------------------------------------- STATIC TEXT

    lblPanelTitle->setText("Raspberry Pi");
    lblStorageHeader->setText("Storage Devices");
    lblLibraryHeader->setText("Device Library");

    m_btnCreateVolume->setText("Create");
    m_btnLoadVolume->setText("Load");

    lblSyncMediaTag->setText("Sync Media");

    // -------------------------------------------------- OBJECT NAMES

    m_treeStorageDevices->setObjectName("PanelTree");
    m_treeStorageDevices->header()->setObjectName("PanelTreeHeader");
    m_treeStorageDevices->verticalScrollBar()->setObjectName("TreeviewScrollbar");
    m_treeStorageDevices->horizontalScrollBar()->setObjectName("TreeviewScrollbar");

    m_lblSelectedLibrary->setObjectName("PanelValue");
    m_lblSelectedDrive->setObjectName("PanelTag");
    m_btnCreateVolume->setObjectName("PanelButton");
    m_btnLoadVolume->setObjectName("PanelButton");

    m_boxLibName->setObjectName("PanelEditHidden");
    m_lblLibAdded->setObjectName("PanelValue");
    m_lblLibAddedTag->setObjectName("PanelTag");

    lblSyncMediaTag->setObjectName("PanelTag");

    m_lblCurrentDevice->setObjectName("PanelTag");
    m_frmStorageSpace->setObjectName("StorageWidget");

    scrollArea->setObjectName("PanelScrollArea");
    frmDisplay->setObjectName("PanelDevice");
    frmControl->setObjectName("PanelControl");

    lblPanelTitle->setObjectName("PanelTitle");
    lblStorageHeader->setObjectName("SectionHeader");
    lblLibraryHeader->setObjectName("SectionHeader");

    frmStorageSection->setObjectName("SectionFrame");
    frmLibrarySection->setObjectName("SectionFrame");

    setObjectName("PanelDevice");
}

void MPanelDevice::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanelDevice::showEvent(QShowEvent *event){
    m_frmSplitter->setSizes({180, width()-180});

    int totalWidth = 0;
    QList<int> colWidths = {0, 240, 85, 85, 85};

    for(int i = 0; i < colWidths.size(); i++){
        m_treeStorageDevices->setColumnWidth(i, colWidths[i]);
        totalWidth += colWidths[i];
    }

    int treeWidth = m_treeStorageDevices->width();
    m_treeStorageDevices->setColumnWidth(0, treeWidth - totalWidth - 2);

    QWidget::showEvent(event);
}

















