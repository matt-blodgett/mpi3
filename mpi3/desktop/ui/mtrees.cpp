#include "mtrees.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QFont>


PanelTrees::PanelTrees(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layoutMain = new QGridLayout;

    lblTreeview = new QLabel(this);
    treeLibrary = new QTreeView(this);

    lblTreeview->setText("Treeview");

    QFont font;

    font.setFamily("Helvetica");
    font.setPointSize(12);

    lblTreeview->setFont(font);
    layoutMain->addWidget(lblTreeview, 0, 0, 1, 1);
    layoutMain->addWidget(treeLibrary, 1, 0, 1, 1);


    layoutMain->setMargin(0);

//    this->setObjectName("panel");
//    this->setStyleSheet("QWidget#panel {background-color: #c669f4}");
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


