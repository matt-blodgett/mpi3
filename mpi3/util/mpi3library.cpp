#include "mpi3library.h"

#include <QRandomGenerator>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QDir>

#include <QDebug>


Mpi3Element::Mpi3Element() : QObject(nullptr){}
Mpi3Element::ElementType Mpi3Element::type() const {
    return Mpi3Element::BaseElement;
}

int const Mpi3Element::PersistentIDLength = 18;
QString const Mpi3Element::BasePrefix = "E:";
QString const Mpi3Element::SongPrefix = "S:";
QString const Mpi3Element::PlaylistPrefix = "P:";
QString const Mpi3Element::FolderPrefix = "F:";
QString const Mpi3Element::LibraryPrefix = "L:";

QString Mpi3Element::pid() const{
    return m_pid;
}
QString Mpi3Element::name() const{
    return m_name;
}
QString Mpi3Element::added() const{
    return m_added;
}


Mpi3Song::Mpi3Song() : Mpi3Element(){}
Mpi3Element::ElementType Mpi3Song::type() const {
    return Mpi3Element::SongElement;
}

QString Mpi3Song::artist() const{
    return m_artist;
}
QString Mpi3Song::album() const{
    return m_album;
}
QString Mpi3Song::time() const{
    return m_time;
}
QString Mpi3Song::path() const{
    return m_path;
}
QString Mpi3Song::kind() const{
    return m_kind;
}

int Mpi3Song::size() const{
    return m_size;
}
int Mpi3Song::bitRate() const{
    return m_bitRate;
}
int Mpi3Song::sampleRate() const{
    return m_sampleRate;
}


Mpi3Playlist::Mpi3Playlist() : Mpi3Element(){}
Mpi3Element::ElementType Mpi3Playlist::type() const {
    return Mpi3Element::PlaylistElement;
}

Mpi3Song* Mpi3Playlist::getSong(const QString &pid) const{
    for(int i = 0; i < songs.size(); i++){
        Mpi3Song *song = songs.at(i);
        if(song->pid() == pid){
            return song;
        }
    }

    return nullptr;
}


Mpi3Folder::Mpi3Folder() : Mpi3Element(){}
Mpi3Element::ElementType Mpi3Folder::type() const {
    return Mpi3Element::FolderElement;
}

Mpi3Playlist *Mpi3Folder::getPlaylist(const QString &pid) const{
    for(int i = 0; i < playlists.size(); i++){
        if(playlists.at(i)->pid() == pid){
            return playlists.at(i);
        }
    }

    return nullptr;
}
Mpi3Folder *Mpi3Folder::getFolder(const QString &pid) const{
    for(int i = 0; i < folders.size(); i++){
        if(folders.at(i)->pid() == pid){
            return folders.at(i);
        }
    }

    return nullptr;
}

Mpi3Library::Mpi3Library() : Mpi3Element(){
    libSongs = new QVector<Mpi3Song*>;
    libPlaylists = new QVector<Mpi3Playlist*>;
    libFolders = new QVector<Mpi3Folder*>;
}
Mpi3Library::~Mpi3Library(){
    delete libSongs;
    delete libPlaylists;
    delete libFolders;
}
Mpi3Element::ElementType Mpi3Library::type() const {
    return Mpi3Element::LibraryElement;
}

