#ifndef MROOT_H
#define MROOT_H

#include "mplayback.h"
#include "mtrees.h"
#include "mpanels.h"

#include <QMainWindow>

#include <QString>
#include <QMenu>


class Mpi3RootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    Mpi3RootDesktop();
    ~Mpi3RootDesktop();

private:
    void initialize();
    void paintEvent(QPaintEvent *event);

private:
    PanelPlayback *frmPlayback = nullptr;
    PanelViews *frmViews = nullptr;
    PanelTrees *frmTrees = nullptr;

private:
    QMenu *fileMenu;
    QAction *m_setTheme = nullptr;


//protected:
//#ifndef QT_NO_CONTEXTMENU
//    void contextMenuEvent(QContextMenuEvent *event) override;
//#endif // QT_NO_CONTEXTMENU

private slots:
    void setTheme();

};


#endif // MROOT_H
