#ifndef MROOT_H
#define MROOT_H

#include "mplayback.h"
#include "mtrees.h"
#include "mpanels.h"

#include <QMainWindow>
#include <QString>


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

private slots:
    void testButton(QString const &str);
};


#endif // MROOT_H
