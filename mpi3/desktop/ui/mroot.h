#ifndef MROOT_H
#define MROOT_H

#include "mplayback.h"
#include "mtrees.h"
#include "mpanels.h"
#include "util/mtheme.h"

#include <QMainWindow>
#include <QSplitter>
#include <QAction>
#include <QMenu>


class Mpi3RootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    Mpi3RootDesktop();
    ~Mpi3RootDesktop();
    void initialize();

private:
    void initializeMenubar();
    void paintEvent(QPaintEvent *event);

private:
    QSplitter *frmLibrary = nullptr;

    PanelPlayback *frmPlayback = nullptr;
    PanelViews *frmViews = nullptr;
    PanelTrees *frmTrees = nullptr;

private:
    Mpi3Theme *themeCurrent = nullptr;

private:
    QMenu *menuFile = nullptr;
    QMenu *menuTheme = nullptr;

    QAction *m_themeSet = nullptr;
    QAction *m_themeRefresh = nullptr;

private slots:
    void themeSet();
    void themeRefresh();

};


#endif // MROOT_H
