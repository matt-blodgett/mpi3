#include "mmenubar.h"
#include "mstyle.h"

#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>

#include <QGridLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QSettings>


#include <QDebug>


#define MPI3_RESIZE_EDGE_WIDTH 4


MMenuBar::MMenuBar(QWidget *parent) : MFrame(parent)
{
    m_status = {false, false, false, false, false, false};
    m_wndParent = static_cast<QMainWindow*>(this->parent());
    m_btnMinimize = new QPushButton(this);
    m_btnMaximize = new QPushButton(this);
    m_btnDeMaximize = new QPushButton(this);
    m_btnClose = new QPushButton(this);

    QGridLayout *gridMenu = new QGridLayout();
    gridMenu->addWidget(m_wndParent->menuBar(), 0, 0, 1, 1);
    gridMenu->addWidget(m_btnMinimize, 0, 2, 1, 1);
    gridMenu->addWidget(m_btnMaximize, 0, 3, 1, 1);
    gridMenu->addWidget(m_btnDeMaximize, 0, 3, 1, 1);
    gridMenu->addWidget(m_btnClose, 0, 4, 1, 1);
    gridMenu->setColumnMinimumWidth(5, 4);
    gridMenu->setColumnStretch(0, 0);
    gridMenu->setColumnStretch(1, 1);
    gridMenu->setMargin(0);
    setLayout(gridMenu);
    m_btnDeMaximize->hide();

    MStyle::setStyle(m_btnMinimize, MStyle::PB_MinimizeWindow);
    MStyle::setStyle(m_btnMaximize, MStyle::PB_MaximizeWindow);
    MStyle::setStyle(m_btnDeMaximize, MStyle::PB_DeMaximizeWindow);
    MStyle::setStyle(m_btnClose, MStyle::PB_CloseWindow);

    connect(m_btnMinimize, &QPushButton::released, m_wndParent, &QMainWindow::showMinimized);
    connect(m_btnMaximize, &QPushButton::released, this, [this](){setMaximized(true);});
    connect(m_btnDeMaximize, &QPushButton::released, this, [this](){setMaximized(false);});
    connect(m_btnClose, &QPushButton::released, m_wndParent, &QMainWindow::close);

    m_wndParent->installEventFilter(this);
}
void MMenuBar::load(QSettings *settings)
{
    settings->beginGroup("WindowGeometry");
    QRect screenSize = QApplication::desktop()->availableGeometry(this);
    int d_rootx = (screenSize.width() / 2) - 400;
    int d_rooty = (screenSize.height() / 2) - 300;
    int wnd_rootx = settings->value("rootx", d_rootx).toInt();
    int wnd_rooty = settings->value("rooty", d_rooty).toInt();
    int wnd_width = settings->value("width", 800).toInt();
    int wnd_height = settings->value("height", 600).toInt();
    bool wnd_maximized = settings->value("maximized", false).toBool();
    settings->endGroup();

    if(wnd_maximized) {
        setMaximized(true);
    }
    else {
        m_wndParent->move(wnd_rootx, wnd_rooty);
        m_wndParent->resize(wnd_width, wnd_height);
    }
}
void MMenuBar::save(QSettings *settings)
{
    settings->beginGroup("WindowGeometry");
    settings->setValue("rootx", m_wndParent->x());
    settings->setValue("rooty", m_wndParent->y());
    settings->setValue("width", m_wndParent->width());
    settings->setValue("height", m_wndParent->height());
    settings->setValue("maximized", isMaximized());
    settings->endGroup();
}

bool MMenuBar::resizingEast()
{
    return m_status[0];
}
bool MMenuBar::resizingWest()
{
    return m_status[1];
}
bool MMenuBar::resizingNorth()
{
    return m_status[2];
}
bool MMenuBar::resizingSouth()
{
    return m_status[3];
}

