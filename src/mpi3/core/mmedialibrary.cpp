#include "mpi3/core/mmedialibrary.h"

#include <QSqlDatabase>
#include <QSqlQuery>
//#include <QSqlRecord>
#include <QSqlError>

#include <QRandomGenerator>
#include <QFile>
#include <QUrl>
#include <QDir>


#include <QDebug>


static QString generatePID(Mpi3::ElementType elementType)
{
    QString pid;

    switch(elementType) {
        case Mpi3::BaseElement: {
            pid = MPI3_PID_PREFIX_ELEMENT;
            break;
        }
        case Mpi3::SongElement: {
            pid = MPI3_PID_PREFIX_SONG;
            break;
        }
        case Mpi3::ContainerElement: {
            pid = MPI3_PID_PREFIX_CONTAINER;
            break;
        }
        case Mpi3::PlaylistElement: {
            pid = MPI3_PID_PREFIX_PLAYLIST;
            break;
        }
        case Mpi3::FolderElement: {
            pid = MPI3_PID_PREFIX_FOLDER;
            break;
        }
        case Mpi3::LibraryElement: {
            pid = MPI3_PID_PREFIX_LIBRARY;
            break;
        }
    }

    const QString pidCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    while(pid.size() < MPI3_PID_STRING_LENGTH) {
        pid += pidCharacters[QRandomGenerator().global()->bounded(0, pidCharacters.length())];
    }

    return pid;
}


MMediaElement::MMediaElement(QObject *parent) : QObject(parent)
{

}
Mpi3::ElementType MMediaElement::type() const
{
    return Mpi3::BaseElement;
}

QString MMediaElement::pid() const
{
    return m_pid;
}
QString MMediaElement::name() const
{
    return m_name;
}
QString MMediaElement::added() const
{
    return m_added;
}


MChildElement::MChildElement(MMediaLibrary *parentLibrary) : MMediaElement(parentLibrary)
{
    m_parentLibrary = parentLibrary;
}


MSong::MSong(MMediaLibrary *parentLibrary) : MChildElement(parentLibrary)
{

}
Mpi3::ElementType MSong::type() const
{
    return Mpi3::SongElement;
}

QString MSong::artist() const
{
    return m_artist;
}
QString MSong::album() const
{
    return m_album;
}
QString MSong::kind() const
{
    return m_kind;
}
QString MSong::path() const
{
    return m_path;
}

double MSong::time() const
{
    return m_time;
}
double MSong::size() const
{
    return m_size;
}

int MSong::bitRate() const
{
    return m_bitRate;
}
int MSong::sampleRate() const
{
    return m_sampleRate;
}


MContainer::MContainer(MMediaLibrary *parentLibrary) : MChildElement(parentLibrary)
{

}
Mpi3::ElementType MContainer::type() const
{
    return Mpi3::ContainerElement;
}

MFolder *MContainer::parentFolder() const
{
    return m_parentFolder;
}


MPlaylist::MPlaylist(MMediaLibrary *parentLibrary) : MContainer(parentLibrary)
{

}
Mpi3::ElementType MPlaylist::type() const
{
    return Mpi3::PlaylistElement;
}

MSongList MPlaylist::songs() const
{
    return m_parentLibrary->getSongList(m_songsPidList);
}
QStringList MPlaylist::songsPidList() const
{
    return m_songsPidList;
}


MFolder::MFolder(MMediaLibrary *parentLibrary) : MContainer(parentLibrary)
{

}
Mpi3::ElementType MFolder::type() const
{
    return Mpi3::FolderElement;
}

MFolderList MFolder::childFolders() const
{
    MFolderList children;
    for(MFolder *f : m_parentLibrary->folders()) {
        if(f->parentFolder() == this) {
            children.append(f);
        }
    }

    return children;
}
MPlaylistList MFolder::childPlaylists() const
{
    MPlaylistList children;
    for(MPlaylist *p : m_parentLibrary->playlists()) {
        if(p->parentFolder() == this) {
            children.append(p);
        }
    }

    return children;
}
MContainerList MFolder::childContainers() const
{
    MContainerList children;
    for(MFolder *f : childFolders()) {
        children.append(f);
    }

    for(MPlaylist *p : childPlaylists()) {
        children.append(p);
    }

    return children;
}


