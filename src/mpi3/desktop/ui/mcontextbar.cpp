#include "mcontextbar.h"
#include "mstyle.h"

#include <QGridLayout>
#include <QRadioButton>

#include <QStyleOption>
#include <QPainter>


#include <QDebug>


MContextBar::MContextBar(QWidget *parent) : QWidget(parent)
{
    initializeLayout();

    m_btnMedia->setStyle(new MStyle(m_btnMedia->style()));
    m_btnLibrary->setStyle(new MStyle(m_btnLibrary->style()));
    m_btnDevice->setStyle(new MStyle(m_btnDevice->style()));

    connect(m_btnMedia, &QRadioButton::released, this, [this]() {changeView(MContextBar::ViewMedia);});
    connect(m_btnLibrary, &QRadioButton::released, this, [this]() {changeView(MContextBar::ViewLibrary);});
    connect(m_btnDevice, &QRadioButton::released, this, [this]() {changeView(MContextBar::ViewDevice);});
}
MContextBar::~MContextBar()
{

}

void MContextBar::initializeLayout()
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
    gridMain->setHorizontalSpacing(0);
    gridMain->setVerticalSpacing(0);
    gridMain->setMargin(0);
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

    m_btnMedia->setObjectName("ButtonContextBar");
    m_btnLibrary->setObjectName("ButtonContextBar");
    m_btnDevice->setObjectName("ButtonContextBar");
    setObjectName("ContextBar");
}

MContextBar::View MContextBar::currentView() const
{
    return m_currentView;
}
void MContextBar::changeView(MContextBar::View view)
{
    m_currentView = view;

    switch(m_currentView) {
        case MContextBar::ViewMedia: {
            m_btnMedia->toggle();
            break;
        }
        case MContextBar::ViewLibrary: {
            m_btnLibrary->toggle();
            break;
        }
        case MContextBar::ViewDevice: {
            m_btnDevice->toggle();
            break;
        }
    }

    emit viewChanged();
}

void MContextBar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QWidget::paintEvent(event);
}



