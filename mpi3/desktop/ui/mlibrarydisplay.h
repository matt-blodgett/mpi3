#pragma once

#ifndef MLIBRARYDISPLAY_H
#define MLIBRARYDISPLAY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QSplitter;
class QLabel;
QT_END_NAMESPACE

class Mpi3TreeViewContainers;
class Mpi3TreeViewSonglist;


class Mpi3PanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit Mpi3PanelLibrary(QWidget *parent = nullptr);
    ~Mpi3PanelLibrary();

private:
    void initializeLayout();

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    Mpi3PanelLibrary::View currentView() const;
    void changeView(Mpi3PanelLibrary::View view);
    void setDisplay(const QString &title);

private:
    Mpi3PanelLibrary::View m_currentView;

private:
    Mpi3TreeViewContainers *m_treeContainers = nullptr;
    Mpi3TreeViewSonglist *m_treeSonglist = nullptr;

    QSplitter *m_frmLibrary = nullptr;
    QWidget *m_frmViews = nullptr;
    QWidget *m_frmTrees = nullptr;

    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;

    QLabel *m_lblPlaylist = nullptr;
    QLabel *m_lblView = nullptr;

private:
    void containerClicked(const QModelIndex &index);

signals:
    void viewChanged();

private:
    void showEvent(QShowEvent *event);
};

#endif
