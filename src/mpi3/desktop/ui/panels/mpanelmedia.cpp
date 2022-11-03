#include "mpi3/desktop/ui/models/mmodelsonglist.h"
#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/desktop/ui/panels/mpanelmedia.h"
#include "mpi3/desktop/ui/frames/mframetreeview.h"
#include "mpi3/desktop/ui/trees/mtreecontainers.h"
#include "mpi3/desktop/ui/trees/mtreelayoutsettings.h"
#include "mpi3/desktop/ui/mstyle.h"
#include "mpi3/core/mmedialibrary.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>

#include <QSplitter>

#include <QSettings>


#include <QDebug>



MPanelMedia::MPanelMedia(QWidget *parent) : MPanel(parent)
{
    m_frmContainers = new MFrameContainers(this);
    m_frmSonglist = new MFrameSonglist(this);

    m_btnSongs = new QRadioButton(this);
    m_btnArtists = new QRadioButton(this);
    m_btnAlbums = new QRadioButton(this);
    m_lblPlaylist = new QLabel(this);
    m_lblView = new QLabel(this);

    m_lblPlaylist->setObjectName("Tag");
    m_lblView->setObjectName("Title");

    m_btnSongs->setStyle(new MProxyStyle(m_btnSongs->style()));
    m_btnArtists->setStyle(new MProxyStyle(m_btnArtists->style()));
    m_btnAlbums->setStyle(new MProxyStyle(m_btnAlbums->style()));


    QSplitter *m_frmSplitter = new QSplitter(this);
    QWidget *m_frmDrawer = new QWidget(this);
    QWidget *m_frmCenter = new QWidget(this);

    QGridLayout *m_gridDrawer = new QGridLayout();
    m_frmDrawer->setLayout(m_gridDrawer);

    QGridLayout *m_gridCenter = new QGridLayout();
    m_frmCenter->setLayout(m_gridCenter);

    QGridLayout *m_gridMain = new QGridLayout();
    m_gridMain->addWidget(m_frmSplitter);
    m_gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(m_gridMain);

    m_frmDrawer->setMinimumWidth(120);
    m_frmDrawer->setMaximumWidth(400);

    m_frmSplitter->addWidget(m_frmDrawer);
    m_frmSplitter->addWidget(m_frmCenter);
    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setStretchFactor(1, 1);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);

    m_frmCenter->setObjectName("PanelCenter");
    m_frmDrawer->setObjectName("PanelDrawer");

    m_gridDrawer->addWidget(m_btnSongs, 0, 0, 1, 1);
    m_gridDrawer->addWidget(m_btnArtists, 1, 0, 1, 1);
    m_gridDrawer->addWidget(m_btnAlbums, 2, 0, 1, 1);
    m_gridDrawer->setRowMinimumHeight(3, 6);
    m_gridDrawer->addWidget(m_lblPlaylist, 4, 0, 1, 1);
    m_gridDrawer->addWidget(m_frmContainers, 5, 0, 1, 1);
    m_gridDrawer->setRowStretch(5, 1);
    m_gridDrawer->setContentsMargins(0, 0, 0, 0);

    m_gridCenter->addWidget(m_lblView, 0, 1, 1, 1);
    m_gridCenter->addWidget(m_frmSonglist, 1, 0 , 1, 2);
    m_gridCenter->setColumnMinimumWidth(0, 8);
    m_gridCenter->setRowMinimumHeight(0, 60);
    m_gridCenter->setColumnStretch(1, 1);
    m_gridCenter->setRowStretch(1, 1);
    m_gridCenter->setContentsMargins(0, 0, 0, 0);

    m_btnSongs->setText("Songs");
    m_btnArtists->setText("Artists");
    m_btnAlbums->setText("Albums");
    m_lblPlaylist->setText("Playlists");

    m_lblPlaylist->setStyleSheet(
        "QLabel {border-top: 1px solid #696969;"
        "font-size: 14px; padding: 4px 2px 4px 2px;}"
    );

    connect(m_btnSongs, &QRadioButton::released, this, &MPanelMedia::viewAllSongs);
    connect(m_btnArtists, &QRadioButton::released, this, &MPanelMedia::viewArtists);
    connect(m_btnAlbums, &QRadioButton::released, this, &MPanelMedia::viewAlbums);
    connect(m_frmContainers, &MFrameContainers::containerSelected, this, &MPanelMedia::viewContainer);

    m_layoutSettingsManager = new MTreeSonglistLayoutSettings(this);
}
void MPanelMedia::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
    m_frmContainers->setLibrary(m_mediaLibrary);
    m_frmSonglist->setLibrary(m_mediaLibrary);
}
void MPanelMedia::load(QSettings *settings)
{
    QStringList pidList;
    pidList.append(m_mediaLibrary->pid());
    for(MContainer *container : m_mediaLibrary->containers()) {
        pidList.append(container->pid());
    }

    settings->beginGroup("TreeViews");

    settings->beginGroup("TreeViewSonglists");
    m_layoutSettingsManager->load(settings, pidList);
    settings->endGroup();

    settings->beginGroup("TreeViewContainers");

    QString pidSelectedContainer = settings->value("SelectedContainer").toString();

    settings->beginGroup("ExpandedContainers");

    QModelIndexList indexes = m_frmContainers->model()->match(
        m_frmContainers->model()->index(0, 0), Qt::DisplayRole, "*", -1, Qt::MatchWildcard | Qt::MatchRecursive);

    for(const QString &pid : settings->childKeys()) {
        QString pidKey = pid;
        pidKey.insert(1, ":");

        for(QModelIndex idx : indexes) {
            if(m_frmContainers->model()->pidAt(idx) == pidKey) {
                m_frmContainers->tree()->expand(idx);
            }
        }
    }

    settings->endGroup();
    settings->endGroup();

    settings->endGroup();

    if(pidSelectedContainer != "") {
        QModelIndexList indexes = m_frmContainers->model()->match(
            m_frmContainers->model()->index(0,0), Qt::DisplayRole, "*", -1, Qt::MatchWildcard | Qt::MatchRecursive);

        for(QModelIndex idx : indexes) {
            if(m_frmContainers->model()->pidAt(idx) == pidSelectedContainer) {
                m_frmContainers->tree()->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);
                break;
            }
        }
    }
    else {
        viewAllSongs();
    }
}
void MPanelMedia::save(QSettings *settings)
{
    QStringList pidList;
    pidList.append(m_mediaLibrary->pid());
    for(MContainer *container : m_mediaLibrary->containers()) {
        pidList.append(container->pid());
    }

    settings->beginGroup("TreeViews");

    settings->beginGroup("TreeViewSonglists");
    m_frmSonglist->saveLayoutSettings();
    m_layoutSettingsManager->save(settings, pidList);
    settings->endGroup();

    settings->beginGroup("TreeViewContainers");
    if(m_frmContainers->tree()->selectionModel()->selectedRows(0).size() == 1) {
        settings->setValue("SelectedContainer", m_frmContainers->model()->pidAt(m_frmContainers->tree()->selectionModel()->currentIndex()));
    }
    else {
        settings->setValue("SelectedContainer", "");
    }
    settings->beginGroup("ExpandedContainers");

    QModelIndexList indexes = m_frmContainers->model()->match(m_frmContainers->model()->index(0,0), Qt::DisplayRole, "*", -1, Qt::MatchWildcard|Qt::MatchRecursive);

    for(QModelIndex idx : indexes) {
        if(m_frmContainers->model()->itemIsFolder(idx)) {
            if(m_frmContainers->tree()->isExpanded(idx)) {
                QString pidKey = m_frmContainers->model()->pidAt(idx);
                pidKey.remove(1, 1);
                settings->setValue(pidKey, true);
            }
        }
    }

    settings->endGroup();
    settings->endGroup();

    settings->endGroup();
}