MMediaLibrary::MMediaLibrary(QObject *parent) : MMediaElement(parent)
{
    m_pid = generatePID(Mpi3::LibraryElement);
    m_name = "New Library";
    m_added = "1970-01-01";
    m_database = QSqlDatabase::addDatabase("QSQLITE", "media-library");
}
Mpi3::ElementType MMediaLibrary::type() const
{
    return Mpi3::LibraryElement;
}

bool MMediaLibrary::load(const QString &path)
{
    qInfo() << "loading library file" << path;

    if (!QFile::exists(path)) {
        qWarning() << "error opening file for read" << path;
        return false;
    }

    emit aboutToLoad();

    if (m_database.isOpen()) {
        m_database.close();
    }
    m_database.setDatabaseName(path);
    if (!m_database.open()) {
        qWarning() << "failed to connect to database";
        qWarning() << m_database.lastError().text();
        return false;
    }

    const QStringList tables = {"songs", "playlists", "folders", "playlistSongs", "settings"};
    for (const QString &table : tables) {
        if (!m_database.tables().contains(table)) {
            qDebug() << "database is improperly configured or corrupt";
            return false;
        }
    }

    qDeleteAll(m_songs);
    m_songs.clear();

    qDeleteAll(m_folders);
    m_folders.clear();

    qDeleteAll(m_playlists);
    m_playlists.clear();

    dbReadAllData();

    m_path = QDir::toNativeSeparators(path);

    emit libraryLoaded();

    qInfo() << "loaded library file" << path;

    return true;
}
bool MMediaLibrary::create(const QString &path)
{
    qInfo() << "creating library file" << path;

    if (QFile::exists(path)) {
        qWarning() << "file already exists, overwriting";
        if (!QFile::remove(path)) {
            qWarning() << "failed to remove existing file" << path;
            return false;
        }
    }

    if (m_database.isOpen()) {
        m_database.close();
    }
    m_database.setDatabaseName(path);
    if (!m_database.open()) {
        qWarning() << "failed to connect to database";
        qWarning() << m_database.lastError().text();
        return false;
    }

    QStringList sqlStmts;
    QFile scriptFile(":/scripts/install.sql");
    if(scriptFile.open(QIODevice::ReadOnly)) {
        QString sqlRaw = scriptFile.readAll();
        sqlStmts = sqlRaw.split(";");
    } else {
        qWarning() << "could not open" << scriptFile.fileName();
        return false;
    }

    QSqlQuery query(m_database);
    for (const QString &stmt : sqlStmts) {
        query.exec(stmt);
    }

    dbInsertSettings();

    m_path = QDir::toNativeSeparators(path);

    qInfo() << "created library file" << path;

    return true;
}
void MMediaLibrary::reset()
{
    qInfo() << "resetting library" << m_pid << "-" << m_name;

    emit aboutToReset();

    qDeleteAll(m_songs);
    m_songs.clear();

    qDeleteAll(m_folders);
    m_folders.clear();

    qDeleteAll(m_playlists);
    m_playlists.clear();

    emit libraryReset();

    qInfo() << "reset library" << m_pid << "-" << m_name;
}

