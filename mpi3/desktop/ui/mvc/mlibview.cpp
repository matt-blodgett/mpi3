#include "mlibview.h"
#include "mlibmodel.h"

#include "core/mmedialibrary.h"


#include "util/mstyle.h"


#include <QSortFilterProxyModel>
#include <QHeaderView>

#include <QDropEvent>
#include <QMimeData>


#include <QDebug>


#define TREESTYLE_OUTLINE "QTreeView {border: 1px solid #FFFFFF;} "\
                            "QTreeView::item:hover {border-top: 1px solid transparent; border-bottom: 1px solid transparent;}"
#define TREESTYLE_REGULAR "QTreeView {border-top: 1px solid #696969; border-bottom: 1px solid #696969;}"\
                            "QTreeView::item:hover {border-top: 1px solid #FFFFFF; border-bottom: 1px solid #FFFFFF;}"


MTreeView::MTreeView(QWidget *parent) : QTreeView(parent){
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);

    m_drawStyle = new MStyle(style());
    setStyle(m_drawStyle);
}


MTreeContainers::MTreeContainers(QWidget *parent) : MTreeView(parent){
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(false);
    setRootIsDecorated(true);
    setHeaderHidden(true);
    setSortingEnabled(false);

    m_modelContainers = new MModelContainers(this);
    setModel(m_modelContainers);
}

MModelContainers *MTreeContainers::modelContainers(){
    return m_modelContainers;
}

void MTreeContainers::dropEvent(QDropEvent *event){
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){

        QVector<QVariant> currentData;
        for(int i = 0; i < model()->rowCount(dropIndex); i++){
            currentData.append(model()->data(model()->index(i, 0, dropIndex)));
        }

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)){
            expand(dropIndex);

            if(action == Qt::MoveAction){

                for(int i = 0; i < model()->rowCount(dropIndex); i++){

                    QModelIndex childIndex = model()->index(i, 0, dropIndex);
                    QVariant indexData = model()->data(childIndex);

                    if(!currentData.contains(indexData)){
                        selectionModel()->select(childIndex, QItemSelectionModel::ClearAndSelect);
                        expand(childIndex);
                    }
                }
            }

            return;
        }
    }
}


MTreeSonglist::MTreeSonglist(QWidget *parent) : MTreeView(parent){
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);
    setSortingEnabled(true);

    m_modelSortFilter = new QSortFilterProxyModel(this);
    m_modelSonglist = new MModelSonglist(this);
    m_modelSortFilter->setSourceModel(m_modelSonglist);
    setModel(m_modelSortFilter);

    m_modelSortFilter->setSortCaseSensitivity(Qt::CaseInsensitive);

    connect(this, &QTreeView::doubleClicked, this, &MTreeSonglist::playItem);
    connect(header(), &QHeaderView::sortIndicatorChanged, this, &MTreeSonglist::sortChanged);
}

void MTreeSonglist::autoFitColumns(){
    for(int i = 0; i < model()->columnCount(); i++){
        resizeColumnToContents(i);
    }
}

QSortFilterProxyModel *MTreeSonglist::modelSortFilter(){
    return m_modelSortFilter;
}
MModelSonglist *MTreeSonglist::modelSonglist(){
    return m_modelSonglist;
}

void MTreeSonglist::setContainer(MMediaContainer *container){

    populateSettings();

    if(!m_containerSettings.contains(container->pid())){
        populateSettings(container);
    }

    m_modelSonglist->setContainer(container);

    QMap<QString, QVariant> settings = m_containerSettings[container->pid()].toMap();
    QList<QVariant> colWidths = settings["widths"].toList();
    QList<QVariant> colHidden = settings["hidden"].toList();
    QList<QVariant> colSort = settings["sort"].toList();

    for(int i = 0; i < model()->columnCount(); i++){
        setColumnWidth(i, colWidths[i].toInt());
        setColumnHidden(i, colHidden[i].toBool());
    }

    sortByColumn(colSort[0].toInt(), static_cast<Qt::SortOrder>(colSort[1].toInt()));
}

void MTreeSonglist::populateSettings(MMediaContainer *container){

    QMap<QString, QVariant> settings;
    QList<QVariant> colWidths;
    QList<QVariant> colHidden;
    QList<QVariant> colSort;

    for(int i = 0; i < model()->columnCount(); i++){
        colWidths.append(80);
        colHidden.append(false);
    }

    colSort = {0, Qt::AscendingOrder};

    settings["widths"] = colWidths;
    settings["hidden"] = colHidden;
    settings["sort"] = colSort;

    m_containerSettings[container->pid()] = settings;
}

