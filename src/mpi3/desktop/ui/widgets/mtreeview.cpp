﻿#include "mtreeview.h"
#include "mstyle.h"

#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QMimeData>
#include <QDropEvent>

#include <QSettings>
#include <QScrollBar>


#include <QDebug>


#define MPI3_TREESTYLE_OUTLINE \
    "QTreeView {border: 1px solid #FFFFFF;} "\
    "QTreeView::item:hover "\
    "{border-top: 1px solid #FFFFFF; "\
    "border-bottom: 1px solid #FFFFFF;}"
//    "{border-top: 1px solid transparent; "
//    "border-bottom: 1px solid transparent;}"

#define MPI3_TREESTYLE_NORMAL \
    "QTreeView {border-top: 1px solid #696969; "\
    "border-bottom: 1px solid #696969;}"\
    "QTreeView::item:hover "\
    "{border-top: 1px solid #FFFFFF; "\
    "border-bottom: 1px solid #FFFFFF;}"


MTreeView::MTreeView(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);

    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setContextMenuPolicy(Qt::CustomContextMenu);

    m_drawStyle = new MProxyStyle(style());
    setStyle(m_drawStyle);
}


MTreeContainers::MTreeContainers(QWidget *parent) : MTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setAlternatingRowColors(false);
    setRootIsDecorated(true);
    setHeaderHidden(true);
    setSortingEnabled(false);
}

void MTreeContainers::dropEvent(QDropEvent *event)
{
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {
            expand(dropIndex);

            if(action == Qt::MoveAction) {

                for(int i = 0; i < model()->rowCount(dropIndex); i++) {
                    QModelIndex childIndex = model()->index(i, 0, dropIndex);
                    QVariant indexData = model()->data(childIndex);
                    selectionModel()->select(childIndex, QItemSelectionModel::ClearAndSelect);
                    expand(childIndex);
                }
            }
        }
    }
}


MTreeSonglist::MTreeSonglist(QWidget *parent) : MTreeView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);
    setHeaderHidden(false);
    setSortingEnabled(true);
}

void MTreeSonglist::autoFitColumns()
{
    for(int i = 0; i < model()->columnCount(); i++) {
        resizeColumnToContents(i);
    }
}

void MTreeSonglist::dragEnterEvent(QDragEnterEvent *event)
{
    Qt::DropAction action = event->proposedAction();

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)){
        setStyleSheet(MPI3_TREESTYLE_OUTLINE);
    }

    QTreeView::dragEnterEvent(event);
}
void MTreeSonglist::dragLeaveEvent(QDragLeaveEvent *event)
{
    setStyleSheet(MPI3_TREESTYLE_NORMAL);
    QTreeView::dragLeaveEvent(event);
}
void MTreeSonglist::dropEvent(QDropEvent *event)
{
    Qt::DropAction action = event->source() == this ? Qt::MoveAction : Qt::CopyAction;

    QModelIndex dropIndex = indexAt(event->pos());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    int rowCount = model()->rowCount();
    int colCount = model()->columnCount();

    if(row < 0){
        row = rowCount;
    }

    if(rowCount == 0){
        rowCount -= 1;
    }

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {
            QItemSelectionModel::SelectionFlag flags = QItemSelectionModel::Select;

            selectionModel()->clear();
            int rowDiff = model()->rowCount() - rowCount;
            for(int r = row; r < row + rowDiff; r++){
                for(int c = 0; c < colCount; c++){
                    selectionModel()->select(model()->index(r, c), flags);
                }
            }
        }
    }

    event->acceptProposedAction();
    event->accept();

    setStyleSheet(MPI3_TREESTYLE_NORMAL);
    QTreeView::dropEvent(event);
}


MTreeSettings::MTreeSettings(QObject *parent) : QObject (parent)
{

}

int MTreeSettings::columnCount() const
{
    return m_columnWidth.size();
}