bool MMenuBar::isMoving()
{
    return m_status[4];
}
bool MMenuBar::isMaximized()
{
    return m_status[5];
}
bool MMenuBar::isResizing()
{
    for(int i = 0; i <= 3; i++){
        if(m_status[i]){
            return true;
        }
    }

    return false;
}

void MMenuBar::setMoving(bool status)
{
    if(!isMaximized() && status){
        resetStatus();
        m_status[4] = status;
    }
}
void MMenuBar::setMaximized(bool status)
{
    resetStatus();
    m_status[5] = status;

    if(status){
        m_btnMaximize->hide();
        m_btnDeMaximize->show();
        m_wndParent->showMaximized();
    }
    else {
        m_btnDeMaximize->hide();
        m_btnMaximize->show();
        m_wndParent->showNormal();
    }
}
void MMenuBar::setResizing(bool east, bool west, bool north, bool south)
{
    if(!isMaximized()){
        resetStatus();
        m_status[0] = east;
        m_status[1] = west;
        m_status[2] = north;
        m_status[3] = south;
    }
}

void MMenuBar::resetStatus()
{
    for(int i = 0; i < m_status.size(); i++){
        m_status[i] = false;
    }
}

bool MMenuBar::eventFilter(QObject *object, QEvent *event)
{
    if(isMaximized()){
        return QWidget::eventFilter(object, event);
    }

    if(event->type() == QEvent::MouseButtonRelease) {
        resetStatus();

        m_lastSize = QSize();
        m_lastRootPoint = QPoint();
        m_lastCursorPoint = QPoint();

        return QWidget::eventFilter(object, event);
    }

    QPoint wnd_pos = m_wndParent->pos();
    int wnd_width = m_wndParent->width();
    int wnd_height = m_wndParent->height();

    if(event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *sc_event = static_cast<QMouseEvent*>(event);

        m_lastSize = m_wndParent->size();
        m_lastRootPoint = wnd_pos;
        m_lastCursorPoint = sc_event->pos();

        int x_event = sc_event->pos().x();
        int y_event = sc_event->pos().y();

        bool west = x_event <= MPI3_RESIZE_EDGE_WIDTH;
        bool east = x_event >= wnd_width - MPI3_RESIZE_EDGE_WIDTH;

        bool north = y_event <= MPI3_RESIZE_EDGE_WIDTH;
        bool south = y_event >= wnd_height - MPI3_RESIZE_EDGE_WIDTH;

        QWidget *child = m_wndParent->childAt(x_event, y_event);

        setResizing(east, west, north, south);
        setMoving(!isResizing() && child == this);
    }
    else if(event->type() == QEvent::MouseMove) {
        QMouseEvent *sc_event = static_cast<QMouseEvent*>(event);

        if(isMoving()){
            int wnd_x = m_wndParent->x();
            int wnd_y = m_wndParent->y();

            int move_x = m_lastCursorPoint.x() - sc_event->x();
            int move_y = m_lastCursorPoint.y() - sc_event->y();

            m_wndParent->move(wnd_x - move_x, wnd_y - move_y);
        }
        else if(isResizing()){

            int wnd_minWidth = m_wndParent->minimumWidth();
            int wnd_minHeight = m_wndParent->minimumHeight();

            int w_diff = sc_event->pos().x() - m_lastCursorPoint.x();
            int h_diff = sc_event->pos().y() - m_lastCursorPoint.y();

            int w_last = m_lastSize.width();
            int h_last = m_lastSize.height();

            int move_rx = cursor().pos().rx();
            int move_ry = cursor().pos().ry();

            int w_adjust = (m_lastRootPoint.rx() + w_last) - (move_rx + wnd_width);
            int h_adjust = (m_lastRootPoint.ry() + h_last) - (move_ry + wnd_height);

            w_adjust += (wnd_width + w_adjust) % 2;
            h_adjust += (wnd_height + h_adjust) % 2;

            // bottom-right
            if(resizingEast() && resizingSouth()) {
                m_wndParent->resize(w_last + w_diff, h_last + h_diff);
            }

            // top-right
            else if(resizingEast() && resizingNorth()) {

                if(wnd_height == wnd_minHeight && h_diff >= 0) {
                    m_wndParent->resize(w_last + w_diff, wnd_height);
                }
                else {
                    move_rx = wnd_pos.rx();
                    m_wndParent->move(move_rx, move_ry);
                    m_wndParent->resize(w_last + w_diff, wnd_height + h_adjust);
                }
            }

            // bottom-left
            else if(resizingWest() && resizingSouth()) {

                if(wnd_width == wnd_minWidth && w_diff >= 0) {
                    m_wndParent->resize(wnd_width, h_last + h_diff);
                }
                else {
                    move_ry = wnd_pos.ry();
                    m_wndParent->move(move_rx, move_ry);
                    m_wndParent->resize(wnd_width + w_adjust, h_last + h_diff);
                }
            }

            // top-left
            else if(resizingWest() && resizingNorth()) {

                if(wnd_height == wnd_minHeight && h_diff >= 0) {
                    move_ry = wnd_pos.ry();
                    h_adjust = 0;
                }

                if(wnd_width == wnd_minWidth && w_diff >= 0) {
                    move_rx = wnd_pos.rx();
                    w_adjust = 0;
                }

                m_wndParent->move(move_rx, move_ry);
                m_wndParent->resize(wnd_width + w_adjust, wnd_height + h_adjust);
            }

            // top
            else if(resizingNorth()) {

                if(wnd_height == wnd_minHeight && h_diff >= 0) {
                    return QWidget::eventFilter(object, event);
                }

                move_rx = wnd_pos.rx();
                m_wndParent->move(move_rx, move_ry);
                m_wndParent->resize(w_last, wnd_height + h_adjust);
            }

            // bottom
            else if(resizingSouth()) {
                m_wndParent->resize(w_last, h_last + h_diff);
            }

            // right
            else if(resizingWest()) {

                if(wnd_width == wnd_minWidth && w_diff >= 0) {
                    return QWidget::eventFilter(object, event);
                }

                move_ry = wnd_pos.ry();
                m_wndParent->move(move_rx, move_ry);
                m_wndParent->resize(wnd_width + w_adjust, h_last);
            }

            // left
            else if (resizingEast()) {
                m_wndParent->resize(w_last + w_diff, h_last);
            }
        }
    }
    else if(event->type() == QEvent::HoverMove) {

        if(isResizing()) {
            return QWidget::eventFilter(object, event);
        }

        QHoverEvent *sc_event = static_cast<QHoverEvent*>(event);

        int x_event = sc_event->pos().x();
        int y_event = sc_event->pos().y();

        bool west = x_event <= 3;
        bool east = x_event >= wnd_width - 3;

        bool north = y_event <= 3;
        bool south = y_event >= wnd_height - 3;

        bool north_edge = y_event < 3;
        bool south_edge = y_event > wnd_height - 3;

        bool west_edge = x_event < 3;
        bool east_edge = x_event > wnd_width - 3;

        // top-left
        if((west && north_edge) || (north && west_edge)) {
            QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        }

        // top-right
        else if((east && north_edge) || (north && east_edge)) {
            QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        }

        // bottom-left
        else if((west && south_edge) || (south && west_edge)) {
            QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        }

        // bottom-right
        else if((east && south_edge) || (south && east_edge)) {
            QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        }

        // left or right
        else if(east || west) {
            QApplication::setOverrideCursor(Qt::SizeHorCursor);
        }

        // top or bottom
        else if(north || south) {
            QApplication::setOverrideCursor(Qt::SizeVerCursor);
        }

        // reset
        else {
            while(QApplication::overrideCursor()) {
                QApplication::restoreOverrideCursor();
            }
        }
    }

    return QWidget::eventFilter(object, event);
}
