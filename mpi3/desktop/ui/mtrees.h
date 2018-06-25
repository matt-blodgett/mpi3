#ifndef MTREES_H
#define MTREES_H

#include <QTreeView>
#include <QLabel>

#include <QStandardItemModel>


class SongModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit SongModel(QObject *parent=nullptr);
    ~SongModel();

};


// ----------------------------------------------------------------------------------------------------
// * LibraryTreeview *
// ----------------------------------------------------------------------------------------------------
class LibraryTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit LibraryTreeview(QWidget *parent=nullptr);
    ~LibraryTreeview();

    SongModel *modelSongs = nullptr;

};


// ----------------------------------------------------------------------------------------------------
// * PanelTrees *
// ----------------------------------------------------------------------------------------------------
class PanelTrees : public QWidget
{
    Q_OBJECT

public:
    explicit PanelTrees(QWidget *parent=nullptr);
    ~PanelTrees();

private:
    QLabel *lblTreeview = nullptr;
    LibraryTreeview *treeLibrary = nullptr;

private:
    void paintEvent(QPaintEvent *event);

};

#endif // MTREES_H
