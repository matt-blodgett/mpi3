#include "mpi3/desktop/ui/mactions.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QProcess>


#include <QDebug>


QString MActions::pathAppData()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}
QString MActions::pathDesktop()
{
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

QString MActions::pathProfile()
{
    return pathAppData() + "/profile.xml";
}
QString MActions::pathLibraryDefault()
{
    return pathAppData() + "/newlibrary" + MPI3_LIBRARY_FILE_EXT;
}
QString MActions::pathLibraryBackups()
{
    return pathAppData() + "/backups";
}

void MActions::openFileLocation(const QString &path)
{
    QStringList processArgs;
    processArgs << "/select," << QDir::toNativeSeparators(path);
    QProcess::startDetached("explorer", processArgs);
}
