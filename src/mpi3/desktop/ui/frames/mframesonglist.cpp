#include "mpi3/desktop/ui/frames/mframesonglist.h"
#include "mpi3/desktop/ui/trees/mtreesonglist.h"
#include "mpi3/desktop/ui/trees/mtreelayoutsettings.h"
#include "mpi3/desktop/ui/models/mmodelcontainers.h"
#include "mpi3/desktop/ui/models/mmodelsonglist.h"
#include "mpi3/desktop/ui/models/mmodelsonglistproxy.h"
#include "mpi3/desktop/ui/mactions.h"
#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/mmediautil.h"
#include "mpi3/core/maudioengine.h"

#include <QFileDialog>
#include <QGridLayout>
#include <QHeaderView>
#include <QMenu>


#include <QDebug>


MFrameSonglist::MFrameSonglist(QWidget *parent) : MFrame(parent)
{
    m_treeSonglist = new MTreeSonglist(this);
    m_modelSonglist = new MModelSonglist(this);
    m_modelSonglistProxy = new MModelSonglistProxy(this);
    m_modelSonglistProxy->setSourceModel(m_modelSonglist);
    m_treeSonglist->setModel(m_modelSonglistProxy);
    m_modelSonglistProxy->setSortCaseSensitivity(Qt::CaseInsensitive);

    QGridLayout *gridMain = new QGridLayout();
    gridMain->addWidget(m_treeSonglist, 0, 0, 1, 1);
    gridMain->setColumnStretch(0, 1);
    gridMain->setRowStretch(0, 1);
    gridMain->setContentsMargins(0, 0, 0, 0);
    setLayout(gridMain);

    connect(m_treeSonglist, &QTreeView::customContextMenuRequested, this, &MFrameSonglist::contextMenuTreeview);
    connect(m_treeSonglist->header(), &QHeaderView::customContextMenuRequested, this, &MFrameSonglist::contextMenuHeader);

    connect(m_treeSonglist, &MTreeSonglist::moveSelected, this, &MFrameSonglist::moveSelected);
    connect(m_treeSonglist, &QTreeView::doubleClicked, this, [this](){playItemSelected();});
}

void MFrameSonglist::itemDetails()
{
    qDebug();
}
void MFrameSonglist::editItem()
{
    qDebug();
}
void MFrameSonglist::cutItems()
{
    qDebug();
}
void MFrameSonglist::copyItems()
{
    qDebug();
}
void MFrameSonglist::pasteItems()
{
    qDebug();
}
void MFrameSonglist::deleteItems()
{
    QModelIndexList removeIndexes = tree()->selectionModel()->selectedRows(0);

    QStringList pidList;
    for(QModelIndex idxProxy : removeIndexes) {
        QModelIndex idxSource = modelProxy()->mapToSource(idxProxy);
        pidList.append(model()->pidFromIndex(idxSource));
    }

    pidList.removeDuplicates();
    for(const QString &pid : pidList) {
        m_mediaLibrary->remove(pid);
    }
}

void MFrameSonglist::importSongs()
{
    MPlaylist *parentPlaylist = m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist());

    QString title = "Add Media Files";
    QString files = "All Files (*.*)";
    QStringList paths = QFileDialog::getOpenFileNames(nullptr, title, MActions::pathDesktop(), files);

    QList<QUrl> urls;
    for(const QString &p : paths) {
        urls.append(QUrl(p));
    }

    if(Mpi3::Core::validMediaFiles(urls)) {
        QStringList pidList;
        for(const QString &p : paths) {
            MSongInfo songInfo;
            if (songInfo.load(p)) {
                MSong *s = m_mediaLibrary->newSong(songInfo.songInfoMap());
                pidList.append(s->pid());
            }
        }

        if(parentPlaylist) {
            QStringList pidListCombined = parentPlaylist->songsPidList();
            pidListCombined.append(pidList);
            m_mediaLibrary->edit(parentPlaylist, "songs", pidListCombined);
        }
    }
}

