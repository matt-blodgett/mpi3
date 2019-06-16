#pragma once

#ifndef MPANEL_H
#define MPANEL_H


#include "mframe.h"

QT_BEGIN_NAMESPACE
class QSettings;
class QGridLayout;
class QSplitter;
class QScrollArea;
class QLabel;
QT_END_NAMESPACE


#include "mglobal.h"


class MPanelSection : public MFrame
{
    Q_OBJECT

public:
    explicit MPanelSection(QWidget *parent = nullptr);

public:
    QGridLayout *gridNorth();
    QGridLayout *gridSouth();
    QGridLayout *gridEast();
    QGridLayout *gridWest();
    QGridLayout *gridBody();
    QGridLayout *gridSection();

private:
    QGridLayout *m_gridNorth = nullptr;
    QGridLayout *m_gridSouth = nullptr;
    QGridLayout *m_gridEast = nullptr;
    QGridLayout *m_gridWest = nullptr;
    QGridLayout *m_gridBody = nullptr;
    QGridLayout *m_gridSection = nullptr;

    QWidget *m_frmBody = nullptr;
    QLabel *m_lblHeader = nullptr;

public:
    QString header() const;
    void setHeader(const QString &header);
};


class MPanel : public MFrame
{
    Q_OBJECT

public:
    explicit MPanel(QWidget *parent = nullptr, bool sectioned = true);
    virtual void setLibrary(MMediaLibrary *library);
    virtual void load(QSettings *settings);
    virtual void save(QSettings *settings);

public:
    QGridLayout *gridMain();
    QGridLayout *gridControl();
    QGridLayout *gridDisplay();

private:
    QGridLayout *m_gridMain = nullptr;
    QGridLayout *m_gridControl = nullptr;
    QGridLayout *m_gridDisplay = nullptr;

private:
    QSplitter *m_frmSplitter = nullptr;
    QWidget *m_frmControl = nullptr;
    QWidget *m_frmDisplay = nullptr;

public:
    QString title() const;
    void setTitle(const QString &title);

    MPanelSection *addSection();
    MPanelSection *addSection(const QString &header);

private:
    QLabel *m_lblPanelTitle = nullptr;
    QScrollArea *m_frmScrollArea = nullptr;

protected:
    void showEvent(QShowEvent *event);
};


#endif
