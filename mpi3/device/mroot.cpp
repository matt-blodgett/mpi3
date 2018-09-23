#include "mroot.h"

//#include "core/mmedialibrary.h"
//#include "core/maudioengine.h"

//#include "util/msettings.h"
//#include "util/mstyle.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QPainter>

#include <QPushButton>


#include <QDebug>


MRootDevice::MRootDevice(QWidget *parent): QMainWindow(parent){}
MRootDevice::~MRootDevice(){
//    m_audioEngine->stop();
//    delete m_audioEngine;
//    delete m_styleSheet;
//    delete m_mediaLibrary;
//    Mpi3::external_libs_deinit();
}

void MRootDevice::initialize(){
//    Mpi3::external_libs_init();
    initializeObjects();
    initializeLayout();
    initializeState();
    initializeStyle();
    centralWidget()->show();
}

void MRootDevice::initializeObjects(){
    m_testButton = new QPushButton(this);
}
void MRootDevice::initializeLayout(){
    QWidget *windowMain = new QWidget(this);
    QGridLayout *gridMain = new QGridLayout();
    gridMain->addWidget(m_testButton, 0, 0, 1, 1);
    gridMain->setRowStretch(2, 1);
    gridMain->setHorizontalSpacing(0);
    gridMain->setVerticalSpacing(0);
    gridMain->setMargin(0);
    windowMain->setLayout(gridMain);

    m_testButton->setText("close");
    connect(m_testButton, &QPushButton::released, this, [this](){this->close();});

    setContentsMargins(1, 1, 1, 1);
    setCentralWidget(windowMain);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    showFullScreen();
}
void MRootDevice::initializeState(){

}
void MRootDevice::initializeStyle(){

}
void MRootDevice::saveSettings(){

}

void MRootDevice::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}
void MRootDevice::closeEvent(QCloseEvent *event){
    saveSettings();
    QMainWindow::closeEvent(event);
}



