void MFrameSonglist::playItemSelected()
{
    if(tree()->selectionModel()->selectedRows().size() == 1) {
        QModelIndex idxCurrent = tree()->currentIndex();
        QModelIndex idxSource = modelProxy()->mapToSource(idxCurrent);
        QString pid = model()->pidFromIndex(idxSource);
        m_pidCurrentPlayingSong = pid;
        emit currentPlayingSongChanged(pid);
    }
}
void MFrameSonglist::playItemShift(const int &shift)
{
    QModelIndex idxCurrent = model()->pidToIndex(m_pidCurrentPlayingSong);
    QModelIndex idxProxy = modelProxy()->mapFromSource(idxCurrent);

    int rowCurrent = idxProxy.row();
    int rowDestination = rowCurrent + shift;
    if (rowDestination >= 0 && rowDestination < model()->rowCount()) {
        QModelIndex nextIndex = modelProxy()->index(rowDestination, 0);
        if (nextIndex.isValid()) {
            tree()->selectionModel()->setCurrentIndex(nextIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            playItemSelected();
        }
    }
}
void MFrameSonglist::playItemNext()
{
    playItemShift(1);
}
void MFrameSonglist::playItemPrev()
{
    playItemShift(-1);
}

void MFrameSonglist::addItemsTo()
{
    qDebug();
}
void MFrameSonglist::removeItemsFrom()
{
    MPlaylist *playlist = m_mediaLibrary->getPlaylist(m_modelSonglist->pidCurrentSonglist());
    if (playlist) {
        QModelIndexList indexList = tree()->selectionModel()->selectedRows(0);
        QStringList pidListRemove;

        for(QModelIndex idx : indexList) {
            pidListRemove << model()->pidFromIndex(idx);
        }

        QStringList pidListUpdate;
        for(const QString &pid : playlist->songsPidList()) {
            if (!pidListRemove.contains(pid)) {
                pidListUpdate << pid;
            }
        }

        m_mediaLibrary->edit(playlist, "songs", pidListUpdate);
    }
}
void MFrameSonglist::openItemFileLocation()
{
    if(tree()->selectionModel()->selectedRows().size() == 1) {
        QModelIndex idxCurrent = tree()->currentIndex();
        QModelIndex idxSource = modelProxy()->mapToSource(idxCurrent);
        QString pid = model()->pidFromIndex(idxSource);
        MSong *s = m_mediaLibrary->getSong(pid);
        MActions::openFileLocation(s->path());
    }
}

MTreeSonglist *MFrameSonglist::tree()
{
    return m_treeSonglist;
}
MModelSonglist *MFrameSonglist::model()
{
    return m_modelSonglist;
}
MModelSonglistProxy *MFrameSonglist::modelProxy()
{
    return m_modelSonglistProxy;
}

void MFrameSonglist::setLibrary(MMediaLibrary *library)
{
    m_mediaLibrary = library;
    model()->setLibrary(m_mediaLibrary);
    m_pidCurrentPlayingSong = QString();
}
void MFrameSonglist::setPlaylist(MPlaylist *playlist)
{
    if(!playlist) {
        model()->setSongList(m_mediaLibrary->songs(), m_mediaLibrary->pid());
    }
    else {
        model()->setSongList(playlist->songs(), playlist->pid());
    }
    m_pidCurrentPlayingSong = QString();
}

void MFrameSonglist::setLayoutSettings(MTreeLayoutSettings *settings)
{
    if(m_layoutSettings) {
        saveLayoutSettings();
    }

    m_layoutSettings = settings;
    if(!m_layoutSettings->columnCount()) {
        m_layoutSettings->setDefaults(tree());
    }

    m_layoutSettings->applyValues(tree(), modelProxy());
}
void MFrameSonglist::saveLayoutSettings()
{
    if(m_layoutSettings) {
        m_layoutSettings->setValues(tree(), modelProxy());
    }
}

void MFrameSonglist::moveSelected(int row)
{
    MPlaylist *playlist = m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist());

    bool isSortedAscending = modelProxy()->sortOrder() == Qt::AscendingOrder;
    bool isSortedRoot = modelProxy()->sortColumn() == 0;

    if(playlist && (isSortedAscending && isSortedRoot)) {

        QModelIndexList moveIndexes = tree()->selectionModel()->selectedRows(0);

        QList<int> indexList;
        for(QModelIndex idx : moveIndexes) {
            indexList.append(idx.row());
        }

        QStringList newList;
        for(int i = 0; i < playlist->songsPidList().size(); i++) {

            if(i == row) {
                for(QModelIndex idx : moveIndexes) {
                    newList.append(model()->pidFromIndex(idx));
                }
            }

            if(!indexList.contains(i)) {
                newList.append(playlist->songsPidList().at(i));
            }
        }

        if(row == model()->rowCount()) {
            for(QModelIndex idx : moveIndexes) {
                newList.append(model()->pidFromIndex(idx));
            }
        }

        m_mediaLibrary->edit(playlist, "songs", newList);
    }
}

