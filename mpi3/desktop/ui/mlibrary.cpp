#include "mlibrary.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPushButton>
#include <QRadioButton>
#include <QSplitter>
#include <QLabel>

#include <QTreeView>

#include <QDebug>


PanelLibrary::PanelLibrary(QWidget *parent) : QWidget(parent){
    frm_library = new QSplitter(this);
    frm_library->setOrientation(Qt::Horizontal);

    frm_views = new QWidget(this);
    frm_trees = new QWidget(this);

    QGridLayout *layoutViews = new QGridLayout;

    btn_songs = new QRadioButton(this);
    btn_artists = new QRadioButton(this);
    btn_playlists = new QRadioButton(this);
    tree_playlists = new QTreeView(this);

    btn_songs->setText("Songs");
    btn_artists->setText("Artists");
    btn_playlists->setText("Playlists");

    btn_songs->toggle();

    connect(btn_songs, &QRadioButton::released, this, [this](){changeView(PanelLibrary::Library);});
    connect(btn_artists, &QPushButton::released, this, [this](){changeView(PanelLibrary::Artists);});
    connect(btn_playlists, &QPushButton::released, this, [this](){changeView(PanelLibrary::Containers);});

    layoutViews->addWidget(btn_songs, 0, 0, 1, 1);
    layoutViews->addWidget(btn_artists, 1, 0, 1, 1);
    layoutViews->addWidget(btn_playlists, 2, 0, 1, 1);
    layoutViews->addWidget(tree_playlists, 3, 0, 1, 1);

    layoutViews->setRowStretch(3, 1);
    layoutViews->setContentsMargins(0, 0, 0, 0);
    layoutViews->setMargin(0);

    layoutViews->setVerticalSpacing(0);
    layoutViews->setHorizontalSpacing(0);

    frm_views->setLayout(layoutViews);

    QGridLayout *layoutTrees = new QGridLayout;

    lbl_view = new QLabel(this);
    tree_library = new QTreeView(this);
    tree_library->setObjectName("LibraryTreeview");
    tree_library->setAlternatingRowColors(true);

    tree_library->setIndentation(12);
    tree_playlists->setIndentation(12);


    QFont font;
    font.setFamily("Helvetica");
    font.setPointSize(12);
    lbl_view->setFont(font);
    lbl_view->setText("Treeview");

    QSpacerItem *s = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);
    layoutTrees->addItem(s, 0, 0, 1, 1);
    layoutTrees->addWidget(lbl_view, 0, 1, 1, 1);
    layoutTrees->addWidget(tree_library, 1, 0, 1, 2);
    layoutTrees->setRowStretch(1, 1);
    layoutTrees->setColumnStretch(1, 1);
    layoutTrees->setRowMinimumHeight(0, 40);
    layoutTrees->setMargin(0);
    frm_trees->setLayout(layoutTrees);

    frm_library->addWidget(frm_views);
    frm_library->addWidget(frm_trees);
    frm_library->setHandleWidth(0);

    frm_views->setMinimumWidth(100);
    frm_views->setMaximumWidth(400);

    frm_library->setChildrenCollapsible(false);
    frm_library->setSizes({120, 10000000});


    QGridLayout *layoutMain = new QGridLayout;
    layoutMain->addWidget(frm_library);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);

    this->setLayout(layoutMain);

    btn_songs->setObjectName("PanelViewsButton");
    btn_artists->setObjectName("PanelViewsButton");
    btn_playlists->setObjectName("PanelViewsButton");

    frm_views->setObjectName("PanelViews");
    frm_trees->setObjectName("PanelTrees");
    tree_playlists->setObjectName("PlaylistsTreeview");
    tree_library->setObjectName("LibraryTreeview");
}

PanelLibrary::~PanelLibrary(){}

PanelLibrary::View PanelLibrary::currentView(){
    return m_currentView;
}

void PanelLibrary::changeView(PanelLibrary::View view){
    m_currentView = view;
    emit viewChanged();
}

void PanelLibrary::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
