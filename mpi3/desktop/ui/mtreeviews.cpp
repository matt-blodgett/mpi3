#include "mtreeviews.h"

#include <QHeaderView>

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

#include <QMimeData>
#include <QUrl>
#include <QList>


LibraryTreeview::LibraryTreeview(QWidget *parent) : QTreeView(parent){
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::SingleSelection);

    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

    setRootIsDecorated(true);
    setHeaderHidden(true);
    setIndentation(12);
}
LibraryTreeview::~LibraryTreeview(){}

void LibraryTreeview::dragEnterEvent(QDragEnterEvent *event){
    const QMimeData *mimeData = event->mimeData();

    if(mimeData->hasUrls()){
//        QStringList paths;
        QList<QUrl> urls = mimeData->urls();

        for(int i = 0; i < urls.size(); i++){
            QString path = urls.at(i).toLocalFile();
            if(path.endsWith(".wav") || path.endsWith(".mp3")){
                paths.append(path);
            }
        }

        if(paths.size() == urls.size()){
            event->acceptProposedAction();
        }
    }
}
void LibraryTreeview::dragMoveEvent(QDragMoveEvent *event){
    QModelIndex index = indexAt(event->pos());
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    event->acceptProposedAction();
}
void LibraryTreeview::dragLeaveEvent(QDragLeaveEvent *event){
    paths.clear();
    event->accept();
}
void LibraryTreeview::dropEvent(QDropEvent *event){
    Q_UNUSED(event);
    emit filesDropped(paths);
    paths.clear();
}


SonglistTreeview::SonglistTreeview(QWidget *parent) : QTreeView(parent){
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);

    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setIndentation(12);
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
                paths.append(path);
            }
        }

        if(paths.size() == urls.size()){
            event->acceptProposedAction();
        }
    }
}
void SonglistTreeview::dragMoveEvent(QDragMoveEvent *event){

    event->acceptProposedAction();
}
void SonglistTreeview::dragLeaveEvent(QDragLeaveEvent *event){
    paths.clear();
    event->accept();
}
void SonglistTreeview::dropEvent(QDropEvent *event){
    Q_UNUSED(event);
    emit filesDropped(paths);
    paths.clear();
}
