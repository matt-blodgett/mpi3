#ifndef MROOT_H
#define MROOT_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QTreeView;
class QMediaPlayer;
class QAction;
class QMenu;
QT_END_NAMESPACE

class PanelLibrary;
class PanelPlayback;
class LibraryModel;
class Mpi3Library;
class Mpi3Theme;


class Mpi3RootDesktop : public QMainWindow
{
    Q_OBJECT

public:
    Mpi3RootDesktop();
    ~Mpi3RootDesktop();

private:
    void initialize();
    void initializeMenubar();

private:
    void paintEvent(QPaintEvent *event);
    void onCustomContextMenu(const QPoint &point);

private:
    PanelLibrary *m_libview = nullptr;
    PanelPlayback *m_playback = nullptr;

    QTreeView *tree_library = nullptr;
    LibraryModel *m_model = nullptr;
    QMediaPlayer *m_audio = nullptr;
    Mpi3Library *m_library = nullptr;

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








