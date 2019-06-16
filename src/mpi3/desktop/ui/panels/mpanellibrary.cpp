#include "mpanellibrary.h"
#include "mmedialibrary.h"
#include "mactions.h"
#include "mstyle.h"

#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QDate>


#include <QDebug>


static const QStringList AutoBackupOptions = {"Continuous", "Daily", "Weekly", "Monthly"};


MPanelLibrary::MPanelLibrary(QWidget *parent) : MPanel(parent)
{
    m_sectionLibrary = addSection();
    m_sectionMedia = addSection();
    m_sectionBackup = addSection();

    m_boxLibName = new QLineEdit(this);
    m_lblLibAddedTag = new QLabel(this);
    m_lblLibAdded = new QLabel(this);
    m_btnLibImport = new QPushButton(this);
    m_btnLibExport = new QPushButton(this);

    m_lblLibPathTag = new QLabel(this);
    m_boxLibPath = new QLineEdit(this);
    m_btnSetLibPath = new QPushButton(this);

    MStyle::setStyle(m_boxLibName, MStyle::LE_Hidden);
    MStyle::setStyle(m_lblLibAddedTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_lblLibAdded, MStyle::LBL_Value);
    MStyle::setStyle(m_btnLibImport, MStyle::PB_Normal);
    MStyle::setStyle(m_btnLibExport, MStyle::PB_Normal);

    MStyle::setStyle(m_lblLibPathTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_boxLibPath, MStyle::LE_Normal);
    MStyle::setStyle(m_btnSetLibPath, MStyle::PB_Normal);

    m_optCopyMedia = new QCheckBox(this);
    m_optOrganizeMedia = new QCheckBox(this);
    m_lblMediaSizeTag = new QLabel(this);
    m_lblMediaSize = new QLabel(this);
    m_lblMediaFileCount = new QLabel(this);
    m_lblMediaLocTag = new QLabel(this);
    m_boxMediaLoc = new QLineEdit(this);
    m_btnSetMediaLoc = new QPushButton(this);

    MStyle::setStyle(m_lblMediaSizeTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_lblMediaSize, MStyle::LBL_Value);
    MStyle::setStyle(m_lblMediaFileCount, MStyle::LBL_Value);
    MStyle::setStyle(m_lblMediaLocTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_boxMediaLoc, MStyle::LE_Normal);
    MStyle::setStyle(m_btnSetMediaLoc, MStyle::PB_Normal);

    m_optBackupLibrary = new QCheckBox(this);
    m_lblBackupFreqTag = new QLabel(this);
    m_cbxBackupFreq = new QComboBox(this);
    m_btnBackupManual = new QPushButton(this);
    m_btnBackupRestore = new QPushButton(this);
    m_lblBackupLocTag = new QLabel(this);
    m_boxBackupLoc = new QLineEdit(this);
    m_btnSetBackupLoc = new QPushButton(this);

//    MStyle::setStyle(m_optBackupLibrary, MStyle::);
    MStyle::setStyle(m_lblBackupFreqTag, MStyle::LBL_Tag);
//    MStyle::setStyle(m_cbxBackupFreq, MStyle::);
    MStyle::setStyle(m_btnBackupManual, MStyle::PB_Normal);
    MStyle::setStyle(m_btnBackupRestore, MStyle::PB_Normal);
    MStyle::setStyle(m_lblMediaLocTag, MStyle::LBL_Tag);
    MStyle::setStyle(m_boxBackupLoc, MStyle::LE_Normal);
    MStyle::setStyle(m_btnSetBackupLoc, MStyle::PB_Normal);

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
    connect(m_btnLibImport, &QPushButton::released, this, &MPanelLibrary::askLibraryImport);
    connect(m_btnLibExport, &QPushButton::released, this, &MPanelLibrary::askLibraryExport);
    connect(m_btnSetLibPath, &QPushButton::released, this, &MPanelLibrary::askLibrarySavePath);
    connect(m_btnSetMediaLoc, &QPushButton::released, this, &MPanelLibrary::askLibraryMediaPath);
    connect(m_btnSetBackupLoc, &QPushButton::released, this, &MPanelLibrary::askLibraryBackupPath);
}
void MPanelLibrary::setLibrary(MMediaLibrary *library)
{
    if(m_mediaLibrary){
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;

    connect(m_mediaLibrary, &MMediaLibrary::librarySaved, this, &MPanelLibrary::setLibraryInfo);
    connect(m_mediaLibrary, &MMediaLibrary::libraryLoaded, this, &MPanelLibrary::setLibraryInfo);
    connect(m_mediaLibrary, &MMediaLibrary::libraryReset, this, &MPanelLibrary::setLibraryInfo);

//    connect(m_mediaLibrary, &MMediaLibrary::mediaPathChanged, this, [this](){setLibraryInfo();});
//    connect(m_mediaLibrary, &MMediaLibrary::backupPathChanged, this, [this](){setLibraryInfo();});
//    connect(m_mediaLibrary, &MMediaLibrary::downloadPathChanged, this, [this](){setLibraryInfo();});

    setLibraryInfo();
}
void MPanelLibrary::load(QSettings *settings)
{
    settings->beginGroup("LibraryManagement");
    allowCopyMedia(settings->value("copyMedia", false).toBool());
    allowOrganizeMedia(settings->value("organizeMedia", false).toBool());
    allowAutoBackups(settings->value("autoBackups", false).toBool());
    settings->endGroup();
}
void MPanelLibrary::save(QSettings *settings)
{
    settings->beginGroup("LibraryManagement");
    settings->setValue("copyMedia", valCopyMedia());
    settings->setValue("organizeMedia", valOrganizeMedia());
    settings->setValue("autoBackups", valAutoBackups());
    settings->endGroup();
}

void MPanelLibrary::setLibraryInfo()
{
    m_boxLibName->setText("");
    m_lblLibAdded->setText("");
    m_boxLibPath->setText("");
    m_boxMediaLoc->setText("");
    m_boxBackupLoc->setText("");

    if(m_mediaLibrary) {
        m_boxLibName->setText(m_mediaLibrary->name());
        m_lblLibAdded->setText(m_mediaLibrary->added());
        m_boxLibPath->setText(m_mediaLibrary->savePath());
        m_boxMediaLoc->setText(m_mediaLibrary->mediaPath());
        m_boxBackupLoc->setText(m_mediaLibrary->backupPath());
    }
}

void MPanelLibrary::allowCopyMedia(bool allow)
{
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
void MPanelLibrary::allowOrganizeMedia(bool allow)
{
    m_optOrganizeMedia->setChecked(allow);
}
void MPanelLibrary::allowAutoBackups(bool allow)
{
    m_lblBackupFreqTag->setDisabled(!allow);
    m_optBackupLibrary->setChecked(allow);
    m_cbxBackupFreq->setDisabled(!allow);
    m_boxBackupLoc->setDisabled(!allow);
    m_btnSetBackupLoc->setDisabled(!allow);
}

bool MPanelLibrary::valCopyMedia() const
{
    return m_optCopyMedia->isChecked();
}
bool MPanelLibrary::valOrganizeMedia() const
{
    return m_optOrganizeMedia->isChecked();
}
bool MPanelLibrary::valAutoBackups() const
{
    return m_optBackupLibrary->isChecked();
}

void MPanelLibrary::askLibraryImport()
{
    QString title = "Open Mpi3Library File";
    QString files = "Mpi3Lib Files (*.mpi3lib)";
    QString path = QFileDialog::getOpenFileName(
        nullptr, title, MActions::pathDesktop(), files);

    if(path != "") {
        m_mediaLibrary->load(path);
    }
}
void MPanelLibrary::askLibraryExport()
{
    QString title = "Export Mpi3Library File";
    QString files = "Mpi3Lib Files (*.mpi3lib)";
    QString path = QFileDialog::getSaveFileName(
        nullptr, title, MActions::pathDesktop(), files);

    if(path != "") {
        QString oldPath = m_mediaLibrary->savePath();
        m_mediaLibrary->save(path);
        m_mediaLibrary->save(oldPath);
    }
}
void MPanelLibrary::askLibrarySavePath()
{
    QString title = "Set Library Save Path";
    QString path = QFileDialog::getExistingDirectory(
        nullptr, title, m_mediaLibrary->savePath());

    if(path != "") {
        path += "/" + m_mediaLibrary->name();
        path += MPI3_LIBRARY_FILE_EXT;
        m_mediaLibrary->save(path);
    }
}
void MPanelLibrary::askLibraryMediaPath()
{
    QString title = "Set Library Media Path";
    QString path = QFileDialog::getExistingDirectory(
        nullptr, title, m_mediaLibrary->savePath());

    if(path != "") {
        m_mediaLibrary->setMediaPath(path);
    }
}
void MPanelLibrary::askLibraryBackupPath()
{
    QString title = "Set Library Backup Path";
    QString files = "Mpi3Lib Files (*.mpi3lib)";
    QString path = QFileDialog::getSaveFileName(
        nullptr, title, MActions::pathDesktop(), files);

    if(path != "") {
        m_mediaLibrary->setBackupPath(path);
    }
}

void MPanelLibrary::backupLibrary()
{
    if(!QDir().exists(MActions::pathLibraryBackups())) {
        QDir().mkdir(MActions::pathLibraryBackups());
    }

    QDate cdate = QDate().currentDate();
    QTime ctime = QTime().currentTime();

    QString yrs = QString::number(cdate.year());
    QString mth = QString::number(cdate.month());
    QString day = QString::number(cdate.day());

    QString hrs = QString::number(ctime.hour());
    QString min = QString::number(ctime.minute());
    QString sec = QString::number(ctime.second());

    if(mth.size() == 1) {
        mth.prepend("0");
    }

    if(day.size() == 1) {
        day.prepend("0");
    }

    if(hrs.size() == 1) {
        hrs.prepend("0");
    }

    if(min.size() == 1) {
        min.prepend("0");
    }

    if(sec.size() == 1) {
        sec.prepend("0");
    }

    QString saveTimeStr = yrs + mth + day + "_" + hrs + min + sec;
    QString saveDir = MActions::pathLibraryBackups();
    QString savePath = "/backup_" + saveTimeStr + MPI3_LIBRARY_FILE_EXT;
    QString oldPath = m_mediaLibrary->savePath();

    m_mediaLibrary->save(saveDir + savePath);
    m_mediaLibrary->save(oldPath);
}
void MPanelLibrary::resetLibrary()
{
    m_mediaLibrary->reset();
    m_mediaLibrary->save();
}
