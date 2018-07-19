#ifndef MTREEVIEWS_H
#define MTREEVIEWS_H

#include <QList>
#include <QUrl>

#include <QTreeView>


class LibraryTreeview : public QTreeView
{
    Q_OBJECT

public:
    explicit LibraryTreeview(QWidget *parent = nullptr);
    ~LibraryTreeview();

signals:
    void filesDropped(QModelIndex index, QList<QUrl> m_urls);

private:
    QList<QUrl> m_urls;
    QStringList m_pids;

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

signals:
    void filesDropped(QModelIndex index, QList<QUrl> m_urls);

private:
    QList<QUrl> m_urls;
    QStringList m_pids;

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);

};


#endif // MTREEVIEWS_H
