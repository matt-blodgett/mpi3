#include "mpanellibrary.h"
#include "core/mmedialibrary.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>

#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTreeView>
#include <QHeaderView>
#include <QScrollBar>
#include <QLabel>


#include <QDebug>


MPanelLibrary::MPanelLibrary(QWidget *parent) : QWidget(parent){
    initializeLayout();

//    m_lblMediaSize->setText("68.01 MB");
//    m_lblMediaFileCount->setText("16 Folders, 76 Files");

    QStringList cbxItems;
    cbxItems << "Continuous";
    cbxItems << "Daily";
    cbxItems << "Weekly";
    cbxItems << "Monthly";

    m_cbxBackupFreq->addItems(cbxItems);

    m_cbxBackupFreq->setEditable(false);

    connect(m_optCopyMedia, &QCheckBox::toggled, this, &MPanelLibrary::allowCopyMedia);
    connect(m_optOrganizeMedia, &QCheckBox::toggled, this, &MPanelLibrary::allowOrganizeMedia);
    connect(m_optBackupLibrary, &QCheckBox::toggled, this, &MPanelLibrary::allowAutoBackups);

    connect(m_btnLibImport, &QPushButton::released, this, &MPanelLibrary::libImport);
    connect(m_btnLibExport, &QPushButton::released, this, &MPanelLibrary::libExport);
    connect(m_btnSetLibPath, &QPushButton::released, this, &MPanelLibrary::libSetSavePath);
    connect(m_btnSetMediaLoc, &QPushButton::released, this, &MPanelLibrary::libSetMediaPath);
    connect(m_btnSetBackupLoc, &QPushButton::released, this, &MPanelLibrary::libSetBackupPath);
}
MPanelLibrary::~MPanelLibrary(){}

void MPanelLibrary::allowCopyMedia(bool allow){
    m_optCopyMedia->setChecked(allow);
    m_optOrganizeMedia->setChecked(false);

    m_optOrganizeMedia->setDisabled(!allow);
    m_btnSetMediaLoc->setDisabled(!allow);
    m_boxMediaLoc->setDisabled(!allow);

    m_lblMediaSize->setVisible(allow);
    m_lblMediaFileCount->setVisible(allow);
}
void MPanelLibrary::allowOrganizeMedia(bool allow){
    m_optOrganizeMedia->setChecked(allow);
}
void MPanelLibrary::allowAutoBackups(bool allow){
    m_optBackupLibrary->setChecked(allow);
    m_cbxBackupFreq->setDisabled(!allow);
}

bool MPanelLibrary::valCopyMedia() const {
    return m_optCopyMedia->isChecked();
}
bool MPanelLibrary::valOrganizeMedia() const {
    return m_optOrganizeMedia->isChecked();
}
bool MPanelLibrary::valAutoBackups() const {
    return m_optBackupLibrary->isChecked();
}

void MPanelLibrary::setLibrary(MMediaLibrary *library){
    m_mediaLibrary = library;

    m_boxLibName->setText(m_mediaLibrary->name());
    m_lblLibAdded->setText(m_mediaLibrary->added());

    m_boxLibPath->setText(m_mediaLibrary->savePath());
    m_boxMediaLoc->setText(m_mediaLibrary->mediaPath());
    m_boxBackupLoc->setText(m_mediaLibrary->backupPath());
}

