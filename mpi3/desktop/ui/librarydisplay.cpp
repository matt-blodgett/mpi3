#include "librarydisplay.h"
#include "mvc/libview.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QHeaderView>
#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QLabel>


PanelLibrary::PanelLibrary(QWidget *parent) : QWidget(parent){
    frm_library = new QSplitter(this);

    frm_views = new QWidget(this);
    frm_trees = new QWidget(this);

    btn_songs = new QRadioButton(this);
    btn_artists = new QRadioButton(this);
    btn_albums = new QRadioButton(this);

    lbl_playlist = new QLabel(this);
    lbl_view = new QLabel(this);

    tree_songlist = new Mpi3TreeView(this);
    tree_containers = new Mpi3TreeView(this);

    QGridLayout *layoutViews = new QGridLayout();
    layoutViews->addWidget(btn_songs, 0, 0, 1, 1);
    layoutViews->addWidget(btn_artists, 1, 0, 1, 1);
    layoutViews->addWidget(btn_albums, 2, 0, 1, 1);
    layoutViews->setRowMinimumHeight(3, 6);
    layoutViews->addWidget(lbl_playlist, 4, 0, 1, 1);
    layoutViews->addWidget(tree_containers, 5, 0, 1, 1);
    layoutViews->setRowStretch(5, 1);
    layoutViews->setVerticalSpacing(0);
    layoutViews->setHorizontalSpacing(0);
    layoutViews->setMargin(0);

    frm_views->setLayout(layoutViews);
    frm_views->setMinimumWidth(100);
    frm_views->setMaximumWidth(400);

    QGridLayout *layoutTrees = new QGridLayout();
    layoutTrees->addWidget(lbl_view, 0, 1, 1, 1);
    layoutTrees->addWidget(tree_songlist, 1, 0, 1, 2);
    layoutTrees->setRowStretch(1, 1);
    layoutTrees->setColumnStretch(1, 1);
    layoutTrees->setRowMinimumHeight(0, 60);
    layoutTrees->setColumnMinimumWidth(0, 8);
    layoutTrees->setHorizontalSpacing(0);
    layoutTrees->setVerticalSpacing(0);
    layoutTrees->setMargin(0);

    frm_trees->setLayout(layoutTrees);

    QGridLayout *layoutMain = new QGridLayout();
    layoutMain->addWidget(frm_library);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);

    setLayout(layoutMain);
    frm_library->addWidget(frm_views);
    frm_library->addWidget(frm_trees);
    frm_library->setHandleWidth(0);
    frm_library->setChildrenCollapsible(false);
    frm_library->setOrientation(Qt::Horizontal);

    btn_songs->setText("Songs");
    btn_artists->setText("Artists");
    btn_albums->setText("Albums");
    lbl_playlist->setText("Playlists");

    tree_containers->setDisplayStyle(Mpi3TreeView::DisplayStyleContainers);
    tree_songlist->setDisplayStyle(Mpi3TreeView::DisplayStyleSonglist);

    connect(btn_songs, &QRadioButton::released, this, [this](){changeView(PanelLibrary::ViewAllSongs);});
    connect(btn_artists, &QPushButton::released, this, [this](){changeView(PanelLibrary::ViewArtists);});
    connect(btn_albums, &QPushButton::released, this, [this](){changeView(PanelLibrary::ViewAlbums);});
    connect(tree_containers, &QTreeView::clicked, this, &PanelLibrary::containerClicked);

    frm_views->setObjectName("PanelViews");
    frm_trees->setObjectName("PanelTrees");
    btn_songs->setObjectName("PanelViewsButton");
    btn_artists->setObjectName("PanelViewsButton");
    btn_albums->setObjectName("PanelViewsButton");
    lbl_view->setObjectName("PanelTreesTitle");
    lbl_playlist->setObjectName("PanelViewsLabel");
    tree_songlist->setObjectName("LibraryTreeview");
    tree_containers->setObjectName("PlaylistsTreeview");
    tree_songlist->header()->setObjectName("LibraryTreeviewHeader");
    tree_containers->header()->setObjectName("PlaylistsTreeviewHeader");
}
PanelLibrary::~PanelLibrary(){}

PanelLibrary::View PanelLibrary::currentView() const{
    return m_currentView;
}
void PanelLibrary::setDisplay(const QString &title){
    lbl_view->setText(title);
}

void PanelLibrary::changeView(PanelLibrary::View view){
    m_currentView = view;

    switch(m_currentView){

        case PanelLibrary::ViewAllSongs: {
            btn_songs->toggle();
            tree_containers->clearSelection();
            break;
        }
        case PanelLibrary::ViewArtists: {
            btn_artists->toggle();
            tree_containers->clearSelection();
            break;
        }
        case PanelLibrary::ViewAlbums: {
            btn_albums->toggle();
            tree_containers->clearSelection();
            break;
        }
        case PanelLibrary::ViewContainer: {
            btn_songs->setAutoExclusive(false);
            btn_artists->setAutoExclusive(false);
            btn_albums->setAutoExclusive(false);

            btn_songs->setChecked(false);
            btn_artists->setChecked(false);
            btn_albums->setChecked(false);

            btn_songs->setAutoExclusive(true);
            btn_artists->setAutoExclusive(true);
            btn_albums->setAutoExclusive(true);

            break;
        }
    }

    emit viewChanged();
}
void PanelLibrary::containerClicked(const QModelIndex &index){
    if(index.isValid()){
        changeView(PanelLibrary::ViewContainer);
    }
}

void PanelLibrary::showEvent(QShowEvent *event){
    frm_library->setSizes({180, width()-180});
    QWidget::showEvent(event);
}
void PanelLibrary::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

