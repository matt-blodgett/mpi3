#include "mpi3/desktop/ui/panels/mpanellibrary.h"
//#include "mpi3/desktop/ui/mactions.h"
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


#include <QScrollArea>

//#include "mpi3/util/msettings.h"
#include <QSettings>

#include <QDebug>


MPanelLibrary::MPanelLibrary(QWidget *parent) : MPanel(parent)
{
    m_lblLibNameTag = new QLabel(this);
    m_boxLibName = new QLineEdit(this);
    m_lblLibAddedTag = new QLabel(this);
    m_lblLibAdded = new QLabel(this);
    m_btnLibImport = new QPushButton(this);
    m_btnLibExport = new QPushButton(this);

    m_lblLibPathTag = new QLabel(this);
    m_boxLibPath = new QLineEdit(this);
    m_btnSetLibPath = new QPushButton(this);

    m_lblLibNameTag->setObjectName("Tag");
    m_boxLibName->setObjectName("Hidden");
    m_lblLibAddedTag->setObjectName("Tag");
    m_lblLibAdded->setObjectName("Value");
    m_btnLibImport->setObjectName("Normal");
    m_btnLibExport->setObjectName("Normal");

    m_lblLibPathTag->setObjectName("Tag");
    m_boxLibPath->setObjectName("Normal");
    m_btnSetLibPath->setObjectName("Normal");

    m_optCopyMedia = new QCheckBox(this);
    m_lblMediaSizeTag = new QLabel(this);
    m_lblMediaSize = new QLabel(this);
    m_lblMediaFileCount = new QLabel(this);
    m_lblMediaLocTag = new QLabel(this);
    m_boxMediaLoc = new QLineEdit(this);
    m_btnSetMediaLoc = new QPushButton(this);

    m_lblMediaSizeTag->setObjectName("Tag");
    m_lblMediaSize->setObjectName("Value");
    m_lblMediaFileCount->setObjectName("Value");
    m_lblMediaLocTag->setObjectName("Tag");
    m_boxMediaLoc->setObjectName("Normal");
    m_btnSetMediaLoc->setObjectName("Normal");

    QGridLayout *m_gridPanel = new QGridLayout();
    QWidget *m_frmPanel = new QWidget(this);
    m_frmPanel->setLayout(m_gridPanel);

    QLabel *m_lblPanelTitle = new QLabel(this);
    m_lblPanelTitle->setObjectName("Title");

    QScrollArea *m_scrollArea = new QScrollArea(this);
    QWidget *m_frmScrollArea = new QWidget(this);
    m_scrollArea->setWidget(m_frmScrollArea);
    m_scrollArea->setWidgetResizable(true);

    m_gridPanel->addWidget(m_lblPanelTitle, 0, 0, 1, 1);
    m_gridPanel->addWidget(m_scrollArea, 1, 0, 1, 1);
    m_gridPanel->setAlignment(m_lblPanelTitle, Qt::AlignCenter);
    m_gridPanel->setContentsMargins(0, 10, 0, 0);
    m_gridPanel->setRowStretch(1, 1);
    m_gridPanel->setColumnStretch(0, 1);

    QGridLayout *m_gridMain = new QGridLayout();
    m_gridMain->addWidget(m_frmPanel);
    m_gridMain->setContentsMargins(0, 0, 0, 0);
    m_gridMain->setColumnStretch(0, 1);
    m_gridMain->setRowStretch(0, 1);
    setLayout(m_gridMain);

    QGridLayout *m_gridScrollArea = new QGridLayout();
    m_frmScrollArea->setLayout(m_gridScrollArea);

    QGridLayout *m_grid0 = new QGridLayout();
    m_grid0->addWidget(m_lblLibNameTag, 0, 0, 1, 1);
    m_grid0->addWidget(m_boxLibName, 0, 1, 1, 1);
    m_grid0->addWidget(m_lblLibAddedTag, 1, 0, 1, 1);
    m_grid0->addWidget(m_lblLibAdded, 1, 1, 1, 1);
    m_grid0->addWidget(m_lblLibPathTag, 2, 0, 1, 1);
    m_grid0->addWidget(m_boxLibPath, 2, 1, 1, 1);
    m_grid0->addWidget(m_btnSetLibPath, 2, 2, 1, 1);
    m_grid0->setContentsMargins(0, 20, 0, 0);
    m_grid0->setRowStretch(0, 0);
    m_grid0->setRowStretch(1, 0);
    m_grid0->setRowStretch(2, 0);
    m_grid0->setRowStretch(3, 1);
    m_grid0->setColumnStretch(1, 1);

    QGridLayout *m_grid1 = new QGridLayout();
    m_grid1->addWidget(m_btnLibImport, 0, 0, 1, 1);
    m_grid1->addWidget(m_btnLibExport, 0, 1, 1, 1);
    m_grid1->setContentsMargins(0, 20, 0, 0);
    m_grid1->setRowStretch(0, 0);
    m_grid1->setRowStretch(1, 1);
    m_grid1->setColumnStretch(0, 0);
    m_grid1->setColumnStretch(1, 0);
    m_grid1->setColumnStretch(2, 1);

    QGridLayout *m_grid2 = new QGridLayout();
    m_grid2->addWidget(m_optCopyMedia, 0, 0, 1, 3);
    m_grid2->addWidget(m_lblMediaLocTag, 1, 0, 1, 1);
    m_grid2->addWidget(m_boxMediaLoc, 1, 1, 1, 1);
    m_grid2->addWidget(m_btnSetMediaLoc, 1, 2, 1, 1);
    m_grid2->setContentsMargins(0, 20, 0, 0);
    m_grid2->setRowStretch(0, 0);
    m_grid2->setRowStretch(1, 0);
    m_grid2->setRowStretch(2, 1);

    QGridLayout *m_grid3 = new QGridLayout();
    m_grid3->addWidget(m_lblMediaSizeTag, 0, 0, 1, 1);
    m_grid3->addWidget(m_lblMediaSize, 0, 1, 1, 1);
    m_grid3->addWidget(m_lblMediaFileCount, 0, 2, 1, 1);
    m_grid3->setContentsMargins(0, 0, 0, 0);
    m_grid3->setRowStretch(0, 0);
    m_grid3->setRowStretch(1, 1);

    m_gridScrollArea->addLayout(m_grid0, 0, 1, 1, 1);
    m_gridScrollArea->addLayout(m_grid1, 1, 1, 1, 1);
    m_gridScrollArea->addLayout(m_grid2, 2, 1, 1, 1);
    m_gridScrollArea->addLayout(m_grid3, 3, 1, 1, 1);
    m_gridScrollArea->setContentsMargins(0, 0, 0, 0);
    m_gridScrollArea->setRowStretch(0, 0);
    m_gridScrollArea->setRowStretch(1, 0);
    m_gridScrollArea->setRowStretch(2, 0);
    m_gridScrollArea->setRowStretch(3, 0);
    m_gridScrollArea->setRowStretch(4, 1);
    m_gridScrollArea->setColumnMinimumWidth(0, 50);
//    m_gridScrollArea->setColumnStretch(0, 0);
    m_gridScrollArea->setColumnStretch(1, 1);
//    m_gridScrollArea->setColumnStretch(2, 0);
    m_gridScrollArea->setColumnMinimumWidth(2, 50);


    m_frmPanel->setObjectName("PanelScrollArea");
    m_frmScrollArea->setObjectName("PanelScrollArea");

//    setTitle("Local Library");
//    m_sectionLibrary->setHeader("Library");
//    m_sectionMedia->setHeader("Media Files");

    m_lblPanelTitle->setText("Library Settings");


    m_btnLibImport->setText("Import");
    m_btnLibExport->setText("Export");
    m_btnSetMediaLoc->setText("Change");
    m_btnSetLibPath->setText("Change");

    m_optCopyMedia->setText("Make Local Copies of Media Files");

    m_lblLibNameTag->setText("Library Name:");
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
    if(m_mediaLibrary) {
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
    QString path = QFileDialog::getExistingDirectory(nullptr, title, m_mediaLibrary->savePath());

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
