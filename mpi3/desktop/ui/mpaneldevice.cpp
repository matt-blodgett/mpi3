#include "mpaneldevice.h"


#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>

#include <QTreeView>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

//#include <QFileSystemModel>
#include <QHeaderView>
#include <QScrollBar>



MPanelDevice::MPanelDevice(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_treeStorageDevices->setFocusPolicy(Qt::NoFocus);

    m_lblCurrentDevice->setText("RASPI (E:)");

    m_treeStorageDevices->setIndentation(0);
    m_treeStorageDevices->setItemsExpandable(false);
    m_treeStorageDevices->setSortingEnabled(true);
    m_treeStorageDevices->setAlternatingRowColors(true);

    m_boxLibName->setText("Raspi Test Library");
    m_lblLibAdded->setText("05/05/2018");

//    QFileSystemModel *fsModel = new QFileSystemModel(this);
//    fsModel->setRootPath("C:/Users/Matt/Desktop");
//    m_treeStorageDevices->setModel(fsModel);

}
MPanelDevice::~MPanelDevice(){}

void MPanelDevice::setModel(QAbstractItemModel *lib){
    m_treeStorageDevices->setModel(lib);
}

void MPanelDevice::initializeLayout(){

    int w_split = 300;
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

    m_treeStorageDevices = new QTreeView(this);

    QGridLayout *layoutStorageWest = new QGridLayout();
    layoutStorageWest->addWidget(m_treeStorageDevices, 1, 0, 1, 1);

    m_treeStorageDevices->setFixedHeight(200);

    QLabel *lblVolumeTag = new QLabel(this);
    QLabel *lblCreateVolumeTag = new QLabel(this);
    QLabel *lblLoadVolumeTag = new QLabel(this);
    m_btnCreateVolume = new QPushButton(this);
    m_btnLoadVolume = new QPushButton(this);

    QGridLayout *layoutStorageEast = new QGridLayout();
    layoutStorageEast->addWidget(lblVolumeTag, 0, 0, 1, 2);
    layoutStorageEast->addWidget(m_btnCreateVolume, 1, 0, 1, 1);
    layoutStorageEast->addWidget(lblCreateVolumeTag, 1, 1, 1, 1);
    layoutStorageEast->addWidget(m_btnLoadVolume, 2, 0, 1, 1);
    layoutStorageEast->addWidget(lblLoadVolumeTag, 2, 1, 1, 1);
    layoutStorageEast->setRowStretch(3, 1);

    m_btnCreateVolume->setFixedWidth(120);
    m_btnLoadVolume->setFixedWidth(120);

    QGridLayout *layoutStorage = new QGridLayout(this);
    layoutStorage->addLayout(layoutStorageWest, 0, 0, 1, 1);
    layoutStorage->addLayout(layoutStorageEast, 0, 2, 1, 1);
    layoutStorage->setColumnMinimumWidth(0, w_split);
    layoutStorage->setColumnMinimumWidth(1, w_separator);
    layoutStorage->setColumnMinimumWidth(2, w_split);
    layoutStorage->setMargin(16);
    frmStorageSection->setLayout(layoutStorage);
    frmStorageSection->setMaximumWidth(w_section);

    // -------------------------------------------------- MEDIA

    QWidget *frmLibrarySection = new QWidget(this);

    m_boxLibName = new QLineEdit(this);
    m_lblLibAdded = new QLabel(this);

    QLabel *lblLibNameTag = new QLabel(this);
    QLabel *lblLibAddedTag = new QLabel(this);

    QGridLayout *layoutLibraryWest = new QGridLayout();
    layoutLibraryWest->addWidget(lblLibNameTag, 0, 0, 1, 1);
    layoutLibraryWest->addWidget(m_boxLibName, 0, 1, 1, 1);
    layoutLibraryWest->addWidget(lblLibAddedTag, 1, 0, 1, 1);
    layoutLibraryWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);

    QGridLayout *layoutLibraryEast = new QGridLayout();

    QGridLayout *layoutLibrary = new QGridLayout(this);
    layoutLibrary->addLayout(layoutLibraryWest, 0, 0, 1, 1);
    layoutLibrary->addLayout(layoutLibraryEast, 0, 2, 1, 1);
    layoutLibrary->setColumnMinimumWidth(0, w_split);
    layoutLibrary->setColumnMinimumWidth(1, w_separator);
    layoutLibrary->setColumnMinimumWidth(2, w_split);
    layoutLibrary->setMargin(16);
    frmLibrarySection->setLayout(layoutLibrary);
    frmLibrarySection->setMaximumWidth(w_section);

    // -------------------------------------------------- MEDIA

    QWidget *frmMediaSection = new QWidget(this);

    m_lblCurrentDevice = new QLabel(this);

    QGridLayout *layoutMediaWest = new QGridLayout();
    layoutMediaWest->addWidget(m_lblCurrentDevice, 0, 0, 1, 1);

    QGridLayout *layoutMediaEast = new QGridLayout();
    m_frmStorageSpace = new QWidget(this);

    QGridLayout *layoutMedia = new QGridLayout(this);
    layoutMedia->addLayout(layoutMediaWest, 0, 0, 1, 1);
    layoutMedia->addLayout(layoutMediaEast, 0, 2, 1, 1);
    layoutMedia->addWidget(m_frmStorageSpace, 1, 0, 1, 3);
    layoutMedia->setColumnMinimumWidth(0, w_split);
    layoutMedia->setColumnMinimumWidth(1, w_separator);
    layoutMedia->setColumnMinimumWidth(2, w_split);
    layoutMedia->setRowMinimumHeight(0, 60);
    layoutMedia->setRowMinimumHeight(1, 60);
    layoutMedia->setMargin(16);
    frmMediaSection->setLayout(layoutMedia);
    frmMediaSection->setMaximumWidth(w_section);

    // -------------------------------------------------- DISPLAY PANEl

    QWidget *frmDisplay = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QLabel *lblPanelTitle = new QLabel(this);

    QLabel *lblStorageHeader = new QLabel(this);
    QLabel *lblLibraryHeader = new QLabel(this);
    QLabel *lblMediaHeader = new QLabel(this);

    QGridLayout *layoutDisplay = new QGridLayout(this);
    layoutDisplay->addWidget(lblPanelTitle, 0, 0, 1, 1);
    layoutDisplay->addWidget(lblStorageHeader, 1, 0, 1, 1);
    layoutDisplay->addWidget(frmStorageSection, 2, 0, 1, 1);
    layoutDisplay->addWidget(lblLibraryHeader, 3, 0, 1, 1);
    layoutDisplay->addWidget(frmLibrarySection, 4, 0, 1, 1);
    layoutDisplay->addWidget(lblMediaHeader, 5, 0, 1, 1);
    layoutDisplay->addWidget(frmMediaSection, 6, 0, 1, 1);
    layoutDisplay->setColumnMinimumWidth(0, w_section);
    layoutDisplay->setRowMinimumHeight(7, 40);
    layoutDisplay->setColumnStretch(0, 1);
    layoutDisplay->setRowStretch(8, 1);
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
    lblLibraryHeader->setText("Library");
    lblMediaHeader->setText("Synced Media");

    lblVolumeTag->setText("Raspberry Pi Media Volumes:");
    m_btnCreateVolume->setText("Create");
    m_btnLoadVolume->setText("Load");
    lblCreateVolumeTag->setText("Create Volume");
    lblLoadVolumeTag->setText("Load Volume");

    lblLibNameTag->setText("Library Name:");
    lblLibAddedTag->setText("Date Created:");

    // -------------------------------------------------- OBJECT NAMES

    m_treeStorageDevices->setObjectName("PanelTree");
    m_treeStorageDevices->header()->setObjectName("PanelTreeHeader");
    m_treeStorageDevices->verticalScrollBar()->setObjectName("TreeviewScrollbar");
    m_treeStorageDevices->horizontalScrollBar()->setObjectName("TreeviewScrollbar");

    lblVolumeTag->setObjectName("PanelTag");
    lblCreateVolumeTag->setObjectName("PanelValue");
    lblLoadVolumeTag->setObjectName("PanelValue");
    m_btnCreateVolume->setObjectName("PanelButton");
    m_btnLoadVolume->setObjectName("PanelButton");

    m_boxLibName->setObjectName("PanelEditHidden");
    m_lblLibAdded->setObjectName("PanelValue");
    lblLibNameTag->setObjectName("PanelTag");
    lblLibAddedTag->setObjectName("PanelTag");

    m_lblCurrentDevice->setObjectName("PanelTag");
    m_frmStorageSpace->setObjectName("StorageWidget");

    scrollArea->setObjectName("PanelScrollArea");
    frmDisplay->setObjectName("PanelDevice");
    frmControl->setObjectName("PanelControl");

    lblPanelTitle->setObjectName("PanelTitle");
    lblStorageHeader->setObjectName("SectionHeader");
    lblLibraryHeader->setObjectName("SectionHeader");
    lblMediaHeader->setObjectName("SectionHeader");

    frmStorageSection->setObjectName("SectionFrame");
    frmLibrarySection->setObjectName("SectionFrame");
    frmMediaSection->setObjectName("SectionFrame");

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
    QWidget::showEvent(event);
}