void MPanelMedia::viewAllSongs()
{
    m_frmContainers->blockSignals(true);
    m_frmContainers->tree()->clearSelection();
    m_frmContainers->blockSignals(false);

    m_lblView->setText("Library");
    m_btnSongs->toggle();

    m_frmSonglist->setPlaylist(nullptr);
    viewChanged();
}
void MPanelMedia::viewArtists()
{
    m_frmContainers->blockSignals(true);
    m_frmContainers->tree()->clearSelection();
    m_frmContainers->blockSignals(false);

    m_lblView->setText("Artists");
    m_btnArtists->toggle();

    m_frmSonglist->setPlaylist(nullptr);
    viewChanged();
}
void MPanelMedia::viewAlbums()
{
    m_frmContainers->blockSignals(true);
    m_frmContainers->tree()->clearSelection();
    m_frmContainers->blockSignals(false);

    m_lblView->setText("Albums");
    m_btnAlbums->toggle();

    m_frmSonglist->setPlaylist(nullptr);
    viewChanged();
}
void MPanelMedia::viewContainer(MContainer *container)
{
    if(!container) {
        viewAllSongs();
        return;
    }

    if(container->type() == Mpi3::PlaylistElement) {
        MPlaylist *playlist = static_cast<MPlaylist*>(container);

        m_btnSongs->setAutoExclusive(false);
        m_btnArtists->setAutoExclusive(false);
        m_btnAlbums->setAutoExclusive(false);

        m_btnSongs->setChecked(false);
        m_btnArtists->setChecked(false);
        m_btnAlbums->setChecked(false);

        m_btnSongs->setAutoExclusive(true);
        m_btnArtists->setAutoExclusive(true);
        m_btnAlbums->setAutoExclusive(true);

        m_lblView->setText(playlist->name());

        m_frmSonglist->setPlaylist(playlist);

        viewChanged();
    }
}

void MPanelMedia::viewChanged()
{
    m_frmSonglist->setLayoutSettings(m_layoutSettingsManager->getLayoutSettings(m_frmSonglist->model()->pidCurrentSonglist()));
}

MFrameSonglist *MPanelMedia::frameSonglist()
{
    return m_frmSonglist;
}
MFrameContainers *MPanelMedia::frameContainers()
{
    return m_frmContainers;
}
