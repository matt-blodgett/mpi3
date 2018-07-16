#include "mtreeviews.h"


#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>

#include <QMimeData>
#include <QUrl>
#include <QList>


SonglistTreeview::SonglistTreeview(QWidget *parent) : QTreeView(parent){
    setAcceptDrops(true);
}
SonglistTreeview::~SonglistTreeview(){}

void SonglistTreeview::dragEnterEvent(QDragEnterEvent *event){

    event->acceptProposedAction();
}
void SonglistTreeview::dragMoveEvent(QDragMoveEvent *event){

    event->acceptProposedAction();
}
void SonglistTreeview::dragLeaveEvent(QDragLeaveEvent *event){
    event->accept();
}
void SonglistTreeview::dropEvent(QDropEvent *event){
    Q_UNUSED(event);
}
