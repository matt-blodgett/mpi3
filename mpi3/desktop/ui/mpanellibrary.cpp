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


MPanelLibrary::MPanelLibrary(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_boxLibName->setText("Library Name");
    m_lblLibAdded->setText("07/07/2017");
    m_lblPlistCount->setText("2");
    m_lblSongCount->setText("20");
    m_lblArtistCount->setText("6");

    m_boxMediaLoc->setText("C:/Users/Matt/Desktop");

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

    // -------------------------------------------------- PROFILE SECTION

    QWidget *frmProfileSection = new QWidget(this);

    m_boxLibName = new QLineEdit(this);
    m_lblLibAdded = new QLabel(this);

    QLabel *lblLibNameTag = new QLabel(this);
    QLabel *lblLibAddedTag = new QLabel(this);

    QGridLayout *layoutProfileWest = new QGridLayout();
    layoutProfileWest->addWidget(lblLibNameTag, 0, 0, 1, 1);
    layoutProfileWest->addWidget(m_boxLibName, 0, 1, 1, 1);
    layoutProfileWest->addWidget(lblLibAddedTag, 1, 0, 1, 1);
    layoutProfileWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    layoutProfileWest->setRowStretch(2, 1);
    layoutProfileWest->setVerticalSpacing(0);
    layoutProfileWest->setHorizontalSpacing(0);
    layoutProfileWest->setMargin(0);

    QLabel *lblPlaylistCountTag = new QLabel(this);
    QLabel *lblSongCountTag = new QLabel(this);
    QLabel *lblArtistCountTag = new QLabel(this);

    m_lblPlistCount = new QLabel(this);
    m_lblSongCount = new QLabel(this);
    m_lblArtistCount = new QLabel(this);

    QGridLayout *layoutProfileEast = new QGridLayout();
    layoutProfileEast->addWidget(lblPlaylistCountTag, 0, 0, 1, 1);
    layoutProfileEast->addWidget(m_lblPlistCount, 0, 1, 1, 1);
    layoutProfileEast->addWidget(lblArtistCountTag, 1, 0, 1, 1);
    layoutProfileEast->addWidget(m_lblArtistCount, 1, 1, 1, 1);
    layoutProfileEast->addWidget(lblSongCountTag, 2, 0, 1, 1);
    layoutProfileEast->addWidget(m_lblSongCount, 2, 1, 1, 1);
    layoutProfileEast->setRowStretch(3, 1);
    layoutProfileEast->setVerticalSpacing(0);
    layoutProfileEast->setHorizontalSpacing(0);
    layoutProfileEast->setMargin(0);

    QGridLayout *layoutProfile = new QGridLayout(this);
    layoutProfile->addLayout(layoutProfileWest, 0, 0, 1, 1);
    layoutProfile->addLayout(layoutProfileEast, 0, 2, 1, 1);
    layoutProfile->setColumnMinimumWidth(0, 295);
    layoutProfile->setColumnMinimumWidth(1, 10);
    layoutProfile->setColumnMinimumWidth(2, 295);
    layoutProfile->setVerticalSpacing(0);
    layoutProfile->setHorizontalSpacing(0);
    layoutProfile->setMargin(0);
    frmProfileSection->setLayout(layoutProfile);
    frmProfileSection->setMaximumWidth(600);

    // -------------------------------------------------- MEDIA SECTION

    QWidget *frmMediaSection = new QWidget(this);

    QWidget *frmMediaEnable = new QWidget(this);
    m_optOrganizeMedia = new QCheckBox(this);

    QGridLayout *layoutMediaNorth = new QGridLayout(this);
    layoutMediaNorth->addWidget(m_optOrganizeMedia);
    layoutMediaNorth->setVerticalSpacing(0);
    layoutMediaNorth->setHorizontalSpacing(0);
    layoutMediaNorth->setMargin(0);
    frmMediaEnable->setLayout(layoutMediaNorth);

    QLabel *lblMediaLocTag = new QLabel(this);
    m_btnSetMediaLoc = new QPushButton(this);
    m_boxMediaLoc = new QLineEdit(this);

    QGridLayout *layoutMediaWest = new QGridLayout();
    layoutMediaWest->addWidget(lblMediaLocTag, 0, 0, 1, 1);
    layoutMediaWest->addWidget(m_boxMediaLoc, 0, 1, 1, 1);
    layoutMediaWest->addWidget(m_btnSetMediaLoc, 0, 2, 1, 1);
    layoutMediaWest->setVerticalSpacing(0);
    layoutMediaWest->setHorizontalSpacing(0);
    layoutMediaWest->setMargin(0);

    QLabel *lblMediaSizeTag = new QLabel(this);
    m_lblMediaSize = new QLabel(this);

    QGridLayout *layoutMediaEast = new QGridLayout();
    layoutMediaEast->addWidget(lblMediaSizeTag, 0, 0, 1, 1);
    layoutMediaEast->addWidget(m_lblMediaSize, 0, 1, 1, 1);
    layoutMediaEast->setVerticalSpacing(0);
    layoutMediaEast->setHorizontalSpacing(0);
    layoutMediaEast->setMargin(0);

    QGridLayout *layoutMedia = new QGridLayout(this);
    layoutMedia->addWidget(frmMediaEnable, 0, 0, 1, 3);
    layoutMedia->addLayout(layoutMediaWest, 1, 0, 1, 1);
    layoutMedia->addLayout(layoutMediaEast, 1, 2, 1, 1);
    layoutMedia->setColumnMinimumWidth(0, 295);
    layoutMedia->setColumnMinimumWidth(1, 10);
    layoutMedia->setColumnMinimumWidth(2, 295);
    layoutMedia->setRowStretch(2, 1);
    layoutMedia->setVerticalSpacing(0);
    layoutMedia->setHorizontalSpacing(0);
    layoutMedia->setMargin(0);
    frmMediaSection->setLayout(layoutMedia);
    frmMediaSection->setMaximumWidth(600);

    // -------------------------------------------------- BACKUPS SECTION

    QWidget *frmBackupSection = new QWidget(this);

    QWidget *frmBackupEnable = new QWidget(this);
    m_optBackupLibrary = new QCheckBox(this);

    QGridLayout *layoutBackupNorth = new QGridLayout(this);
    layoutBackupNorth->addWidget(m_optBackupLibrary);
    layoutBackupNorth->setVerticalSpacing(0);
    layoutBackupNorth->setHorizontalSpacing(0);
    layoutBackupNorth->setMargin(0);
    frmBackupEnable->setLayout(layoutBackupNorth);

    QLabel *lblBackupLocTag = new QLabel(this);
    m_btnSetBackupLoc = new QPushButton(this);
    m_boxBackupLoc = new QLineEdit(this);

    QLabel *lblBackupFreqTag = new QLabel(this);
    m_cbxBackupFreq = new QComboBox(this);

    QGridLayout *layoutBackupWest = new QGridLayout();
    layoutBackupWest->addWidget(lblBackupLocTag, 0, 0, 1, 1);
    layoutBackupWest->addWidget(m_boxBackupLoc, 0, 1, 1, 1);
    layoutBackupWest->addWidget(m_btnSetBackupLoc, 0, 2, 1, 1);
    layoutBackupWest->addWidget(lblBackupFreqTag, 1, 0, 1, 1);
    layoutBackupWest->addWidget(m_cbxBackupFreq, 1, 1, 1, 1);
    layoutBackupWest->setRowStretch(2, 1);
    layoutBackupWest->setVerticalSpacing(0);
    layoutBackupWest->setHorizontalSpacing(0);
    layoutBackupWest->setMargin(0);

    QLabel *lblBackupHistTag = new QLabel(this);
    m_treeBackupHist = new QTreeView(this);

    QGridLayout *layoutBackupEast = new QGridLayout();
    layoutBackupEast->addWidget(lblBackupHistTag, 0, 0, 1, 1);
    layoutBackupEast->addWidget(m_treeBackupHist, 1, 0, 1, 1);
    layoutBackupEast->setRowStretch(1, 1);
    layoutBackupEast->setVerticalSpacing(0);
    layoutBackupEast->setHorizontalSpacing(0);
    layoutBackupEast->setMargin(0);

    QGridLayout *layoutBackup = new QGridLayout(this);
    layoutBackup->addWidget(frmBackupEnable, 0, 0, 1, 3);
    layoutBackup->addLayout(layoutBackupWest, 1, 0, 1, 1);
    layoutBackup->addLayout(layoutBackupEast, 1, 2, 1, 1);
    layoutBackup->setColumnMinimumWidth(0, 295);
    layoutBackup->setColumnMinimumWidth(1, 10);
    layoutBackup->setColumnMinimumWidth(2, 295);
    layoutBackup->setRowStretch(2, 1);
    layoutBackup->setVerticalSpacing(0);
    layoutBackup->setHorizontalSpacing(0);
    layoutBackup->setMargin(0);
    frmBackupSection->setLayout(layoutBackup);
    frmBackupSection->setMaximumWidth(600);

    // -------------------------------------------------- DISPLAY PANEl

    QWidget *frmDisplay = new QWidget(this);
    QScrollArea *scrollArea = new QScrollArea(this);

    QLabel *lblPanelTitle = new QLabel(this);
    QLabel *lblPofileHeader = new QLabel(this);
    QLabel *lblMediaHeader = new QLabel(this);
    QLabel *lblBackupHeader = new QLabel(this);

    QGridLayout *layoutDisplay = new QGridLayout(this);
    layoutDisplay->addWidget(lblPanelTitle, 0, 0, 1, 1);
    layoutDisplay->addWidget(lblPofileHeader, 1, 0, 1, 1);
    layoutDisplay->addWidget(frmProfileSection, 2, 0, 1, 1);
    layoutDisplay->addWidget(lblMediaHeader, 3, 0, 1, 1);
    layoutDisplay->addWidget(frmMediaSection, 4, 0, 1, 1);
    layoutDisplay->addWidget(lblBackupHeader, 5, 0, 1, 1);
    layoutDisplay->addWidget(frmBackupSection, 6, 0, 1, 1);
    layoutDisplay->setRowMinimumHeight(0, 0);
    layoutDisplay->setRowMinimumHeight(1, 0);
    layoutDisplay->setRowMinimumHeight(2, 100);
    layoutDisplay->setRowMinimumHeight(3, 0);
    layoutDisplay->setRowMinimumHeight(4, 100);
    layoutDisplay->setRowMinimumHeight(5, 0);
    layoutDisplay->setRowMinimumHeight(6, 100);
    layoutDisplay->setRowStretch(7, 1);
    layoutDisplay->setColumnMinimumWidth(0, 600);
    layoutDisplay->setColumnStretch(0, 1);
    layoutDisplay->setHorizontalSpacing(0);
    layoutDisplay->setVerticalSpacing(0);
    layoutDisplay->setMargin(10);
    frmDisplay->setLayout(layoutDisplay);

    scrollArea->setWidget(frmDisplay);
    scrollArea->setWidgetResizable(true);
    scrollArea->setLineWidth(0);

    // -------------------------------------------------- MAIN PANEl

    m_frmLibrary = new QSplitter(this);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmLibrary);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);
    layoutMain->setSpacing(0);
    layoutMain->setContentsMargins(0, 0, 0, 0);
    setLayout(layoutMain);

    m_frmLibrary->addWidget(frmControl);
    m_frmLibrary->addWidget(scrollArea);
    m_frmLibrary->setHandleWidth(0);
    m_frmLibrary->setChildrenCollapsible(false);
    m_frmLibrary->setOrientation(Qt::Horizontal);
    m_frmLibrary->setStretchFactor(1, 1);

    // -------------------------------------------------- STATIC TEXT

    lblPanelTitle->setText("Local Library");
    lblPofileHeader->setText("Profile");
    lblMediaHeader->setText("Media Files");
    lblBackupHeader->setText("Backups");

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

    lblLibNameTag->setText("Library Name:");
    lblLibAddedTag->setText("Date Created:");
    lblPlaylistCountTag->setText("Playlists:");
    lblSongCountTag->setText("Songs:");
    lblArtistCountTag->setText("Artists:");

    // -------------------------------------------------- OBJECT NAMES

    m_boxLibName->setObjectName("PanelLibraryEdit");
    m_lblLibAdded->setObjectName("PanelLibraryValue");
    m_lblPlistCount->setObjectName("PanelLibraryValue");
    m_lblArtistCount->setObjectName("PanelLibraryValue");
    m_lblSongCount->setObjectName("PanelLibraryValue");

    lblLibNameTag->setObjectName("PanelLibraryTag");
    lblLibAddedTag->setObjectName("PanelLibraryTag");
    lblPlaylistCountTag->setObjectName("PanelLibraryTag");
    lblSongCountTag->setObjectName("PanelLibraryTag");
    lblArtistCountTag->setObjectName("PanelLibraryTag");

    m_btnLibImport->setObjectName("PanelLibraryButton");
    m_btnLibExport->setObjectName("PanelLibraryButton");
    m_btnLibReset->setObjectName("PanelLibraryButton");
    m_btnLibOpenFile->setObjectName("PanelLibraryButton");

    scrollArea->setObjectName("PanelScrollArea");
    frmDisplay->setObjectName("PanelLibrary");
    frmControl->setObjectName("PanelControl");

    lblPanelTitle->setObjectName("PanelTitle");
    lblPofileHeader->setObjectName("SectionHeader");
    lblMediaHeader->setObjectName("SectionHeader");
    lblBackupHeader->setObjectName("SectionHeader");

    frmProfileSection->setObjectName("SectionFrame");
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
    m_frmLibrary->setSizes({180, width()-180});
    QWidget::showEvent(event);
}













