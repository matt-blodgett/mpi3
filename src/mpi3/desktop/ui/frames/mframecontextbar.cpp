#include "mpi3/desktop/ui/frames/mframecontextbar.h"
#include "mpi3/desktop/ui/mstyle.h"

#include <QGridLayout>
#include <QRadioButton>


#include <QDebug>


MFrameContextBar::MFrameContextBar(QWidget *parent) : MFrame(parent)
{
    // -------------------------------------------------- CONTEXT BAR

    m_btnMedia = new QRadioButton(this);
    m_btnLibrary = new QRadioButton(this);
    m_btnDevice = new QRadioButton(this);

    QGridLayout *gridMain = new QGridLayout(this);
    gridMain->addWidget(m_btnMedia, 1, 1, 1, 1);
    gridMain->addWidget(m_btnLibrary, 1, 2, 1, 1);
    gridMain->addWidget(m_btnDevice, 1, 3, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setColumnStretch(4, 1);
    gridMain->setRowMinimumHeight(0, 1);
    gridMain->setRowMinimumHeight(1, 28);
    gridMain->setRowMinimumHeight(2, 1);
    gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(gridMain);

    m_btnMedia->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnLibrary->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_btnDevice->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    m_btnMedia->setFixedHeight(22);
    m_btnLibrary->setFixedHeight(22);
    m_btnDevice->setFixedHeight(22);

    m_btnMedia->setFixedWidth(100);
    m_btnLibrary->setFixedWidth(100);
    m_btnDevice->setFixedWidth(100);

    // -------------------------------------------------- STATIC TEXT

    m_btnMedia->setText("Media");
    m_btnLibrary->setText("Library");
    m_btnDevice->setText("Device");

    // -------------------------------------------------- OBJECT NAMES

    m_btnMedia->setStyle(new MProxyStyle(m_btnMedia->style()));
    m_btnLibrary->setStyle(new MProxyStyle(m_btnLibrary->style()));
    m_btnDevice->setStyle(new MProxyStyle(m_btnDevice->style()));

    connect(m_btnMedia, &QRadioButton::released, this, [this]() {changeView(MFrameContextBar::ViewMedia);});
    connect(m_btnLibrary, &QRadioButton::released, this, [this]() {changeView(MFrameContextBar::ViewLibrary);});
    connect(m_btnDevice, &QRadioButton::released, this, [this]() {changeView(MFrameContextBar::ViewDevice);});
}

MFrameContextBar::View MFrameContextBar::currentView() const
{
    return m_currentView;
}
void MFrameContextBar::changeView(MFrameContextBar::View view)
{
    m_currentView = view;

    switch(m_currentView) {
        case MFrameContextBar::ViewMedia: {
            m_btnMedia->toggle();
            break;
        }
        case MFrameContextBar::ViewLibrary: {
            m_btnLibrary->toggle();
            break;
        }
        case MFrameContextBar::ViewDevice: {
            m_btnDevice->toggle();
            break;
        }
    }

    emit contextPanelChanged();
}