void Mpi3Library::load(const QString &path){
    QFile loadFile(path);
    if (loadFile.open(QIODevice::ReadOnly)){
        libFolders->clear();
        libPlaylists->clear();
        libSongs->clear();

        QDomDocument xml;
        xml.setContent(loadFile.readAll());

        QDomElement root = xml.documentElement();
        m_pid = root.namedItem("pid").toElement().text();
        m_name = root.namedItem("name").toElement().text();
        m_added = root.namedItem("added").toElement().text();
        m_filepath = path;

        QDomNodeList xmlSongs = root.namedItem("songs").toElement().childNodes();
        QDomNodeList xmlPlaylists = root.namedItem("playlists").toElement().childNodes();
        QDomNodeList xmlFolders = root.namedItem("folders").toElement().childNodes();

        for(int i = 0; i < xmlSongs.length(); i++){
            Mpi3Song *song = new Mpi3Song();
            song->m_pid = xmlSongs.at(i).namedItem("pid").toElement().text();
            song->m_name = xmlSongs.at(i).namedItem("name").toElement().text();
            song->m_added = xmlSongs.at(i).namedItem("added").toElement().text();

            song->m_artist = xmlSongs.at(i).namedItem("artist").toElement().text();
            song->m_album = xmlSongs.at(i).namedItem("album").toElement().text();
            song->m_time = xmlSongs.at(i).namedItem("time").toElement().text();
            song->m_path = xmlSongs.at(i).namedItem("path").toElement().text();
            song->m_kind = xmlSongs.at(i).namedItem("kind").toElement().text();

            song->m_size = xmlSongs.at(i).namedItem("size").toElement().text().toInt();
            song->m_bitRate = xmlSongs.at(i).namedItem("bitRate").toElement().text().toInt();
            song->m_sampleRate = xmlSongs.at(i).namedItem("sampleRate").toElement().text().toInt();

            libSongs->append(song);
        }

        QMap<QString, QStringList> childElementMap;

        for(int i = 0; i < xmlPlaylists.length(); i++){
            Mpi3Playlist *playlist = new Mpi3Playlist();
            playlist->m_pid = xmlPlaylists.at(i).namedItem("pid").toElement().text();
            playlist->m_name = xmlPlaylists.at(i).namedItem("name").toElement().text();
            playlist->m_added = xmlPlaylists.at(i).namedItem("added").toElement().text();

            QStringList childElements;
            QDomNodeList childSongs = xmlPlaylists.at(i).namedItem("childSongs").childNodes();
            for(int c_index = 0; c_index < childSongs.size(); c_index++){
                childElements.append(childSongs.at(c_index).toElement().text());
            }

            if(childElements.size() > 0){
                childElementMap[playlist->m_pid] = childElements;
            }

            libPlaylists->append(playlist);
        }

        for(int i = 0; i < xmlFolders.length(); i++){
            Mpi3Folder *folder = new Mpi3Folder();
            folder->m_pid = xmlFolders.at(i).namedItem("pid").toElement().text();
            folder->m_name = xmlFolders.at(i).namedItem("name").toElement().text();
            folder->m_added = xmlFolders.at(i).namedItem("added").toElement().text();

            QStringList childElements;
            QDomNodeList childFolders = xmlFolders.at(i).namedItem("childFolders").childNodes();
            for(int c_index = 0; c_index < childFolders.size(); c_index++){
                childElements.append(childFolders.at(c_index).toElement().text());
            }

            QDomNodeList childPlaylists = xmlFolders.at(i).namedItem("childPlaylists").childNodes();
            for(int c_index = 0; c_index < childPlaylists.size(); c_index++){
                childElements.append(childPlaylists.at(c_index).toElement().text());
            }

            if(childElements.size() > 0){
                childElementMap[folder->m_pid] = childElements;
            }

            libFolders->append(folder);
        }

        for(int i = 0; i < childElementMap.keys().size(); i++){
            QString pidParent = childElementMap.keys().at(i);
            QStringList pidsChildren = childElementMap[pidParent];

            Mpi3Folder *parentFolder = getFolder(pidParent);

            if(!parentFolder){
                Mpi3Playlist *parentPlaylist = getPlaylist(pidParent);
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    parentPlaylist->songs.append(getSong(pidsChildren.at(c_index)));
                }
            }
            else {
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    Mpi3Folder *childFolder = getFolder(pidsChildren.at(c_index));
                    if(childFolder){
                        parentFolder->folders.append(childFolder);
                        childFolder->parent = parentFolder;
                    }
                }
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    Mpi3Playlist *childPlaylist = getPlaylist(pidsChildren.at(c_index));
                    if(childPlaylist){
                        parentFolder->playlists.append(childPlaylist);
                        childPlaylist->parent = parentFolder;
                    }
                }
            }
        }
    }
    else {
        m_pid = generatePID(Mpi3Element::LibraryElement);
        m_name = "New Library";
        m_added = "03/07/2018";
        m_filepath = path;
        save();
    }
}
void Mpi3Library::save(const QString &path){
    if(!path.isNull()){
        m_filepath = path;
    }

    QFile saveFile(m_filepath);
    if(saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QDomDocument xml("Mpi3Library");
        QDomElement root = xml.createElement("library");
        xml.appendChild(root);

        xmlWriteElement(xml, root, "pid", pid());
        xmlWriteElement(xml, root, "name", name());
        xmlWriteElement(xml, root, "added", added());

        QDomElement xmlSongs = xml.createElement("songs");
        QDomElement xmlPlaylists = xml.createElement("playlists");
        QDomElement xmlFolders = xml.createElement("folders");

        root.appendChild(xmlSongs);
        root.appendChild(xmlPlaylists);
        root.appendChild(xmlFolders);

        for(int i = 0; i < libSongs->size(); i++){
            Mpi3Song *song = libSongs->at(i);
            QDomElement songElement = xml.createElement("song");

            xmlWriteElement(xml, songElement, "pid", song->pid());
            xmlWriteElement(xml, songElement, "name", song->name());
            xmlWriteElement(xml, songElement, "added", song->added());

            xmlWriteElement(xml, songElement, "artist", song->m_artist);
            xmlWriteElement(xml, songElement, "album", song->m_album);
            xmlWriteElement(xml, songElement, "time", song->m_time);
            xmlWriteElement(xml, songElement, "path", song->m_path);
            xmlWriteElement(xml, songElement, "kind", song->m_kind);

            xmlWriteElement(xml, songElement, "size", QString::number(song->m_size));
            xmlWriteElement(xml, songElement, "bitRate", QString::number(song->m_bitRate));
            xmlWriteElement(xml, songElement, "sampleRate", QString::number(song->m_sampleRate));

            xmlSongs.appendChild(songElement);
        }

        for(int i = 0; i < libPlaylists->size(); i++){
            Mpi3Playlist *playlist = libPlaylists->at(i);
            QDomElement playlistElement = xml.createElement("playlist");

            xmlWriteElement(xml, playlistElement, "pid", playlist->pid());
            xmlWriteElement(xml, playlistElement, "name", playlist->name());
            xmlWriteElement(xml, playlistElement, "added", playlist->added());

            QDomElement playlistSongs = xml.createElement("childSongs");
            for(int c_index = 0; c_index < playlist->songs.size(); c_index++){
                Mpi3Song *childSong = playlist->songs.at(c_index);
                xmlWriteElement(xml, playlistSongs, "pid", childSong->pid());
            }

            playlistElement.appendChild(playlistSongs);
            xmlPlaylists.appendChild(playlistElement);
        }

        for(int i = 0; i < libFolders->size(); i++){
            Mpi3Folder *folder = libFolders->at(i);
            QDomElement folderElement = xml.createElement("folder");

            xmlWriteElement(xml, folderElement, "pid", folder->pid());
            xmlWriteElement(xml, folderElement, "name", folder->name());
            xmlWriteElement(xml, folderElement, "added", folder->added());

            QDomElement folderFolders = xml.createElement("childFolders");
            for(int c_index = 0; c_index < folder->folders.size(); c_index++){
                Mpi3Folder *childFolder = folder->folders.at(c_index);
                xmlWriteElement(xml, folderFolders, "pid", childFolder->pid());
            }

            QDomElement folderPlaylists = xml.createElement("childPlaylists");
            for(int c_index = 0; c_index < folder->playlists.size(); c_index++){
                Mpi3Playlist *childPlaylist = folder->playlists.at(c_index);
                xmlWriteElement(xml, folderPlaylists, "pid", childPlaylist->pid());
            }

            folderElement.appendChild(folderFolders);
            folderElement.appendChild(folderPlaylists);
            xmlFolders.appendChild(folderElement);
        }

        QTextStream xmlStream(&saveFile);
        xmlStream << xml.toString();
    }
}

