#include "mcontextpanel.h"

#include <QStyleOption>
#include <QPainter>
#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QPushButton>
#include <QTreeView>
#include <QHeaderView>
#include <QScrollBar>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>


static const int WidthSplit = 400;
static const int WidthSeparator = 10;
static const int WidthSection = (WidthSplit * 2) + WidthSeparator;


MPanelSection::MPanelSection(QWidget *parent) : QWidget(parent){
    m_lblHeader = new QLabel(this);
    m_frmBody = new QWidget(this);

    m_gridNorth = new QGridLayout();
    m_gridSouth = new QGridLayout();
    m_gridEast = new QGridLayout();
    m_gridWest = new QGridLayout();

    m_gridBody = new QGridLayout();
    m_gridBody->addLayout(m_gridNorth, 0, 0, 1, 3);
    m_gridBody->addLayout(m_gridEast, 1, 2, 1, 1);
    m_gridBody->addLayout(m_gridWest, 1, 0, 1, 1);
    m_gridBody->addLayout(m_gridSouth, 2, 0, 1, 3);
    m_gridBody->setColumnMinimumWidth(0, WidthSplit);
    m_gridBody->setColumnMinimumWidth(1, WidthSeparator);
    m_gridBody->setColumnMinimumWidth(2, WidthSplit);
    m_gridBody->setMargin(16);
    m_frmBody->setLayout(m_gridBody);
    m_frmBody->setFixedWidth(WidthSection);

    m_gridSection = new QGridLayout();
    m_gridSection->addWidget(m_lblHeader, 0, 0, 1, 1);
    m_gridSection->addWidget(m_frmBody, 1, 0, 1, 1);
    m_gridSection->setMargin(0);
    setLayout(m_gridSection);

    m_lblHeader->setObjectName("SectionHeader");
    m_frmBody->setObjectName("SectionFrame");
}

QGridLayout *MPanelSection::gridBody(){
    return m_gridBody;
}
QGridLayout *MPanelSection::gridNorth(){
    return m_gridNorth;
}
QGridLayout *MPanelSection::gridSouth(){
    return m_gridSouth;
}
QGridLayout *MPanelSection::gridEast(){
    return m_gridEast;
}
QGridLayout *MPanelSection::gridWest(){
    return m_gridWest;
}

QString MPanelSection::header() const {
    return m_lblHeader->text();
}
void MPanelSection::setHeader(const QString &header){
    m_lblHeader->setText(header);
}

MPanelContext::MPanelContext(QWidget *parent) : QWidget(parent){
    m_frmSplitter = new QSplitter(this);
    m_frmControl = new QWidget(this);
    m_frmDisplay = new QWidget(this);
    m_frmScrollArea = new QScrollArea(this);
    m_lblPanelTitle = new QLabel(this);

    m_frmScrollArea->setWidget(m_frmDisplay);
    m_frmSplitter->addWidget(m_frmControl);
    m_frmSplitter->addWidget(m_frmScrollArea);

    m_gridMain = new QGridLayout();
    m_gridMain->addWidget(m_frmSplitter);
    m_gridMain->setMargin(0);
    setLayout(m_gridMain);

    m_gridControl = new QGridLayout();
    m_frmControl->setLayout(m_gridControl);

    m_gridDisplay = new QGridLayout();
    m_gridDisplay->addWidget(m_lblPanelTitle, 0, 0, 1, 1);
    m_gridDisplay->setColumnMinimumWidth(0, WidthSection);
    m_gridDisplay->setColumnStretch(0, 1);
    m_gridDisplay->setRowStretch(0, 1);
    m_gridDisplay->setMargin(20);
    m_frmDisplay->setLayout(m_gridDisplay);

    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setStretchFactor(1, 1);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);
    m_frmControl->setMinimumWidth(120);
    m_frmControl->setMaximumWidth(400);
    m_frmScrollArea->setWidgetResizable(true);
    m_lblPanelTitle->setFixedWidth(WidthSection);

    m_lblPanelTitle->setObjectName("PanelTitle");
    m_frmDisplay->setObjectName("PanelDisplay");
    m_frmControl->setObjectName("PanelControl");
    m_frmScrollArea->setObjectName("PanelScrollArea");
}

QGridLayout *MPanelContext::gridMain(){
    return m_gridMain;
}
QGridLayout *MPanelContext::gridControl(){
    return m_gridControl;
}
QGridLayout *MPanelContext::gridDisplay(){
    return m_gridDisplay;
}

QString MPanelContext::title() const {
    return m_lblPanelTitle->text();
}
void MPanelContext::setTitle(const QString &title){
    m_lblPanelTitle->setText(title);
}

MPanelSection *MPanelContext::addSection(){
    MPanelSection *section = new MPanelSection(this);

    int row = m_gridDisplay->rowCount();
    m_gridDisplay->addWidget(section, row, 0, 1, 1);
    m_gridDisplay->setRowStretch(row + 1, 1);

    for(int i = 0; i < row; i++){
        m_gridDisplay->setRowStretch(i, 0);
    }

    return section;
}
MPanelSection *MPanelContext::addSection(const QString &header){
    MPanelSection *section = addSection();
    section->setHeader(header);
    return section;
}

QLabel *MPanelContext::addLabelTag(){
    QLabel *label = new QLabel(this);
    label->setObjectName("PanelTag");
    return label;
}
QLabel *MPanelContext::addLabelValue(){
    QLabel *label = new QLabel(this);
    label->setObjectName("PanelValue");
    return label;
}
QLineEdit *MPanelContext::addLineEdit(){
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName("PanelEdit");
    return edit;
}
QLineEdit *MPanelContext::addLineEditHidden(){
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName("PanelEditHidden");
    return edit;
}
QPushButton *MPanelContext::addButton(){
    QPushButton *button = new QPushButton(this);
    button->setObjectName("PanelButton");
    button->setFixedWidth(120);
    return button;
}
QTreeView *MPanelContext::addTreeView(){
    QTreeView *tree = new QTreeView(this);
    tree->setObjectName("PanelTree");
    tree->header()->setObjectName("PanelTreeHeader");
    tree->verticalScrollBar()->setObjectName("TreeviewScrollbar");
    tree->horizontalScrollBar()->setObjectName("TreeviewScrollbar");
    return tree;
}
QCheckBox *MPanelContext::addCheckBox(){
    QCheckBox *check = new QCheckBox(this);
    check->setObjectName("PanelCheck");
    return check;
}
QComboBox *MPanelContext::addComboBox(){
    QComboBox *combo = new QComboBox(this);
    combo->setObjectName("PanelCombo");
    return combo;
}

void MPanelContext::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanelContext::showEvent(QShowEvent *event){
    m_frmSplitter->setSizes({180, width()-180});
    QWidget::showEvent(event);
}






