void MTreeSettings::setDefaults(QTreeView *tree)
{
    m_columnWidth.clear();
    m_columnIsHidden.clear();

    for(int i = 0; i < tree->model()->columnCount(); i++){
        m_columnWidth.append(80);
        m_columnIsHidden.append(false);
    }

    m_sortColumn = 0;
    m_sortOrder = Qt::SortOrder::AscendingOrder;
}
void MTreeSettings::setValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
{
    m_columnWidth.clear();
    m_columnIsHidden.clear();

    for(int i = 0; i < tree->model()->columnCount(); i++){
        m_columnWidth.append(tree->columnWidth(i));
        m_columnIsHidden.append(tree->isColumnHidden(i));
    }

    m_sortColumn = sfpModel->sortColumn();
    m_sortOrder = sfpModel->sortOrder();
}
void MTreeSettings::applyValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
{
    for(int i = 0; i < tree->model()->columnCount(); i++){
        tree->setColumnWidth(i, m_columnWidth[i]);
        tree->setColumnHidden(i, m_columnIsHidden[i]);
    }

    sfpModel->sort(m_sortColumn, m_sortOrder);
}


MTreeSettingsCollection::MTreeSettingsCollection(QObject *parent) : QObject(parent)
{

}

MTreeSettings *MTreeSettingsCollection::getContainer(const QString &pid)
{
    return m_settingsMap.value(pid, nullptr);
}
MTreeSettings *MTreeSettingsCollection::addContainer(const QString &pid)
{
    MTreeSettings *treeSettings = getContainer(pid);

    if(!treeSettings){
        treeSettings = new MTreeSettings(this);
        m_settingsMap[pid] = treeSettings;
    }

    return treeSettings;
}

void MTreeSettingsCollection::save(QSettings *settings, const QStringList &pidlist)
{
    emit aboutToSave();

    QMap<QString, MTreeSettings*>::iterator iter;
    for(iter = m_settingsMap.begin(); iter != m_settingsMap.end(); iter++){

        QString pidKey = iter.key();
        pidKey.remove(1, 1);

        if(pidlist.contains(iter.key())){
            settings->beginGroup(pidKey);

            QString strWidths;
            QString strHidden;
            QString strSort;

            MTreeSettings *treeSettings = iter.value();
            for(int i = 0; i < treeSettings->columnCount(); i++) {
                strWidths += QString::number(treeSettings->m_columnWidth[i]) + ";";
                strHidden += QString(treeSettings->m_columnIsHidden[i] ? "1" : "0") + ";";
            }

            strSort = QString::number(treeSettings->m_sortColumn) + ";";
            strSort += QString::number(treeSettings->m_sortOrder);

            settings->setValue("widths", strWidths);
            settings->setValue("hidden", strHidden);
            settings->setValue("sort", strSort);

            settings->endGroup();
        }
    }

    emit completedSaving();
}
void MTreeSettingsCollection::load(QSettings *settings, const QStringList &pidlist)
{
    m_settingsMap.clear();

    for(QString pid : pidlist) {

        QString pidKey = pid;
        pidKey.remove(1, 1);

        if(settings->childGroups().contains(pidKey)){
            settings->beginGroup(pidKey);

            QStringList strWidths = settings->value("widths", QString()).toString().split(";");
            QStringList strHidden = settings->value("hidden", QString()).toString().split(";");
            QStringList strSort = settings->value("sort", QString()).toString().split(";");

            MTreeSettings *treeSettings = new MTreeSettings(this);
            for(int i = 0; i < strHidden.size(); i++) {
                treeSettings->m_columnWidth.append(strWidths.at(i).toInt());
                treeSettings->m_columnIsHidden.append(strHidden[i] == "1");
            }

            treeSettings->m_sortColumn = strSort[0].toInt();
            treeSettings->m_sortOrder = static_cast<Qt::SortOrder>(strSort[1].toInt());

            m_settingsMap[pid] = treeSettings;

            settings->endGroup();
        }
    }
}
