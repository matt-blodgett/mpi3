#include "mpi3/desktop/ui/widgets/mtreeview.h"
#include "mpi3/desktop/ui/mstyle.h"

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

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {

            selectionModel()->setCurrentIndex(model()->index(row, col, parentIndex), QItemSelectionModel::ClearAndSelect);
//            expand(dropIndex);

//            if(action == Qt::MoveAction) {

//                for(int i = 0; i < model()->rowCount(dropIndex); i++) {
//                    QModelIndex childIndex = model()->index(i, 0, dropIndex);
//                    QVariant indexData = model()->data(childIndex);
//                    selectionModel()->select(childIndex, QItemSelectionModel::ClearAndSelect);
//                    expand(childIndex);
//                }
//            }
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

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {
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

    QModelIndex dropIndex = indexAt(event->position().toPoint());
    QModelIndex parentIndex = dropIndex.parent();

    int row = dropIndex.row();
    int col = dropIndex.column();

    int rowCount = model()->rowCount();
    int colCount = model()->columnCount();

    if(row < 0) {
        row = rowCount;
    }

    if(rowCount == 0) {
        rowCount -= 1;
    }

    if(model()->canDropMimeData(event->mimeData(), action, row, col, parentIndex)) {

        if(action == Qt::MoveAction) {
            emit moveSelected(row);
        }
        else if(model()->dropMimeData(event->mimeData(), action, row, col, parentIndex)) {
            QItemSelectionModel::SelectionFlag flags = QItemSelectionModel::Select;

            selectionModel()->clear();
            int rowDiff = model()->rowCount() - rowCount;
            for(int r = row; r < row + rowDiff; r++) {
                for(int c = 0; c < colCount; c++) {
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


MTreeViewLayoutSettings::MTreeViewLayoutSettings(QObject *parent) : QObject (parent)
{

}

int MTreeViewLayoutSettings::columnCount() const
{
    return m_columnWidth.size();
}

void MTreeViewLayoutSettings::setDefaults(QTreeView *tree)
{
    m_columnWidth.clear();
    m_columnIsHidden.clear();

    for(int i = 0; i < tree->model()->columnCount(); i++) {
        m_columnWidth.append(80);
        m_columnIsHidden.append(false);
    }

    m_sortColumn = 0;
    m_sortOrder = Qt::SortOrder::AscendingOrder;
}
void MTreeViewLayoutSettings::setValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
{
    m_columnWidth.clear();
    m_columnIsHidden.clear();

    for(int i = 0; i < tree->model()->columnCount(); i++) {
        m_columnWidth.append(tree->columnWidth(i));
        m_columnIsHidden.append(tree->isColumnHidden(i));
    }

    m_sortColumn = sfpModel->sortColumn();
    m_sortOrder = sfpModel->sortOrder();
}
void MTreeViewLayoutSettings::applyValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
{
    for(int i = 0; i < tree->model()->columnCount(); i++) {
        tree->setColumnWidth(i, m_columnWidth[i]);
        tree->setColumnHidden(i, m_columnIsHidden[i]);
    }

    sfpModel->sort(m_sortColumn, m_sortOrder);
}


MTreeSonglistLayoutSettings::MTreeSonglistLayoutSettings(QObject *parent) : QObject(parent)
{

}

MTreeViewLayoutSettings *MTreeSonglistLayoutSettings::getLayoutSettings(const QString &pid)
{
    MTreeViewLayoutSettings *layoutSettings = m_settingsMap.value(pid, nullptr);

    if(!layoutSettings) {
        layoutSettings = new MTreeViewLayoutSettings(this);
        m_settingsMap[pid] = layoutSettings;
    }

    return layoutSettings;
}

void MTreeSonglistLayoutSettings::save(QSettings *settings, const QStringList &pidList)
{
    QMap<QString, MTreeViewLayoutSettings*>::iterator iter;
    for(iter = m_settingsMap.begin(); iter != m_settingsMap.end(); iter++) {

        QString pidKey = iter.key();
        pidKey.remove(1, 1);

        if(pidList.contains(iter.key())) {
            settings->beginGroup(pidKey);

            QString strWidths;
            QString strHidden;
            QString strSort;

            MTreeViewLayoutSettings *layoutSettings = iter.value();
            for(int i = 0; i < layoutSettings->columnCount(); i++) {
                strWidths += QString::number(layoutSettings->m_columnWidth[i]) + ";";
                strHidden += QString(layoutSettings->m_columnIsHidden[i] ? "1" : "0") + ";";
            }

            strSort = QString::number(layoutSettings->m_sortColumn) + ";";
            strSort += QString::number(layoutSettings->m_sortOrder);

            settings->setValue("widths", strWidths);
            settings->setValue("hidden", strHidden);
            settings->setValue("sort", strSort);

            settings->endGroup();
        }
    }
}
void MTreeSonglistLayoutSettings::load(QSettings *settings, const QStringList &pidList)
{
    m_settingsMap.clear();

    for(const QString &pid : pidList) {

        QString pidKey = pid;
        pidKey.remove(1, 1);

        if(settings->childGroups().contains(pidKey)) {
            settings->beginGroup(pidKey);

            QStringList strWidths = settings->value("widths", QString()).toString().split(";");
            QStringList strHidden = settings->value("hidden", QString()).toString().split(";");
            QStringList strSort = settings->value("sort", QString()).toString().split(";");

            MTreeViewLayoutSettings *layoutSettings = new MTreeViewLayoutSettings(this);
            for(int i = 0; i < strHidden.size(); i++) {
                layoutSettings->m_columnWidth.append(strWidths.at(i).toInt());
                layoutSettings->m_columnIsHidden.append(strHidden[i] == "1");
            }

            layoutSettings->m_sortColumn = strSort[0].toInt();
            layoutSettings->m_sortOrder = static_cast<Qt::SortOrder>(strSort[1].toInt());

            m_settingsMap[pid] = layoutSettings;

            settings->endGroup();
        }
    }
}


MTreeDrives::MTreeDrives(QWidget *parent) : QTreeView(parent)
{
    setAcceptDrops(false);
    setDragEnabled(false);
    viewport()->setAcceptDrops(false);
    setDragDropMode(QAbstractItemView::NoDragDrop);

    setExpandsOnDoubleClick(false);
    setFocusPolicy(Qt::NoFocus);
    setSelectionBehavior(QAbstractItemView::SelectRows);

//    setEditTriggers(QTreeView::SelectedClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);

    m_drawStyle = new MProxyStyle(style());
    setStyle(m_drawStyle);
}
