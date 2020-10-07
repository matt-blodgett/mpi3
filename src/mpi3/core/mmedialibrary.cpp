#include "mpi3/core/mmedialibrary.h"
#include "mpi3/core/maudioengine.h"

#include <QRandomGenerator>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QDir>


#include <QDebug>


static void xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tag, const QString &text)
{
    QDomElement e = xml.createElement(tag);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
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
    for(MFolder *f : m_parentLibrary->folders()){
        if(f->parentFolder() == this){
            children.append(f);
        }
    }

    return children;
}
MPlaylistList MFolder::childPlaylists() const
{
    MPlaylistList children;
    for(MPlaylist *p : m_parentLibrary->playlists()){
        if(p->parentFolder() == this){
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
    m_added = "01/01/1990"; // change to current date
}
Mpi3::ElementType MMediaLibrary::type() const
{
    return Mpi3::LibraryElement;
}

bool MMediaLibrary::load(const QString &filePath)
{
    qInfo() << "loading library from file" << filePath;

    QFile loadFile(filePath);
    if(!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "error opening file for read" << filePath;
        return false;
    }

    QDomDocument xml;
    if(!xml.setContent(loadFile.readAll())){
        qWarning() << "xml parsing error in library file" << filePath << "";
        return false;
    }

    emit aboutToLoad();

    m_songs.clear();
    m_folders.clear();
    m_playlists.clear();

    QDomElement root = xml.documentElement();
    m_pid = root.namedItem("pid").toElement().text();
    m_name = root.namedItem("name").toElement().text();
    m_added = root.namedItem("added").toElement().text();
    qDebug() << "found library" << m_pid << "-" << m_name;

    QString oldPath = m_savePath;
    m_savePath = QDir::toNativeSeparators(filePath);
    emit libraryChanged(this);

    setMediaPath(root.namedItem("mediaPath").toElement().text());
    setBackupPath(root.namedItem("backupPath").toElement().text());
    setDownloadPath(root.namedItem("downloadPath").toElement().text());

    QDomNodeList xmlSongs = root.namedItem("songs").toElement().childNodes();
    QDomNodeList xmlPlaylists = root.namedItem("playlists").toElement().childNodes();
    QDomNodeList xmlFolders = root.namedItem("folders").toElement().childNodes();

    for(int i = 0; i < xmlSongs.length(); i++) {
        QDomNode node = xmlSongs.at(i);
        MSong *s = new MSong(this);

        s->m_pid = node.namedItem("pid").toElement().text();
        s->m_name = node.namedItem("name").toElement().text();
        s->m_added = node.namedItem("added").toElement().text();

        s->m_artist = node.namedItem("artist").toElement().text();
        s->m_album = node.namedItem("album").toElement().text();
        s->m_kind = node.namedItem("kind").toElement().text();

        s->m_path = node.namedItem("path").toElement().text();

        s->m_time = node.namedItem("time").toElement().text().toDouble();
        s->m_size = node.namedItem("size").toElement().text().toDouble();

        s->m_bitRate = node.namedItem("bitRate").toElement().text().toInt();
        s->m_sampleRate = node.namedItem("sampleRate").toElement().text().toInt();

        m_songs.append(s);
    }

    QMap<MFolder*, QString> folderParents;
    for(int i = 0; i < xmlFolders.length(); i++) {
        QDomNode node = xmlFolders.at(i);
        MFolder *f = new MFolder(this);

        f->m_pid = node.namedItem("pid").toElement().text();
        f->m_name = node.namedItem("name").toElement().text();
        f->m_added = node.namedItem("added").toElement().text();

        QString parentPID = node.namedItem("parent").toElement().text();
        if(parentPID != ""){
            folderParents[f] = parentPID;
        }

        m_folders.append(f);
    }

    QMap<MFolder*, QString>::iterator iter;
    for(iter = folderParents.begin(); iter != folderParents.end(); iter++){
        iter.key()->m_parentFolder = getFolder(iter.value());
    }

    for(int i = 0; i < xmlPlaylists.length(); i++) {
        QDomNode node = xmlPlaylists.at(i);
        MPlaylist *p = new MPlaylist(this);

        p->m_pid = node.namedItem("pid").toElement().text();
        p->m_name = node.namedItem("name").toElement().text();
        p->m_added = node.namedItem("added").toElement().text();

        QString parentPID = node.namedItem("parent").toElement().text();
        p->m_parentFolder = getFolder(parentPID);

        QDomNodeList childSongs = node.namedItem("childSongs").childNodes();
        for(int c = 0; c < childSongs.size(); c++) {
            p->m_songsPidList.append(childSongs.at(c).toElement().text());
        }

        m_playlists.append(p);
    }

    emit libraryLoaded();

    qInfo()
        << "loaded library"
        << m_pid << "-"
        << m_name << "from"
        << m_savePath << "with"
        << m_folders.size() << "folders,"
        << m_playlists.size() << "playlists, and"
        << m_songs.size() << "songs";

    return true;
}
bool MMediaLibrary::save(const QString &filePath)
{
    qInfo() << "saving library" << m_pid << "to file" << (filePath.isNull() ? m_savePath : filePath);
    Q_ASSERT_X(!(m_savePath.isNull() && filePath.isNull()), "MMediaLibrary::save", "saving to null path");

    QFile saveFile(filePath.isNull() ? m_savePath : filePath);
    if(!saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "error opening file for write" << filePath;
        return false;
    }

    emit aboutToSave();

    if(!filePath.isNull() && filePath != m_savePath){
        QString oldPath = m_savePath;
        m_savePath = QDir::toNativeSeparators(filePath);
        emit libraryChanged(this);
    }

    QDomDocument xml("Mpi3Library");
    QDomElement root = xml.createElement("library");
    xml.appendChild(root);

    xmlWriteElement(xml, root, "pid", m_pid);
    xmlWriteElement(xml, root, "name", m_name);
    xmlWriteElement(xml, root, "added", m_added);
    xmlWriteElement(xml, root, "mediaPath", m_mediaPath);
    xmlWriteElement(xml, root, "backupPath", m_backupPath);
    xmlWriteElement(xml, root, "downloadPath", m_downloadPath);

    QDomElement xmlSongs = xml.createElement("songs");
    QDomElement xmlPlaylists = xml.createElement("playlists");
    QDomElement xmlFolders = xml.createElement("folders");

    root.appendChild(xmlSongs);
    root.appendChild(xmlPlaylists);
    root.appendChild(xmlFolders);

    for(MSong *s : m_songs){
        QDomElement songElement = xml.createElement("song");

        xmlWriteElement(xml, songElement, "pid", s->m_pid);
        xmlWriteElement(xml, songElement, "name", s->m_name);
        xmlWriteElement(xml, songElement, "added", s->m_added);

        xmlWriteElement(xml, songElement, "artist", s->m_artist);
        xmlWriteElement(xml, songElement, "album", s->m_album);
        xmlWriteElement(xml, songElement, "kind", s->m_kind);

        xmlWriteElement(xml, songElement, "path", s->m_path);

        xmlWriteElement(xml, songElement, "time", QString::number(s->m_time));
        xmlWriteElement(xml, songElement, "size", QString::number(s->m_size));

        xmlWriteElement(xml, songElement, "bitRate", QString::number(s->m_bitRate));
        xmlWriteElement(xml, songElement, "sampleRate", QString::number(s->m_sampleRate));

        xmlSongs.appendChild(songElement);
    }

    for(MFolder *f : m_folders){
        QDomElement folderElement = xml.createElement("folder");

        xmlWriteElement(xml, folderElement, "pid", f->m_pid);
        xmlWriteElement(xml, folderElement, "name", f->m_name);
        xmlWriteElement(xml, folderElement, "added", f->m_added);

        MFolder *parentFolder = f->m_parentFolder;
        xmlWriteElement(xml, folderElement, "parent",
            parentFolder ? parentFolder->m_pid : "");

        xmlFolders.appendChild(folderElement);
    }

    for(MPlaylist *p : m_playlists){
        QDomElement playlistElement = xml.createElement("playlist");

        xmlWriteElement(xml, playlistElement, "pid", p->m_pid);
        xmlWriteElement(xml, playlistElement, "name", p->m_name);
        xmlWriteElement(xml, playlistElement, "added", p->m_added);

        MFolder *parentFolder = p->m_parentFolder;
        xmlWriteElement(xml, playlistElement, "parent",
            parentFolder ? parentFolder->m_pid : "");

        QDomElement playlistSongs = xml.createElement("childSongs");
        for(QString pid : p->m_songsPidList) {
            xmlWriteElement(xml, playlistSongs, "pid", pid);
        }

        playlistElement.appendChild(playlistSongs);
        xmlPlaylists.appendChild(playlistElement);
    }

    QTextStream xmlStream(&saveFile);
    xmlStream << xml.toString();

    emit librarySaved();

    qInfo()
        << "saved library"
        << m_pid << "-"
        << m_name << "to"
        << m_savePath;

    return true;
}
void MMediaLibrary::reset()
{
    qInfo() << "resetting library" << m_pid << "-" << m_name;

    emit aboutToReset();

    // TODO: check this deletes the objects?
    m_songs.clear();
    m_playlists.clear();
    m_folders.clear();

    m_mediaPath = QString();
    m_backupPath = QString();
    m_downloadPath = QString();

    emit libraryReset();
}

QString MMediaLibrary::savePath() const
{
    return m_savePath;
}
QString MMediaLibrary::mediaPath() const
{
    return m_mediaPath;
}
QString MMediaLibrary::backupPath() const
{
    return m_backupPath;
}
QString MMediaLibrary::downloadPath() const
{
    return m_downloadPath;
}

bool MMediaLibrary::setMediaPath(const QString &dirPath)
{
    m_mediaPath = QDir::toNativeSeparators(dirPath);
    emit libraryChanged(this);
    return true;
}
bool MMediaLibrary::setBackupPath(const QString &dirPath)
{
    m_backupPath = QDir::toNativeSeparators(dirPath);
    emit libraryChanged(this);
    return true;
}
bool MMediaLibrary::setDownloadPath(const QString &dirPath)
{
    m_downloadPath = QDir::toNativeSeparators(dirPath);
    emit libraryChanged(this);
    return true;
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
    for(MFolder *f : m_folders){
        children.append(f);
    }
    for(MPlaylist *p : m_playlists){
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

QString MMediaLibrary::generatePID(Mpi3::ElementType elementType)
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

MSong *MMediaLibrary::newSong(const QString &filePath)
{
    if(!QFile::exists(filePath)){
        qWarning() << "error opening file for read" << filePath;
        return nullptr;
    }

    MSong *song = new MSong(this);
    song->m_pid = generatePID(Mpi3::SongElement);
    song->m_path = QDir::toNativeSeparators(QUrl(filePath).toLocalFile());
    while(song->m_path.startsWith("\\")) {
        song->m_path.remove(0, 1);
    }

    MSongInfo info;
    if(info.load(filePath)) {
        song->m_name = info.title;
        song->m_artist = info.artist;
        song->m_album = info.album;
        song->m_kind = info.kind;
        song->m_path = filePath;

        song->m_size = info.size;
        song->m_time = info.time;

        song->m_bitRate = info.bitRate;
        song->m_sampleRate = info.sampleRate;

        qDebug().nospace()
            << "loaded song info from "
            << " path=" << song->m_path
            << " name=" << song->m_name
            << " artist=" << song->m_artist
            << " album=" << song->m_album
            << " kind=" << song->m_kind
            << " size=" << song->m_size
            << " time=" << song->m_time
            << " bitRate=" << song->m_bitRate
            << " sampleRate=" << song->m_sampleRate;
    }
    else {
        song->m_size = static_cast<double>(QFileInfo(filePath).size());
    }

    if(song->m_name.isEmpty() || song->m_name.isNull()) {
        song->m_name = QUrl(filePath).fileName();
        QStringList splitExt = song->m_name.split(".");
        if(splitExt.size() > 0) {
            song->m_name = splitExt.at(0);
        }
    }

    m_songs.append(song);
    emit songCreated(song);
    qDebug() << "created new song" << song->m_pid << "-" << song->m_name;
    return song;
}
MFolder *MMediaLibrary::newFolder(MFolder *parentFolder, const QString &name)
{
    MFolder *folder = new MFolder(this);
    folder->m_pid = generatePID(Mpi3::FolderElement);
    folder->m_parentFolder = parentFolder;
    folder->m_name = name;

    if(folder->m_name.isNull()){
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

    m_folders.append(folder);
    emit folderCreated(folder);
    qDebug() << "created new folder" << folder->m_pid << "-" << folder->m_name;
    return folder;
}
MPlaylist *MMediaLibrary::newPlaylist(MFolder *parentFolder, const QString &name)
{
    MPlaylist *playlist = new MPlaylist(this);
    playlist->m_pid = generatePID(Mpi3::PlaylistElement);
    playlist->m_parentFolder = parentFolder;
    playlist->m_name = name;

    if(playlist->m_name.isNull()) {
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

    m_playlists.append(playlist);
    emit playlistCreated(playlist);
    qDebug() << "created new playlist" << playlist->m_pid << "-" << playlist->m_name;
    return playlist;
}

bool MMediaLibrary::edit(MSong *childSong, const QString &property, const QVariant &value)
{
    bool success = false;

    if (property == "name") {
        childSong->m_name = value.toString();
        success = true;
    }
    else if (property == "artist") {
        childSong->m_artist = value.toString();
        success = true;
    }
    else if (property == "album") {
        childSong->m_album = value.toString();
        success = true;
    }

    if (success) {
        emit songChanged(childSong);
        qDebug() << "edited song" << property << ":" << childSong->m_pid << "-" << childSong->m_name;
    }

    return success;
}
bool MMediaLibrary::edit(MFolder *childFolder, const QString &property, const QVariant &value)
{
    if (property == "name") {
        childFolder->m_name = value.toString();
        emit folderChanged(childFolder);
        qDebug() << "edited folder" << property << ":" << childFolder->m_pid << "-" << childFolder->m_name;
        return true;
    }
    else if (property == "parentFolder") {
        return edit(static_cast<MContainer*>(childFolder), property, value);
    }
    return false;
    }
bool MMediaLibrary::edit(MPlaylist *childPlaylist, const QString &property, const QVariant &value)
{
    if (property == "name") {
        childPlaylist->m_name = value.toString();
        emit playlistChanged(childPlaylist);
        qDebug() << "edited playlist" << property << ":" << childPlaylist->m_pid << "-" << childPlaylist->m_name;
        return true;
    }
    else if (property == "songs") {
        childPlaylist->m_songsPidList = value.toStringList();
        emit playlistSongsChanged(childPlaylist);
        qDebug() << "edited playlist" << property << ":" << childPlaylist->m_pid << "-" << childPlaylist->m_name;
        return true;
    }
    else if (property == "parentFolder") {
        return edit(static_cast<MContainer*>(childPlaylist), property, value);
    }
    return false;
}
bool MMediaLibrary::edit(MContainer *childContainer, const QString &property, const QVariant &value)
{
    if (property == "name") {
        childContainer->m_name = value.toString();
        if (childContainer->type() == Mpi3::FolderElement) {
            emit folderChanged(static_cast<MFolder*>(childContainer));
        }
        else if (childContainer->type() == Mpi3::PlaylistElement) {
            emit playlistChanged(static_cast<MPlaylist*>(childContainer));
        }
        qDebug() << "edited container" << property << ":" << childContainer->m_pid << "-" << childContainer->m_name;
        return true;
    }
    else if (property == "parentFolder") {
        if (value.isNull()) {
            childContainer->m_parentFolder = nullptr;
        }
        else {
            MFolder *parentFolder = getFolder(value.toString());
            if (parentFolder) {
                childContainer->m_parentFolder = parentFolder;
            }
            else {
                return false;
            }
        }
        emit parentFolderChanged(childContainer);
        qDebug() << "edited container" << property << ":" << childContainer->m_pid << "-" << childContainer->m_name;
        return true;
    }
    return false;
}

bool MMediaLibrary::remove(MSong *childSong)
{
    m_songs.removeAll(childSong);
    emit songDeleted(childSong);

    for(MPlaylist *p : m_playlists){
        p->m_songsPidList.removeAll(childSong->pid());
    }

    qDebug() << "deleted song" << childSong->m_pid << "-" << childSong->m_name;
    return true;

}
bool MMediaLibrary::remove(MFolder *childFolder)
{
    m_folders.removeAll(childFolder);
    emit folderDeleted(childFolder);

    for(MPlaylist *p : childFolder->childPlaylists()){
        remove(p);
    }
    for(MFolder *f : childFolder->childFolders()){
        remove(f);
    }

    qDebug() << "deleted folder" << childFolder->m_pid << "-" << childFolder->m_name;
    return true;
}
bool MMediaLibrary::remove(MPlaylist *childPlaylist)
{
    m_playlists.removeAll(childPlaylist);
    emit playlistDeleted(childPlaylist);
    qDebug() << "deleted playlist" << childPlaylist->m_pid << "-" << childPlaylist->m_name;
    return true;
}
bool MMediaLibrary::remove(MContainer *childContainer)
{
    return remove(childContainer->pid());
}
bool MMediaLibrary::remove(const QString &pid)
{
    for(MFolder *f : m_folders){
        if(f->pid() == pid){
            return remove(f);
        }
    }
    for(MPlaylist *p : m_playlists){
        if(p->pid() == pid){
            return remove(p);
        }
    }
    for(MSong *s : m_songs){
        if(s->pid() == pid){
            return remove(s);
        }
    }
    return false;
}