void MMediaLibrary::dbReadAllData() {
    QSqlQuery query(m_database);
    QString sql;

    // Settings
    sql = "SELECT `key`, `value` FROM `settings`;";
    query.exec(sql);

    while (query.next()) {
        QString key = query.value(0).toString();
        QString value = query.value(1).toString();

        if (key == "pid") {
            m_pid = value;
        }
        else if (key == "name") {
            m_name = value;
        }
        else if (key == "added") {
            m_added = value;
        }
        else if (key == "localMediaPath") {
            m_localMediaPath = value;
        }
    }

    // Songs
    sql = "SELECT `pid`, `name`, `added`, `artist`, `album`, `kind`, `path`, `time`, `size`, `bitRate`, `sampleRate` FROM `songs`;";
    query.exec(sql);

    while (query.next()) {
        MSong *song = new MSong(this);

        song->m_pid = query.value(0).toString();
        song->m_name = query.value(1).toString();
        song->m_added = query.value(2).toString();
        song->m_artist = query.value(3).toString();
        song->m_album = query.value(4).toString();
        song->m_kind = query.value(5).toString();
        song->m_path = query.value(6).toString();
        song->m_time = query.value(7).toDouble();
        song->m_size = query.value(8).toDouble();
        song->m_bitRate = query.value(9).toInt();
        song->m_sampleRate = query.value(10).toInt();

        m_songs.append(song);
    }

    qDebug() << "loaded songs:" << m_songs.length();

    // Folders
    sql = "SELECT `pid`, `name`, `added`, `parent` FROM `folders`;";
    query.exec(sql);

    QHash<MFolder*, QString> folderParents;
    while (query.next()) {
        MFolder *folder = new MFolder(this);

        folder->m_pid = query.value(0).toString();
        folder->m_name = query.value(1).toString();
        folder->m_added = query.value(2).toString();

        if (!query.value(3).isNull()) {
            folderParents[folder] = query.value(3).toString();
        }

        m_folders.append(folder);
    }

    qDebug() << "loaded folders:" << m_folders.length();

    QHash<MFolder*, QString>::iterator iter;
    for(iter = folderParents.begin(); iter != folderParents.end(); iter++) {
        iter.key()->m_parentFolder = getFolder(iter.value());
    }

    // Playlists
    sql = "SELECT `pid`, `name`, `added`, `parent` FROM `playlists`;";
    query.exec(sql);

    while (query.next()) {
        MPlaylist *playlist = new MPlaylist(this);

        playlist->m_pid = query.value(0).toString();
        playlist->m_name = query.value(1).toString();
        playlist->m_added = query.value(2).toString();
        playlist->m_parentFolder = getFolder(query.value(3).toString());

        m_playlists.append(playlist);
    }

    // Playlist Songs
    sql = "SELECT `pidSong` FROM `playlistSongs` WHERE `pidPlaylist` = ?;";

    for (MPlaylist *playlist : m_playlists) {
        query.prepare(sql);
        query.bindValue(0, playlist->pid());
        query.exec();

        while (query.next()) {
            playlist->m_songsPidList << query.value(0).toString();
        }
    }

    qDebug() << "loaded playlists:" << m_playlists.length();
}
void MMediaLibrary::dbInsertSettings() {
    QSqlQuery query(m_database);
    QString sql;

    QStringList stmt;
    stmt << "INSERT INTO `settings` "
         << "(`key`, `value`) "
         << "VALUES "
         << "(?, ?), "
         << "(?, ?), "
         << "(?, ?), "
         << "(?, ?); ";
    sql = stmt.join("");

    query.prepare(sql);
    query.bindValue(0, "pid");
    query.bindValue(1, m_pid);
    query.bindValue(2, "name");
    query.bindValue(3, m_name);
    query.bindValue(4, "added");
    query.bindValue(5, m_added);
    query.bindValue(6, "localMediaPath");
    query.bindValue(7, m_localMediaPath);
    query.exec();

    qDebug() << "inserted settings";
}
void MMediaLibrary::dbInsertElement(MChildElement *element)
{
    QSqlQuery query(m_database);

    if (element->type() == Mpi3::SongElement) {
        MSong *song = static_cast<MSong*>(element);

        QStringList stmt;
        stmt << "INSERT INTO `songs` "
             << "(`pid`, `name`, `added`, `artist`, `album`, `kind`, `path`, `time`, `size`, `bitRate`, `sampleRate`) "
             << "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?); ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, song->pid());
        query.bindValue(1, song->name());
        query.bindValue(2, song->added());
        query.bindValue(3, song->artist());
        query.bindValue(4, song->album());
        query.bindValue(5, song->kind());
        query.bindValue(6, song->path());
        query.bindValue(7, song->time());
        query.bindValue(8, song->size());
        query.bindValue(9, song->bitRate());
        query.bindValue(10, song->sampleRate());
        query.exec();

        qDebug() << "inserted song" << song->pid();
    }
    else if (element->type() == Mpi3::FolderElement) {
        MFolder *folder = static_cast<MFolder*>(element);

        QStringList stmt;
        stmt << "INSERT INTO `folders` "
             << "(`pid`, `name`, `added`, `parent`) "
             << "VALUES (?, ?, ?, ?); ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, folder->pid());
        query.bindValue(1, folder->name());
        query.bindValue(2, folder->added());
        query.bindValue(3, folder->parentFolder() ? folder->parentFolder()->pid() : QString());
        query.exec();

        qDebug() << "inserted folder" << folder->pid();
    }
    else if (element->type() == Mpi3::PlaylistElement) {
        MPlaylist *playlist = static_cast<MPlaylist*>(element);

        QStringList stmt;
        stmt << "INSERT INTO `playlists` "
             << "(`pid`, `name`, `added`, `parent`) "
             << "VALUES (?, ?, ?, ?); ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, playlist->pid());
        query.bindValue(1, playlist->name());
        query.bindValue(2, playlist->added());
        query.bindValue(3, playlist->parentFolder() ? playlist->parentFolder()->pid() : QString());
        query.exec();

        qDebug() << "inserted playlist" << playlist->pid();
    }
}
void MMediaLibrary::dbUpdateElement(MChildElement *element)
{
    QSqlQuery query(m_database);

    if (element->type() == Mpi3::SongElement) {
        MSong *song = static_cast<MSong*>(element);

        QStringList stmt;
        stmt << "UPDATE `songs` "
             << "SET "
             << "`name` = ?, "
             << "`added` = ?, "
             << "`artist` = ?, "
             << "`album` = ?, "
             << "`kind` = ?, "
             << "`path` = ?, "
             << "`time` = ?, "
             << "`size` = ?, "
             << "`bitRate` = ?, "
             << "`sampleRate` = ? "
             << "WHERE `pid` = ?; ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, song->name());
        query.bindValue(1, song->added());
        query.bindValue(2, song->artist());
        query.bindValue(3, song->album());
        query.bindValue(4, song->kind());
        query.bindValue(5, song->path());
        query.bindValue(6, song->time());
        query.bindValue(7, song->size());
        query.bindValue(8, song->bitRate());
        query.bindValue(9, song->sampleRate());
        query.bindValue(10, song->pid());
        query.exec();

        qDebug() << "updated song" << song->pid();
    }
    else if (element->type() == Mpi3::FolderElement) {
        MFolder *folder = static_cast<MFolder*>(element);

        QStringList stmt;
        stmt << "UPDATE `folders` "
             << "SET "
             << "`name` = ?, "
             << "`added` = ?, "
             << "`parent` = ? "
             << "WHERE `pid` = ?; ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, folder->name());
        query.bindValue(1, folder->added());
        query.bindValue(2, folder->parentFolder() ? folder->parentFolder()->pid() : QString());
        query.bindValue(3, folder->pid());
        query.exec();

        qDebug() << "updated folder" << folder->pid();
    }
    else if (element->type() == Mpi3::PlaylistElement) {
        MPlaylist *playlist = static_cast<MPlaylist*>(element);

        QStringList stmt;
        stmt << "UPDATE `playlists` "
             << "SET "
             << "`name` = ?, "
             << "`added` = ?, "
             << "`parent` = ? "
             << "WHERE `pid` = ?; ";
        QString sql = stmt.join("");

        query.prepare(sql);
        query.bindValue(0, playlist->name());
        query.bindValue(1, playlist->added());
        query.bindValue(2, playlist->parentFolder() ? playlist->parentFolder()->pid() : QString());
        query.bindValue(3, playlist->pid());
        query.exec();

        qDebug() << "updated playlist" << playlist->pid();
    }
}
void MMediaLibrary::dbDeleteElement(MChildElement *element)
{
    QSqlQuery query(m_database);

    if (element->type() == Mpi3::SongElement) {
        QString sql = "DELETE FROM `songs` WHERE `pid` = ?";
        query.prepare(sql);
        query.bindValue(0, element->pid());
        query.exec();

        qDebug() << "deleted song" << element->pid();
    }
    else if (element->type() == Mpi3::FolderElement) {
        QString sql = "DELETE FROM `folders` WHERE `pid` = ?";
        query.prepare(sql);
        query.bindValue(0, element->pid());
        query.exec();

        qDebug() << "deleted folder" << element->pid();
    }
    else if (element->type() == Mpi3::PlaylistElement) {
        QString sql = "DELETE FROM `playlists` WHERE `pid` = ?";
        query.prepare(sql);
        query.bindValue(0, element->pid());
        query.exec();

        qDebug() << "deleted playlist" << element->pid();
    }
}

