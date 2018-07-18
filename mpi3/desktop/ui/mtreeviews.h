#ifndef MTREEVIEWS_H
#define MTREEVIEWS_H

#include <QTreeView>


class LibraryTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit LibraryTreeview(QWidget *parent = nullptr);
    ~LibraryTreeview();

signals:
    void filesDropped(QStringList droppedFiles);

private:
    QStringList paths;

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

};


class SonglistTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit SonglistTreeview(QWidget *parent = nullptr);
    ~SonglistTreeview();

private:
    QStringList paths;

signals:
    void filesDropped(QStringList droppedFiles);

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

};


#endif // MTREEVIEWS_H
