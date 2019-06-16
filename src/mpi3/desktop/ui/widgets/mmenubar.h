#pragma once

#ifndef MMENUBAR_H
#define MMENUBAR_H


#include "mframe.h"

QT_BEGIN_NAMESPACE
class QSettings;
class QMainWindow;
class QPushButton;
QT_END_NAMESPACE


// TODO: Move window to directly under mouse cursor position to prevent flickering and jumping
class MMenuBar : public MFrame
{
    Q_OBJECT

public:
    explicit MMenuBar(QWidget *parent = nullptr);
    void load(QSettings *settings);
    void save(QSettings *settings);

public:
    bool isMaximized();
    void setMaximized(bool status);

private:
    bool resizingEast();
    bool resizingWest();
    bool resizingNorth();
    bool resizingSouth();

    bool isMoving();
    bool isResizing();

    void setMoving(bool status);
    void setResizing(bool east, bool west, bool north, bool south);

    void resetStatus();

    QList<bool> m_status;
    QSize m_lastSize;
    QPoint m_lastRootPoint;
    QPoint m_lastCursorPoint;

private:
    QMainWindow *m_wndParent = nullptr;
    QPushButton *m_btnMaximize = nullptr;
    QPushButton *m_btnMinimize = nullptr;
    QPushButton *m_btnDeMaximize = nullptr;
    QPushButton *m_btnClose = nullptr;

protected:
    bool eventFilter(QObject *object, QEvent *event);
};


#endif