void MMediaLibrary::dbUpdatePlaylistSongs(MPlaylist *playlist) {
    QSqlQuery query(m_database);
    QString sql;

    sql = "DELETE FROM `playlistSongs` WHERE `pidPlaylist` = ?;";
    query.prepare(sql);
    query.bindValue(0, playlist->pid());
    query.exec();

    sql = "INSERT INTO `playlistSongs` (`pidPlaylist`, `pidSong`) VALUES (?, ?);";

    for (const QString &pidSong : playlist->m_songsPidList) {
        query.prepare(sql);
        query.bindValue(0, playlist->pid());
        query.bindValue(1, pidSong);
        query.exec();
        qDebug() << "inserted song" << pidSong;
    }

    qDebug() << "update playlist songs" << playlist->pid();

}

QString MMediaLibrary::path() const
{
    return m_path;
}
QString MMediaLibrary::localMediaPath() const
{
    return m_localMediaPath;
}
void MMediaLibrary::setLocalMediaPath(const QString &path)
{
    m_localMediaPath = path;
    emit libraryChanged(this);
}

MSongList MMediaLibrary::songs() const
{
    return m_songs;
}
MFolderList MMediaLibrary::folders() const
{
    return m_folders;
}
MPlaylistList MMediaLibrary::playlists() const
{
    return m_playlists;
}
MContainerList MMediaLibrary::containers() const
{
    MContainerList children;
    for(MFolder *f : m_folders) {
        children.append(f);
    }
    for(MPlaylist *p : m_playlists) {
        children.append(p);
    }

    return children;
}