void MTreeSonglist::populateSettings(){

    if(m_modelSonglist->container()){

        QMap<QString, QVariant> settings;
        QList<QVariant> colWidths;
        QList<QVariant> colHidden;
        QList<QVariant> colSort;

        for(int i = 0; i < model()->columnCount(); i++){
            colWidths.append(columnWidth(i));
            colHidden.append(isColumnHidden(i));
        }

        colSort = {m_modelSortFilter->sortColumn(), m_modelSortFilter->sortOrder()};

        settings["widths"] = colWidths;
        settings["hidden"] = colHidden;
        settings["sort"] = colSort;

        m_containerSettings[m_modelSonglist->container()->pid()] = settings;
    }
}

void MTreeSonglist::playNextItem(){
    shiftItem(1);
}
void MTreeSonglist::playPrevItem(){
    shiftItem(-1);
}

void MTreeSonglist::playItem(const QModelIndex &idx){

    MSong *song = m_modelSonglist->songAt(idx);

    m_playbackContainer = m_modelSonglist->container();
    m_playbackSong = song;

    populateQueue();

    emit playbackChanged(song);
}
void MTreeSonglist::shiftItem(int offset){

    if(m_playbackQueue.size() == 0){
        return;
    }

    int idx_song = m_playbackQueue.indexOf(m_playbackSong);
    idx_song += offset;

    if(idx_song < m_playbackQueue.size() && idx_song >= 0){

        MSong *song = m_playbackQueue[idx_song];
        m_playbackSong = song;

        emit playbackChanged(song);

        if(m_modelSonglist->container() == m_playbackContainer){

            QItemSelectionModel::SelectionFlags flag;

            flag = QItemSelectionModel::Rows;
            flag |= QItemSelectionModel::ClearAndSelect;

            QModelIndex idx_select = m_modelSortFilter->index(idx_song, 0);
            selectionModel()->select(idx_select, flag);
        }
    }
}

void MTreeSonglist::populateQueue(){

    m_playbackQueue.clear();

    for(int i = 0; i < m_modelSortFilter->rowCount(); i++){
        QModelIndex idx_sort = m_modelSortFilter->index(i, 0);
        QModelIndex idx_source = m_modelSortFilter->mapToSource(idx_sort);
        m_playbackQueue.append(m_modelSonglist->songAt(idx_source));
    }
}
void MTreeSonglist::sortChanged(int logicalIndex, Qt::SortOrder order){
    Q_UNUSED(logicalIndex)
    Q_UNUSED(order)

    if(m_modelSonglist->container() == m_playbackContainer){
        populateQueue();
    }
}

bool MTreeSonglist::allowDragMove(){
    bool rootColumn = m_modelSortFilter->sortColumn() == 0;
    bool ascending =  m_modelSortFilter->sortOrder() == Qt::AscendingOrder;
    bool playlist = m_modelSonglist->container()->type() == Mpi3::PlaylistElement;
    return rootColumn && ascending && playlist;
}
void MTreeSonglist::dragEnterEvent(QDragEnterEvent *event){
    QTreeView::dragEnterEvent(event);
}
void MTreeSonglist::dragMoveEvent(QDragMoveEvent *event){
    setStyleSheet(allowDragMove() ? TREESTYLE_REGULAR : TREESTYLE_OUTLINE);
    QTreeView::dragMoveEvent(event);
}
void MTreeSonglist::dragLeaveEvent(QDragLeaveEvent *event){
    setStyleSheet(TREESTYLE_REGULAR);
    QTreeView::dragLeaveEvent(event);
}
void MTreeSonglist::dropEvent(QDropEvent *event){
    setStyleSheet(TREESTYLE_REGULAR);

    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    if(action == Qt::MoveAction && !allowDragMove()){
        return;
    }

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    int row_count = model()->rowCount();
    int sel_rows = selectionModel()->selectedRows().size();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)){
            selectionModel()->clear();

            int select_rows = row + action == Qt::MoveAction ? sel_rows : model()->rowCount() - row_count;
            for(int r = row; r < select_rows; r++){
                for(int c = 0; c < model()->columnCount(); c++){
                    selectionModel()->select(model()->index(r, c), QItemSelectionModel::Select);
                }
            }
        }
    }

    event->acceptProposedAction();
    event->accept();

    QTreeView::dropEvent(event);
}

