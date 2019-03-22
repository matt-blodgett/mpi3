#pragma once

#ifndef MTREEVIEW_H
#define MTREEVIEW_H


#include <QTreeView>
#include <QMap>
QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QSettings;
QT_END_NAMESPACE


class MProxyStyle;


class MTreeView : public QTreeView
{
    Q_OBJECT

public:
    explicit MTreeView(QWidget *parent = nullptr);

protected:
    MProxyStyle *m_drawStyle = nullptr;
};


class MTreeContainers : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeContainers(QWidget *parent = nullptr);

protected:
    void dropEvent(QDropEvent *event) override;
};


class MTreeSonglist : public MTreeView
{
    Q_OBJECT

public:
    explicit MTreeSonglist(QWidget *parent = nullptr);

public:
    void autoFitColumns();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
};


class MTreeSettings : public QObject
{
    Q_OBJECT

friend class MTreeSettingsCollection;

public:
    explicit MTreeSettings(QObject *parent = nullptr);

private:
    QList<int> m_columnWidth;
    QList<bool> m_columnIsHidden;

    int m_sortColumn = 0;
    Qt::SortOrder m_sortOrder = Qt::SortOrder::AscendingOrder;

public:
    int columnCount() const;

    void setDefaults(QTreeView *tree);
    void setValues(QTreeView *tree, QSortFilterProxyModel *sfpModel);
    void applyValues(QTreeView *tree, QSortFilterProxyModel *sfpModel);
};


class MTreeSettingsCollection : public QObject
{
    Q_OBJECT

public:
    explicit MTreeSettingsCollection(QObject *parent = nullptr);

public:
    MTreeSettings *getContainer(const QString &pid);
    MTreeSettings *addContainer(const QString &pid);

    void save(QSettings *settings, const QStringList &pidlist);
    void load(QSettings *settings, const QStringList &pidlist);

private:
    QMap<QString, MTreeSettings*> m_settingsMap;

signals:
    void aboutToSave();
    void completedSaving();
};


#endif
