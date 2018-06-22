#include "mtrees.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>
#include <QFont>


// ----------------------------------------------------------------------------------------------------
// * LibraryTreeview *
// ----------------------------------------------------------------------------------------------------
LibraryTreeview::LibraryTreeview(QWidget *parent) : QTreeView(parent)
{
    this->setObjectName("LibraryTreeview");
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

    layoutMain->addWidget(lblTreeview, 0, 0, 1, 1);
    layoutMain->addWidget(treeLibrary, 1, 0, 1, 1);
    layoutMain->setMargin(0);

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
