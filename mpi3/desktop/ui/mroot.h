#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QMediaPlayer;
class QAction;
class QMenu;
QT_END_NAMESPACE

class PanelLibrary;
class PanelPlayback;
class Mpi3Theme;


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
    QMediaPlayer *m_audio = nullptr;

    PanelPlayback *m_playback = nullptr;
    PanelLibrary *m_library = nullptr;

    Mpi3Theme *m_theme = nullptr;

private:
    QMenu *menu_file = nullptr;
    QMenu *menu_theme = nullptr;

    QAction *m_themeSet = nullptr;
    QAction *m_themeRefresh = nullptr;

private slots:
    void themeSet();
    void themeRefresh();

};


#endif // MROOT_H
