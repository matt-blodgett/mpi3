#include "mtreeviews.h"

#include <QHeaderView>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

#include <QMimeData>
#include <QList>
#include <QUrl>

#include <QItemSelection>
//#include <QDrag>


#include <QDebug>


LibraryTreeview::LibraryTreeview(QWidget *parent) : QTreeView(parent){
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);

    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setAcceptDrops(true);
    setDragEnabled(true);

    setDropIndicatorShown(true);

    setRootIsDecorated(true);
    setHeaderHidden(true);
    setIndentation(12);
}
LibraryTreeview::~LibraryTreeview(){}

void LibraryTreeview::dragEnterEvent(QDragEnterEvent *event){
    m_urls.clear();

    const QMimeData *mimeData = event->mimeData();

//    if(event->source()){
//        if(event->source() != this){
//            event->setDropAction(Qt::CopyAction);
//        }
//    }

    if(mimeData->hasUrls()){
        QList<QUrl> urls = mimeData->urls();

        for(int i = 0; i < urls.size(); i++){
            QString path = urls.at(i).toLocalFile();
            if(path.endsWith(".wav") || path.endsWith(".mp3")){
                m_urls.append(QUrl(path));
            }
        }

        if(m_urls.size() == urls.size()){
            event->acceptProposedAction();
        }
    }
    else if(mimeData->hasText()) {
//        qDebug() << mimeData->text();
        event->acceptProposedAction();
    }
}
void LibraryTreeview::dragMoveEvent(QDragMoveEvent *event){
    QModelIndex index = indexAt(event->pos());
    selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    event->acceptProposedAction();
}
void LibraryTreeview::dragLeaveEvent(QDragLeaveEvent *event){
    m_urls.clear();
    event->accept();
}
void LibraryTreeview::dropEvent(QDropEvent *event){
    QModelIndex index = indexAt(event->pos());
    emit filesDropped(index, m_urls);
    m_urls.clear();
}


SonglistTreeview::SonglistTreeview(QWidget *parent) : QTreeView(parent){
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
//    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

//    setDefaultDropAction(Qt::MoveAction);
//    setEditTriggers(QAbstractItemView::SelectedClicked);

    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setIndentation(12);

    setSelectionBehavior(QAbstractItemView::SelectRows);
}
SonglistTreeview::~SonglistTreeview(){}

void SonglistTreeview::dragEnterEvent(QDragEnterEvent *event){
    const QMimeData *mimeData = event->mimeData();

    if(mimeData->hasUrls()){
//        QStringList paths;
        QList<QUrl> urls = mimeData->urls();

        for(int i = 0; i < urls.size(); i++){
            QString path = urls.at(i).toLocalFile();
            if(path.endsWith(".wav") || path.endsWith(".mp3")){
                m_urls.append(path);
            }
        }

        if(m_urls.size() == urls.size()){
            event->acceptProposedAction();
        }
    }
    else if(mimeData->hasText()) {
//        qDebug() << mimeData->text();
        event->acceptProposedAction();
    }
}
void SonglistTreeview::dragMoveEvent(QDragMoveEvent *event){
    QModelIndex index = indexAt(event->pos());
    selectionModel()->select(index, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    event->acceptProposedAction();
}
void SonglistTreeview::dragLeaveEvent(QDragLeaveEvent *event){
    m_urls.clear();
    event->accept();
}
void SonglistTreeview::dropEvent(QDropEvent *event){
    QModelIndex index = indexAt(event->pos());
    emit filesDropped(index, m_urls);
    m_urls.clear();
}



//void SonglistTreeview::startDrag(Qt::DropActions supportedActions){
//    QDrag *drag = new QDrag(this);

//    drag->setMimeData(model()->mimeData(selectionModel()->selectedIndexes()));
//    drag->exec(supportedActions);
//}











