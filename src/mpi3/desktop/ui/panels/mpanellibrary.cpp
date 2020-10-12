#include "mpi3/desktop/ui/panels/mpanellibrary.h"
#include "mpi3/desktop/ui/mactions.h"
#include "mpi3/desktop/ui/mstyle.h"
#include "mpi3/core/mmedialibrary.h"

#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QDate>


#include "mpi3/util/msettings.h"

#include <QDebug>


MPanelLibrary::MPanelLibrary(QWidget *parent) : MPanel(parent)
{
    m_sectionLibrary = addSection();
    m_sectionMedia = addSection();

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

//    MStyle::setStyle(m_optBackupLibrary, MStyle::);
//    MStyle::setStyle(m_cbxBackupFreq, MStyle::);
    MStyle::setStyle(m_lblMediaLocTag, MStyle::LBL_Tag);

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
    media_gridWest->setColumnStretch(1, 1);
    media_gridWest->setRowStretch(1, 1);

    media_gridEast->addWidget(m_lblMediaSizeTag, 0, 0, 1, 1);
    media_gridEast->addWidget(m_lblMediaSize, 0, 1, 1, 1);
    media_gridEast->addWidget(m_lblMediaFileCount, 0, 2, 1, 1);
    media_gridEast->setColumnStretch(3, 1);
    media_gridEast->setRowStretch(1, 1);

    media_gridSouth->addWidget(m_lblMediaLocTag, 0, 0, 1, 1);
    media_gridSouth->addWidget(m_boxMediaLoc, 1, 0, 1, 1);
    media_gridSouth->addWidget(m_btnSetMediaLoc, 1, 1, 1, 1);

    setTitle("Local Library");
    m_sectionLibrary->setHeader("Library");
    m_sectionMedia->setHeader("Media Files");

    m_btnLibImport->setText("Import");
    m_btnLibExport->setText("Export");
    m_btnSetMediaLoc->setText("Change");
    m_btnSetLibPath->setText("Change");

    m_optCopyMedia->setText("Make Local Copies of Files");

    m_lblLibAddedTag->setText("Date Created:");
    m_lblLibPathTag->setText("File Location:");
    m_lblMediaSizeTag->setText("Size on Disk:");
    m_lblMediaLocTag->setText("Media Folder:");

    connect(m_optCopyMedia, &QCheckBox::toggled, this, &MPanelLibrary::allowCopyMedia);
    connect(m_btnLibImport, &QPushButton::released, this, &MPanelLibrary::askLibraryImport);
    connect(m_btnLibExport, &QPushButton::released, this, &MPanelLibrary::askLibraryExport);
    connect(m_btnSetLibPath, &QPushButton::released, this, &MPanelLibrary::askLibrarySavePath);
    connect(m_btnSetMediaLoc, &QPushButton::released, this, &MPanelLibrary::askLibraryMediaPath);
}
void MPanelLibrary::setLibrary(MMediaLibrary *library)
{
    if(m_mediaLibrary){
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;

    connect(m_optCopyMedia, &QCheckBox::toggled, this, &MPanelLibrary::setLocalMediaPath);

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
    m_boxMediaLoc->setText(settings->value("copyMediaPath", "").toString());
    settings->endGroup();
}
void MPanelLibrary::save(QSettings *settings)
{
    settings->beginGroup("LibraryManagement");
    settings->setValue("copyMedia", m_optCopyMedia->isChecked());
    settings->setValue("copyMediaPath", m_boxMediaLoc->text());
    settings->endGroup();
}

void MPanelLibrary::setLocalMediaPath(bool allow)
{
    if (allow && !m_boxMediaLoc->text().isNull() && !m_boxMediaLoc->text().isEmpty()) {
        m_mediaLibrary->setLocalMediaPath(m_boxMediaLoc->text());
    }
    else {
        m_mediaLibrary->setLocalMediaPath(QString());
    }
}

void MPanelLibrary::setLibraryInfo()
{
    m_boxLibName->setText("");
    m_lblLibAdded->setText("");
    m_boxLibPath->setText("");
    m_boxMediaLoc->setText("");

    if(m_mediaLibrary) {
        m_boxLibName->setText(m_mediaLibrary->name());
        m_lblLibAdded->setText(m_mediaLibrary->added());
        m_boxLibPath->setText(m_mediaLibrary->savePath());
        m_boxMediaLoc->setText(m_mediaLibrary->localMediaPath());
    }
}

void MPanelLibrary::allowCopyMedia(bool allow)
{
    m_optCopyMedia->setChecked(allow);

    m_lblMediaSizeTag->setDisabled(!allow);
    m_lblMediaLocTag->setDisabled(!allow);
    m_btnSetMediaLoc->setDisabled(!allow);
    m_boxMediaLoc->setDisabled(!allow);

    m_lblMediaSize->setVisible(allow);
    m_lblMediaFileCount->setVisible(allow);
}


void MPanelLibrary::askLibraryImport()
{
//    QString title = "Open Mpi3Library File";
//    QString files = "Mpi3Lib Files (*.mpi3lib)";
//    QString path = QFileDialog::getOpenFileName(
//        nullptr, title, MActions::pathDesktop(), files);

//    if(path != "") {
//        m_mediaLibrary->load(path);
//    }
    qDebug();
}
void MPanelLibrary::askLibraryExport()
{
//    QString title = "Export Mpi3Library File";
//    QString files = "Mpi3Lib Files (*.mpi3lib)";
//    QString path = QFileDialog::getSaveFileName(
//        nullptr, title, MActions::pathDesktop(), files);

//    if(path != "") {
//        QString oldPath = m_mediaLibrary->savePath();
//        m_mediaLibrary->save(path);
//        m_mediaLibrary->save(oldPath);
//    }
    qDebug();
}
void MPanelLibrary::askLibrarySavePath()
{
//    QString title = "Set Library Save Path";
//    QString path = QFileDialog::getExistingDirectory(
//        nullptr, title, m_mediaLibrary->savePath());

//    if(path != "") {
//        path += "/" + m_mediaLibrary->name();
//        path += MPI3_LIBRARY_FILE_EXT;
//        m_mediaLibrary->save(path);
//    }
    qDebug();
}
void MPanelLibrary::askLibraryMediaPath()
{
    QString title = "Set Library Media Path";
    QString path = QFileDialog::getExistingDirectory(
        nullptr, title, m_mediaLibrary->savePath());

    if(path != "") {
        m_boxMediaLoc->setText(QDir::toNativeSeparators(path));
        m_optCopyMedia->setChecked(true);
        setLocalMediaPath(true);
    }
}

void MPanelLibrary::resetLibrary()
{
    m_mediaLibrary->reset();
    m_mediaLibrary->save();
}
