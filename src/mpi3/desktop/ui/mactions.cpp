#include "mactions.h"

#include <QFileDialog>


#include <QStandardPaths>
#include <QProcess>


#include <QDebug>


#include "mglobal.h"



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
