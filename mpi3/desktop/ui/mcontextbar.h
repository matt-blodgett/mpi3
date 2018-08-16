#pragma once

#ifndef MCONTEXTBAR_H
#define MCONTEXTBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
QT_END_NAMESPACE


class MContextBar : public QWidget
{
    Q_OBJECT

public:
    explicit MContextBar(QWidget *parent = nullptr);
    ~MContextBar();

private:
    void initializeLayout();

public:
    enum View {
        ViewMedia,
        ViewLibrary,
        ViewDevice
    };

    MContextBar::View currentView() const;
    void changeView(MContextBar::View view);

private:
    MContextBar::View m_currentView;

    QRadioButton *m_btnMedia = nullptr;
    QRadioButton *m_btnLibrary = nullptr;
    QRadioButton *m_btnDevice = nullptr;

signals:
    void viewChanged();

private:
    void paintEvent(QPaintEvent *event);

};


#endif
