#include "mmedialibrary.h"
#include "maudioengine.h"

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
static QMap<QString, QVariant> plistDict(const QDomNode &parentNode)
{
    QMap<QString, QVariant> dict;
    for(int i = 0; i < parentNode.childNodes().size(); i++) {
        QDomNode childNode = parentNode.childNodes().at(i);

        if(childNode.toElement().tagName() == "key") {

            QDomElement keyNode = childNode.toElement();
            QDomElement valueNode = parentNode.childNodes().at(i + 1).toElement();

            if(valueNode.tagName() == "dict") {
                dict[keyNode.text()] = plistDict(valueNode);
            }
            else if(valueNode.tagName() == "array") {
                QList<QVariant> arrayItems;
                for(int cnode = 0; cnode < valueNode.childNodes().size(); cnode++) {
                    arrayItems.append(plistDict(valueNode.childNodes().at(cnode)));
                }
                dict[keyNode.text()] = arrayItems;
            }
            else {
                dict[keyNode.text()] = parentNode.childNodes().at(i + 1).toElement().text();
            }
        }
    }

    return dict;
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

void MMediaElement::setName(const QString &value)
{
    QString oldName = m_name;
    m_name = value;

    notifyPropertyChanged("name", oldName);
}

void MMediaElement::notifyPropertyChanged(const QString &propertyName, const QVariant &oldPropertyValue)
{
    MMediaLibrary *parentLibrary = static_cast<MMediaLibrary*>(this->parent());
    if(this->type() != Mpi3::LibraryElement && !parentLibrary){
        qWarning() << this->metaObject()->className() << "instance has no parent library!";
        return;
    }

    QVariant newPropertyValue = this->property(propertyName.toStdString().c_str());

    qDebug()
        << this->metaObject()->className()
        << this->m_pid << this->m_name
        << "- property" << propertyName << "changed from"
        << oldPropertyValue.toString() << "to"
        << newPropertyValue.toString();

    Q_ASSERT_X(newPropertyValue.isValid(),
               "MMediaElement::notifyPropertyChanged",
               "invalid new property value");

    if(this->type() == Mpi3::SongElement){
        emit parentLibrary->songChanged(static_cast<MSong*>(this));
    }
    else if(this->type() == Mpi3::PlaylistElement){
        emit parentLibrary->playlistChanged(static_cast<MPlaylist*>(this));
    }
    else if(this->type() == Mpi3::FolderElement){
        emit parentLibrary->folderChanged(static_cast<MFolder*>(this));
    }
    else if(this->type() == Mpi3::LibraryElement){
        emit parentLibrary->libraryChanged(static_cast<MMediaLibrary*>(this));
    }
}


MSong::MSong(MMediaLibrary *parent) : MMediaElement(parent)
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

QString MSong::timeString() const
{
    return MMediaLibrary::timeToString(m_time);
}
QString MSong::sizeString() const
{
    return MMediaLibrary::sizeToString(m_size);
}

int MSong::bitRate() const
{
    return m_bitRate;
}
int MSong::sampleRate() const
{
    return m_sampleRate;
}

void MSong::setArtist(const QString &value)
{
    QString oldValue = m_artist;
    m_artist = value;

    notifyPropertyChanged("artist", oldValue);
}
void MSong::setAlbum(const QString &value)
{
    QString oldValue = m_album;
    m_album = value;

    notifyPropertyChanged("album", oldValue);
}

MContainer::MContainer(MMediaLibrary *parent) : MMediaElement(parent)
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
void MContainer::setParentFolder(MFolder *folder)
{
    MMediaLibrary *parentLibrary = static_cast<MMediaLibrary*>(this->parent());
    Q_ASSERT_X(parentLibrary, "MContainer::setParentFolder", "container has no parent library");

    if(this->type() == Mpi3::FolderElement){
        // TODO: also check if new parent folder is a child folder (recursive)
        Q_ASSERT_X(folder != static_cast<MFolder*>(this), "MContainer::setParentFolder",
                   "child folder's parent folder set to instance of itself");
    }

    QString oldPID = m_parentFolder ? m_parentFolder->pid() : parentLibrary->pid();
    QString oldName = m_parentFolder ? m_parentFolder->name() : "root";
    QString newPID = folder ? folder->pid() : parentLibrary->pid();
    QString newName = folder ? folder->name() : "root";

    qDebug()
        << this->metaObject()->className()
        << this->pid() << this->name()
        << "- parent folder changed from"
        << oldPID << oldName << "to"
        << newPID << newName;

    m_parentFolder = folder;

    emit parentLibrary->parentFolderChanged(this);
}


MPlaylist::MPlaylist(MMediaLibrary *parent) : MContainer(parent)
{

}
Mpi3::ElementType MPlaylist::type() const
{
    return Mpi3::PlaylistElement;
}

MSongList MPlaylist::songs() const
{
    return m_songs;
}

void MPlaylist::insert(int index, MSong *song)
{
    m_songs.insert(index, song);
    notifyContentsChanged();
}
void MPlaylist::insert(int index, const MSongList &songlist)
{
    for(MSong *s : songlist){
        m_songs.insert(index, s);
    }

    notifyContentsChanged();
}

void MPlaylist::append(MSong *song)
{
    m_songs.append(song);
    notifyContentsChanged();
}
void MPlaylist::append(const MSongList &songlist)
{
    for(MSong *s : songlist){
        m_songs.append(s);
    }
    notifyContentsChanged();
}

void MPlaylist::prepend(MSong *song)
{
    m_songs.prepend(song);
    notifyContentsChanged();
}
void MPlaylist::prepend(const MSongList &songlist)
{
    for(MSong *s : songlist){
        m_songs.prepend(s);
    }

    notifyContentsChanged();
}

void MPlaylist::move(int from, int to)
{
    m_songs.move(from, to);
    notifyContentsChanged();
}
void MPlaylist::move(QList<int> indexes, int to)
{
    MSongList moveList;
    for(int i : indexes){
        moveList.append(m_songs.at(i));
    }

    for(MSong *s : moveList){
        m_songs.remove(m_songs.indexOf(s));
    }

    if(to >= m_songs.size()){
        to = m_songs.size();
    }

    for(MSong *s : moveList){
        m_songs.insert(to, s);
    }

    notifyContentsChanged();
}

void MPlaylist::remove(int index)
{
    m_songs.remove(index);
    notifyContentsChanged();
}
void MPlaylist::remove(QList<int> indexes)
{
    MSongList removeList;
    for(int i : indexes){
        removeList.append(m_songs.at(i));
    }

    for(MSong *s : removeList){
        m_songs.remove(m_songs.indexOf(s));
    }

    notifyContentsChanged();
}

void MPlaylist::clear()
{
    m_songs.clear();
    notifyContentsChanged();
}

void MPlaylist::notifyContentsChanged()
{
    MMediaLibrary *parentLibrary = static_cast<MMediaLibrary*>(this->parent());
    if(this->type() != Mpi3::LibraryElement && !parentLibrary){
        qWarning() << this->metaObject()->className() << "instance has no parent library!";
        return;
    }

    qDebug()
        << this->metaObject()->className()
        << this->pid() << this->name()
        << "- playlist contents changed";

    emit parentLibrary->playlistContentsChanged(this);
}


MFolder::MFolder(MMediaLibrary *parent) : MContainer(parent)
{

}
Mpi3::ElementType MFolder::type() const
{
    return Mpi3::FolderElement;
}

MSongList MFolder::childSongs(bool recursive) const
{
    Q_ASSERT_X(static_cast<MMediaLibrary*>(this->parent()),
        "MFolder::childSongs", "folder instance has no parent library");

    MSongList children;
    for(MPlaylist *p : childPlaylists(recursive)){
        for(MSong *s : p->songs()){
            if(!children.contains(s)){
                children.append(s);
            }
        }
    }

    return children;
}
MFolderList MFolder::childFolders(bool recursive) const
{
    MMediaLibrary *parentLibrary = static_cast<MMediaLibrary*>(this->parent());
    Q_ASSERT_X(parentLibrary, "MFolder::childFolders",
               "folder instance has no parent library");

    MFolderList children;
    for(MFolder *f : parentLibrary->folders()){
        if(f->parentFolder() == this){
            children.append(f);

            if(recursive){
                for(MFolder *child : f->childFolders(recursive)){
                    children.append(child);
                }
            }
        }
    }

    return children;
}
MPlaylistList MFolder::childPlaylists(bool recursive) const
{
    MMediaLibrary *parentLibrary = static_cast<MMediaLibrary*>(this->parent());
    Q_ASSERT_X(parentLibrary, "MFolder::childPlaylists",
               "folder instance has no parent library");

    MPlaylistList children;
    for(MPlaylist *p : parentLibrary->playlists()){
        if(p->parentFolder() == this){
            children.append(p);
        }
    }

    if(recursive){
        for(MFolder *f : childFolders(recursive)){
            for(MPlaylist *p : f->childPlaylists()){
                children.append(p);
            }
        }
    }

    return children;
}
MContainerList MFolder::childContainers(bool recursive) const
{
    Q_ASSERT_X(static_cast<MMediaLibrary*>(this->parent()),
               "MFolder::childContainers",
               "folder instance has no parent library");

    MContainerList children;
    for(MFolder *f : childFolders(recursive)) {
        children.append(f);
    }

    for(MPlaylist *p : childPlaylists(recursive)) {
        children.append(p);
    }

    return children;
}


MMediaLibrary::MMediaLibrary(QObject *parent) : MMediaElement(parent)
{
    m_pid = generatePID(Mpi3::LibraryElement);
    m_name = "New Library";
    m_added = "14/03/1994"; // change to current date
}
Mpi3::ElementType MMediaLibrary::type() const
{
    return Mpi3::LibraryElement;
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
MElementList MMediaLibrary::elements(Mpi3::ElementType filterType) const
{
    bool getAllElements = filterType == Mpi3::BaseElement;
    bool getContainers = filterType == Mpi3::ContainerElement;

    MElementList children;
    if(filterType == Mpi3::FolderElement || getAllElements || getContainers) {
        for(MFolder *f : m_folders) {
            children.append(f);
        }
    }
    if(filterType == Mpi3::PlaylistElement || getAllElements || getContainers) {
        for(MPlaylist *p : m_playlists) {
            children.append(p);
        }
    }
    if(filterType == Mpi3::SongElement || getAllElements) {
        for(MSong *s : m_songs) {
            children.append(s);
        }
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
MMediaElement *MMediaLibrary::getElement(const QString &pid) const
{
    return MMediaLibrary::pidSearch<MElementList, MMediaElement>(elements(), pid);
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

void MMediaLibrary::setMediaPath(const QString &dirPath)
{
    QString oldPath = m_mediaPath;
    QString newPath = QDir::toNativeSeparators(dirPath);

    m_mediaPath = newPath;
    notifyPropertyChanged("mediaPath", oldPath);
}
void MMediaLibrary::setBackupPath(const QString &dirPath)
{
    QString oldPath = m_backupPath;
    QString newPath = QDir::toNativeSeparators(dirPath);

    m_backupPath = newPath;
    notifyPropertyChanged("backupPath", oldPath);
}
void MMediaLibrary::setDownloadPath(const QString &dirPath)
{
    QString oldPath = m_backupPath;
    QString newPath = QDir::toNativeSeparators(dirPath);

    m_downloadPath = newPath;
    notifyPropertyChanged("downloadPath", oldPath);
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
    notifyPropertyChanged("savePath", oldPath);

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
            p->m_songs.append(getSong(childSongs.at(c).toElement().text()));
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
        notifyPropertyChanged("savePath", oldPath);
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
        for(MSong *s : p->m_songs) {
            xmlWriteElement(xml, playlistSongs, "pid", s->m_pid);
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

bool MMediaLibrary::importItunesPlist(const QString &filePath, MFolder *parentFolder)
{
    qInfo() << "loading itunes playlists from file" << filePath;

    QFile loadFile(filePath);
    if(!loadFile.open(QIODevice::ReadOnly)) {
        qWarning() << "error opening file for read" << filePath;
        return false;
    }

    QDomDocument xml;
    if(!xml.setContent(loadFile.readAll())){
        qWarning() << "xml parsing error in itunes playlist file" << filePath;
        return false;
    }

    QDomNode root = xml.documentElement().childNodes().at(0);
    QMap<QString, QVariant> rootDict = plistDict(root);

    QMap<QString, MSong*> songs;
    QMap<QString, QVariant> plistTracks = rootDict["Tracks"].toMap();
    for(int i = 0; i < plistTracks.keys().size(); i++) {
        QString key = plistTracks.keys().at(i);
        QMap<QString, QVariant> track = plistTracks[key].toMap();

        MSong *song = new MSong(this);
        song->m_name = track["Name"].toString();
        song->m_added = track["Date Added"].toString();
        song->m_artist = track["Artist"].toString();
        song->m_album = track["Album"].toString();
        song->m_path = track["Location"].toString();
        song->m_kind = track["Kind"].toString();

        song->m_time = track["Total Time"].toInt();
        song->m_size = track["Size"].toInt();
        song->m_bitRate = track["Bit Rate"].toInt();
        song->m_sampleRate = track["Sample Rate"].toInt();

        song->m_path = QDir::toNativeSeparators(song->m_path);
        song->m_path = QUrl(song->m_path).toLocalFile();
        QString localHost = "\\\\localhost\\";
        if(song->m_path.startsWith(localHost)) {
            song->m_path = song->m_path.right(song->m_path.size() - localHost.size());
        }

        songs[key] = song;

        m_songs.append(song);
        emit songCreated(song);
    }

    QList<QVariant> plistPlaylists = rootDict["Playlists"].toList();
    for(int i = 0; i < plistPlaylists.size(); i++) {
        QMap<QString, QVariant> itunesPlaylist = plistPlaylists.at(i).toMap();

        MPlaylist *playlist = new MPlaylist(this);
        playlist->m_name = itunesPlaylist["Name"].toString();
        playlist->m_added = "";
        playlist->m_parentFolder = parentFolder;

        QList<QVariant> playlistTrackIDs = itunesPlaylist["Playlist Items"].toList();
        for(int j = 0; j < playlistTrackIDs.size(); j++) {
            QMap<QString, QVariant> trackID = playlistTrackIDs.at(j).toMap();
            playlist->m_songs.append(songs[trackID["Track ID"].toString()]);
        }

        m_playlists.append(playlist);
        emit playlistCreated(playlist);
    }

    qInfo()
        << "loaded itunes playlist file" << filePath
        << "with" << plistPlaylists.size() << "playlists"
        << "and" << plistTracks.size() << "songs";

    return true;
}

bool MMediaLibrary::validMediaFiles(QUrl mediaUrl)
{
    if(mediaUrl.toString().endsWith(".mp3")
        || mediaUrl.toString().endsWith(".wav")) {
        return true;
    }

    return false;
}
bool MMediaLibrary::validMediaFiles(QList<QUrl> mediaUrls)
{
    if(mediaUrls.size() == 0) {
        return false;
    }

    for(int i = 0; i < mediaUrls.size(); i++) {
        if(!validMediaFiles(mediaUrls.at(i))) {
            return false;
        }
    }

    return true;
}

MSongList MMediaLibrary::songsFromBytes(QByteArray pidBytes) const
{
    QStringList pidStrings;

    int i = 0;
    int length = MPI3_PID_STRING_LENGTH;
    while(i + length <= pidBytes.size()) {
        pidStrings.append(pidBytes.mid(i, length));
        i += length;
    }

    MSongList extractedSongs;
    for(QString p : pidStrings) {
        MSong *s = getSong(p);
        if(s && !extractedSongs.contains(s)) {
            extractedSongs.append(s);
        }
    }

    return extractedSongs;
}
QByteArray MMediaLibrary::songsToBytes(MSongList songlist)
{
    QByteArray pidBytes;
    for(MSong *s : songlist) {
        pidBytes.append(s->pid());
    }

    return pidBytes;
}
QList<QUrl> MMediaLibrary::songsToPaths(MSongList songlist)
{
    QList<QUrl> songUrls;
    for(MSong *s : songlist) {
        songUrls.append(s->path());
    }

    return songUrls;
}

QString MMediaLibrary::timeToString(double time)
{
    int sctime = static_cast<int>(time);
    int seconds = sctime % 60;
    int minutes = (sctime - seconds) / 60;

    QString secs = QString::number(seconds);
    QString mins = QString::number(minutes);

    if(secs.size() == 1) {
        secs.prepend("0");
    }

    return QString(mins + ":" + secs);
}
QString MMediaLibrary::sizeToString(double size, int prec)
{
    const QList<QString> fileSizeSuffixes = {"KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};

    QString unit("B");
    QStringListIterator i(fileSizeSuffixes);
    while(size >= 1024.0 && i.hasNext()) {
        unit = i.next();
        size /= 1024.0;
    }

    return QString().setNum(size,'f', prec) + " " + unit;
}
QString MMediaLibrary::percentToString(double percent, int prec)
{
    return QString().setNum(percent * 100.0,'f', prec) + " %";
}

MMediaLibrary *MMediaLibrary::createRaspiVolume(const QString &rootPath)
{
    qInfo() << "creating raspberrypi library file" << rootPath;

    MMediaLibrary *raspiLib = new MMediaLibrary();
    raspiLib->m_pid = generatePID(Mpi3::LibraryElement);
    raspiLib->m_name = "New Raspi Library";
    raspiLib->m_added = "14/03/1994";

    QString rootDir = rootPath + ".mpi3";
    QString libPath = rootDir + "/lib.mpi3lib";

    QDir().mkdir(rootDir);
    raspiLib->save(libPath);

    return raspiLib;
}
MMediaLibrary *MMediaLibrary::loadRaspiVolume(const QString &rootPath)
{
    qInfo() << "loading raspberrypi library file" << rootPath;

    MMediaLibrary *raspiLib = nullptr;

    QString rootDir = rootPath + ".mpi3";
    QString libPath = rootDir + "/lib.mpi3lib";

    if(QDir().exists(libPath)) {
        raspiLib = new MMediaLibrary();
        raspiLib->load(libPath);
    }

    return raspiLib;
}
bool MMediaLibrary::detectRaspiVolume(const QString &rootPath)
{
    QString rootDir = rootPath + ".mpi3";
    QString libPath = rootDir + "/lib.mpi3lib";
    return QDir().exists(libPath);
}

QString MMediaLibrary::generatePID(Mpi3::ElementType elementType) const
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

    for(MMediaElement *e : elements(elementType)) {
        if(e->m_pid == pid) {
            return generatePID(elementType);
        }
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
    song->m_path = QDir::toNativeSeparators(QUrl(filePath).toLocalFile());;
    while(song->m_path.startsWith("\\")) {
        song->m_path.remove(0, 1);
    }

    MSongInfo info;
    info.load(filePath);
    if(info.loaded) {
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

    if(song->m_name.isEmpty() || song->m_name.isNull()) {
        song->m_name = QUrl(filePath).fileName();
        QStringList splitExt = song->m_name.split(".");
        if(splitExt.size() > 0) {
            song->m_name = splitExt.at(0);
        }
    }

    m_songs.append(song);
    emit songCreated(song);

    qDebug()
        << "created new song"
        << song->m_pid << "-"
        << song->m_name;

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

    qDebug()
        << "created new folder"
        << folder->m_pid << "-"
        << folder->m_name;

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

    qDebug()
        << "created new playlist"
        << playlist->m_pid << "-"
        << playlist->m_name;

    return playlist;
}

void MMediaLibrary::remove(MSong *childSong)
{
    // TODO: remove song from playlists
    m_songs.remove(m_songs.indexOf(childSong));
    emit songDeleted(childSong);

    qDebug()
        << "deleted song"
        << childSong->m_pid << "-"
        << childSong->m_name;
}
void MMediaLibrary::remove(MFolder *childFolder)
{
    m_folders.remove(m_folders.indexOf(childFolder));
    emit folderDeleted(childFolder);

    qDebug()
        << "deleted folder"
        << childFolder->m_pid << "-"
        << childFolder->m_name;

    for(MContainer *c : childFolder->childContainers()){
        remove(c);
    }
}
void MMediaLibrary::remove(MPlaylist *childPlaylist)
{
    m_playlists.remove(m_playlists.indexOf(childPlaylist));
    emit playlistDeleted(childPlaylist);

    qDebug()
        << "deleted playlist"
        << childPlaylist->m_pid << "-"
        << childPlaylist->m_name;
}
void MMediaLibrary::remove(MContainer *childContainer)
{
    remove(childContainer->pid());
}
void MMediaLibrary::remove(const QString &pid)
{
    for(MFolder *f : m_folders){
        if(f->pid() == pid){
            remove(f);
            return;
        }
    }
    for(MPlaylist *p : m_playlists){
        if(p->pid() == pid){
            remove(p);
            return;
        }
    }
    for(MSong *s : m_songs){
        if(s->pid() == pid){
            remove(s);
            return;
        }
    }
}
