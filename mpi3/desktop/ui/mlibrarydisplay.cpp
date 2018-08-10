#include "mlibrarydisplay.h"
#include "mvc/mlibview.h"
#include "util/mstyle.h"

#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QLabel>


MPanelLibrary::MPanelLibrary(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_btnSongs->setText("Songs");
    m_btnArtists->setText("Artists");
    m_btnAlbums->setText("Albums");
    m_lblPlaylist->setText("Playlists");

    m_btnSongs->setStyle(new MStyle(m_btnSongs->style()));
    m_btnArtists->setStyle(new MStyle(m_btnArtists->style()));
    m_btnAlbums->setStyle(new MStyle(m_btnAlbums->style()));

    connect(m_btnSongs, &QRadioButton::released, this, [this](){changeView(MPanelLibrary::ViewAllSongs);});
    connect(m_btnArtists, &QPushButton::released, this, [this](){changeView(MPanelLibrary::ViewArtists);});
    connect(m_btnAlbums, &QPushButton::released, this, [this](){changeView(MPanelLibrary::ViewAlbums);});
    connect(m_treeContainers, &QTreeView::clicked, this, &MPanelLibrary::containerClicked);

    m_frmViews->setObjectName("PanelViews");
    m_frmTrees->setObjectName("PanelTrees");
    m_btnSongs->setObjectName("PanelViewsButton");
    m_btnArtists->setObjectName("PanelViewsButton");
    m_btnAlbums->setObjectName("PanelViewsButton");
    m_lblView->setObjectName("PanelTreesTitle");
    m_lblPlaylist->setObjectName("PanelViewsLabel");
    m_treeSonglist->setObjectName("SonglistTreeview");
    m_treeContainers->setObjectName("ContainersTreeview");
    m_treeSonglist->header()->setObjectName("SonglistTreeviewHeader");
    m_treeContainers->header()->setObjectName("ContainersTreeviewHeader");
}
MPanelLibrary::~MPanelLibrary(){}

void MPanelLibrary::initializeLayout(){
    m_frmLibrary = new QSplitter(this);

    m_frmViews = new QWidget(this);
    m_frmTrees = new QWidget(this);

    m_btnSongs = new QRadioButton(this);
    m_btnArtists = new QRadioButton(this);
    m_btnAlbums = new QRadioButton(this);

    m_lblPlaylist = new QLabel(this);
    m_lblView = new QLabel(this);

    m_treeContainers = new MTreeContainers(this);
    m_treeSonglist = new MTreeSonglist(this);

    QGridLayout *layoutViews = new QGridLayout();
    layoutViews->addWidget(m_btnSongs, 0, 0, 1, 1);
    layoutViews->addWidget(m_btnArtists, 1, 0, 1, 1);
    layoutViews->addWidget(m_btnAlbums, 2, 0, 1, 1);
    layoutViews->setRowMinimumHeight(3, 6);
    layoutViews->addWidget(m_lblPlaylist, 4, 0, 1, 1);
    layoutViews->addWidget(m_treeContainers, 5, 0, 1, 1);
    layoutViews->setRowStretch(5, 1);
    layoutViews->setVerticalSpacing(0);
    layoutViews->setHorizontalSpacing(0);
    layoutViews->setMargin(0);

    m_frmViews->setLayout(layoutViews);
    m_frmViews->setMinimumWidth(100);
    m_frmViews->setMaximumWidth(400);

    QGridLayout *layoutTrees = new QGridLayout();
    layoutTrees->addWidget(m_lblView, 0, 1, 1, 1);
    layoutTrees->addWidget(m_treeSonglist, 1, 0, 1, 2);
    layoutTrees->setRowStretch(1, 1);
    layoutTrees->setColumnStretch(1, 1);
    layoutTrees->setRowMinimumHeight(0, 60);
    layoutTrees->setColumnMinimumWidth(0, 8);
    layoutTrees->setHorizontalSpacing(0);
    layoutTrees->setVerticalSpacing(0);
    layoutTrees->setMargin(0);

    m_frmTrees->setLayout(layoutTrees);

    QGridLayout *layoutMain = new QGridLayout();
    layoutMain->addWidget(m_frmLibrary);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);

    setLayout(layoutMain);
    m_frmLibrary->addWidget(m_frmViews);
    m_frmLibrary->addWidget(m_frmTrees);
    m_frmLibrary->setHandleWidth(0);
    m_frmLibrary->setChildrenCollapsible(false);
    m_frmLibrary->setOrientation(Qt::Horizontal);
}

MPanelLibrary::View MPanelLibrary::currentView() const{
    return m_currentView;
}
void MPanelLibrary::changeView(MPanelLibrary::View view){
    m_currentView = view;

    switch(m_currentView){

        case MPanelLibrary::ViewAllSongs: {
            m_btnSongs->toggle();
            m_treeContainers->clearSelection();
            break;
        }
        case MPanelLibrary::ViewArtists: {
            m_btnArtists->toggle();
            m_treeContainers->clearSelection();
            break;
        }
        case MPanelLibrary::ViewAlbums: {
            m_btnAlbums->toggle();
            m_treeContainers->clearSelection();
            break;
        }
        case MPanelLibrary::ViewContainer: {
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
void MPanelLibrary::setDisplay(const QString &title){
    m_lblView->setText(title);
}
void MPanelLibrary::containerClicked(const QModelIndex &index){
    if(index.isValid()){
        changeView(MPanelLibrary::ViewContainer);
    }
}

void MPanelLibrary::showEvent(QShowEvent *event){
    m_frmLibrary->setSizes({180, width()-180});
    QWidget::showEvent(event);
}