MFolderList MMediaLibrary::rootFolders() const
{
    return MMediaLibrary::rootSearch<MFolderList, MFolder>(m_folders);
}
MPlaylistList MMediaLibrary::rootPlaylists() const
{
    return MMediaLibrary::rootSearch<MPlaylistList, MPlaylist>(m_playlists);
}
MContainerList MMediaLibrary::rootContainers() const
{
    return MMediaLibrary::rootSearch<MContainerList, MContainer>(containers());
}

MSong *MMediaLibrary::getSong(const QString &pid) const
{
    return MMediaLibrary::pidSearch<MSongList, MSong>(m_songs, pid);
}
MFolder *MMediaLibrary::getFolder(const QString &pid) const
{
    return MMediaLibrary::pidSearch<MFolderList, MFolder>(m_folders, pid);
}
MPlaylist *MMediaLibrary::getPlaylist(const QString &pid) const
{
    return MMediaLibrary::pidSearch<MPlaylistList, MPlaylist>(m_playlists, pid);
}
MContainer *MMediaLibrary::getContainer(const QString &pid) const
{
    return MMediaLibrary::pidSearch<MContainerList, MContainer>(containers(), pid);
}

MSongList MMediaLibrary::getSongList(const QStringList &pidList) const
{
    return MMediaLibrary::pidSearchList<MSongList, MSong>(m_songs, pidList);
}
MFolderList MMediaLibrary::getFolderList(const QStringList &pidList) const
{
    return MMediaLibrary::pidSearchList<MFolderList, MFolder>(m_folders, pidList);
}
MPlaylistList MMediaLibrary::getPlaylistList(const QStringList &pidList) const
{
    return MMediaLibrary::pidSearchList<MPlaylistList, MPlaylist>(m_playlists, pidList);
}
MContainerList MMediaLibrary::getContainerList(const QStringList &pidList) const
{
    return MMediaLibrary::pidSearchList<MContainerList, MContainer>(containers(), pidList);
}

