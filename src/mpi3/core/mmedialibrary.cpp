#include "mpi3/core/mmedialibrary.h"

#include <QRandomGenerator>
#include <QDomDocument>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QUrl>
#include <QDir>

#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QAudioOutput>
#include <QEventLoop>
#include <QTimer>


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
    for(MFolder *folder : m_parentLibrary->folders()) {
        if(folder->parentFolder() == this) {
            children.append(folder);
        }
    }
    return children;
}
MPlaylistList MFolder::childPlaylists() const
{
    MPlaylistList children;
    for(MPlaylist *playlist : m_parentLibrary->playlists()) {
        if(playlist->parentFolder() == this) {
            children.append(playlist);
        }
    }
    return children;
}
MContainerList MFolder::childContainers() const
{
    MContainerList children;
    for(MFolder *folder : childFolders()) {
        children.append(folder);
    }
    for(MPlaylist *playlist : childPlaylists()) {
        children.append(playlist);
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
    if(!xml.setContent(loadFile.readAll())) {
        qWarning() << "xml parsing error in library file" << filePath << "";
        return false;
    }

    emit libraryLoading();

    m_songs.clear();
    m_folders.clear();
    m_playlists.clear();

    QDomElement root = xml.documentElement();
    m_pid = root.namedItem("pid").toElement().text();
    m_name = root.namedItem("name").toElement().text();
    m_added = root.namedItem("added").toElement().text();
    m_localMediaPath = root.namedItem("localMediaPath").toElement().text();
    qDebug() << "found library" << m_pid << "-" << m_name;

    m_savePath = QDir::toNativeSeparators(filePath);
    emit libraryChanged(this);

    QDomNodeList xmlSongs = root.namedItem("songs").toElement().childNodes();
    QDomNodeList xmlPlaylists = root.namedItem("playlists").toElement().childNodes();
    QDomNodeList xmlFolders = root.namedItem("folders").toElement().childNodes();

    for(int i = 0; i < xmlSongs.length(); i++) {
        QDomNode node = xmlSongs.at(i);
        MSong *song = new MSong(this);

        song->m_pid = node.namedItem("pid").toElement().text();
        song->m_name = node.namedItem("name").toElement().text();
        song->m_added = node.namedItem("added").toElement().text();

        song->m_artist = node.namedItem("artist").toElement().text();
        song->m_album = node.namedItem("album").toElement().text();
        song->m_kind = node.namedItem("kind").toElement().text();

        song->m_path = node.namedItem("path").toElement().text();

        song->m_time = node.namedItem("time").toElement().text().toDouble();
        song->m_size = node.namedItem("size").toElement().text().toDouble();

        song->m_bitRate = node.namedItem("bitRate").toElement().text().toInt();
        song->m_sampleRate = node.namedItem("sampleRate").toElement().text().toInt();

        m_songs.append(song);
    }

    QHash<MFolder*, QString> folderParents;
    for(int i = 0; i < xmlFolders.length(); i++) {
        QDomNode node = xmlFolders.at(i);
        MFolder *folder = new MFolder(this);

        folder->m_pid = node.namedItem("pid").toElement().text();
        folder->m_name = node.namedItem("name").toElement().text();
        folder->m_added = node.namedItem("added").toElement().text();

        QString parentPID = node.namedItem("parent").toElement().text();
        if(parentPID != "") {
            folderParents[folder] = parentPID;
        }

        m_folders.append(folder);
    }

    QHash<MFolder*, QString>::iterator iter;
    for(iter = folderParents.begin(); iter != folderParents.end(); iter++) {
        iter.key()->m_parentFolder = getFolder(iter.value());
    }

    for(int i = 0; i < xmlPlaylists.length(); i++) {
        QDomNode node = xmlPlaylists.at(i);
        MPlaylist *playlist = new MPlaylist(this);

        playlist->m_pid = node.namedItem("pid").toElement().text();
        playlist->m_name = node.namedItem("name").toElement().text();
        playlist->m_added = node.namedItem("added").toElement().text();

        QString parentPID = node.namedItem("parent").toElement().text();
        playlist->m_parentFolder = getFolder(parentPID);

        QDomNodeList childSongs = node.namedItem("childSongs").childNodes();
        for(int c = 0; c < childSongs.size(); c++) {
            playlist->m_songsPidList.append(childSongs.at(c).toElement().text());
        }

        m_playlists.append(playlist);
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
        qDebug() << saveFile.errorString();
        return false;
    }

    emit librarySaving();

    if(!filePath.isNull() && filePath != m_savePath) {
        m_savePath = QDir::toNativeSeparators(filePath);
        emit libraryChanged(this);
    }

    QDomDocument xml("Mpi3Library");
    QDomElement root = xml.createElement("library");
    xml.appendChild(root);

    xmlWriteElement(xml, root, "pid", m_pid);
    xmlWriteElement(xml, root, "name", m_name);
    xmlWriteElement(xml, root, "added", m_added);
    xmlWriteElement(xml, root, "localMediaPath", m_localMediaPath);

    QDomElement xmlSongs = xml.createElement("songs");
    QDomElement xmlPlaylists = xml.createElement("playlists");
    QDomElement xmlFolders = xml.createElement("folders");

    root.appendChild(xmlSongs);
    root.appendChild(xmlPlaylists);
    root.appendChild(xmlFolders);

    for(MSong *song : m_songs) {
        QDomElement songElement = xml.createElement("song");

        xmlWriteElement(xml, songElement, "pid", song->m_pid);
        xmlWriteElement(xml, songElement, "name", song->m_name);
        xmlWriteElement(xml, songElement, "added", song->m_added);

        xmlWriteElement(xml, songElement, "artist", song->m_artist);
        xmlWriteElement(xml, songElement, "album", song->m_album);
        xmlWriteElement(xml, songElement, "kind", song->m_kind);

        xmlWriteElement(xml, songElement, "path", song->m_path);

        xmlWriteElement(xml, songElement, "time", QString::number(song->m_time));
        xmlWriteElement(xml, songElement, "size", QString::number(song->m_size));

        xmlWriteElement(xml, songElement, "bitRate", QString::number(song->m_bitRate));
        xmlWriteElement(xml, songElement, "sampleRate", QString::number(song->m_sampleRate));

        xmlSongs.appendChild(songElement);
    }

    for(MFolder *folder : m_folders) {
        QDomElement folderElement = xml.createElement("folder");

        xmlWriteElement(xml, folderElement, "pid", folder->m_pid);
        xmlWriteElement(xml, folderElement, "name", folder->m_name);
        xmlWriteElement(xml, folderElement, "added", folder->m_added);

        MFolder *parentFolder = folder->m_parentFolder;
        xmlWriteElement(xml, folderElement, "parent", parentFolder ? parentFolder->m_pid : "");

        xmlFolders.appendChild(folderElement);
    }

    for(MPlaylist *playlist : m_playlists) {
        QDomElement playlistElement = xml.createElement("playlist");

        xmlWriteElement(xml, playlistElement, "pid", playlist->m_pid);
        xmlWriteElement(xml, playlistElement, "name", playlist->m_name);
        xmlWriteElement(xml, playlistElement, "added", playlist->m_added);

        MFolder *parentFolder = playlist->m_parentFolder;
        xmlWriteElement(xml, playlistElement, "parent",
            parentFolder ? parentFolder->m_pid : "");

        QDomElement playlistSongs = xml.createElement("childSongs");
        for(const QString &pid : playlist->m_songsPidList) {
            xmlWriteElement(xml, playlistSongs, "pid", pid);
        }

        playlistElement.appendChild(playlistSongs);
        xmlPlaylists.appendChild(playlistElement);
    }

    QTextStream xmlStream(&saveFile);
    xmlStream << xml.toString();

    emit librarySaved();

    qInfo() << "saved library" << m_pid << "-" << m_name << "to" << m_savePath;

    return true;
}
void MMediaLibrary::reset()
{
    qInfo() << "resetting library" << m_pid << "-" << m_name;

    emit libraryResetting();

    // TODO: check this deletes the objects?
    m_songs.clear();
    m_playlists.clear();
    m_folders.clear();

    emit libraryReset();
}

QString MMediaLibrary::savePath() const
{
    return m_savePath;
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
    for(MFolder *folder : m_folders) {
        children.append(folder);
    }
    for(MPlaylist *playlist : m_playlists) {
        children.append(playlist);
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

MSong *MMediaLibrary::newSong(const QUrl &url)
{
    QMediaPlayer *mediaPlayer = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(url);

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    connect(mediaPlayer, &QMediaPlayer::metaDataChanged, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(100);
    loop.exec();

    if (mediaPlayer->metaData().isEmpty()) {
        qWarning() << "error reading file metadata from" << url.toString();
        return nullptr;
    }

    QMediaMetaData mediaMetaData = mediaPlayer->metaData();
    QFileInfo fileInfo = QFileInfo(mediaPlayer->source().toLocalFile());

    MSong *song = new MSong(this);
    song->m_pid = generatePID(Mpi3::SongElement);
    song->m_name = mediaMetaData.value(QMediaMetaData::Title).toString();
    song->m_path = QDir::toNativeSeparators(url.toLocalFile());
    song->m_artist = mediaMetaData.value(QMediaMetaData::Author).toString();
    song->m_album = mediaMetaData.value(QMediaMetaData::AlbumTitle).toString();
    song->m_kind = fileInfo.suffix();

    song->m_size = fileInfo.size();
    song->m_time = mediaMetaData.value(QMediaMetaData::Duration).toDouble();

    song->m_bitRate = mediaMetaData.value(QMediaMetaData::AudioBitRate).toInt();
    // TODO: Not sure how to calculate
    song->m_sampleRate = 44100;

    delete mediaPlayer;
    delete audioOutput;

    if(song->m_name.isEmpty()) {
        song->m_name = url.fileName();
        QStringList splitExt = song->m_name.split(".");
        if(splitExt.size() > 0) {
            song->m_name = splitExt.at(0);
        }
    }

//    if(!m_localMediaPath.isNull() && !m_localMediaPath.isEmpty()) {
//        QString localPath = m_localMediaPath;
//        if(!song->m_artist.isNull() && !song->m_artist.isEmpty()) {
//            localPath += "/";
//            localPath += song->m_artist;
//            if (!QDir(localPath).exists()) {
//                QDir().mkdir(localPath);
//            }
//        }
//        localPath += "/";
//        // Duplicate song names??
//        // Use tail of song->m_path to determine file name
//        localPath += song->m_name;
//        localPath += ".";
//        localPath += song->m_kind;

//        if(QFile::copy(song->m_path, localPath)) {
//            qDebug() << "copied song from" << song->m_path << "to" << localPath;
//            song->m_path = QDir::toNativeSeparators(localPath);
//        }
//    }

    m_songs.append(song);
    emit songCreated(song);

    qDebug().nospace()
        << "created new song "
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

    if(folder->m_name.isNull()) {
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
        emit songChanged(song);
        qDebug() << "edited song" << property << ":" << song->m_pid << "-" << song->m_name;
    }

    return success;
}
bool MMediaLibrary::edit(MFolder *folder, const QString &property, const QVariant &value)
{
    if (property == "name") {
        folder->m_name = value.toString();
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
        emit playlistChanged(playlist);
        qDebug() << "edited playlist" << property << ":" << playlist->m_pid << "-" << playlist->m_name;
        return true;
    }
    else if (property == "songs") {
        playlist->m_songsPidList = value.toStringList();
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
        container->m_name = value.toString();
        if (container->type() == Mpi3::FolderElement) {
            emit folderChanged(static_cast<MFolder*>(container));
        }
        else if (container->type() == Mpi3::PlaylistElement) {
            emit playlistChanged(static_cast<MPlaylist*>(container));
        }
        qDebug() << "edited container" << property << ":" << container->m_pid << "-" << container->m_name;
        return true;
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
        emit parentFolderChanged(container);
        qDebug() << "edited container" << property << ":" << container->m_pid << "-" << container->m_name;
        return true;
    }
    return false;
}

bool MMediaLibrary::remove(MSong *song)
{
    m_songs.removeAll(song);
    emit songDeleted(song);

    for(MPlaylist *playlist : m_playlists) {
        playlist->m_songsPidList.removeAll(song->pid());
    }

    qDebug() << "deleted song" << song->m_pid << "-" << song->m_name;
    return true;

}
bool MMediaLibrary::remove(MFolder *folder)
{
    m_folders.removeAll(folder);
    emit folderDeleted(folder);

    for(MPlaylist *childPlaylist : folder->childPlaylists()) {
        remove(childPlaylist);
    }
    for(MFolder *childFolder : folder->childFolders()) {
        remove(childFolder);
    }

    qDebug() << "deleted folder" << folder->m_pid << "-" << folder->m_name;
    return true;
}
bool MMediaLibrary::remove(MPlaylist *playlist)
{
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
    for(MFolder *folder : m_folders) {
        if(folder->pid() == pid) {
            return remove(folder);
        }
    }
    for(MPlaylist *playlist : m_playlists) {
        if(playlist->pid() == pid) {
            return remove(playlist);
        }
    }
    for(MSong *song : m_songs) {
        if(song->pid() == pid) {
            return remove(song);
        }
    }
    return false;
}
