#include "mpanelmedia.h"
#include "mvc/mlibview.h"
#include "util/mstyle.h"

#include <QGridLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>


#include <QDebug>


MPanelMedia::MPanelMedia(QWidget *parent) : MPanel(parent){
    initializeLayoutType(false);
    initializeLayout();

    m_btnSongs->setText("Songs");
    m_btnArtists->setText("Artists");
    m_btnAlbums->setText("Albums");
    m_lblPlaylist->setText("Playlists");

    m_lblView->setObjectName("PanelTitle");
    m_treeSonglist->setObjectName("TreeviewSonglist");
    m_treeContainers->setObjectName("TreeviewContainers");
    m_lblPlaylist->setStyleSheet("QLabel {border-top: 1px solid #696969;"
                                 "font-size: 14px; padding: 4px 2px 4px 2px;}");

    m_btnSongs->setStyle(new MStyle(m_btnSongs->style()));
    m_btnArtists->setStyle(new MStyle(m_btnArtists->style()));
    m_btnAlbums->setStyle(new MStyle(m_btnAlbums->style()));

    connect(m_btnSongs, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewAllSongs);});
    connect(m_btnArtists, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewArtists);});
    connect(m_btnAlbums, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewAlbums);});
}

void MPanelMedia::initializeLayout(){
    m_treeContainers = new MTreeContainers(this);
    m_treeSonglist = new MTreeSonglist(this);

    addTreeView(m_treeContainers);
    addTreeView(m_treeSonglist);

    m_btnSongs = addRadioButton();
    m_btnArtists = addRadioButton();
    m_btnAlbums = addRadioButton();
    m_lblPlaylist = addLabelTag();
    m_lblView = addLabelTag();

    m_btnSongs->setParent(this);
    m_btnArtists->setParent(this);
    m_btnAlbums->setParent(this);

    gridControl()->addWidget(m_btnSongs, 0, 0, 1, 1);
    gridControl()->addWidget(m_btnArtists, 1, 0, 1, 1);
    gridControl()->addWidget(m_btnAlbums, 2, 0, 1, 1);
    gridControl()->setRowMinimumHeight(3, 6);
    gridControl()->addWidget(m_lblPlaylist, 4, 0, 1, 1);
    gridControl()->addWidget(m_treeContainers, 5, 0, 1, 1);
    gridControl()->setRowStretch(5, 1);
    gridControl()->setVerticalSpacing(0);
    gridControl()->setHorizontalSpacing(0);
    gridControl()->setMargin(0);

    gridDisplay()->addWidget(m_lblView, 0, 1, 1, 1);
    gridDisplay()->addWidget(m_treeSonglist, 1, 0, 1, 2);
    gridDisplay()->setColumnMinimumWidth(0, 8);
    gridDisplay()->setRowMinimumHeight(0, 60);
    gridDisplay()->setColumnStretch(1, 1);
    gridDisplay()->setRowStretch(1, 1);
    gridDisplay()->setMargin(0);
}

MPanelMedia::View MPanelMedia::currentView() const{
    return m_currentView;
}
void MPanelMedia::changeView(MPanelMedia::View view){

    m_treeContainers->selectionModel()->blockSignals(true);

    m_currentView = view;
    switch(m_currentView){

        case MPanelMedia::ViewAllSongs: {
            m_btnSongs->toggle();
            m_treeContainers->clearSelection();
            break;
        }
        case MPanelMedia::ViewArtists: {
            m_btnArtists->toggle();
            m_treeContainers->clearSelection();
            break;
        }
        case MPanelMedia::ViewAlbums: {
            m_btnAlbums->toggle();
            m_treeContainers->clearSelection();
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

            break;
        }
    }

    m_treeContainers->selectionModel()->blockSignals(false);
    m_treeContainers->update();

    emit viewChanged();
}
void MPanelMedia::setDisplay(const QString &title){
    m_lblView->setText(title);
}

MTreeContainers *MPanelMedia::treeContainers(){
    return m_treeContainers;
}
MTreeSonglist *MPanelMedia::treeSonglist(){
    return m_treeSonglist;
}
