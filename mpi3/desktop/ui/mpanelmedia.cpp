#include "mpanelmedia.h"
#include "mvc/mlibview.h"
#include "util/mstyle.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QLabel>
#include <QScrollBar>


MPanelMedia::MPanelMedia(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_btnSongs->setStyle(new MStyle(m_btnSongs->style()));
    m_btnArtists->setStyle(new MStyle(m_btnArtists->style()));
    m_btnAlbums->setStyle(new MStyle(m_btnAlbums->style()));

    connect(m_btnSongs, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewAllSongs);});
    connect(m_btnArtists, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewArtists);});
    connect(m_btnAlbums, &QRadioButton::released, this, [this](){changeView(MPanelMedia::ViewAlbums);});
    connect(m_treeContainers, &QTreeView::clicked, this, &MPanelMedia::containerClicked);
}
MPanelMedia::~MPanelMedia(){}

void MPanelMedia::initializeLayout(){

    // -------------------------------------------------- CONTROL PANEl

    QWidget *frmControl = new QWidget(this);

    m_btnSongs = new QRadioButton(this);
    m_btnArtists = new QRadioButton(this);
    m_btnAlbums = new QRadioButton(this);

    QLabel *lblPlaylist = new QLabel(this);
    m_treeContainers = new MTreeContainers(this);

    QGridLayout *layoutControl = new QGridLayout(this);
    layoutControl->addWidget(m_btnSongs, 0, 0, 1, 1);
    layoutControl->addWidget(m_btnArtists, 1, 0, 1, 1);
    layoutControl->addWidget(m_btnAlbums, 2, 0, 1, 1);
    layoutControl->setRowMinimumHeight(3, 6);
    layoutControl->addWidget(lblPlaylist, 4, 0, 1, 1);
    layoutControl->addWidget(m_treeContainers, 5, 0, 1, 1);
    layoutControl->setRowStretch(5, 1);
    layoutControl->setVerticalSpacing(0);
    layoutControl->setHorizontalSpacing(0);
    layoutControl->setMargin(0);
    frmControl->setLayout(layoutControl);
    frmControl->setMinimumWidth(120);
    frmControl->setMaximumWidth(400);

    // -------------------------------------------------- TREEVIEWS

    QWidget *frmTreeViews = new QWidget(this);

    m_lblView = new QLabel(this);
    m_treeSonglist = new MTreeSonglist(this);

    QGridLayout *layoutTrees = new QGridLayout(this);
    layoutTrees->addWidget(m_lblView, 0, 1, 1, 1);
    layoutTrees->addWidget(m_treeSonglist, 1, 0, 1, 2);
    layoutTrees->setRowStretch(1, 1);
    layoutTrees->setColumnStretch(1, 1);
    layoutTrees->setRowMinimumHeight(0, 60);
    layoutTrees->setColumnMinimumWidth(0, 8);
    layoutTrees->setHorizontalSpacing(0);
    layoutTrees->setVerticalSpacing(0);
    layoutTrees->setMargin(0);
    frmTreeViews->setLayout(layoutTrees);

    // -------------------------------------------------- MEDIA PANEl

    m_frmSplitter = new QSplitter(this);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmSplitter);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);
    setLayout(layoutMain);

    m_frmSplitter->addWidget(frmControl);
    m_frmSplitter->addWidget(frmTreeViews);
    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);
    m_frmSplitter->setStretchFactor(1, 1);

    // -------------------------------------------------- STATIC TEXT

    m_btnSongs->setText("Songs");
    m_btnArtists->setText("Artists");
    m_btnAlbums->setText("Albums");
    lblPlaylist->setText("Playlists");

    // -------------------------------------------------- OBJECT NAMES

    frmControl->setObjectName("PanelControl");
    frmTreeViews->setObjectName("PanelTrees");
    m_btnSongs->setObjectName("PanelTreesButton");
    m_btnArtists->setObjectName("PanelTreesButton");
    m_btnAlbums->setObjectName("PanelTreesButton");
    m_lblView->setObjectName("PanelTreesTitle");
    lblPlaylist->setObjectName("PanelTreesLabel");

    m_treeSonglist->setObjectName("TreeviewSonglist");
    m_treeContainers->setObjectName("TreeviewContainers");

    m_treeSonglist->header()->setObjectName("TreeviewSonglistHeader");
    m_treeContainers->header()->setObjectName("TreeviewContainersHeader");

    m_treeSonglist->verticalScrollBar()->setObjectName("TreeviewScrollbar");
    m_treeContainers->verticalScrollBar()->setObjectName("TreeviewScrollbar");

    m_treeSonglist->horizontalScrollBar()->setObjectName("TreeviewScrollbar");
    m_treeContainers->horizontalScrollBar()->setObjectName("TreeviewScrollbar");
}

MPanelMedia::View MPanelMedia::currentView() const{
    return m_currentView;
}
void MPanelMedia::changeView(MPanelMedia::View view){
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

void MPanelMedia::containerClicked(const QModelIndex &index){
    if(index.isValid()){
        changeView(MPanelMedia::ViewContainer);
    }
}

void MPanelMedia::showEvent(QShowEvent *event){
    m_frmSplitter->setSizes({180, width()-180});
    QWidget::showEvent(event);
}