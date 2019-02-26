#include "mpanel.h"

#include <QGridLayout>
#include <QSplitter>
#include <QScrollArea>
#include <QLabel>

#include "mstyle.h"


#include <QDebug>


static const int WidthSplit = 400;
static const int WidthSeparator = 10;
static const int WidthSection = (WidthSplit * 2) + WidthSeparator;


MPanelSection::MPanelSection(QWidget *parent) : MFrame(parent)
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

    MStyle::setStyle(m_lblHeader, MStyle::LBL_Header);
    m_frmBody->setObjectName("PanelSection");
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


MPanel::MPanel(QWidget *parent, bool sectioned) : MFrame(parent)
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

    if(sectioned){
        m_lblPanelTitle = new QLabel(this);
        m_frmScrollArea = new QScrollArea(this);

        MStyle::setStyle(m_lblPanelTitle, MStyle::LBL_Title);

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
void MPanel::setLibrary(MMediaLibrary *library)
{
    Q_UNUSED(library)
}
void MPanel::load(QSettings *settings)
{
    Q_UNUSED(settings)
}
void MPanel::save(QSettings *settings)
{
    Q_UNUSED(settings)
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

// TODO: static class variable with panel width
void MPanel::showEvent(QShowEvent *event)
{

    m_frmSplitter->setSizes({180, width()-180});

    foreach(MPanelSection *panel, findChildren<MPanelSection*>()) {
        panel->setFixedHeight(panel->height());
    }

    QWidget::showEvent(event);
}























