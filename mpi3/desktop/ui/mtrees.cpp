#include "mtrees.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QFont>
#include <QSpacerItem>


SongModel::SongModel(QObject *parent) : QStandardItemModel(parent)
{
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(column));
            this->setItem(row, column, item);
        }
    }


    QStringList headers;
    headers.append("Header 1");
    headers.append("Header 2");
    headers.append("Header 3");
    headers.append("Header 4");

    this->setHorizontalHeaderLabels(headers);


//    QStandardItemModel model;
//    QStandardItem *parentItem = model.invisibleRootItem();
//    for (int i = 0; i < 4; ++i) {
//        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
//        parentItem->appendRow(item);
//        parentItem = item;
//    }
}


SongModel::~SongModel()
{

}

// ----------------------------------------------------------------------------------------------------
// * LibraryTreeview *
// ----------------------------------------------------------------------------------------------------
LibraryTreeview::LibraryTreeview(QWidget *parent) : QTreeView(parent)
{
    this->setObjectName("LibraryTreeview");

    modelSongs = new SongModel(this);

    this->setModel(modelSongs);

    this->setColumnHidden(0, true);

    this->setAlternatingRowColors(true);

}

LibraryTreeview::~LibraryTreeview()
{

}


// ----------------------------------------------------------------------------------------------------
// * PanelTrees *
// ----------------------------------------------------------------------------------------------------
PanelTrees::PanelTrees(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("PanelTrees");

    lblTreeview = new QLabel(this);
    treeLibrary = new LibraryTreeview(this);

    QFont font;
    font.setFamily("Helvetica");
    font.setPointSize(12);
    lblTreeview->setFont(font);
    lblTreeview->setText("Treeview");


    QGridLayout *layoutMain = new QGridLayout;


    QSpacerItem *s = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding);

    layoutMain->addItem(s, 0, 0, 1, 1);

    layoutMain->addWidget(lblTreeview, 0, 1, 1, 1);

    layoutMain->addWidget(treeLibrary, 1, 0, 1, 2);

    layoutMain->setRowStretch(1, 1);
    layoutMain->setColumnStretch(1, 1);

    layoutMain->setRowMinimumHeight(0, 40);
    layoutMain->setMargin(0);
//    layoutMain->setHorizontalSpacing(0);
//    layoutMain->setVerticalSpacing(0);


    this->setLayout(layoutMain);
}

PanelTrees::~PanelTrees()
{

}

void PanelTrees::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);

    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}
