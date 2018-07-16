#ifndef MTREEVIEWS_H
#define MTREEVIEWS_H

#include <QTreeView>


class SonglistTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit SonglistTreeview(QWidget *parent = nullptr);
    ~SonglistTreeview();

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};


#endif // MTREEVIEWS_H