MSong *MMediaLibrary::newSong(const QMap<QString, QVariant> &songInfoMap)
{
    MSong *song = new MSong(this);
    song->m_pid = generatePID(Mpi3::SongElement);
    song->m_name = songInfoMap["title"].toString();
//    song->m_added = ?
    song->m_path = QDir::toNativeSeparators(songInfoMap["path"].toString());
    song->m_artist = songInfoMap["artist"].toString();
    song->m_album = songInfoMap["album"].toString();
    song->m_kind = songInfoMap["kind"].toString();

    song->m_size = songInfoMap["size"].toDouble();
    song->m_time = songInfoMap["time"].toDouble();

    song->m_bitRate = songInfoMap["bitRate"].toInt();
    song->m_sampleRate = songInfoMap["sampleRate"].toInt();

    if(song->m_name.isEmpty()) {
        song->m_name = QUrl(songInfoMap["path"].toString()).fileName();
        QStringList splitExt = song->m_name.split(".");
        if(splitExt.size() > 0) {
            song->m_name = splitExt.at(0);
        }
    }

    if(!m_localMediaPath.isEmpty()) {
        QString localPath = m_localMediaPath;
        if(!song->m_artist.isEmpty()) {
            localPath += "/";
            localPath += song->m_artist;
            if (!QDir(localPath).exists()) {
                QDir().mkdir(localPath);
            }
        }
        localPath += "/";
        // Duplicate song names??
        // Use tail of song->m_path to determine file name
        localPath += song->m_name;
        localPath += ".";
        localPath += song->m_kind;

        if(QFile::copy(song->m_path, localPath)) {
            qDebug() << "copied song from" << song->m_path << "to" << localPath;
            song->m_path = QDir::toNativeSeparators(localPath);
        }
    }

    dbInsertElement(song);
    m_songs.append(song);
    emit songCreated(song);

    qDebug().nospace()
        << "created song "
        << " path=" << song->m_path
        << " name=" << song->m_name
        << " artist=" << song->m_artist
        << " album=" << song->m_album
        << " kind=" << song->m_kind
        << " size=" << song->m_size
        << " time=" << song->m_time
        << " bitRate=" << song->m_bitRate
        << " sampleRate=" << song->m_sampleRate;

    return song;
}
MFolder *MMediaLibrary::newFolder(MFolder *parentFolder, const QString &name)
{
    MFolder *folder = new MFolder(this);
    folder->m_pid = generatePID(Mpi3::FolderElement);
    folder->m_parentFolder = parentFolder;
    folder->m_name = name;

    if(folder->m_name.isEmpty()) {
        QString newName = "New Folder";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < m_folders.size()) {
            MFolder *f = m_folders.at(i++);
            if(f->name() == newName + append) {
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }

        folder->m_name = newName + append;
    }

    dbInsertElement(folder);
    m_folders.append(folder);
    emit folderCreated(folder);
    qDebug() << "created folder" << folder->m_pid << "-" << folder->m_name;
    return folder;
}
MPlaylist *MMediaLibrary::newPlaylist(MFolder *parentFolder, const QString &name)
{
    MPlaylist *playlist = new MPlaylist(this);
    playlist->m_pid = generatePID(Mpi3::PlaylistElement);
    playlist->m_parentFolder = parentFolder;
    playlist->m_name = name;

    if(playlist->m_name.isEmpty()) {
        QString newName = "New Playlist";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < m_playlists.size()) {
            MPlaylist *p = m_playlists.at(i++);
            if(p->name() == newName + append) {
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }

        playlist->m_name = newName + append;
    }

    dbInsertElement(playlist);
    m_playlists.append(playlist);
    emit playlistCreated(playlist);
    qDebug() << "created playlist" << playlist->m_pid << "-" << playlist->m_name;
    return playlist;
}

