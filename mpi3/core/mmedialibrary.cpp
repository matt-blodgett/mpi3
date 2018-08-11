#include "mmedialibrary.h"

#include <QRandomGenerator>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QDir>

#include <QDebug>


#define PID_MAX_LENGTH 18

static const QString PrefixBase     = "E:";
static const QString PrefixSong     = "S:";
static const QString PrefixPlaylist = "P:";
static const QString PrefixFolder   = "F:";
static const QString PrefixLibrary  = "L:";


MMediaElement::MMediaElement() : QObject(nullptr){}
Mpi3::ElementType MMediaElement::type() const {
    return Mpi3::BaseElement;
}

QString MMediaElement::pid() const{
    return m_pid;
}
QString MMediaElement::name() const{
    return m_name;
}
QString MMediaElement::added() const{
    return m_added;
}


MSong::MSong() : MMediaElement(){}
Mpi3::ElementType MSong::type() const {
    return Mpi3::SongElement;
}

QString MSong::artist() const{
    return m_artist;
}
QString MSong::album() const{
    return m_album;
}
QString MSong::path() const{
    return m_path;
}
QString MSong::kind() const{
    return m_kind;
}

int MSong::time() const{
    return m_time;
}
int MSong::size() const{
    return m_size;
}
int MSong::bitRate() const{
    return m_bitRate;
}
int MSong::sampleRate() const{
    return m_sampleRate;
}


MPlaylist::MPlaylist() : MMediaElement(){}
Mpi3::ElementType MPlaylist::type() const {
    return Mpi3::PlaylistElement;
}

MSong* MPlaylist::getSong(const QString &pid) const{
    for(int i = 0; i < songs.size(); i++){
        MSong *song = songs.at(i);
        if(song->pid() == pid){
            return song;
        }
    }

    return nullptr;
}


MFolder::MFolder() : MMediaElement(){}
Mpi3::ElementType MFolder::type() const {
    return Mpi3::FolderElement;
}

MPlaylist *MFolder::getPlaylist(const QString &pid) const{
    for(int i = 0; i < playlists.size(); i++){
        if(playlists.at(i)->pid() == pid){
            return playlists.at(i);
        }
    }

    return nullptr;
}
MFolder *MFolder::getFolder(const QString &pid) const{
    for(int i = 0; i < folders.size(); i++){
        if(folders.at(i)->pid() == pid){
            return folders.at(i);
        }
    }

    return nullptr;
}

MMediaLibrary::MMediaLibrary() : MMediaElement(){
    libSongs = new QVector<MSong*>;
    libPlaylists = new QVector<MPlaylist*>;
    libFolders = new QVector<MFolder*>;
}
MMediaLibrary::~MMediaLibrary(){
    delete libSongs;
    delete libPlaylists;
    delete libFolders;
}
Mpi3::ElementType MMediaLibrary::type() const {
    return Mpi3::LibraryElement;
}

