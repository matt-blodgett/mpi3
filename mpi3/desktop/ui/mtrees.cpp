#include "mtrees.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QFont>


PanelTrees::PanelTrees(QWidget *parent)
    : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;

    lblTreeview = new QLabel(this);
    treeLibrary = new QTreeView(this);

    lblTreeview->setText("Treeview");

    QFont font;

    font.setFamily("Helvetica");
    font.setPointSize(12);

    lblTreeview->setFont(font);
    layout->addWidget(lblTreeview, 0, 0, 1, 1);
    layout->addWidget(treeLibrary, 1, 0, 1, 1);

    layout->setMargin(0);

//    this->setObjectName("panel");
//    this->setStyleSheet("QWidget#panel {background-color: #c669f4}");
    this->setLayout(layout);
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


