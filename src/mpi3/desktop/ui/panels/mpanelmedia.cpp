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

    m_btnSongs->setParent(this);
    m_btnArtists->setParent(this);
    m_btnAlbums->setParent(this);

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

    m_lblPlaylist->setStyleSheet("QLabel {border-top: 1px solid #696969;"
                                 "font-size: 14px; padding: 4px 2px 4px 2px;}");

    connect(m_frmContainers, &MFrameContainers::containerSelected, this, &MPanelMedia::changeContainer);
    connect(m_btnSongs, &QRadioButton::released, this, [this]() {changeView(MPanelMedia::ViewAllSongs);});
    connect(m_btnArtists, &QRadioButton::released, this, [this]() {changeView(MPanelMedia::ViewArtists);});
    connect(m_btnAlbums, &QRadioButton::released, this, [this]() {changeView(MPanelMedia::ViewAlbums);});

    m_treeSettingsCollection = new MTreeSettingsCollection(this);
    connect(m_treeSettingsCollection, &MTreeSettingsCollection::aboutToSave,
            m_frmSonglist, &MFrameSonglist::saveTreeSettings);
}
void MPanelMedia::setLibrary(MMediaLibrary *library)
{
    if(m_mediaLibrary){
        disconnect(m_mediaLibrary, nullptr, this, nullptr);
    }

    m_mediaLibrary = library;

    m_frmContainers->model()->setLibrary(m_mediaLibrary);
    m_frmSonglist->model()->setLibrary(m_mediaLibrary);

    disconnect(m_mediaLibrary, nullptr, this, nullptr);
    connect(m_mediaLibrary, &MMediaLibrary::aboutToReset, this, [this]() {changeView(MPanelMedia::ViewAllSongs);});
    connect(m_mediaLibrary, &MMediaLibrary::completedLoading, this, [this]() {changeView(MPanelMedia::ViewAllSongs);});
}
void MPanelMedia::load(QSettings *settings)
{
    m_frmContainers->tree()->expandAll(); // TODO: save expanded folders

    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    foreach(MPlaylist *playlist, m_mediaLibrary->playlists()) {
        pidlist.append(playlist->pid());
    }
    foreach(MFolder *folder, m_mediaLibrary->folders()) {
        pidlist.append(folder->pid());
    }

    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->load(settings, pidlist);
    QString pidSonglist = settings->value("songlist").toString();
    settings->endGroup();

    if(pidSonglist != m_mediaLibrary->pid() && pidlist.contains(pidSonglist)) {
        MMediaContainer *container = m_mediaLibrary->getContainer(pidSonglist);
        changeView(MPanelMedia::ViewContainer, container);
    }
    else {
        changeView(MPanelMedia::ViewAllSongs); // TODO: save previous view
    }

}
void MPanelMedia::save(QSettings *settings)
{
    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    foreach(MPlaylist *playlist, m_mediaLibrary->playlists()) {
        pidlist.append(playlist->pid());
    }
    foreach(MFolder *folder, m_mediaLibrary->folders()) {
        pidlist.append(folder->pid());
    }

    // TODO: implement save method per each panel
    // TODO: eg. m_panelMedia->save(settings);
    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->save(settings, pidlist);
    settings->setValue("songlist", m_frmSonglist->container()->pid());
    settings->endGroup();
}

MPanelMedia::View MPanelMedia::currentView() const
{
    return m_currentView;
}
MPanelMedia::View MPanelMedia::previousView() const
{
    return m_previousView;
}
void MPanelMedia::changeView(MPanelMedia::View view, MMediaContainer *container)
{
    m_frmContainers->tree()->selectionModel()->blockSignals(true);

    m_previousView = m_currentView;
    m_currentView = view;

    if(m_currentView == MPanelMedia::ViewContainer && !container){
        m_currentView = MPanelMedia::ViewAllSongs;
    }

    switch(m_currentView) {

        case MPanelMedia::ViewAllSongs: {
            m_btnSongs->toggle();
            m_frmContainers->tree()->clearSelection();
            setContainer(m_mediaLibrary, "Library");
            break;
        }
        case MPanelMedia::ViewArtists: {
            m_btnArtists->toggle();
            m_frmContainers->tree()->clearSelection();
            setContainer(m_mediaLibrary, "Artists");
            break;
        }
        case MPanelMedia::ViewAlbums: {
            m_btnAlbums->toggle();
            m_frmContainers->tree()->clearSelection();
            setContainer(m_mediaLibrary, "Albums");
            break;
        }
        case MPanelMedia::ViewContainer: {
            m_btnSongs->setAutoExclusive(false);
            m_btnArtists->setAutoExclusive(false);
            m_btnAlbums->setAutoExclusive(false);

            m_btnSongs->setChecked(false);
            m_btnArtists->setChecked(false);
            m_btnAlbums->setChecked(false);

            m_btnSongs->setAutoExclusive(true);
            m_btnArtists->setAutoExclusive(true);
            m_btnAlbums->setAutoExclusive(true);

            QModelIndex idx = m_frmContainers->model()->getIndex(container->pid());
            m_frmContainers->tree()->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::ClearAndSelect);

            setContainer(container);
            break;
        }
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

void MPanelMedia::changeContainer(MMediaContainer *container)
{
    if(container){
        changeView(MPanelMedia::ViewContainer, container);
    }
    else {
        changeView(MPanelMedia::ViewAllSongs);
    }
}
void MPanelMedia::setContainer(MMediaContainer *container, const QString &title)
{
    if(title.isEmpty()){
        m_lblView->setText(container->name());
    }
    else {
        m_lblView->setText(title);
    }

    MTreeSettings *treeSettings = m_treeSettingsCollection->getContainer(container->pid());
    if(!treeSettings){
        treeSettings = m_treeSettingsCollection->addContainer(container->pid());
    }

    m_frmSonglist->setTreeSettings(treeSettings);
    m_frmSonglist->setContainer(container);
}