QString Mpi3Library::filepath() const {
    return m_filepath;
}

QString Mpi3Library::generatePID(Mpi3Element::ElementType elemType) const{
    QString pid;
    switch(elemType){
        case Mpi3Element::BaseElement: {
            pid = Mpi3Element::BasePrefix;
            break;
        }
        case Mpi3Element::SongElement: {
            pid = Mpi3Element::SongPrefix;
            break;
        }
        case Mpi3Element::PlaylistElement: {
            pid = Mpi3Element::PlaylistPrefix;
            break;
        }
        case Mpi3Element::FolderElement: {
            pid = Mpi3Element::FolderPrefix;
            break;
        }
        case Mpi3Element::LibraryElement: {
            pid = Mpi3Element::LibraryPrefix;
            break;
        }
    }

    QString clist = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    while(pid.size() < Mpi3Element::PersistentIDLength){
        pid += clist[QRandomGenerator().global()->bounded(0, 36)];
    }

    if(elemType == Mpi3Element::SongElement){
        for(int i = 0; i < libSongs->size(); i++){
            if(libSongs->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }
    else if(elemType == Mpi3Element::PlaylistElement){
        for(int i = 0; i < libPlaylists->size(); i++){
            if(libPlaylists->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }
    else if(elemType == Mpi3Element::FolderElement){
        for(int i = 0; i < libFolders->size(); i++){
            if(libFolders->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }

    return pid;
}
void Mpi3Library::xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text) const{
    QDomElement e = xml.createElement(tagname);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
}
QMap<QString, QVariant> Mpi3Library::plistDict(const QDomNode &parentNode) const{

    QMap<QString, QVariant> dict;

    for(int i = 0; i < parentNode.childNodes().size(); i++){
        QDomNode childNode = parentNode.childNodes().at(i);

        if(childNode.toElement().tagName() == "key"){

            QDomElement keyNode = childNode.toElement();
            QDomElement valueNode = parentNode.childNodes().at(i + 1).toElement();

            if(valueNode.tagName() == "dict"){
                dict[keyNode.text()] = plistDict(valueNode);
            }
            else if(valueNode.tagName() == "array"){
                QList<QVariant> arrayItems;
                for(int cnode = 0; cnode < valueNode.childNodes().size(); cnode++){
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

void Mpi3Library::importItunesPlist(const QString &path, Mpi3Folder *parentFolder){
    QFile loadFile(path);
    if(loadFile.open(QIODevice::ReadOnly)){

        QDomDocument xml;
        xml.setContent(loadFile.readAll());

        QDomNode root = xml.documentElement().childNodes().at(0);
        QMap<QString, QVariant> rootDict = plistDict(root);

        QMap<QString, Mpi3Song*> songs;
        QMap<QString, QVariant> plistTracks = rootDict["Tracks"].toMap();
        for(int i = 0; i < plistTracks.keys().size(); i++){
            QString key = plistTracks.keys().at(i);
            QMap<QString, QVariant> track = plistTracks[key].toMap();

            Mpi3Song *song = newSong();
            song->m_name = track["Name"].toString();
            song->m_added = track["Date Added"].toString();
            song->m_artist = track["Artist"].toString();
            song->m_album = track["Album"].toString();
            song->m_time = track["Total Time"].toString();
            song->m_path = track["Location"].toString();
            song->m_kind = track["Kind"].toString();
            song->m_size = track["Size"].toInt();
            song->m_bitRate = track["Bit Rate"].toInt();
            song->m_sampleRate = track["Sample Rate"].toInt();

            song->m_path = QDir::toNativeSeparators(song->m_path);
            song->m_path = QUrl(song->m_path).toLocalFile();
            QString localHost = "\\\\localhost\\";
            if(song->m_path.startsWith(localHost)){
                song->m_path = song->m_path.right(song->m_path.size() - localHost.size());
            }

            insert(song);
            songs[key] = song;
        }

        QList<QVariant> plistPlaylists = rootDict["Playlists"].toList();
        for(int i = 0; i < plistPlaylists.size(); i++){
            QMap<QString, QVariant> itunesPlaylist = plistPlaylists.at(i).toMap();

            Mpi3Playlist *playlist = newPlaylist();
            playlist->m_name = itunesPlaylist["Name"].toString();
            playlist->m_added = "";
            insert(playlist, parentFolder);

            QList<QVariant> playlistTrackIDs = itunesPlaylist["Playlist Items"].toList();
            for(int j = 0; j < playlistTrackIDs.size(); j++){
                QMap<QString, QVariant> trackID = playlistTrackIDs.at(j).toMap();
                insert(songs[trackID["Track ID"].toString()], playlist, j);
            }
        }
    }
}

bool Mpi3Library::validMediaFiles(QUrl mediaUrl){
    if(mediaUrl.toString().endsWith(".mp3") || mediaUrl.toString().endsWith(".wav")){
        return true;
    }

    return false;
}
bool Mpi3Library::validMediaFiles(QList<QUrl> mediaUrls){
    if(mediaUrls.size() == 0){
        return false;
    }

    for(int i = 0; i < mediaUrls.size(); i++){
        if(!validMediaFiles(mediaUrls.at(i))){
            return false;
        }
    }

    return true;
}

QVector<Mpi3Song*> Mpi3Library::songsFromData(QByteArray pidBytes) const {
    QStringList pidStrings;

    int i = 0;
    int length = Mpi3Element::PersistentIDLength;
    while(i + length <= pidBytes.size()){
        pidStrings.append(pidBytes.mid(i, length));
        i += length;
    }

    QVector<Mpi3Song*> extractedSongs;
    for(int i = 0; i < pidStrings.size(); i++){
        Mpi3Song *song = getSong(pidStrings.at(i));
        if(song && !extractedSongs.contains(song)){
            extractedSongs.append(song);
        }
    }

    return extractedSongs;
}

QVector<Mpi3Folder*> Mpi3Library::childFolders(Mpi3Folder *parentFolder) const{
    QVector<Mpi3Folder*> folders;
    for(int i = 0; i < libFolders->size(); i++){
        Mpi3Folder *f = libFolders->at(i);
        if(f->parent == parentFolder){
            folders.append(f);
        }
    }

    return folders;
}
QVector<Mpi3Playlist*> Mpi3Library::childPlaylists(Mpi3Folder *parentFolder) const{
    QVector<Mpi3Playlist*> playlists;
    for(int i = 0; i < libPlaylists->size(); i++){
        Mpi3Playlist *p = libPlaylists->at(i);
        if(p->parent == parentFolder){
            playlists.append(p);
        }
    }

    return playlists;
}

QVector<Mpi3Song*> Mpi3Library::allChildSongs(Mpi3Folder *parentFolder) const{
    if(!parentFolder){
        return *libSongs;
    }

    QVector<Mpi3Song*> songs;
    QVector<Mpi3Playlist*> nestedPlaylists = allChildPlaylists(parentFolder);
    for(int i = 0; i < nestedPlaylists.size(); i++){
        Mpi3Playlist *playlist = nestedPlaylists.at(i);
        for(int j = 0; j < playlist->songs.size(); j++){
            Mpi3Song *addSong = playlist->songs.at(j);
            if(!songs.contains(addSong)){
                songs.append(addSong);
            }
        }
    }

    return songs;
}
QVector<Mpi3Playlist*> Mpi3Library::allChildPlaylists(Mpi3Folder *parentFolder) const{
    if(!parentFolder){
        return *libPlaylists;
    }

    QVector<Mpi3Playlist*> playlists;
    for(int i = 0; i < parentFolder->playlists.size(); i++){
        playlists.append(parentFolder->playlists.at(i));
    }

    for(int i = 0; i < parentFolder->folders.size(); i++){
        Mpi3Folder *childFolder = parentFolder->folders.at(i);
        QVector<Mpi3Playlist*> nestedPlaylists = allChildPlaylists(childFolder);
        for(int j = 0; j < nestedPlaylists.size(); j++){
            playlists.append(nestedPlaylists.at(j));
        }
    }

    return playlists;
}
QVector<Mpi3Folder*> Mpi3Library::allChildFolders(Mpi3Folder *parentFolder) const{
    if(!parentFolder){
        return *libFolders;
    }

    QVector<Mpi3Folder*> folders;
    for(int i = 0; i < parentFolder->folders.size(); i++){
        Mpi3Folder *childFolder = parentFolder->folders.at(i);
        folders.append(childFolder);

        QVector<Mpi3Folder*> nestedFolders = allChildFolders(childFolder);
        for(int j = 0; j < nestedFolders.size(); j++){
            folders.append(nestedFolders.at(j));
        }
    }

    return folders;
}

Mpi3Element* Mpi3Library::getElement(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }
    else if(pid.startsWith(Mpi3Element::SongPrefix)){
        for(int i = 0; i < libSongs->size(); i++){
            if(libSongs->at(i)->pid() == pid){
                return libSongs->at(i);
            }
        }
    }
    else if(pid.startsWith(Mpi3Element::PlaylistPrefix)){
        for(int i = 0; i < libPlaylists->size(); i++){
            if(libPlaylists->at(i)->pid() == pid){
                return libPlaylists->at(i);
            }
        }
    }
    else if(pid.startsWith(Mpi3Element::FolderPrefix)){
        for(int i = 0; i < libFolders->size(); i++){
            if(libFolders->at(i)->pid() == pid){
                return libFolders->at(i);
            }
        }
    }

    return nullptr;
}
Mpi3Song* Mpi3Library::getSong(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libSongs->size(); i++){
        Mpi3Song *s = libSongs->at(i);
        if(s->pid() == pid){
            return s;
        }
    }

    return nullptr;
}
Mpi3Playlist* Mpi3Library::getPlaylist(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libPlaylists->size(); i++){
        Mpi3Playlist *p = libPlaylists->at(i);
        if(p->pid() == pid){
            return p;
        }
    }

    return nullptr;
}
Mpi3Folder* Mpi3Library::getFolder(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libFolders->size(); i++){
        Mpi3Folder *f = libFolders->at(i);
        if(f->pid() == pid){
            return f;
        }
    }

    return nullptr;
}

Mpi3Song* Mpi3Library::newSong(const QString &path) const{
    Mpi3Song *song = new Mpi3Song();
    song->m_pid = generatePID(Mpi3Element::SongElement);

    if(!path.isNull()){
        song->m_name = QUrl(path).fileName();
        song->m_path = path;
    }

    return song;
}
Mpi3Playlist* Mpi3Library::newPlaylist(bool named) const{
    Mpi3Playlist *playlist = new Mpi3Playlist();
    playlist->m_pid = generatePID(Mpi3Element::PlaylistElement);

    if(named){
        QString name = "New Playlist";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < libPlaylists->size()){
            Mpi3Playlist *p = libPlaylists->at(i++);
            if(p->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }
        playlist->m_name = name + append;
    }

    return playlist;
}
Mpi3Folder* Mpi3Library::newFolder(bool named) const{
    Mpi3Folder *folder = new Mpi3Folder();
    folder->m_pid = generatePID(Mpi3Element::FolderElement);

    if(named){
        QString name = "New Folder";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < libFolders->size()){
            Mpi3Folder *f = libFolders->at(i++);
            if(f->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }
        folder->m_name = name + append;
    }

    return folder;
}

void Mpi3Library::modify(const QString &pid, const QString &value){
    if(pid == m_pid){
        m_name = value;
        emit elementModified(this);
    }
    else {
        Mpi3Folder *folder = getFolder(pid);
        if(folder){
            folder->m_name = value;
            emit elementModified(folder);
        }
        else {
            Mpi3Playlist *playlist = getPlaylist(pid);
            if(playlist){
                playlist->m_name = value;
                emit elementModified(playlist);
            }
        }
    }
}
void Mpi3Library::modify(Mpi3Song *song, const QString &value, Mpi3Song::MutableProperty songProperty){
    switch(songProperty){
        case Mpi3Song::SongName: {
            song->m_name = value;
            break;
        }
        case Mpi3Song::SongArtist: {
            song->m_artist = value;
            break;
        }
        case Mpi3Song::SongAlbum: {
            song->m_album = value;
            break;
        }
    }

    emit elementModified(song);
}

void Mpi3Library::insert(Mpi3Song *inSong, Mpi3Playlist *toPlaylist, int position){
    if(toPlaylist){
        int index = position == -1 ? toPlaylist->songs.size() : position;
        toPlaylist->songs.insert(index, inSong);
        emit elementInserted(inSong, toPlaylist);
    }
    if(!libSongs->contains(inSong)){
        libSongs->append(inSong);
        emit elementInserted(inSong, this);
    }
}
void Mpi3Library::insert(Mpi3Playlist *inPlaylist, Mpi3Folder *toFolder, int position){
    if(toFolder){
        int index = position == -1 ? toFolder->playlists.size() : position;
        toFolder->playlists.insert(index, inPlaylist);
        inPlaylist->parent = toFolder;
        emit elementInserted(inPlaylist, toFolder);
    }

    if(!libPlaylists->contains(inPlaylist)){
        libPlaylists->append(inPlaylist);
        emit elementInserted(inPlaylist, this);
    }
}
void Mpi3Library::insert(Mpi3Folder *inFolder, Mpi3Folder *toFolder, int position){
    if(toFolder){
        int index = position == -1 ? toFolder->folders.size() : position;
        toFolder->folders.insert(index, inFolder);
        inFolder->parent = toFolder;
        emit elementInserted(inFolder, toFolder);
    }

    if(!libFolders->contains(inFolder)){
        libFolders->append(inFolder);
        emit elementInserted(inFolder, this);
    }
}

void Mpi3Library::move(Mpi3Song* moveSong, Mpi3Playlist *inPlaylist, int position){
    Q_UNUSED(moveSong);
    Q_UNUSED(inPlaylist);
    Q_UNUSED(position);
}
void Mpi3Library::move(Mpi3Playlist *movePlaylist, Mpi3Folder *toFolder, int position){
    Q_UNUSED(movePlaylist);
    Q_UNUSED(toFolder);
    Q_UNUSED(position);
}
void Mpi3Library::move(Mpi3Folder *moveFolder, Mpi3Folder *toFolder, int position){
    Q_UNUSED(moveFolder);
    Q_UNUSED(toFolder);
    Q_UNUSED(position);
}

void Mpi3Library::remove(Mpi3Song *remSong, Mpi3Playlist *fromPlaylist){
    fromPlaylist->songs.removeAll(remSong);
    emit elementRemoved(remSong, fromPlaylist);
}
void Mpi3Library::remove(Mpi3Playlist *remPlaylist, Mpi3Folder *fromFolder){
    fromFolder->playlists.removeAll(remPlaylist);
    emit elementRemoved(remPlaylist, fromFolder);
}
void Mpi3Library::remove(Mpi3Folder *remFolder, Mpi3Folder *fromFolder){
    fromFolder->folders.removeAll(remFolder);
    emit elementRemoved(remFolder, fromFolder);
}

void Mpi3Library::discard(Mpi3Song *remSong){
    for(int i = 0; i < libPlaylists->size(); i++){
        Mpi3Playlist *playlist = libPlaylists->at(i);
        if(playlist->songs.contains(remSong)){
            playlist->songs.removeAll(remSong);
        }
    }

    libSongs->removeAll(remSong);
    emit elementDeleted(remSong->pid(), Mpi3Element::SongElement);
    delete remSong;
}
void Mpi3Library::discard(Mpi3Playlist *remPlaylist){
    if(remPlaylist->parent){
        remPlaylist->parent->playlists.removeAll(remPlaylist);
    }

    QVector<QString> pidChildren;
    for(int i = 0; i < remPlaylist->songs.size(); i++){
        pidChildren.append(remPlaylist->songs.at(i)->pid());
    }

    libPlaylists->removeAll(remPlaylist);
    emit elementDeleted(remPlaylist->pid(), Mpi3Element::PlaylistElement, pidChildren);
    delete remPlaylist;
}
void Mpi3Library::discard(Mpi3Folder *remFolder){
    if(remFolder->parent){
        remFolder->parent->folders.removeAll(remFolder);
    }

    QVector<QString> pidChildren;
    QVector<Mpi3Playlist*> nestedPlaylists = allChildPlaylists(remFolder);
    QVector<Mpi3Folder*> nestedFolders = allChildFolders(remFolder);

    for(int i = 0; i < nestedPlaylists.size(); i++){
        Mpi3Playlist *playlist = nestedPlaylists.at(i);
        pidChildren.append(playlist->pid());
        libPlaylists->removeAll(playlist);
        delete playlist;
    }

    for(int i = 0; i < nestedFolders.size(); i++){
        Mpi3Folder *folder = nestedFolders.at(i);
        pidChildren.append(folder->pid());
        libFolders->removeAll(folder);
        delete folder;
    }

    libFolders->removeAll(remFolder);
    emit elementDeleted(remFolder->pid(), Mpi3Element::FolderElement, pidChildren);
    delete remFolder;
}
