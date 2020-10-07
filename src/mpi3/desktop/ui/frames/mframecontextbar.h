#pragma once

#ifndef MFRAMECONTEXTBAR_H
#define MFRAMECONTEXTBAR_H


#include "mpi3/desktop/ui/frames/mframe.h"

QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE


class MFrameContextBar : public MFrame
{
    Q_OBJECT

public:
    explicit MFrameContextBar(QWidget *parent = nullptr);

public:
    enum View {
        ViewMedia,
        ViewLibrary,
        ViewDevice
    };

    MFrameContextBar::View currentView() const;
    void changeView(MFrameContextBar::View view);

private:
    MFrameContextBar::View m_currentView;
    QRadioButton *m_btnMedia = nullptr;
    QRadioButton *m_btnLibrary = nullptr;
    QRadioButton *m_btnDevice = nullptr;

signals:
    void contextPanelChanged();

};


#endif