void MFrameSonglist::contextMenuHeader(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actAutoFitOne = new QAction(menuContext);
    QAction *actAutoFitAll = new QAction(menuContext);

    actAutoFitOne->setText("Auto Fit Column");
    actAutoFitAll->setText("Auto All Fit Columns");

    int col = tree()->indexAt(point).column();
    connect(actAutoFitOne, &QAction::triggered, this, [=](){tree()->resizeColumnToContents(col);});
    connect(actAutoFitAll, &QAction::triggered, this, [=](){tree()->autoFitColumns();});

    menuContext->addAction(actAutoFitOne);
    menuContext->addAction(actAutoFitAll);
    menuContext->addSeparator();

    for(int i = 0; i < model()->columnCount(); i++) {
        QAction *act = new QAction(menuContext);
        act->setText(model()->headerData(i, Qt::Horizontal).toString());

        bool isHidden = tree()->isColumnHidden(i);
        act->setCheckable(true);
        act->setChecked(!isHidden);

        connect(act, &QAction::triggered, this, [=](){tree()->setColumnHidden(i, !isHidden);});
        menuContext->addAction(act);
    }

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;
}
void MFrameSonglist::contextMenuTreeview(const QPoint &point)
{
    QMenu *menuContext = new QMenu(this);

    QAction *actPlayItem = new QAction(menuContext);
    QAction *actEditItem = new QAction(menuContext);
    QAction *actItemDetails = new QAction(menuContext);
    QMenu *menuAddItemsTo = new QMenu(menuContext);
    QAction *actCutItems = new QAction(menuContext);
    QAction *actCopyItems = new QAction(menuContext);
    QAction *actPasteItems = new QAction(menuContext);
    QAction *actOpenItem = new QAction(menuContext);
    QAction *actSelectAll = new QAction(menuContext);
    QAction *actSelectNone = new QAction(menuContext);
    QAction *actImportSongs = new QAction(menuContext);
    QAction *actRemoveItem = new QAction(menuContext);
    QAction *actDeleteItem = new QAction(menuContext);

    actPlayItem->setText("Play");
    actEditItem->setText("Edit");
    actItemDetails->setText("Details");
    menuAddItemsTo->setTitle("Add to Playlist...");
    actCutItems->setText("Cut");
    actCopyItems->setText("Copy");
    actPasteItems->setText("Paste");
    actOpenItem->setText("Open File Location");
    actSelectAll->setText("Select All");
    actSelectNone->setText("Select None");
    actImportSongs->setText("Import Songs");
    actRemoveItem->setText("Remove from Playlist");
    actDeleteItem->setText("Delete from Library");

    menuContext->addAction(actPlayItem);
    menuContext->addAction(actEditItem);
    menuContext->addAction(actItemDetails);
    menuContext->addMenu(menuAddItemsTo);
    menuContext->addSeparator();
    menuContext->addAction(actCutItems);
    menuContext->addAction(actCopyItems);
    menuContext->addAction(actPasteItems);
    menuContext->addAction(actOpenItem);
    menuContext->addSeparator();
    menuContext->addAction(actSelectAll);
    menuContext->addAction(actSelectNone);
    menuContext->addSeparator();
    menuContext->addAction(actImportSongs);
    menuContext->addSeparator();
    menuContext->addAction(actRemoveItem);
    menuContext->addAction(actDeleteItem);

    QModelIndex idx = tree()->indexAt(point);

    if(!idx.isValid()) {
        actPlayItem->setDisabled(true);
        actEditItem->setDisabled(true);
        actItemDetails->setDisabled(true);
        menuAddItemsTo->setDisabled(true);
        actCopyItems->setDisabled(true);
        actCutItems->setDisabled(true);
        actOpenItem->setDisabled(true);
        actRemoveItem->setDisabled(true);
        actDeleteItem->setDisabled(true);
    }

    if(tree()->selectionModel()->selectedRows().size() > 1) {
        actPlayItem->setDisabled(true);
        actEditItem->setDisabled(true);
        actItemDetails->setDisabled(true);
        actOpenItem->setDisabled(true);
    }

    if(!m_mediaLibrary->getPlaylist(model()->pidCurrentSonglist())) {
        actRemoveItem->setDisabled(true);
    }

    connect(actPlayItem, &QAction::triggered, this, [=](){playItemSelected();});
    connect(actEditItem, &QAction::triggered, this, [=](){editItem();});
    connect(actItemDetails, &QAction::triggered, this, [=](){itemDetails();});

    connect(actCutItems, &QAction::triggered, this, [this](){cutItems();});
    connect(actCopyItems, &QAction::triggered, this, [this](){copyItems();});
    connect(actPasteItems, &QAction::triggered, this, [this](){pasteItems();});
    connect(actOpenItem, &QAction::triggered, this, [this](){openItemFileLocation();});

    connect(actSelectAll, &QAction::triggered, this, [this](){tree()->selectAll();});
    connect(actSelectNone, &QAction::triggered, this, [this](){tree()->selectionModel()->clear();});

    connect(actImportSongs, &QAction::triggered, this, [this](){importSongs();});

    connect(actRemoveItem, &QAction::triggered, this, [this](){removeItemsFrom();});
    connect(actDeleteItem, &QAction::triggered, this, [this](){deleteItems();});

    menuContext->exec(tree()->mapToGlobal(point));
    delete menuContext;
}

