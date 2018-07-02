#include "mlibrary.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPushButton>
#include <QSplitter>
#include <QLabel>

#include <QTreeView>


PanelLibrary::PanelLibrary(QWidget *parent) : QWidget(parent)
{
    frm_library = new QSplitter(this);
    frm_library->setOrientation(Qt::Horizontal);

    frm_views = new QWidget(this);
    frm_trees = new QWidget(this);

    QGridLayout *layoutViews = new QGridLayout;

    btn_songs = new QPushButton(this);
    btn_artists = new QPushButton(this);
    btn_playlists = new QPushButton(this);

    btn_songs->setText("Songs");
    btn_artists->setText("Artists");
    btn_playlists->setText("Playlists");

    layoutViews->addWidget(btn_songs, 0, 0, 1, 1);
    layoutViews->addWidget(btn_artists, 1, 0, 1, 1);
    layoutViews->addWidget(btn_playlists, 2, 0, 1, 1);

    layoutViews->setRowStretch(3, 1);
    layoutViews->setMargin(0);
    frm_views->setLayout(layoutViews);

    QGridLayout *layoutTrees = new QGridLayout;

    lbl_view = new QLabel(this);
    tree_library = new QTreeView(this);
    tree_library->setObjectName("LibraryTreeview");
    tree_library->setAlternatingRowColors(true);


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
    frm_library->setHandleWidth(8);


    QGridLayout *layoutMain = new QGridLayout;
    layoutMain->addWidget(frm_library);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setMargin(0);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);

    this->setLayout(layoutMain);

    frm_views->setObjectName("PanelViews");
    frm_trees->setObjectName("PanelTrees");
    tree_library->setObjectName("LibraryTreeview");
}

PanelLibrary::~PanelLibrary()
{

}

void PanelLibrary::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
