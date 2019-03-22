#include "mpanelmedia.h"
#include "mframetreeview.h"
#include "mmodelsonglist.h"
#include "mmodelcontainers.h"
#include "mmedialibrary.h"
#include "mtreeview.h"
#include "mstyle.h"

#include <QGridLayout>
#include <QRadioButton>
#include <QLabel>


#include <QDebug>
#include <QSettings>


MPanelMedia::MPanelMedia(QWidget *parent) : MPanel(parent, false)
{
    m_frmContainers = new MFrameContainers(this);
    m_frmSonglist = new MFrameSonglist(this);

    m_btnSongs = new QRadioButton(this);
    m_btnArtists = new QRadioButton(this);
    m_btnAlbums = new QRadioButton(this);
    m_lblPlaylist = new QLabel(this);
    m_lblView = new QLabel(this);

//    MStyle::setStyle(m_btnSongs, MStyle::);
//    MStyle::setStyle(m_btnArtists, MStyle::);
//    MStyle::setStyle(m_btnAlbums, MStyle::);
    MStyle::setStyle(m_lblPlaylist, MStyle::LBL_Tag);
    MStyle::setStyle(m_lblView, MStyle::LBL_Title);

    m_btnSongs->setStyle(new MProxyStyle(m_btnSongs->style()));
    m_btnArtists->setStyle(new MProxyStyle(m_btnArtists->style()));
    m_btnAlbums->setStyle(new MProxyStyle(m_btnAlbums->style()));

    gridControl()->addWidget(m_btnSongs, 0, 0, 1, 1);
    gridControl()->addWidget(m_btnArtists, 1, 0, 1, 1);
    gridControl()->addWidget(m_btnAlbums, 2, 0, 1, 1);
    gridControl()->setRowMinimumHeight(3, 6);
    gridControl()->addWidget(m_lblPlaylist, 4, 0, 1, 1);
    gridControl()->addWidget(m_frmContainers, 5, 0, 1, 1);
    gridControl()->setRowStretch(5, 1);
    gridControl()->setVerticalSpacing(0);
    gridControl()->setHorizontalSpacing(0);
    gridControl()->setMargin(0);

    gridDisplay()->addWidget(m_lblView, 0, 1, 1, 1);
    gridDisplay()->addWidget(m_frmSonglist, 1, 0 , 1, 2);
    gridDisplay()->setColumnMinimumWidth(0, 8);
    gridDisplay()->setRowMinimumHeight(0, 60);
    gridDisplay()->setColumnStretch(1, 1);
    gridDisplay()->setRowStretch(1, 1);
    gridDisplay()->setMargin(0);

    m_btnSongs->setText("Songs");
    m_btnArtists->setText("Artists");
    m_btnAlbums->setText("Albums");
    m_lblPlaylist->setText("Playlists");

    m_lblPlaylist->setStyleSheet(
        "QLabel {border-top: 1px solid #696969;"
        "font-size: 14px; padding: 4px 2px 4px 2px;}");

    connect(m_btnSongs, &QRadioButton::released, this, &MPanelMedia::viewAllSongs);
    connect(m_btnArtists, &QRadioButton::released, this, &MPanelMedia::viewArtists);
    connect(m_btnAlbums, &QRadioButton::released, this, &MPanelMedia::viewAlbums);
    connect(m_frmContainers, &MFrameContainers::containerSelected, this, &MPanelMedia::viewContainer);

    m_treeSettingsCollection = new MTreeSettingsCollection(this);
    connect(m_treeSettingsCollection, &MTreeSettingsCollection::aboutToSave, m_frmSonglist, &MFrameSonglist::saveTreeSettings);
}
void MPanelMedia::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
    m_frmContainers->model()->setLibrary(m_mediaLibrary);
    m_frmSonglist->model()->setLibrary(m_mediaLibrary);
}
void MPanelMedia::load(QSettings *settings)
{
    // TODO: save expanded folders
    m_frmContainers->tree()->expandAll();

    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    for(MContainer *c : m_mediaLibrary->containers()){
        pidlist.append(c->pid());
    }

    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->load(settings, pidlist);
    QString pidSonglist = settings->value("songlist").toString();
    settings->endGroup();

    if(pidSonglist != "" && pidlist.contains(pidSonglist)){
        viewContainer(m_mediaLibrary->getContainer(pidSonglist));
    }
    else {
        // TODO: save previous view
        viewAllSongs();
    }
}
void MPanelMedia::save(QSettings *settings)
{
    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    for(MContainer *c : m_mediaLibrary->containers()){
        pidlist.append(c->pid());
    }

    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->save(settings, pidlist);
    MContainer *container = m_frmSonglist->model()->container();
    settings->setValue("songlist", container ? container->pid() : "");
    settings->endGroup();
}

void MPanelMedia::viewAllSongs()
{
    m_lblView->setText("Library");
    m_btnSongs->toggle();

    m_frmSonglist->model()->setContainer(nullptr);
    viewChanged();
}
void MPanelMedia::viewArtists()
{
    m_lblView->setText("Artists");
    m_btnArtists->toggle();

    m_frmSonglist->model()->setContainer(nullptr);
    viewChanged();
}
void MPanelMedia::viewAlbums()
{
    m_lblView->setText("Albums");
    m_btnAlbums->toggle();

    m_frmSonglist->model()->setContainer(nullptr);
    viewChanged();
}
void MPanelMedia::viewContainer(MContainer *container)
{
    if(!container){
        viewAllSongs();
        return;
    }

    m_btnSongs->setAutoExclusive(false);
    m_btnArtists->setAutoExclusive(false);
    m_btnAlbums->setAutoExclusive(false);

    m_btnSongs->setChecked(false);
    m_btnArtists->setChecked(false);
    m_btnAlbums->setChecked(false);

    m_btnSongs->setAutoExclusive(true);
    m_btnArtists->setAutoExclusive(true);
    m_btnAlbums->setAutoExclusive(true);

    m_lblView->setText(container->name());
    m_frmSonglist->model()->setContainer(container);
    viewChanged();
}

void MPanelMedia::viewChanged()
{
    MContainer *container = m_frmSonglist->model()->container();
    QString pid = container ? container->pid() : m_mediaLibrary->pid();
    MTreeSettings *treeSettings = m_treeSettingsCollection->getContainer(pid);
    if(!treeSettings) {
        // TODO: Default getContainer() to add if it doesn't exist
        treeSettings = m_treeSettingsCollection->addContainer(pid);
    }
    m_frmSonglist->setTreeSettings(treeSettings);

    m_frmContainers->tree()->selectionModel()->blockSignals(true);
    if(container){
        QModelIndex idx = m_frmContainers->model()->getIndex(container->pid());
        QItemSelectionModel::SelectionFlag flag = QItemSelectionModel::ClearAndSelect;
        m_frmContainers->tree()->selectionModel()->select(idx, flag);
    }
    else {
        m_frmContainers->tree()->clearSelection();
    }
    m_frmContainers->tree()->selectionModel()->blockSignals(false);
    m_frmContainers->tree()->update();
}

MFrameSonglist *MPanelMedia::frameSonglist()
{
    return m_frmSonglist;
}
MFrameContainers *MPanelMedia::frameContainers()
{
    return m_frmContainers;
}
MTreeSettingsCollection *MPanelMedia::treeSettingsCollection()
{
    return m_treeSettingsCollection;
}
