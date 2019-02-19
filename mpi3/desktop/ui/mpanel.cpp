#include "mpanel.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QRadioButton>
#include <QPushButton>
#include <QTreeView>
#include <QHeaderView>
#include <QScrollBar>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>


#include <QDebug>


static const int WidthSplit = 400;
static const int WidthSeparator = 10;
static const int WidthSection = (WidthSplit * 2) + WidthSeparator;


MPanelSection::MPanelSection(QWidget *parent) : QWidget(parent)
{
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

QGridLayout *MPanelSection::gridBody()
{
    return m_gridBody;
}
QGridLayout *MPanelSection::gridNorth()
{
    return m_gridNorth;
}
QGridLayout *MPanelSection::gridSouth()
{
    return m_gridSouth;
}
QGridLayout *MPanelSection::gridEast()
{
    return m_gridEast;
}
QGridLayout *MPanelSection::gridWest()
{
    return m_gridWest;
}

QString MPanelSection::header() const
{
    return m_lblHeader->text();
}
void MPanelSection::setHeader(const QString &header)
{
    m_lblHeader->setText(header);
}

MPanel::MPanel(QWidget *parent) : QWidget(parent)
{
    m_frmSplitter = new QSplitter(this);
    m_frmControl = new QWidget(this);
    m_frmDisplay = new QWidget(this);

    m_gridControl = new QGridLayout();
    m_frmControl->setLayout(m_gridControl);

    m_gridDisplay = new QGridLayout();
    m_frmDisplay->setLayout(m_gridDisplay);

    m_gridMain = new QGridLayout();
    m_gridMain->addWidget(m_frmSplitter);
    m_gridMain->setMargin(0);
    setLayout(m_gridMain);

    m_frmControl->setMinimumWidth(120);
    m_frmControl->setMaximumWidth(400);

    m_frmSplitter->addWidget(m_frmControl);
    m_frmSplitter->setHandleWidth(0);
    m_frmSplitter->setStretchFactor(1, 1);
    m_frmSplitter->setChildrenCollapsible(false);
    m_frmSplitter->setOrientation(Qt::Horizontal);

    m_frmDisplay->setObjectName("PanelDisplay");
    m_frmControl->setObjectName("PanelControl");
}

QGridLayout *MPanel::gridMain()
{
    return m_gridMain;
}
QGridLayout *MPanel::gridControl()
{
    return m_gridControl;
}
QGridLayout *MPanel::gridDisplay()
{
    return m_gridDisplay;
}

void MPanel::initializeLayoutType(bool sectioned)
{
    if(sectioned) {
        m_lblPanelTitle = new QLabel(this);
        m_frmScrollArea = new QScrollArea(this);

        m_lblPanelTitle->setObjectName("PanelTitle");
        m_frmScrollArea->setObjectName("PanelScrollArea");

        m_frmScrollArea->setWidget(m_frmDisplay);
        m_frmSplitter->addWidget(m_frmScrollArea);

        m_gridDisplay->addWidget(m_lblPanelTitle, 0, 0, 1, 1);
        m_gridDisplay->setAlignment(m_lblPanelTitle, Qt::AlignCenter);
        m_gridDisplay->setColumnMinimumWidth(0, WidthSection);
        m_gridDisplay->setColumnStretch(0, 1);
        m_gridDisplay->setRowStretch(0, 1);
        m_gridDisplay->setMargin(20);

        m_lblPanelTitle->setFixedWidth(WidthSection);
        m_frmScrollArea->setWidgetResizable(true);
    }
    else {
        m_frmSplitter->addWidget(m_frmDisplay);
    }
}

QString MPanel::title() const
{
    return m_lblPanelTitle->text();
}
void MPanel::setTitle(const QString &title)
{
    m_lblPanelTitle->setText(title);
}

MPanelSection *MPanel::addSection()
{
    MPanelSection *section = new MPanelSection(this);

    int row = m_gridDisplay->rowCount();
    m_gridDisplay->addWidget(section, row, 0, 1, 1);
    m_gridDisplay->setRowStretch(row + 1, 1);

    for(int i = 0; i < row; i++) {
        m_gridDisplay->setRowStretch(i, 0);
    }

    return section;
}
MPanelSection *MPanel::addSection(const QString &header)
{
    MPanelSection *section = addSection();
    section->setHeader(header);
    return section;
}

QLabel *MPanel::addLabelTag()
{
    QLabel *label = new QLabel(this);
    label->setObjectName("PanelTag");
    return label;
}
QLabel *MPanel::addLabelValue()
{
    QLabel *label = new QLabel(this);
    label->setObjectName("PanelValue");
    return label;
}
QLineEdit *MPanel::addLineEdit()
{
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName("PanelEdit");
    return edit;
}
QLineEdit *MPanel::addLineEditHidden()
{
    QLineEdit *edit = new QLineEdit(this);
    edit->setObjectName("PanelEditHidden");
    return edit;
}
QPushButton *MPanel::addPushButton()
{
    QPushButton *push = new QPushButton(this);
    push->setObjectName("PanelPushButton");
    push->setFixedWidth(120);
    return push;
}
QTreeView *MPanel::addTreeView()
{
    QTreeView *tree = new QTreeView(this);
    addTreeView(tree);
    return tree;
}
QCheckBox *MPanel::addCheckBox()
{
    QCheckBox *check = new QCheckBox(this);
    check->setObjectName("PanelCheck");
    return check;
}
QComboBox *MPanel::addComboBox()
{
    QComboBox *combo = new QComboBox(this);
    combo->setObjectName("PanelCombo");
    return combo;
}
QRadioButton *MPanel::addRadioButton()
{
    QRadioButton *radio = new QRadioButton(this);
    radio->setObjectName("PanelRadioButton");
    return radio;
}

void MPanel::addTreeView(QTreeView *tree)
{
    tree->setObjectName("PanelTree");
    tree->header()->setObjectName("PanelTreeHeader");
    tree->verticalScrollBar()->setObjectName("TreeviewScrollbar");
    tree->horizontalScrollBar()->setObjectName("TreeviewScrollbar");
}

void MPanel::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanel::showEvent(QShowEvent *event)
{
    m_frmSplitter->setSizes({180, width()-180});

    foreach(MPanelSection *panel, findChildren<MPanelSection*>()) {
        panel->setFixedHeight(panel->height());
    }

    QWidget::showEvent(event);
}






















