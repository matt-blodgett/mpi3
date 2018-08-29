#include "mpanellibrary.h"
#include "core/mmedialibrary.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>


#include <QDebug>


static const QStringList AutoBackupOptions = {"Continuous", "Daily", "Weekly", "Monthly"};


MPanelLibrary::MPanelLibrary(QWidget *parent) : MPanelContext(parent){
    initializeLayoutType(true);
    initializeLayout();

    setTitle("Local Library");
    m_sectionLibrary->setHeader("Library");
    m_sectionMedia->setHeader("Media Files");
    m_sectionBackup->setHeader("Backups");

    m_btnLibImport->setText("Import");
    m_btnLibExport->setText("Export");
    m_btnSetMediaLoc->setText("Change");
    m_btnSetLibPath->setText("Change");
    m_btnSetBackupLoc->setText("Change");
    m_btnBackupRestore->setText("Restore Backup");
    m_btnBackupManual->setText("Backup Now");

    m_optCopyMedia->setText("Make Local Copies of Files");
    m_optOrganizeMedia->setText("Keep Files Organized");
    m_optBackupLibrary->setText("Backup Library Automatically");

    m_lblLibAddedTag->setText("Date Created:");
    m_lblLibPathTag->setText("File Location:");
    m_lblMediaSizeTag->setText("Size on Disk:");
    m_lblMediaLocTag->setText("Media Folder:");
    m_lblBackupLocTag->setText("Backup Folder:");
    m_lblBackupFreqTag->setText("Backup Frequency:");

    m_cbxBackupFreq->addItems(AutoBackupOptions);
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

void MPanelLibrary::initializeLayout(){

    m_sectionLibrary = addSection();
    m_sectionMedia = addSection();
    m_sectionBackup = addSection();

    m_boxLibName = addLineEditHidden();
    m_lblLibAddedTag = addLabelTag();
    m_lblLibAdded = addLabelValue();
    m_btnLibImport = addPushButton();
    m_btnLibExport = addPushButton();
    m_lblLibPathTag = addLabelTag();
    m_boxLibPath = addLineEdit();
    m_btnSetLibPath = addPushButton();

    m_optCopyMedia = addCheckBox();
    m_optOrganizeMedia = addCheckBox();
    m_lblMediaSizeTag = addLabelTag();
    m_lblMediaSize = addLabelValue();
    m_lblMediaFileCount = addLabelValue();
    m_lblMediaLocTag = addLabelTag();
    m_boxMediaLoc = addLineEdit();
    m_btnSetMediaLoc = addPushButton();

    m_optBackupLibrary = addCheckBox();
    m_lblBackupFreqTag = addLabelTag();
    m_cbxBackupFreq = addComboBox();
    m_btnBackupManual = addPushButton();
    m_btnBackupRestore = addPushButton();
    m_lblBackupLocTag = addLabelTag();
    m_boxBackupLoc = addLineEdit();
    m_btnSetBackupLoc = addPushButton();

    QGridLayout *library_gridWest = m_sectionLibrary->gridWest();
    QGridLayout *library_gridEast = m_sectionLibrary->gridEast();
    QGridLayout *library_gridSouth = m_sectionLibrary->gridSouth();

    library_gridWest->addWidget(m_boxLibName, 0, 0, 1, 3);
    library_gridWest->addWidget(m_lblLibAddedTag, 1, 0, 1, 1);
    library_gridWest->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    library_gridWest->setRowStretch(2, 1);
    library_gridWest->setColumnStretch(2, 1);

    library_gridEast->addWidget(m_btnLibImport, 0, 1, 1, 1);
    library_gridEast->addWidget(m_btnLibExport, 1, 1, 1, 1);
    library_gridEast->setColumnStretch(0, 1);
    library_gridEast->setRowStretch(1, 1);

    library_gridSouth->addWidget(m_lblLibPathTag, 0, 0, 1, 1);
    library_gridSouth->addWidget(m_boxLibPath, 1, 0, 1, 1);
    library_gridSouth->addWidget(m_btnSetLibPath, 1, 1, 1, 1);

    QGridLayout *media_gridWest = m_sectionMedia->gridWest();
    QGridLayout *media_gridEast = m_sectionMedia->gridEast();
    QGridLayout *media_gridSouth = m_sectionMedia->gridSouth();

    media_gridWest->addWidget(m_optCopyMedia, 0, 0, 1, 1);
    media_gridWest->addWidget(m_optOrganizeMedia, 1, 0, 1, 1);
    media_gridWest->setColumnStretch(1, 1);
    media_gridWest->setRowStretch(2, 1);

    media_gridEast->addWidget(m_lblMediaSizeTag, 0, 0, 1, 1);
    media_gridEast->addWidget(m_lblMediaSize, 0, 1, 1, 1);
    media_gridEast->addWidget(m_lblMediaFileCount, 0, 2, 1, 1);
    media_gridEast->setColumnStretch(3, 1);
    media_gridEast->setRowStretch(1, 1);

    media_gridSouth->addWidget(m_lblMediaLocTag, 0, 0, 1, 1);
    media_gridSouth->addWidget(m_boxMediaLoc, 1, 0, 1, 1);
    media_gridSouth->addWidget(m_btnSetMediaLoc, 1, 1, 1, 1);

    QGridLayout *backup_gridWest = m_sectionBackup->gridWest();
    QGridLayout *backup_gridEast = m_sectionBackup->gridEast();
    QGridLayout *backup_gridSouth = m_sectionBackup->gridSouth();

    backup_gridWest->addWidget(m_optBackupLibrary, 0, 0, 1, 3);
    backup_gridWest->addWidget(m_lblBackupFreqTag, 2, 0, 1, 1);
    backup_gridWest->addWidget(m_cbxBackupFreq, 3, 0, 1, 1);
    backup_gridWest->setRowMinimumHeight(1, 10);

    backup_gridEast->addWidget(m_btnBackupManual, 0, 1, 1, 1);
    backup_gridEast->addWidget(m_btnBackupRestore, 1, 1, 1, 1);
    backup_gridEast->setColumnStretch(0, 1);
    backup_gridEast->setRowStretch(2, 1);

    backup_gridSouth->addWidget(m_lblBackupLocTag, 0, 0, 1, 1);
    backup_gridSouth->addWidget(m_boxBackupLoc, 1, 0, 1, 1);
    backup_gridSouth->addWidget(m_btnSetBackupLoc, 1, 1, 1, 1);

}

void MPanelLibrary::setLibrary(MMediaLibrary *library){
    m_mediaLibrary = library;

    m_boxLibName->setText("");
    m_lblLibAdded->setText("");
    m_boxLibPath->setText("");
    m_boxMediaLoc->setText("");
    m_boxBackupLoc->setText("");

    if(m_mediaLibrary){
        m_boxLibName->setText(m_mediaLibrary->name());
        m_lblLibAdded->setText(m_mediaLibrary->added());
        m_boxLibPath->setText(m_mediaLibrary->savePath());
        m_boxMediaLoc->setText(m_mediaLibrary->mediaPath());
        m_boxBackupLoc->setText(m_mediaLibrary->backupPath());
    }
}

void MPanelLibrary::allowCopyMedia(bool allow){
    m_optCopyMedia->setChecked(allow);
    m_optOrganizeMedia->setChecked(false);

    m_optOrganizeMedia->setDisabled(!allow);
    m_lblMediaSizeTag->setDisabled(!allow);
    m_lblMediaLocTag->setDisabled(!allow);
    m_btnSetMediaLoc->setDisabled(!allow);
    m_boxMediaLoc->setDisabled(!allow);

    m_lblMediaSize->setVisible(allow);
    m_lblMediaFileCount->setVisible(allow);
}
void MPanelLibrary::allowOrganizeMedia(bool allow){
    m_optOrganizeMedia->setChecked(allow);
}
void MPanelLibrary::allowAutoBackups(bool allow){
    m_lblBackupFreqTag->setDisabled(!allow);
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










































