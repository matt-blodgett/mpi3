#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H


#include "mcontextpanel.h"
class MTreeContainers;
class MTreeSonglist;


class MPanelMedia : public MPanelContext
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);

private:
    void initializeLayout();

public:
    enum View {
        ViewAllSongs,
        ViewArtists,
        ViewAlbums,
        ViewContainer
    };

    MPanelMedia::View currentView() const;
    void changeView(MPanelMedia::View view);
    void setDisplay(const QString &title);

    MTreeContainers *treeContainers();
    MTreeSonglist *treeSonglist();

private:
    void containerClicked(const QModelIndex &index);

private:
    MPanelMedia::View m_currentView;

private:
    MTreeContainers *m_treeContainers = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;

    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;
    QLabel *m_lblPlaylist = nullptr;
    QLabel *m_lblView = nullptr;

signals:
    void viewChanged();
};

#endif



















