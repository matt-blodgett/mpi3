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

    // ?????
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

    connect(m_btnSongs, &QRadioButton::released, m_frmSonglist->model(), &MModelSonglist::viewAllSongs);
    connect(m_btnArtists, &QRadioButton::released, m_frmSonglist->model(), &MModelSonglist::viewArtists);
    connect(m_btnAlbums, &QRadioButton::released, m_frmSonglist->model(), &MModelSonglist::viewAlbums);
    connect(m_frmContainers, &MFrameContainers::containerSelected, m_frmSonglist->model(), &MModelSonglist::viewContainer);
    connect(m_frmSonglist->model(), &MModelSonglist::viewChanged, this, &MPanelMedia::viewChanged);

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
    m_frmContainers->tree()->expandAll(); // TODO: save expanded folders

    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    for(MContainer *c : m_mediaLibrary->containers()){
        pidlist.append(c->pid());
    }

    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->load(settings, pidlist);
    QString pidSonglist = settings->value("songlist").toString();
    settings->endGroup();

//    if(pidSonglist != m_mediaLibrary->pid() && pidlist.contains(pidSonglist)) {
////        MContainer *container = m_mediaLibrary->getContainer(pidSonglist);
////        changeView(MPanelMedia::ViewContainer, container);
//    }
//    else {
////        changeView(MPanelMedia::ViewAllSongs); // TODO: save previous view
//    }
}
void MPanelMedia::save(QSettings *settings)
{
    QStringList pidlist;
    pidlist.append(m_mediaLibrary->pid());
    for(MContainer *c : m_mediaLibrary->containers()){
        pidlist.append(c->pid());
    }

    // TODO: implement save method per each panel
    // TODO: eg. m_panelMedia->save(settings);
    settings->beginGroup("TreeViews");
    m_treeSettingsCollection->save(settings, pidlist);
    settings->setValue("songlist", m_frmSonglist->model()->currentPID());
    settings->endGroup();
}

void MPanelMedia::viewChanged()
{
    m_frmContainers->tree()->selectionModel()->blockSignals(true);

    QString pid = m_frmSonglist->model()->currentPID();
    MTreeSettings *treeSettings = m_treeSettingsCollection->getContainer(pid);
    if(!treeSettings) {
        treeSettings = m_treeSettingsCollection->addContainer(pid);
    }

    m_frmSonglist->setTreeSettings(treeSettings);

    switch(m_frmSonglist->model()->currentView()) {

        case MModelSonglist::ViewAllSongs: {
            m_btnSongs->toggle();
            m_lblView->setText("Library");
            m_frmContainers->tree()->clearSelection();
            break;
        }
        case MModelSonglist::ViewArtists: {
            m_btnArtists->toggle();
            m_lblView->setText("Artists");
            m_frmContainers->tree()->clearSelection();
            break;
        }
        case MModelSonglist::ViewAlbums: {
            m_btnAlbums->toggle();
            m_lblView->setText("Albums");
            m_frmContainers->tree()->clearSelection();
            break;
        }
        case MModelSonglist::ViewContainer: {
            m_btnSongs->setAutoExclusive(false);
            m_btnArtists->setAutoExclusive(false);
            m_btnAlbums->setAutoExclusive(false);

            m_btnSongs->setChecked(false);
            m_btnArtists->setChecked(false);
            m_btnAlbums->setChecked(false);

            m_btnSongs->setAutoExclusive(true);
            m_btnArtists->setAutoExclusive(true);
            m_btnAlbums->setAutoExclusive(true);

            MContainer *container = m_mediaLibrary->getContainer(pid);
            m_lblView->setText(container->name());

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