void MMediaLibrary::load(const QString &path){
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
            MSong *song = new MSong();
            song->m_pid = xmlSongs.at(i).namedItem("pid").toElement().text();
            song->m_name = xmlSongs.at(i).namedItem("name").toElement().text();
            song->m_added = xmlSongs.at(i).namedItem("added").toElement().text();

            song->m_artist = xmlSongs.at(i).namedItem("artist").toElement().text();
            song->m_album = xmlSongs.at(i).namedItem("album").toElement().text();
            song->m_path = xmlSongs.at(i).namedItem("path").toElement().text();
            song->m_kind = xmlSongs.at(i).namedItem("kind").toElement().text();

            song->m_time = xmlSongs.at(i).namedItem("time").toElement().text().toInt();
            song->m_size = xmlSongs.at(i).namedItem("size").toElement().text().toInt();
            song->m_bitRate = xmlSongs.at(i).namedItem("bitRate").toElement().text().toInt();
            song->m_sampleRate = xmlSongs.at(i).namedItem("sampleRate").toElement().text().toInt();

            libSongs->append(song);
        }

        QMap<QString, QStringList> childElementMap;

        for(int i = 0; i < xmlPlaylists.length(); i++){
            MPlaylist *playlist = new MPlaylist();
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
            MFolder *folder = new MFolder();
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

            MFolder *parentFolder = getFolder(pidParent);

            if(!parentFolder){
                MPlaylist *parentPlaylist = getPlaylist(pidParent);
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    parentPlaylist->songs.append(getSong(pidsChildren.at(c_index)));
                }
            }
            else {
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    MFolder *childFolder = getFolder(pidsChildren.at(c_index));
                    if(childFolder){
                        parentFolder->folders.append(childFolder);
                        childFolder->parent = parentFolder;
                    }
                }
                for(int c_index = 0; c_index < pidsChildren.size(); c_index++){
                    MPlaylist *childPlaylist = getPlaylist(pidsChildren.at(c_index));
                    if(childPlaylist){
                        parentFolder->playlists.append(childPlaylist);
                        childPlaylist->parent = parentFolder;
                    }
                }
            }
        }
    }
    else {
        m_pid = generatePID(Mpi3::LibraryElement);
        m_name = "New Library";
        m_added = "03/07/2018";
        m_filepath = path;
        save();
    }
}
void MMediaLibrary::save(const QString &path){
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
            MSong *song = libSongs->at(i);
            QDomElement songElement = xml.createElement("song");

            xmlWriteElement(xml, songElement, "pid", song->pid());
            xmlWriteElement(xml, songElement, "name", song->name());
            xmlWriteElement(xml, songElement, "added", song->added());

            xmlWriteElement(xml, songElement, "artist", song->m_artist);
            xmlWriteElement(xml, songElement, "album", song->m_album);
            xmlWriteElement(xml, songElement, "path", song->m_path);
            xmlWriteElement(xml, songElement, "kind", song->m_kind);

            xmlWriteElement(xml, songElement, "time", QString::number(song->m_time));
            xmlWriteElement(xml, songElement, "size", QString::number(song->m_size));
            xmlWriteElement(xml, songElement, "bitRate", QString::number(song->m_bitRate));
            xmlWriteElement(xml, songElement, "sampleRate", QString::number(song->m_sampleRate));

            xmlSongs.appendChild(songElement);
        }

        for(int i = 0; i < libPlaylists->size(); i++){
            MPlaylist *playlist = libPlaylists->at(i);
            QDomElement playlistElement = xml.createElement("playlist");

            xmlWriteElement(xml, playlistElement, "pid", playlist->pid());
            xmlWriteElement(xml, playlistElement, "name", playlist->name());
            xmlWriteElement(xml, playlistElement, "added", playlist->added());

            QDomElement playlistSongs = xml.createElement("childSongs");
            for(int c_index = 0; c_index < playlist->songs.size(); c_index++){
                MSong *childSong = playlist->songs.at(c_index);
                xmlWriteElement(xml, playlistSongs, "pid", childSong->pid());
            }

            playlistElement.appendChild(playlistSongs);
            xmlPlaylists.appendChild(playlistElement);
        }

        for(int i = 0; i < libFolders->size(); i++){
            MFolder *folder = libFolders->at(i);
            QDomElement folderElement = xml.createElement("folder");

            xmlWriteElement(xml, folderElement, "pid", folder->pid());
            xmlWriteElement(xml, folderElement, "name", folder->name());
            xmlWriteElement(xml, folderElement, "added", folder->added());

            QDomElement folderFolders = xml.createElement("childFolders");
            for(int c_index = 0; c_index < folder->folders.size(); c_index++){
                MFolder *childFolder = folder->folders.at(c_index);
                xmlWriteElement(xml, folderFolders, "pid", childFolder->pid());
            }

            QDomElement folderPlaylists = xml.createElement("childPlaylists");
            for(int c_index = 0; c_index < folder->playlists.size(); c_index++){
                MPlaylist *childPlaylist = folder->playlists.at(c_index);
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

QString MMediaLibrary::filepath() const {
    return m_filepath;
}

QString MMediaLibrary::generatePID(Mpi3::ElementType elemType) const{
    QString pid;
    switch(elemType){
        case Mpi3::BaseElement: {
            pid = PrefixBase;
            break;
        }
        case Mpi3::SongElement: {
            pid = PrefixSong;
            break;
        }
        case Mpi3::PlaylistElement: {
            pid = PrefixPlaylist;
            break;
        }
        case Mpi3::FolderElement: {
            pid = PrefixFolder;
            break;
        }
        case Mpi3::LibraryElement: {
            pid = PrefixLibrary;
            break;
        }
    }

    QString clist = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    while(pid.size() < PID_MAX_LENGTH){
        pid += clist[QRandomGenerator().global()->bounded(0, 36)];
    }

    if(elemType == Mpi3::SongElement){
        for(int i = 0; i < libSongs->size(); i++){
            if(libSongs->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }
    else if(elemType == Mpi3::PlaylistElement){
        for(int i = 0; i < libPlaylists->size(); i++){
            if(libPlaylists->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }
    else if(elemType == Mpi3::FolderElement){
        for(int i = 0; i < libFolders->size(); i++){
            if(libFolders->at(i)->pid() == pid){
                return generatePID(elemType);
            }
        }
    }

    return pid;
}
void MMediaLibrary::xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text) const{
    QDomElement e = xml.createElement(tagname);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
}
QMap<QString, QVariant> MMediaLibrary::plistDict(const QDomNode &parentNode) const{

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

void MMediaLibrary::importItunesPlist(const QString &path, MFolder *parentFolder){
    QFile loadFile(path);
    if(loadFile.open(QIODevice::ReadOnly)){

        QDomDocument xml;
        xml.setContent(loadFile.readAll());

        QDomNode root = xml.documentElement().childNodes().at(0);
        QMap<QString, QVariant> rootDict = plistDict(root);

        QMap<QString, MSong*> songs;
        QMap<QString, QVariant> plistTracks = rootDict["Tracks"].toMap();
        for(int i = 0; i < plistTracks.keys().size(); i++){
            QString key = plistTracks.keys().at(i);
            QMap<QString, QVariant> track = plistTracks[key].toMap();

            MSong *song = newSong();
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
            if(song->m_path.startsWith(localHost)){
                song->m_path = song->m_path.right(song->m_path.size() - localHost.size());
            }

            insert(song);
            songs[key] = song;
        }

        QList<QVariant> plistPlaylists = rootDict["Playlists"].toList();
        for(int i = 0; i < plistPlaylists.size(); i++){
            QMap<QString, QVariant> itunesPlaylist = plistPlaylists.at(i).toMap();

            MPlaylist *playlist = newPlaylist();
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

bool MMediaLibrary::validMediaFiles(QUrl mediaUrl) const{
    if(mediaUrl.toString().endsWith(".mp3") || mediaUrl.toString().endsWith(".wav")){
        return true;
    }

    return false;
}
bool MMediaLibrary::validMediaFiles(QList<QUrl> mediaUrls) const{
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

QList<QUrl> MMediaLibrary::songsToPaths(QVector<MSong*> songObjects) const {
    QList<QUrl> songUrls;
    for(int i = 0; i < songObjects.size(); i++){
        songUrls.append(songObjects.at(i)->path());
    }

    return songUrls;
}
QVector<MSong*> MMediaLibrary::songsFromData(QByteArray pidBytes) const {
    QStringList pidStrings;

    int i = 0;
    int length = PID_MAX_LENGTH;
    while(i + length <= pidBytes.size()){
        pidStrings.append(pidBytes.mid(i, length));
        i += length;
    }

    QVector<MSong*> extractedSongs;
    for(int i = 0; i < pidStrings.size(); i++){
        MSong *song = getSong(pidStrings.at(i));
        if(song && !extractedSongs.contains(song)){
            extractedSongs.append(song);
        }
    }

    return extractedSongs;
}
QByteArray MMediaLibrary::songsToData(QVector<MSong*> songObjects) const{
    QByteArray pidBytes;
    for(int i = 0; i < songObjects.size(); i++){
        pidBytes.append(songObjects.at(i)->pid());
    }

    return pidBytes;
}

QVector<MFolder*> MMediaLibrary::childFolders() const{
    QVector<MFolder*> folders;
    for(int i = 0; i < libFolders->size(); i++){
        MFolder *f = libFolders->at(i);
        if(!f->parent){
            folders.append(f);
        }
    }

    return folders;
}
QVector<MPlaylist*> MMediaLibrary::childPlaylists() const{
    QVector<MPlaylist*> playlists;
    for(int i = 0; i < libPlaylists->size(); i++){
        MPlaylist *p = libPlaylists->at(i);
        if(!p->parent){
            playlists.append(p);
        }
    }

    return playlists;
}

QVector<MSong*> MMediaLibrary::allChildSongs(MFolder *parentFolder) const{
    if(!parentFolder){
        return *libSongs;
    }

    QVector<MSong*> songs;
    QVector<MPlaylist*> nestedPlaylists = allChildPlaylists(parentFolder);
    for(int i = 0; i < nestedPlaylists.size(); i++){
        MPlaylist *playlist = nestedPlaylists.at(i);
        for(int j = 0; j < playlist->songs.size(); j++){
            MSong *addSong = playlist->songs.at(j);
            if(!songs.contains(addSong)){
                songs.append(addSong);
            }
        }
    }

    return songs;
}
QVector<MPlaylist*> MMediaLibrary::allChildPlaylists(MFolder *parentFolder) const{
    if(!parentFolder){
        return *libPlaylists;
    }

    QVector<MPlaylist*> playlists;
    for(int i = 0; i < parentFolder->playlists.size(); i++){
        playlists.append(parentFolder->playlists.at(i));
    }

    for(int i = 0; i < parentFolder->folders.size(); i++){
        MFolder *childFolder = parentFolder->folders.at(i);
        QVector<MPlaylist*> nestedPlaylists = allChildPlaylists(childFolder);
        for(int j = 0; j < nestedPlaylists.size(); j++){
            playlists.append(nestedPlaylists.at(j));
        }
    }

    return playlists;
}
QVector<MFolder*> MMediaLibrary::allChildFolders(MFolder *parentFolder) const{
    if(!parentFolder){
        return *libFolders;
    }

    QVector<MFolder*> folders;
    for(int i = 0; i < parentFolder->folders.size(); i++){
        MFolder *childFolder = parentFolder->folders.at(i);
        folders.append(childFolder);

        QVector<MFolder*> nestedFolders = allChildFolders(childFolder);
        for(int j = 0; j < nestedFolders.size(); j++){
            folders.append(nestedFolders.at(j));
        }
    }

    return folders;
}

MMediaElement* MMediaLibrary::getElement(const QString &pid){
    if(pid.isNull()){
        return nullptr;
    }
    else if(pid.startsWith(PrefixSong)){
        for(int i = 0; i < libSongs->size(); i++){
            if(libSongs->at(i)->pid() == pid){
                return libSongs->at(i);
            }
        }
    }
    else if(pid.startsWith(PrefixPlaylist)){
        for(int i = 0; i < libPlaylists->size(); i++){
            if(libPlaylists->at(i)->pid() == pid){
                return libPlaylists->at(i);
            }
        }
    }
    else if(pid.startsWith(PrefixFolder)){
        for(int i = 0; i < libFolders->size(); i++){
            if(libFolders->at(i)->pid() == pid){
                return libFolders->at(i);
            }
        }
    }
    else if(pid.startsWith(PrefixLibrary)){
        return this;
    }

    return nullptr;
}
MSong* MMediaLibrary::getSong(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libSongs->size(); i++){
        MSong *s = libSongs->at(i);
        if(s->pid() == pid){
            return s;
        }
    }

    return nullptr;
}
MPlaylist* MMediaLibrary::getPlaylist(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libPlaylists->size(); i++){
        MPlaylist *p = libPlaylists->at(i);
        if(p->pid() == pid){
            return p;
        }
    }

    return nullptr;
}
MFolder* MMediaLibrary::getFolder(const QString &pid) const{
    if(pid.isNull()){
        return nullptr;
    }

    for(int i = 0; i < libFolders->size(); i++){
        MFolder *f = libFolders->at(i);
        if(f->pid() == pid){
            return f;
        }
    }

    return nullptr;
}

MSong* MMediaLibrary::newSong(const QString &path) const{
    MSong *song = new MSong();
    song->m_pid = generatePID(Mpi3::SongElement);

    if(!path.isNull()){
        song->m_name = QUrl(path).fileName();
        song->m_path = path;
    }

    return song;
}
MPlaylist* MMediaLibrary::newPlaylist(bool named) const{
    MPlaylist *playlist = new MPlaylist();
    playlist->m_pid = generatePID(Mpi3::PlaylistElement);

    if(named){
        QString name = "New Playlist";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < libPlaylists->size()){
            MPlaylist *p = libPlaylists->at(i++);
            if(p->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }
        playlist->m_name = name + append;
    }

    return playlist;
}
MFolder* MMediaLibrary::newFolder(bool named) const{
    MFolder *folder = new MFolder();
    folder->m_pid = generatePID(Mpi3::FolderElement);

    if(named){
        QString name = "New Folder";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < libFolders->size()){
            MFolder *f = libFolders->at(i++);
            if(f->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }
        folder->m_name = name + append;
    }

    return folder;
}

void MMediaLibrary::modify(const QString &pid, const QString &value){
    MMediaElement *modifyElement = getElement(pid);
    if(modifyElement){
        modifyElement->m_name = value;
        emit elementModified(modifyElement);
    }
}
void MMediaLibrary::modify(MSong *song, const QString &value, MSong::MutableProperty songProperty){
    switch(songProperty){
        case MSong::SongName: {
            song->m_name = value;
            break;
        }
        case MSong::SongArtist: {
            song->m_artist = value;
            break;
        }
        case MSong::SongAlbum: {
            song->m_album = value;
            break;
        }
    }

    emit elementModified(song);
}

void MMediaLibrary::insert(MSong *inSong, MPlaylist *toPlaylist, int atPosition){
    if(toPlaylist){
        toPlaylist->songs.insert(qBound(0, atPosition, toPlaylist->songs.size()), inSong);
        emit elementInserted(inSong, toPlaylist);
    }
    if(!libSongs->contains(inSong)){
        libSongs->append(inSong);
        emit elementInserted(inSong, nullptr);
    }
}
void MMediaLibrary::insert(MPlaylist *inPlaylist, MFolder *toFolder, int atPosition){
    if(toFolder){
        toFolder->playlists.insert(qBound(0, atPosition, toFolder->playlists.size()), inPlaylist);
        inPlaylist->parent = toFolder;
        emit elementInserted(inPlaylist, toFolder);
    }

    if(!libPlaylists->contains(inPlaylist)){
        libPlaylists->append(inPlaylist);
        emit elementInserted(inPlaylist, nullptr);
    }
}
void MMediaLibrary::insert(MFolder *inFolder, MFolder *toFolder, int atPosition){
    if(toFolder){
        toFolder->folders.insert(qBound(0, atPosition, toFolder->folders.size()), inFolder);
        inFolder->parent = toFolder;
        emit elementInserted(inFolder, toFolder);
    }

    if(!libFolders->contains(inFolder)){
        libFolders->append(inFolder);
        emit elementInserted(inFolder, nullptr);
    }
}

void MMediaLibrary::remove(MSong *remSong, MPlaylist *fromPlaylist){
    fromPlaylist->songs.removeAll(remSong);
    emit elementRemoved(remSong, fromPlaylist);
}
void MMediaLibrary::remove(MPlaylist *remPlaylist, MFolder *fromFolder){
    fromFolder->playlists.removeAll(remPlaylist);
    emit elementRemoved(remPlaylist, fromFolder);
}
void MMediaLibrary::remove(MFolder *remFolder, MFolder *fromFolder){
    fromFolder->folders.removeAll(remFolder);
    emit elementRemoved(remFolder, fromFolder);
}

void MMediaLibrary::move(MSong* moveSong, MPlaylist *parentPlaylist, int toPosition){
    int fromPosition = parentPlaylist->songs.indexOf(moveSong);
    parentPlaylist->songs.move(fromPosition, qBound(0, toPosition, parentPlaylist->songs.size()));
    emit elementMoved(moveSong, parentPlaylist);
}
void MMediaLibrary::move(MPlaylist *movePlaylist, MFolder *toFolder, int toPosition){
    MFolder *parentFolder = movePlaylist->parent;
    if(parentFolder){
        parentFolder->playlists.removeAll(movePlaylist);
    }

    movePlaylist->parent = toFolder;

    if(toFolder){
        toFolder->playlists.insert(qBound(0, toPosition, toFolder->playlists.size()), movePlaylist);
    }

    qDebug() << "";
    qDebug() << "move" << movePlaylist->name();
    if(toFolder){
        qDebug() << "to" << toFolder->name();
    }
    else {
        qDebug() << "to root";
    }
    emit elementMoved(movePlaylist, toFolder);
}
void MMediaLibrary::move(MFolder *moveFolder, MFolder *toFolder, int toPosition){
    MFolder *parentFolder = moveFolder->parent;
    if(parentFolder){
        parentFolder->folders.removeAll(moveFolder);
    }

    moveFolder->parent = toFolder;

    if(toFolder){
        qDebug() << qBound(0, toPosition, toFolder->folders.size());
        toFolder->folders.insert(qBound(0, toPosition, toFolder->folders.size()), moveFolder);
    }

    qDebug() << "";
    qDebug() << "move" << moveFolder->name();
    if(toFolder){
        qDebug() << "to" << toFolder->name();
    }
    else {
        qDebug() << "to root";
    }

    emit elementMoved(moveFolder, toFolder);
}

void MMediaLibrary::discard(MSong *remSong){
    for(int i = 0; i < libPlaylists->size(); i++){
        MPlaylist *playlist = libPlaylists->at(i);
        if(playlist->songs.contains(remSong)){
            playlist->songs.removeAll(remSong);
        }
    }

    libSongs->removeAll(remSong);
    emit elementDeleted(remSong->pid(), Mpi3::SongElement);
//    delete remSong;
}
void MMediaLibrary::discard(MPlaylist *remPlaylist){
    if(remPlaylist->parent){
        remPlaylist->parent->playlists.removeAll(remPlaylist);
    }

    QVector<QString> pidChildren;
    for(int i = 0; i < remPlaylist->songs.size(); i++){
        pidChildren.append(remPlaylist->songs.at(i)->pid());
    }

    libPlaylists->removeAll(remPlaylist);
    emit elementDeleted(remPlaylist->pid(), Mpi3::PlaylistElement, pidChildren);
//    delete remPlaylist;
}
void MMediaLibrary::discard(MFolder *remFolder){
    if(remFolder->parent){
        remFolder->parent->folders.removeAll(remFolder);
    }

    QVector<QString> pidChildren;
    QVector<MPlaylist*> nestedPlaylists = allChildPlaylists(remFolder);
    QVector<MFolder*> nestedFolders = allChildFolders(remFolder);

    for(int i = 0; i < nestedPlaylists.size(); i++){
        MPlaylist *playlist = nestedPlaylists.at(i);
        pidChildren.append(playlist->pid());
        libPlaylists->removeAll(playlist);
//        delete playlist;
    }

    for(int i = 0; i < nestedFolders.size(); i++){
        MFolder *folder = nestedFolders.at(i);
        pidChildren.append(folder->pid());
        libFolders->removeAll(folder);
//        delete folder;
    }

    libFolders->removeAll(remFolder);
    emit elementDeleted(remFolder->pid(), Mpi3::FolderElement, pidChildren);
//    delete remFolder;
}