void MPanelLibrary::initializeLayout(){

    int w_split = 400;
    int w_separator = 10;
    int w_section = (w_split * 2) + w_separator;

    // -------------------------------------------------- CONTROL PANEl

    QWidget *frmControl = new QWidget(this);

    QGridLayout *layoutControl = new QGridLayout(this);
    frmControl->setLayout(layoutControl);
    frmControl->setMinimumWidth(120);
    frmControl->setMaximumWidth(400);

    // -------------------------------------------------- LIBRARY SECTION

    QWidget *frmLibrarySection = new QWidget(this);

    m_boxLibName = new QLineEdit(this);
    m_lblLibAdded = new QLabel(this);

    QLabel *lblLibAddedTag = new QLabel(this);

    QGridLayout *layoutLibraryWest = new QGridLayout();
    layoutLibraryWest->addWidget(m_boxLibName, 0, 0, 1, 3);
    layoutLibraryWest->addWidget(lblLibAddedTag, 1, 0, 1, 1);
    layoutLibraryWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    layoutLibraryWest->setRowStretch(2, 1);
    layoutLibraryWest->setColumnStretch(2, 1);

    m_btnLibImport = new QPushButton(this);
    m_btnLibExport = new QPushButton(this);

    QGridLayout *layoutLibraryEast = new QGridLayout();
    layoutLibraryEast->addWidget(m_btnLibImport, 0, 1, 1, 1);
    layoutLibraryEast->addWidget(m_btnLibExport, 1, 1, 1, 1);
    layoutLibraryEast->setColumnStretch(0, 1);
    layoutLibraryEast->setRowStretch(1, 1);

    m_btnLibImport->setFixedWidth(120);
    m_btnLibExport->setFixedWidth(120);

    QLabel *lblLibPathTag = new QLabel(this);
    m_boxLibPath = new QLineEdit(this);
    m_btnSetLibPath = new QPushButton(this);

    QGridLayout *layoutLibrarySouth = new QGridLayout();
    layoutLibrarySouth->addWidget(lblLibPathTag, 0, 0, 1, 1);
    layoutLibrarySouth->addWidget(m_boxLibPath, 1, 0, 1, 1);
    layoutLibrarySouth->addWidget(m_btnSetLibPath, 1, 1, 1, 1);

    m_btnSetLibPath->setFixedWidth(120);

    QGridLayout *layoutLibrary = new QGridLayout(this);
    layoutLibrary->addLayout(layoutLibraryWest, 1, 0, 1, 1);
    layoutLibrary->addLayout(layoutLibraryEast, 1, 2, 1, 1);
    layoutLibrary->addLayout(layoutLibrarySouth, 2, 0, 1, 3);
    layoutLibrary->setRowMinimumHeight(1, w_separator);
    layoutLibrary->setColumnMinimumWidth(0, w_split);
    layoutLibrary->setColumnMinimumWidth(1, w_separator);
    layoutLibrary->setColumnMinimumWidth(2, w_split);
    layoutLibrary->setMargin(16);
    frmLibrarySection->setLayout(layoutLibrary);
    frmLibrarySection->setMaximumWidth(w_section);

    // -------------------------------------------------- MEDIA SECTION

    QWidget *frmMediaSection = new QWidget(this);

    m_optCopyMedia = new QCheckBox(this);
    m_optOrganizeMedia = new QCheckBox(this);

    QGridLayout *layoutMediaWest = new QGridLayout();
    layoutMediaWest->addWidget(m_optCopyMedia, 0, 0, 1, 1);
    layoutMediaWest->addWidget(m_optOrganizeMedia, 1, 0, 1, 1);
    layoutMediaWest->setColumnStretch(1, 1);
    layoutMediaWest->setRowStretch(2, 1);

    QLabel *lblMediaSizeTag = new QLabel(this);
    m_lblMediaSize = new QLabel(this);
    m_lblMediaFileCount = new QLabel(this);

    QGridLayout *layoutMediaEast = new QGridLayout();
    layoutMediaEast->addWidget(lblMediaSizeTag, 0, 0, 1, 1);
    layoutMediaEast->addWidget(m_lblMediaSize, 0, 1, 1, 1);
    layoutMediaEast->addWidget(m_lblMediaFileCount, 0, 2, 1, 1);
    layoutMediaEast->setColumnStretch(3, 1);
    layoutMediaEast->setRowStretch(1, 1);

    QLabel *lblMediaLocTag = new QLabel(this);
    m_boxMediaLoc = new QLineEdit(this);
    m_btnSetMediaLoc = new QPushButton(this);

    QGridLayout *layoutMediaSouth = new QGridLayout();
    layoutMediaSouth->addWidget(lblMediaLocTag, 0, 0, 1, 1);
    layoutMediaSouth->addWidget(m_boxMediaLoc, 1, 0, 1, 1);
    layoutMediaSouth->addWidget(m_btnSetMediaLoc, 1, 1, 1, 1);

    m_btnSetMediaLoc->setFixedWidth(100);

    QGridLayout *layoutMedia = new QGridLayout(this);
    layoutMedia->addLayout(layoutMediaWest, 0, 0, 1, 1);
    layoutMedia->addLayout(layoutMediaEast, 0, 2, 1, 1);
    layoutMedia->addLayout(layoutMediaSouth, 2, 0, 1, 3);
    layoutMedia->setRowMinimumHeight(1, w_separator);
    layoutMedia->setColumnMinimumWidth(0, w_split);
    layoutMedia->setColumnMinimumWidth(1, w_separator);
    layoutMedia->setColumnMinimumWidth(2, w_split);
    layoutMedia->setMargin(16);
    frmMediaSection->setLayout(layoutMedia);
    frmMediaSection->setMaximumWidth(w_section);

    // -------------------------------------------------- BACKUPS SECTION

    QWidget *frmBackupSection = new QWidget(this);

    QLabel *lblBackupHistTag = new QLabel(this);
    m_treeBackupHist = new QTreeView(this);

    m_optBackupLibrary = new QCheckBox(this);
    QLabel *lblBackupFreqTag = new QLabel(this);
    m_cbxBackupFreq = new QComboBox(this);

    QGridLayout *layoutBackupWest = new QGridLayout();
    layoutBackupWest->addWidget(m_optBackupLibrary, 0, 0, 1, 3);
    layoutBackupWest->addWidget(lblBackupFreqTag, 2, 0, 1, 1);
    layoutBackupWest->addWidget(m_cbxBackupFreq, 3, 0, 1, 1);
    layoutBackupWest->setRowMinimumHeight(1, w_separator);

    m_btnBackupManual = new QPushButton(this);
    m_btnBackupRestore = new QPushButton(this);

    QGridLayout *layoutBackupEast = new QGridLayout();
    layoutBackupEast->addWidget(m_btnBackupManual, 0, 1, 1, 1);
    layoutBackupEast->addWidget(m_btnBackupRestore, 1, 1, 1, 1);
    layoutBackupEast->setColumnStretch(0, 1);
    layoutBackupEast->setRowStretch(2, 1);

    m_btnBackupManual->setFixedWidth(120);
    m_btnBackupRestore->setFixedWidth(120);

    QLabel *lblBackupLocTag = new QLabel(this);
    m_boxBackupLoc = new QLineEdit(this);
    m_btnSetBackupLoc = new QPushButton(this);

    QGridLayout *layoutBackupSouth = new QGridLayout();
    layoutBackupSouth->addWidget(lblBackupLocTag, 0, 0, 1, 1);
    layoutBackupSouth->addWidget(m_boxBackupLoc, 1, 0, 1, 1);
    layoutBackupSouth->addWidget(m_btnSetBackupLoc, 1, 1, 1, 1);

    m_btnSetBackupLoc->setFixedWidth(120);

    QGridLayout *layoutBackup = new QGridLayout(this);
    layoutBackup->addLayout(layoutBackupWest, 0, 0, 1, 1);
    layoutBackup->addLayout(layoutBackupEast, 0, 2, 1, 1);
    layoutBackup->addLayout(layoutBackupSouth, 2, 0, 1, 3);
    layoutBackup->setRowMinimumHeight(1, w_separator);
    layoutBackup->setColumnMinimumWidth(0, w_split);
    layoutBackup->setColumnMinimumWidth(1, w_separator);
    layoutBackup->setColumnMinimumWidth(2, w_split);
    layoutBackup->setMargin(16);
    frmBackupSection->setLayout(layoutBackup);
    frmBackupSection->setMaximumWidth(w_section);

    // -------------------------------------------------- DISPLAY PANEl

    QWidget *frmDisplay = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    QLabel *lblPanelTitle = new QLabel(this);

    QLabel *lblLibraryHeader = new QLabel(this);
    QLabel *lblMediaHeader = new QLabel(this);
    QLabel *lblBackupHeader = new QLabel(this);

    QGridLayout *layoutDisplay = new QGridLayout(this);
    layoutDisplay->addWidget(lblPanelTitle, 0, 0, 1, 1);
    layoutDisplay->addWidget(lblLibraryHeader, 1, 0, 1, 1);
    layoutDisplay->addWidget(frmLibrarySection, 2, 0, 1, 1);
    layoutDisplay->addWidget(lblMediaHeader, 3, 0, 1, 1);
    layoutDisplay->addWidget(frmMediaSection, 4, 0, 1, 1);
    layoutDisplay->addWidget(lblBackupHeader, 5, 0, 1, 1);
    layoutDisplay->addWidget(frmBackupSection, 6, 0, 1, 1);
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

    // -------------------------------------------------- STATIC CONNECTIONS

    connect(m_optCopyMedia, &QCheckBox::toggled, [=](){lblMediaLocTag->setDisabled(!m_optCopyMedia->isChecked());});
    connect(m_optCopyMedia, &QCheckBox::toggled, [=](){lblMediaSizeTag->setDisabled(!m_optCopyMedia->isChecked());});

    connect(m_optBackupLibrary, &QCheckBox::toggled, [=](){lblBackupFreqTag->setDisabled(!m_optBackupLibrary->isChecked());});

    // -------------------------------------------------- STATIC TEXT

    lblPanelTitle->setText("Local Library");
    lblLibraryHeader->setText("Library");
    lblMediaHeader->setText("Media Files");
    lblBackupHeader->setText("Backups");
    m_btnSetLibPath->setText("Change");

    lblLibAddedTag->setText("Date Created:");
    lblLibPathTag->setText("File Location:");

    m_optCopyMedia->setText("Make Local Copies of Files");
    m_optOrganizeMedia->setText("Keep Files Organized");
    lblMediaSizeTag->setText("Size on Disk:");
    lblMediaLocTag->setText("Media Folder:");
    m_btnSetMediaLoc->setText("Change");

    m_optBackupLibrary->setText("Backup Library Automatically");
    lblBackupLocTag->setText("Backup Folder:");
    lblBackupFreqTag->setText("Backup Frequency:");
    lblBackupHistTag->setText("Backup History");
    m_btnBackupRestore->setText("Restore Backup");
    m_btnBackupManual->setText("Backup Now");
    m_btnSetBackupLoc->setText("Change");

    m_btnLibImport->setText("Import");
    m_btnLibExport->setText("Export");

    // -------------------------------------------------- OBJECT NAMES

    m_boxLibName->setObjectName("PanelEditHidden");
    m_lblLibAdded->setObjectName("PanelValue");
    m_boxLibPath->setObjectName("PanelEdit");
    m_btnSetLibPath->setObjectName("PanelButton");
    lblLibAddedTag->setObjectName("PanelTag");
    lblLibPathTag->setObjectName("PanelTag");

    m_optCopyMedia->setObjectName("PanelCheck");
    m_optOrganizeMedia->setObjectName("PanelCheck");
    m_boxMediaLoc->setObjectName("PanelEdit");
    lblMediaLocTag->setObjectName("PanelTag");
    lblMediaSizeTag->setObjectName("PanelTag");
    m_lblMediaSize->setObjectName("PanelTag");
    m_lblMediaFileCount->setObjectName("PanelValue");
    m_btnSetMediaLoc->setObjectName("PanelButton");

    m_treeBackupHist->setObjectName("PanelTree");
    m_treeBackupHist->header()->setObjectName("PanelTreeHeader");

    m_optBackupLibrary->setObjectName("PanelCheck");
    lblBackupLocTag->setObjectName("PanelTag");
    m_btnSetBackupLoc->setObjectName("PanelButton");
    lblBackupFreqTag->setObjectName("PanelTag");
    lblBackupHistTag->setObjectName("PanelTag");
    m_cbxBackupFreq->setObjectName("PanelCombo");
    m_boxBackupLoc->setObjectName("PanelEdit");
    m_btnBackupManual->setObjectName("PanelButton");
    m_btnBackupRestore->setObjectName("PanelButton");

    m_btnLibImport->setObjectName("PanelButton");
    m_btnLibExport->setObjectName("PanelButton");

    scrollArea->setObjectName("PanelScrollArea");
    frmDisplay->setObjectName("PanelLibrary");
    frmControl->setObjectName("PanelControl");

    lblPanelTitle->setObjectName("PanelTitle");
    lblLibraryHeader->setObjectName("SectionHeader");
    lblMediaHeader->setObjectName("SectionHeader");
    lblBackupHeader->setObjectName("SectionHeader");

    frmLibrarySection->setObjectName("SectionFrame");
    frmMediaSection->setObjectName("SectionFrame");
    frmBackupSection->setObjectName("SectionFrame");

    setObjectName("PanelLibrary");
}

void MPanelLibrary::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanelLibrary::showEvent(QShowEvent *event){
    m_frmSplitter->setSizes({180, width()-180});
    QWidget::showEvent(event);
}