bool MMediaLibrary::edit(MSong *song, const QString &property, const QVariant &value)
{
    bool success = false;

    if (property == "name") {
        song->m_name = value.toString();
        success = true;
    }
    else if (property == "artist") {
        song->m_artist = value.toString();
        success = true;
    }
    else if (property == "album") {
        song->m_album = value.toString();
        success = true;
    }

    if (success) {
        dbUpdateElement(song);
        emit songChanged(song);
        qDebug() << "edited song" << property << ":" << song->m_pid << "-" << song->m_name;
    }

    return success;
}
bool MMediaLibrary::edit(MFolder *folder, const QString &property, const QVariant &value)
{
    if (property == "name") {
        folder->m_name = value.toString();
        dbUpdateElement(folder);
        emit folderChanged(folder);
        qDebug() << "edited folder" << property << ":" << folder->m_pid << "-" << folder->m_name;
        return true;
    }
    else if (property == "parentFolder") {
        return edit(static_cast<MContainer*>(folder), property, value);
    }
    return false;
    }
bool MMediaLibrary::edit(MPlaylist *playlist, const QString &property, const QVariant &value)
{
    if (property == "name") {
        playlist->m_name = value.toString();
        dbUpdateElement(playlist);
        emit playlistChanged(playlist);
        qDebug() << "edited playlist" << property << ":" << playlist->m_pid << "-" << playlist->m_name;
        return true;
    }
    else if (property == "songs") {
        playlist->m_songsPidList = value.toStringList();
        dbUpdatePlaylistSongs(playlist);
        emit playlistSongsChanged(playlist);
        qDebug() << "edited playlist" << property << ":" << playlist->m_pid << "-" << playlist->m_name;
        return true;
    }
    else if (property == "parentFolder") {
        return edit(static_cast<MContainer*>(playlist), property, value);
    }
    return false;
}
bool MMediaLibrary::edit(MContainer *container, const QString &property, const QVariant &value)
{
    if (property == "name") {
        if (container->type() == Mpi3::FolderElement) {
            return edit(static_cast<MFolder*>(container), property, value);
        }
        else if (container->type() == Mpi3::PlaylistElement) {
            return edit(static_cast<MPlaylist*>(container), property, value);
        }
    }
    else if (property == "parentFolder") {
        if (value.isNull()) {
            container->m_parentFolder = nullptr;
        }
        else {
            MFolder *parentFolder = getFolder(value.toString());
            if (parentFolder) {
                container->m_parentFolder = parentFolder;
            }
            else {
                return false;
            }
        }
        dbUpdateElement(container);
        emit parentFolderChanged(container);
        qDebug() << "edited container" << property << ":" << container->m_pid << "-" << container->m_name;
        return true;
    }
    return false;
}

bool MMediaLibrary::remove(MSong *song)
{
    for(MPlaylist *p : m_playlists) {
        p->m_songsPidList.removeAll(song->pid());
    }

    dbDeleteElement(song);
    m_songs.removeAll(song);
    emit songDeleted(song);
    qDebug() << "deleted song" << song->m_pid << "-" << song->m_name;
    return true;
}
bool MMediaLibrary::remove(MFolder *folder)
{
    for(MPlaylist *p : folder->childPlaylists()) {
        remove(p);
    }
    for(MFolder *f : folder->childFolders()) {
        remove(f);
    }

    dbDeleteElement(folder);
    m_folders.removeAll(folder);
    emit folderDeleted(folder);
    qDebug() << "deleted folder" << folder->m_pid << "-" << folder->m_name;
    return true;
}
bool MMediaLibrary::remove(MPlaylist *playlist)
{
    dbDeleteElement(playlist);
    m_playlists.removeAll(playlist);
    emit playlistDeleted(playlist);
    qDebug() << "deleted playlist" << playlist->m_pid << "-" << playlist->m_name;
    return true;
}
bool MMediaLibrary::remove(MContainer *container)
{
    return remove(container->pid());
}
bool MMediaLibrary::remove(const QString &pid)
{
    for(MFolder *f : m_folders) {
        if(f->pid() == pid) {
            return remove(f);
        }
    }
    for(MPlaylist *p : m_playlists) {
        if(p->pid() == pid) {
            return remove(p);
        }
    }
    for(MSong *s : m_songs) {
        if(s->pid() == pid) {
            return remove(s);
        }
    }
    return false;
}
