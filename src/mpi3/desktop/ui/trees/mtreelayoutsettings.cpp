#include "mpi3/desktop/ui/trees/mtreelayoutsettings.h"
#include "mpi3/desktop/ui/mstyle.h"

#include <QTreeView>
#include <QSortFilterProxyModel>
#include <QMimeData>
#include <QDropEvent>

#include <QSettings>


#include <QDebug>


MTreeLayoutSettings::MTreeLayoutSettings(QObject *parent) : QObject (parent)
{

}

int MTreeLayoutSettings::columnCount() const
{
    return m_columnWidth.size();
}

void MTreeLayoutSettings::setDefaults(QTreeView *tree)
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
void MTreeLayoutSettings::setValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
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
void MTreeLayoutSettings::applyValues(QTreeView *tree, QSortFilterProxyModel *sfpModel)
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

MTreeLayoutSettings *MTreeSonglistLayoutSettings::getLayoutSettings(const QString &pid)
{
    MTreeLayoutSettings *layoutSettings = m_settingsMap.value(pid, nullptr);

    if(!layoutSettings) {
        layoutSettings = new MTreeLayoutSettings(this);
        m_settingsMap[pid] = layoutSettings;
    }

    return layoutSettings;
}

void MTreeSonglistLayoutSettings::save(QSettings *settings, const QStringList &pidList)
{
    QMap<QString, MTreeLayoutSettings*>::iterator iter;
    for(iter = m_settingsMap.begin(); iter != m_settingsMap.end(); iter++) {

        QString pidKey = iter.key();
        pidKey.remove(1, 1);

        if(pidList.contains(iter.key())) {
            settings->beginGroup(pidKey);

            QString strWidths;
            QString strHidden;
            QString strSort;

            MTreeLayoutSettings *layoutSettings = iter.value();
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

            MTreeLayoutSettings *layoutSettings = new MTreeLayoutSettings(this);
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
