#pragma once

#ifndef MPANELMEDIA_H
#define MPANELMEDIA_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QRadioButton;
class QSplitter;
class QLabel;
QT_END_NAMESPACE

class MTreeContainers;
class MTreeSonglist;


class MPanelMedia : public QWidget
{
    Q_OBJECT

public:
    explicit MPanelMedia(QWidget *parent = nullptr);
    ~MPanelMedia();

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

private:
    MPanelMedia::View m_currentView;

    MTreeContainers *m_treeContainers = nullptr;
    MTreeSonglist *m_treeSonglist = nullptr;

    QSplitter *m_frmSplitter = nullptr;
    QRadioButton *m_btnSongs = nullptr;
    QRadioButton *m_btnArtists = nullptr;
    QRadioButton *m_btnAlbums = nullptr;
    QLabel *m_lblView = nullptr;

private:
    void containerClicked(const QModelIndex &index);

signals:
    void viewChanged();

private:
    void showEvent(QShowEvent *event);
};

#endif
