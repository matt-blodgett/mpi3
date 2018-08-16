#include "mpanellibrary.h"

#include <QStyleOption>
#include <QPainter>

#include <QGridLayout>
#include <QSplitter>
#include <QPushButton>
#include <QLabel>
#include <QScrollBar>
#include <QScrollArea>


MPanelLibrary::MPanelLibrary(QWidget *parent) : QWidget(parent){
    initializeLayout();

    m_btnLibImport->setAutoRepeatDelay(1);

    m_btnLibImport->setText("Import");

    m_lblPanelTitle->setText("Local Library");
    m_lblPofileHeader->setText("Profile");
    m_lblMediaHeader->setText("Media Files");
    m_lblBackupHeader->setText("Backups");


    m_frmDisplay->setObjectName("PanelLibrary");
    m_frmControl->setObjectName("PanelViews");
    m_btnLibImport->setObjectName("PanelLibraryButton");
    m_lblPanelTitle->setObjectName("PanelTitle");
    m_lblPofileHeader->setObjectName("SectionHeader");
    m_lblMediaHeader->setObjectName("SectionHeader");
    m_lblBackupHeader->setObjectName("SectionHeader");
    m_frmProfileSection->setObjectName("SectionFrame");
    m_frmMediaSection->setObjectName("SectionFrame");
    m_frmBackupSection->setObjectName("SectionFrame");
    setObjectName("PanelLibrary");
}
MPanelLibrary::~MPanelLibrary(){}

void MPanelLibrary::initializeLayout(){

    m_frmLibrary = new QSplitter(this);

    m_frmControl = new QWidget(this);
    m_btnLibImport = new QPushButton(this);

    m_frmDisplay = new QWidget(this);

    m_lblPanelTitle = new QLabel(this);
    m_lblPofileHeader = new QLabel(this);
    m_lblMediaHeader = new QLabel(this);
    m_lblBackupHeader = new QLabel(this);

    m_frmProfileSection = new QWidget(this);
    m_frmMediaSection = new QWidget(this);
    m_frmBackupSection = new QWidget(this);

//    QGridLayout *layoutProfile = new QGridLayout(this);

    QGridLayout *layoutControl = new QGridLayout(this);
    layoutControl->addWidget(m_btnLibImport, 0, 0, 1, 1);
    layoutControl->setRowStretch(1, 1);
    layoutControl->setVerticalSpacing(0);
    layoutControl->setHorizontalSpacing(0);
    layoutControl->setMargin(0);
    m_frmControl->setLayout(layoutControl);
    m_frmControl->setMinimumWidth(100);
    m_frmControl->setMaximumWidth(400);

//    QScrollBar *scrollbar = new QScrollBar(this);


    QGridLayout *layoutDisplay = new QGridLayout(this);
    layoutDisplay->addWidget(m_lblPanelTitle, 0, 0, 1, 2);
    layoutDisplay->addWidget(m_lblPofileHeader, 1, 0, 1, 2);
    layoutDisplay->addWidget(m_frmProfileSection, 2, 0, 1, 2);
    layoutDisplay->addWidget(m_lblMediaHeader, 3, 0, 1, 2);
    layoutDisplay->addWidget(m_frmMediaSection, 4, 0, 1, 2);
    layoutDisplay->addWidget(m_lblBackupHeader, 5, 0, 1, 2);
    layoutDisplay->addWidget(m_frmBackupSection, 6, 0, 1, 2);
    layoutDisplay->setRowMinimumHeight(0, 0);
    layoutDisplay->setRowMinimumHeight(1, 0);
    layoutDisplay->setRowMinimumHeight(2, 100);
    layoutDisplay->setRowMinimumHeight(3, 0);
    layoutDisplay->setRowMinimumHeight(4, 100);
    layoutDisplay->setRowMinimumHeight(5, 0);
    layoutDisplay->setRowMinimumHeight(6, 100);
    layoutDisplay->setRowStretch(7, 1);
    layoutDisplay->setColumnMinimumWidth(0, 300);
    layoutDisplay->setColumnMinimumWidth(1, 300);
    layoutDisplay->setColumnStretch(0, 1);
    layoutDisplay->setColumnStretch(1, 1);
    layoutDisplay->setHorizontalSpacing(0);
    layoutDisplay->setVerticalSpacing(0);
    layoutDisplay->setMargin(10);
    m_frmDisplay->setLayout(layoutDisplay);

    QGridLayout *layoutMain = new QGridLayout(this);
    layoutMain->addWidget(m_frmLibrary);
    layoutMain->setColumnStretch(0, 1);
    layoutMain->setRowStretch(0, 1);
    layoutMain->setHorizontalSpacing(0);
    layoutMain->setVerticalSpacing(0);
    layoutMain->setMargin(0);
    setLayout(layoutMain);


    QScrollArea *scrollarea = new QScrollArea(this);
    scrollarea->setWidget(m_frmDisplay);
    scrollarea->setWidgetResizable(true);

    layoutMain->setSpacing(0);
    layoutMain->setContentsMargins(0, 0, 0, 0);

    scrollarea->setLineWidth(0);
    scrollarea->setObjectName("PanelScrollArea");


    m_frmLibrary->addWidget(m_frmControl);
    m_frmLibrary->addWidget(scrollarea);
    m_frmLibrary->setHandleWidth(0);
    m_frmLibrary->setChildrenCollapsible(false);
    m_frmLibrary->setOrientation(Qt::Horizontal);
    m_frmLibrary->setStretchFactor(1, 1);
}

void MPanelLibrary::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MPanelLibrary::showEvent(QShowEvent *event){
    m_frmLibrary->setSizes({180, width()-180});
    QWidget::showEvent(event);
}
