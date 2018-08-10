#pragma once

#ifndef MLIBRARYDISPLAY_H
#define MLIBRARYDISPLAY_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QSplitter;
class QLabel;
QT_END_NAMESPACE

class MTreeContainers;
class MTreeSonglist;


class MPanelLibrary : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelLibrary(QWidget *parent = nullptr);
    ~MPanelLibrary();

private:
    void initializeLayout();

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    MPanelLibrary::View currentView() const;
    void changeView(MPanelLibrary::View view);
    void setDisplay(const QString &title);

private:
    MPanelLibrary::View m_currentView;

private:
    MTreeContainers *m_treeContainers = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;

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
