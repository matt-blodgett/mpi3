#include "mpanellibrary.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QTreeView>
#include <QHeaderView>


MPanelLibrary::MPanelLibrary(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_boxLibName->setText("Library Name");
    m_lblLibAdded->setText("07/07/2017");
    m_lblPlistCount->setText("2");
    m_lblSongCount->setText("20");
    m_lblArtistCount->setText("6");

    m_lblMediaSize->setText("68.01 MB");
    m_lblMediaFileCount->setText("16 Files, 10 Folders");
    m_boxMediaLoc->setText("C:/Users/Matt/Desktop");
    m_boxBackupLoc->setText("F:/Backups/Media");
}
MPanelLibrary::~MPanelLibrary(){}

void MPanelLibrary::initializeLayout(){

    // -------------------------------------------------- CONTROL PANEl

    QWidget *frmControl = new QWidget(this);

    m_btnLibImport = new QPushButton(this);
    m_btnLibExport = new QPushButton(this);
    m_btnLibReset = new QPushButton(this);
    m_btnLibOpenFile = new QPushButton(this);

    QGridLayout *layoutControl = new QGridLayout(this);
    layoutControl->addWidget(m_btnLibImport, 0, 0, 1, 1);
    layoutControl->addWidget(m_btnLibExport, 1, 0, 1, 1);
    layoutControl->addWidget(m_btnLibReset, 2, 0, 1, 1);
    layoutControl->addWidget(m_btnLibOpenFile, 3, 0, 1, 1);
    layoutControl->setRowStretch(4, 1);
    layoutControl->setVerticalSpacing(0);
    layoutControl->setHorizontalSpacing(0);
    layoutControl->setMargin(0);
    frmControl->setLayout(layoutControl);
    frmControl->setMinimumWidth(120);
    frmControl->setMaximumWidth(400);

    m_btnLibImport->setFixedWidth(120);
    m_btnLibExport->setFixedWidth(120);
    m_btnLibReset->setFixedWidth(120);
    m_btnLibOpenFile->setFixedWidth(120);

    // -------------------------------------------------- LIBRARY SECTION

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
    layoutLibraryWest->setRowStretch(2, 1);
    layoutLibraryWest->setVerticalSpacing(0);
    layoutLibraryWest->setHorizontalSpacing(0);
    layoutLibraryWest->setMargin(0);

    QLabel *lblPlaylistCountTag = new QLabel(this);
    QLabel *lblSongCountTag = new QLabel(this);
    QLabel *lblArtistCountTag = new QLabel(this);

    m_lblPlistCount = new QLabel(this);
    m_lblSongCount = new QLabel(this);
    m_lblArtistCount = new QLabel(this);

    QGridLayout *layoutLibraryEast = new QGridLayout();
    layoutLibraryEast->addWidget(lblPlaylistCountTag, 0, 0, 1, 1);
    layoutLibraryEast->addWidget(m_lblPlistCount, 0, 1, 1, 1);
    layoutLibraryEast->addWidget(lblArtistCountTag, 1, 0, 1, 1);
    layoutLibraryEast->addWidget(m_lblArtistCount, 1, 1, 1, 1);
    layoutLibraryEast->addWidget(lblSongCountTag, 2, 0, 1, 1);
    layoutLibraryEast->addWidget(m_lblSongCount, 2, 1, 1, 1);
    layoutLibraryEast->setColumnStretch(2, 1);
    layoutLibraryEast->setVerticalSpacing(0);
    layoutLibraryEast->setHorizontalSpacing(0);
    layoutLibraryEast->setMargin(0);

    QGridLayout *layoutLibrary = new QGridLayout(this);
    layoutLibrary->addLayout(layoutLibraryWest, 1, 0, 1, 1);
    layoutLibrary->addLayout(layoutLibraryEast, 1, 2, 1, 1);
    layoutLibrary->setColumnMinimumWidth(0, 345);
    layoutLibrary->setColumnMinimumWidth(1, 10);
    layoutLibrary->setColumnMinimumWidth(2, 345);
    layoutLibrary->setVerticalSpacing(0);
    layoutLibrary->setHorizontalSpacing(0);
    layoutLibrary->setMargin(12);
    frmLibrarySection->setLayout(layoutLibrary);
    frmLibrarySection->setMaximumWidth(700);

    // -------------------------------------------------- MEDIA SECTION

    QWidget *frmMediaSection = new QWidget(this);

    m_optOrganizeMedia = new QCheckBox(this);

    QLabel *lblMediaLocTag = new QLabel(this);
    m_boxMediaLoc = new QLineEdit(this);

    QLabel *lblMediaSizeTag = new QLabel(this);
    m_lblMediaSize = new QLabel(this);
    m_lblMediaFileCount = new QLabel(this);

    QGridLayout *layoutMediaWest = new QGridLayout();
    layoutMediaWest->addWidget(lblMediaLocTag, 0, 0, 1, 1);
    layoutMediaWest->addWidget(m_boxMediaLoc, 0, 1, 1, 1);
    layoutMediaWest->addWidget(lblMediaSizeTag, 1, 0, 1, 1);
    layoutMediaWest->addWidget(m_lblMediaSize, 1, 1, 1, 1);
    layoutMediaWest->addWidget(m_lblMediaFileCount, 2, 1, 1, 1);
    layoutMediaWest->setVerticalSpacing(0);
    layoutMediaWest->setHorizontalSpacing(0);
    layoutMediaWest->setMargin(0);

    m_btnSetMediaLoc = new QPushButton(this);

    QGridLayout *layoutMediaEast = new QGridLayout();
    layoutMediaEast->addWidget(m_btnSetMediaLoc, 0, 0, 1, 1);
    layoutMediaEast->setColumnStretch(1, 1);
    layoutMediaEast->setRowStretch(1, 1);
    layoutMediaEast->setVerticalSpacing(0);
    layoutMediaEast->setHorizontalSpacing(0);
    layoutMediaEast->setMargin(0);

    m_btnSetMediaLoc->setFixedWidth(100);

    QGridLayout *layoutMedia = new QGridLayout(this);
    layoutMedia->addWidget(m_optOrganizeMedia, 0, 0, 1, 3);
    layoutMedia->addLayout(layoutMediaWest, 1, 0, 1, 1);
    layoutMedia->addLayout(layoutMediaEast, 1, 2, 1, 1);
    layoutMedia->setColumnMinimumWidth(0, 345);
    layoutMedia->setColumnMinimumWidth(1, 10);
    layoutMedia->setColumnMinimumWidth(2, 345);
    layoutMedia->setVerticalSpacing(0);
    layoutMedia->setHorizontalSpacing(0);
    layoutMedia->setMargin(12);
    frmMediaSection->setLayout(layoutMedia);
    frmMediaSection->setMaximumWidth(700);

    // -------------------------------------------------- BACKUPS SECTION

    QWidget *frmBackupSection = new QWidget(this);

    m_optBackupLibrary = new QCheckBox(this);

    QLabel *lblBackupLocTag = new QLabel(this);
    m_boxBackupLoc = new QLineEdit(this);

    QLabel *lblBackupFreqTag = new QLabel(this);
    m_cbxBackupFreq = new QComboBox(this);

    QGridLayout *layoutBackupWest = new QGridLayout();
    layoutBackupWest->addWidget(lblBackupLocTag, 0, 0, 1, 1);
    layoutBackupWest->addWidget(m_boxBackupLoc, 0, 1, 1, 1);

    layoutBackupWest->addWidget(lblBackupFreqTag, 1, 0, 1, 1);
    layoutBackupWest->addWidget(m_cbxBackupFreq, 1, 1, 1, 1);
    layoutBackupWest->setRowStretch(2, 1);
    layoutBackupWest->setVerticalSpacing(0);
    layoutBackupWest->setHorizontalSpacing(0);
    layoutBackupWest->setMargin(0);

    QLabel *lblBackupHistTag = new QLabel(this);
    m_treeBackupHist = new QTreeView(this);

    m_btnSetBackupLoc = new QPushButton(this);

    QGridLayout *layoutBackupEast = new QGridLayout();
    layoutBackupEast->addWidget(m_btnSetBackupLoc, 0, 0, 1, 1);
//    layoutBackupEast->addWidget(lblBackupHistTag, 0, 0, 1, 1);
//    layoutBackupEast->addWidget(m_treeBackupHist, 1, 0, 1, 1);
    layoutBackupEast->setColumnStretch(1, 1);
    layoutBackupEast->setRowStretch(1, 1);
    layoutBackupEast->setVerticalSpacing(0);
    layoutBackupEast->setHorizontalSpacing(0);
    layoutBackupEast->setMargin(0);

    m_btnSetBackupLoc->setFixedWidth(100);

    QGridLayout *layoutBackup = new QGridLayout(this);
    layoutBackup->addWidget(m_optBackupLibrary, 0, 0, 1, 3);
    layoutBackup->addLayout(layoutBackupWest, 1, 0, 1, 1);
    layoutBackup->addLayout(layoutBackupEast, 1, 2, 1, 1);
    layoutBackup->setColumnMinimumWidth(0, 345);
    layoutBackup->setColumnMinimumWidth(1, 10);
    layoutBackup->setColumnMinimumWidth(2, 345);
    layoutBackup->setVerticalSpacing(0);
    layoutBackup->setHorizontalSpacing(0);
    layoutBackup->setMargin(12);
    frmBackupSection->setLayout(layoutBackup);
    frmBackupSection->setMaximumWidth(700);

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
    layoutDisplay->setColumnMinimumWidth(0, 700);
    layoutDisplay->setColumnStretch(0, 1);
    layoutDisplay->setRowStretch(7, 1);
    layoutDisplay->setHorizontalSpacing(0);
    layoutDisplay->setVerticalSpacing(0);
    layoutDisplay->setMargin(12);
    frmDisplay->setLayout(layoutDisplay);

    scrollArea->setWidget(frmDisplay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setLineWidth(0);

    // -------------------------------------------------- MAIN PANEl

    m_frmSplitter = new QSplitter(this);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmSplitter);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);
    layoutMain->setSpacing(0);
    layoutMain->setContentsMargins(0, 0, 0, 0);
    setLayout(layoutMain);

    m_frmSplitter->addWidget(frmControl);
    m_frmSplitter->addWidget(scrollArea);
    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);
    m_frmSplitter->setStretchFactor(1, 1);

    // -------------------------------------------------- STATIC TEXT

    lblPanelTitle->setText("Local Library");
    lblLibraryHeader->setText("Library");
    lblMediaHeader->setText("Media Files");
    lblBackupHeader->setText("Backups");

    lblLibNameTag->setText("Library Name:");
    lblLibAddedTag->setText("Date Created:");
    lblPlaylistCountTag->setText("Playlists:");
    lblSongCountTag->setText("Songs:");
    lblArtistCountTag->setText("Artists:");

    m_optOrganizeMedia->setText("Consolidate and Organize Media Files");
    lblMediaLocTag->setText("Media Folder:");
    m_btnSetMediaLoc->setText("Change");
    lblMediaSizeTag->setText("Size on Disk:");

    m_optBackupLibrary->setText("Backup Library Automatically");
    lblBackupLocTag->setText("Backup Folder:");
    m_btnSetBackupLoc->setText("Change");
    lblBackupFreqTag->setText("Backup Frequency:");
    lblBackupHistTag->setText("Backup History");

    m_btnLibImport->setText("Import");
    m_btnLibExport->setText("Export");
    m_btnLibReset->setText("Reset");
    m_btnLibOpenFile->setText("Show In Explorer");

    // -------------------------------------------------- OBJECT NAMES

    m_boxLibName->setObjectName("PanelEditHidden");
    m_lblLibAdded->setObjectName("PanelValue");
    m_lblPlistCount->setObjectName("PanelValue");
    m_lblArtistCount->setObjectName("PanelValue");
    m_lblSongCount->setObjectName("PanelValue");
    lblLibNameTag->setObjectName("PanelTag");
    lblLibAddedTag->setObjectName("PanelTag");
    lblPlaylistCountTag->setObjectName("PanelTag");
    lblSongCountTag->setObjectName("PanelTag");
    lblArtistCountTag->setObjectName("PanelTag");

    m_optOrganizeMedia->setObjectName("PanelCheck");
    m_boxMediaLoc->setObjectName("PanelEdit");
    lblMediaLocTag->setObjectName("PanelTag");
    lblMediaSizeTag->setObjectName("PanelTag");
    m_lblMediaSize->setObjectName("PanelValue");
    m_lblMediaFileCount->setObjectName("PanelValue");

    m_optBackupLibrary->setObjectName("PanelCheck");
    m_treeBackupHist->setObjectName("PanelTree");
    m_treeBackupHist->header()->setObjectName("PanelTreeHeader");
    lblBackupLocTag->setObjectName("PanelTag");
    m_btnSetBackupLoc->setObjectName("PanelButton");
    lblBackupFreqTag->setObjectName("PanelTag");
    lblBackupHistTag->setObjectName("PanelTag");
    m_cbxBackupFreq->setObjectName("PanelCombo");
    m_boxBackupLoc->setObjectName("PanelEdit");

    m_btnLibImport->setObjectName("PanelButton");
    m_btnLibExport->setObjectName("PanelButton");
    m_btnLibReset->setObjectName("PanelButton");
    m_btnLibOpenFile->setObjectName("PanelButton");

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













