#pragma once

#ifndef MTREELAYOUTSETTINGS_H
#define MTREELAYOUTSETTINGS_H


#include <QObject>
#include <QMap>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QSettings;
class QTreeView;
QT_END_NAMESPACE


class MTreeLayoutSettings : public QObject
{
    Q_OBJECT

private:
    explicit MTreeLayoutSettings(QObject *parent = nullptr);
    friend class MTreeSonglistLayoutSettings;

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


class MTreeSonglistLayoutSettings : public QObject
{
    Q_OBJECT

public:
    explicit MTreeSonglistLayoutSettings(QObject *parent = nullptr);

public:
    MTreeLayoutSettings *getLayoutSettings(const QString &pid);

    void save(QSettings *settings, const QStringList &pidList);
    void load(QSettings *settings, const QStringList &pidList);

private:
    QMap<QString, MTreeLayoutSettings*> m_settingsMap;
};


#endif
