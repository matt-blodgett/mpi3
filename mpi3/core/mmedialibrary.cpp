#include "mmedialibrary.h"
#include "maudioengine.h"

#include <QRandomGenerator>
#include <QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QUrl>
#include <QDir>


#define PID_MAX_LENGTH 18
static const QString PrefixBase         = "E:";
static const QString PrefixSong         = "S:";
static const QString PrefixContainer    = "C:";
static const QString PrefixPlaylist     = "P:";
static const QString PrefixFolder       = "F:";
static const QString PrefixLibrary      = "L:";


static void xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text){
    QDomElement e = xml.createElement(tagname);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
}
static QMap<QString, QVariant> plistDict(const QDomNode &parentNode){

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


MMediaElement::MMediaElement(QObject *parent) : QObject(parent){}
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
QString MSong::kind() const{
    return m_kind;
}
QString MSong::path() const {
    return QDir::toNativeSeparators(QUrl(m_path).toLocalFile());;
}
QString MSong::url() const {
    return m_path;
}

double MSong::time() const{
    return m_time;
}
double MSong::size() const{
    return m_size;
}

int MSong::bitRate() const{
    return m_bitRate;
}
int MSong::sampleRate() const{
    return m_sampleRate;
}

QString MSong::majorBrand() const{
    return m_majorBrand;
}
QString MSong::minorVersion() const{
    return m_minorVersion;
}
QString MSong::compatibleBrands() const{
    return m_compatibleBrands;
}
QString MSong::encoder() const{
    return m_encoder;
}

MMediaContainer::MMediaContainer(){}
Mpi3::ElementType MMediaContainer::type() const {
    return Mpi3::ContainerElement;
}

QVector<MSong*> MMediaContainer::songs() const {
    return QVector<MSong*>();
}
QVector<MPlaylist*> MMediaContainer::playlists() const {
    return QVector<MPlaylist*>();
}
QVector<MFolder*> MMediaContainer::folders() const {
    return QVector<MFolder*>();
}

MFolder *MMediaContainer::parentFolder() const {
    if(m_parent){
        if(m_parent->type() == Mpi3::FolderElement){
            return static_cast<MFolder*>(m_parent);
        }
    }

    return nullptr;
}

QVector<MMediaElement*> MMediaContainer::childElements(Mpi3::ElementType elemType) const{

    QVector<MMediaElement*> children;

    bool baseElem = elemType == Mpi3::BaseElement;
    bool containerElem = elemType == Mpi3::ContainerElement;

    if(elemType == Mpi3::FolderElement || baseElem || containerElem){
        foreach(MFolder *folder, folders()){
            children.push_back(folder);
        }
    }

    if(elemType == Mpi3::PlaylistElement || baseElem || containerElem){
        foreach(MPlaylist *playlist, playlists()){
            children.push_back(playlist);
        }
    }

    if(elemType == Mpi3::SongElement || baseElem){
        foreach(MSong *song, songs()){
            children.push_back(song);
        }
    }

    return children;
}
MMediaElement *MMediaContainer::getElement(const QString &pid, Mpi3::ElementType elemType) const{

    foreach(MMediaElement *element, childElements(elemType)){
        if(element->pid() == pid){
            return element;
        }
    }

    return nullptr;
}

MMediaContainer *MMediaContainer::getContainer(const QString &pid) const {
    return static_cast<MMediaContainer*>(getElement(pid, Mpi3::ContainerElement));
}
MPlaylist *MMediaContainer::getPlaylist(const QString &pid) const{
    return static_cast<MPlaylist*>(getElement(pid, Mpi3::PlaylistElement));
}
MFolder *MMediaContainer::getFolder(const QString &pid) const{
    return static_cast<MFolder*>(getElement(pid, Mpi3::FolderElement));
}
MSong *MMediaContainer::getSong(const QString &pid) const{
    return static_cast<MSong*>(getElement(pid, Mpi3::SongElement));
}


MPlaylist::MPlaylist() : MMediaContainer(){}
Mpi3::ElementType MPlaylist::type() const {
    return Mpi3::PlaylistElement;
}
QVector<MSong*> MPlaylist::songs() const {
    return m_songs;
}


MFolder::MFolder() : MMediaContainer(){}
Mpi3::ElementType MFolder::type() const {
    return Mpi3::FolderElement;
}

QVector<MSong*> MFolder::songs() const{

    QVector<MSong*> childsongs;

    foreach(MPlaylist *playlist, playlists()){
        foreach(MSong *song, playlist->songs()){
            if(!childsongs.contains(song)){
                childsongs.push_back(song);
            }
        }
    }

    foreach(MFolder *folder, folders()){
        foreach(MSong* song, folder->songs()){
            if(!childsongs.contains(song)){
                childsongs.append(song);
            }
        }
    }

    return childsongs;
}
QVector<MPlaylist*> MFolder::playlists() const{
    return m_playlists;
}
QVector<MFolder*> MFolder::folders() const{
    return m_folders;
}
QVector<MMediaContainer*> MFolder::childContainers() const {

    QVector<MMediaContainer*> containers;

    foreach(MFolder *folder, m_folders){
        containers.push_back(folder);

        foreach(MMediaContainer *child, folder->childContainers()){
            containers.push_back(child);
        }
    }

    foreach(MPlaylist *playlist, m_playlists){
        containers.push_back(playlist);
    }

    return containers;
}


MMediaLibrary::MMediaLibrary() : MMediaContainer(){}
Mpi3::ElementType MMediaLibrary::type() const {
    return Mpi3::LibraryElement;
}

QString MMediaLibrary::filepath() const {
    return m_filepath;
}
void MMediaLibrary::load(const QString &path){

    QFile loadFile(path);
    if (loadFile.open(QIODevice::ReadOnly)){

        m_libFolders.clear();
        m_libPlaylists.clear();
        m_libSongs.clear();

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
            song->m_kind = xmlSongs.at(i).namedItem("kind").toElement().text();

            song->m_path = xmlSongs.at(i).namedItem("path").toElement().text();

            song->m_time = xmlSongs.at(i).namedItem("time").toElement().text().toDouble();
            song->m_size = xmlSongs.at(i).namedItem("size").toElement().text().toDouble();

            song->m_bitRate = xmlSongs.at(i).namedItem("bitRate").toElement().text().toInt();
            song->m_sampleRate = xmlSongs.at(i).namedItem("sampleRate").toElement().text().toInt();

            song->m_majorBrand = xmlSongs.at(i).namedItem("majorBrand").toElement().text();
            song->m_minorVersion = xmlSongs.at(i).namedItem("minorVersion").toElement().text();
            song->m_compatibleBrands = xmlSongs.at(i).namedItem("compatibleBrands").toElement().text();
            song->m_encoder = xmlSongs.at(i).namedItem("encoder").toElement().text();

            m_libSongs.append(song);
        }

        for(int i = 0; i < xmlPlaylists.length(); i++){
            MPlaylist *playlist = new MPlaylist();
            playlist->m_pid = xmlPlaylists.at(i).namedItem("pid").toElement().text();
            playlist->m_name = xmlPlaylists.at(i).namedItem("name").toElement().text();
            playlist->m_added = xmlPlaylists.at(i).namedItem("added").toElement().text();

            QDomNodeList childSongs = xmlPlaylists.at(i).namedItem("childSongs").childNodes();
            for(int c_index = 0; c_index < childSongs.size(); c_index++){
                playlist->m_songs.append(getSong(childSongs.at(c_index).toElement().text()));
            }

            m_libPlaylists.append(playlist);
        }

        for(int i = 0; i < xmlFolders.length(); i++){
            MFolder *folder = new MFolder();
            folder->m_pid = xmlFolders.at(i).namedItem("pid").toElement().text();
            folder->m_name = xmlFolders.at(i).namedItem("name").toElement().text();
            folder->m_added = xmlFolders.at(i).namedItem("added").toElement().text();

            QDomNodeList childPlaylists = xmlFolders.at(i).namedItem("childPlaylists").childNodes();
            for(int c_index = 0; c_index < childPlaylists.size(); c_index++){
                MPlaylist *chPlaylist = getPlaylist(childPlaylists.at(c_index).toElement().text());

                folder->m_playlists.append(chPlaylist);
                chPlaylist->m_parent = folder;
            }

            m_libFolders.append(folder);
        }

        for(int i = 0; i < xmlFolders.length(); i++){
            MFolder *folder = getFolder(xmlFolders.at(i).namedItem("pid").toElement().text());

            QDomNodeList childFolders = xmlFolders.at(i).namedItem("childFolders").childNodes();
            for(int c_index = 0; c_index < childFolders.size(); c_index++){
                MFolder *chFolder = getFolder(childFolders.at(c_index).toElement().text());

                folder->m_folders.append(chFolder);
                chFolder->m_parent = folder;
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

        xmlWriteElement(xml, root, "pid", m_pid);
        xmlWriteElement(xml, root, "name", m_name);
        xmlWriteElement(xml, root, "added", m_added);

        QDomElement xmlSongs = xml.createElement("songs");
        QDomElement xmlPlaylists = xml.createElement("playlists");
        QDomElement xmlFolders = xml.createElement("folders");

        root.appendChild(xmlSongs);
        root.appendChild(xmlPlaylists);
        root.appendChild(xmlFolders);

        for(int i = 0; i < m_libSongs.size(); i++){
            MSong *song = m_libSongs.at(i);
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

            xmlWriteElement(xml, songElement, "majorBrand", song->m_majorBrand);
            xmlWriteElement(xml, songElement, "minorVersion", song->m_minorVersion);
            xmlWriteElement(xml, songElement, "compatibleBrands", song->m_compatibleBrands);
            xmlWriteElement(xml, songElement, "encoder", song->m_encoder);

            xmlSongs.appendChild(songElement);
        }

        for(int i = 0; i < m_libPlaylists.size(); i++){
            MPlaylist *playlist = m_libPlaylists.at(i);
            QDomElement playlistElement = xml.createElement("playlist");

            xmlWriteElement(xml, playlistElement, "pid", playlist->m_pid);
            xmlWriteElement(xml, playlistElement, "name", playlist->m_name);
            xmlWriteElement(xml, playlistElement, "added", playlist->m_added);

            MFolder *parentFolder = playlist->m_parent;
            xmlWriteElement(xml, playlistElement, "parent", parentFolder ? parentFolder->m_pid : "");

            QDomElement playlistSongs = xml.createElement("childSongs");
            foreach(MSong *childSong, playlist->m_songs){
                xmlWriteElement(xml, playlistSongs, "pid", childSong->m_pid);
            }

            playlistElement.appendChild(playlistSongs);
            xmlPlaylists.appendChild(playlistElement);
        }

        for(int i = 0; i < m_libFolders.size(); i++){
            MFolder *folder = m_libFolders.at(i);
            QDomElement folderElement = xml.createElement("folder");

            xmlWriteElement(xml, folderElement, "pid", folder->m_pid);
            xmlWriteElement(xml, folderElement, "name", folder->m_name);
            xmlWriteElement(xml, folderElement, "added", folder->m_added);

            MFolder *parentFolder = folder->m_parent;
            xmlWriteElement(xml, folderElement, "parent", parentFolder ? parentFolder->m_pid : "");

            QDomElement folderFolders = xml.createElement("childFolders");
            foreach(MFolder *childFolder, folder->m_folders){
                xmlWriteElement(xml, folderFolders, "pid", childFolder->m_pid);
            }

            QDomElement folderPlaylists = xml.createElement("childPlaylists");
            foreach(MPlaylist *childPlaylist, folder->m_playlists){
                xmlWriteElement(xml, folderPlaylists, "pid", childPlaylist->m_pid);
            }

            folderElement.appendChild(folderFolders);
            folderElement.appendChild(folderPlaylists);
            xmlFolders.appendChild(folderElement);
        }

        QTextStream xmlStream(&saveFile);
        xmlStream << xml.toString();
    }
}
void MMediaLibrary::reset(){
    m_libSongs.clear();
    m_libPlaylists.clear();
    m_libFolders.clear();
}

QVector<MSong*> MMediaLibrary::songs() const{
    return m_libSongs;
}
QVector<MPlaylist*> MMediaLibrary::playlists() const{
    return m_libPlaylists;
}
QVector<MFolder*> MMediaLibrary::folders() const{
    return m_libFolders;
}

QVector<MFolder*> MMediaLibrary::rootFolders() const{

    QVector<MFolder*> childFolders;

    foreach(MFolder *folder, folders()){
        if(!folder->m_parent){
            childFolders.append(folder);
        }
    }

    return childFolders;
}
QVector<MPlaylist*> MMediaLibrary::rootPlaylists() const{

    QVector<MPlaylist*> childPlaylists;

    foreach(MPlaylist *playlist, m_libPlaylists){
        if(!playlist->m_parent){
            childPlaylists.append(playlist);
        }
    }

    return childPlaylists;
}

MSong *MMediaLibrary::newSong(const QString &path) const{
    MSong *song = new MSong();
    song->m_pid = generatePID(Mpi3::SongElement);

    if(!path.isNull()){

        MSongInfo info;
        info.load(path);

        if(info.loaded){
            song->m_name = info.title;
            song->m_artist = info.artist;
            song->m_album = info.album;
            song->m_kind = info.kind;
            song->m_path = path;

            song->m_size = info.size;
            song->m_time = info.time;

            song->m_bitRate = info.bitRate;
            song->m_sampleRate = info.sampleRate;

            song->m_majorBrand = info.majorBrand;
            song->m_minorVersion = info.minorVersion;
            song->m_compatibleBrands = info.compatibleBrands;
            song->m_encoder = info.encoder;
        }

        if(song->m_name.isEmpty() || song->m_name.isNull()){
            song->m_name = QUrl(path).fileName();
            QStringList splitExt = song->m_name.split(".");
            if(splitExt.size() > 0){
                song->m_name = splitExt.at(0);
            }
        }
    }

    return song;
}
MPlaylist *MMediaLibrary::newPlaylist(bool named) const{
    MPlaylist *playlist = new MPlaylist();
    playlist->m_pid = generatePID(Mpi3::PlaylistElement);

    if(named){
        QString name = "New Playlist";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < m_libPlaylists.size()){
            MPlaylist *p = m_libPlaylists.at(i++);
            if(p->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }

        playlist->m_name = name + append;
    }

    return playlist;
}
MFolder *MMediaLibrary::newFolder(bool named) const{
    MFolder *folder = new MFolder();
    folder->m_pid = generatePID(Mpi3::FolderElement);

    if(named){
        QString name = "New Folder";
        QString append = "";
        int duplicates = 1;

        int i = 0;
        while(i < m_libFolders.size()){
            MFolder *f = m_libFolders.at(i++);
            if(f->name() == name + append){
                append = " " + QString::number(duplicates++);
                i = 0;
            }
        }

        folder->m_name = name + append;
    }

    return folder;
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
        case Mpi3::ContainerElement: {
            pid = PrefixContainer;
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

    foreach(MMediaElement *element, childElements(elemType)){
        if(element->m_pid == pid){
            return generatePID(elemType);
        }
    }

    return pid;
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

bool MMediaLibrary::validMediaFiles(QUrl mediaUrl){
    if(mediaUrl.toString().endsWith(".mp3") || mediaUrl.toString().endsWith(".wav")){
        return true;
    }

    return false;
}
bool MMediaLibrary::validMediaFiles(QList<QUrl> mediaUrls){
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

QVector<MSong*> MMediaLibrary::songsFromBytes(QByteArray pidBytes) const {
    QStringList pidStrings;

    int i = 0;
    int length = PID_MAX_LENGTH;
    while(i + length <= pidBytes.size()){
        pidStrings.append(pidBytes.mid(i, length));
        i += length;
    }

    QVector<MSong*> extractedSongs;
    foreach(QString pidstr, pidStrings){
        MSong *song = getSong(pidstr);
        if(song && !extractedSongs.contains(song)){
            extractedSongs.append(song);
        }
    }

    return extractedSongs;
}
QByteArray MMediaLibrary::songsToBytes(QVector<MSong*> songObjects){
    QByteArray pidBytes;
    foreach(MSong *song, songObjects){
        pidBytes.append(song->pid());
    }

    return pidBytes;
}
QList<QUrl> MMediaLibrary::songsToPaths(QVector<MSong*> songObjects) {
    QList<QUrl> songUrls;
    foreach(MSong *song, songObjects){
        songUrls.push_back(song->path());
    }

    return songUrls;
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
        toPlaylist->m_songs.insert(qBound(0, atPosition, toPlaylist->m_songs.size()), inSong);
        emit elementInserted(inSong, toPlaylist);
    }

    if(!m_libSongs.contains(inSong)){
        m_libSongs.append(inSong);
        emit elementInserted(inSong, this);
    }
}
void MMediaLibrary::insert(MPlaylist *inPlaylist, MFolder *toFolder, int atPosition){

    if(toFolder){
        int pos = qBound(0, atPosition, toFolder->m_playlists.size());
        toFolder->m_playlists.insert(pos, inPlaylist);
        emit elementInserted(inPlaylist, toFolder);
    }

    inPlaylist->m_parent = toFolder;

    if(!m_libPlaylists.contains(inPlaylist)){
        m_libPlaylists.append(inPlaylist);
        emit elementInserted(inPlaylist, this);
    }
}
void MMediaLibrary::insert(MFolder *inFolder, MFolder *toFolder, int atPosition){

    if(toFolder){
        int pos = qBound(0, atPosition, toFolder->m_folders.size());
        toFolder->m_folders.insert(pos, inFolder);
        emit elementInserted(inFolder, toFolder);
    }

    inFolder->m_parent = toFolder;

    if(!m_libFolders.contains(inFolder)){
        m_libFolders.append(inFolder);
        emit elementInserted(inFolder, this);
    }
}

void MMediaLibrary::remove(MSong *remSong, MPlaylist *fromPlaylist){
    fromPlaylist->m_songs.removeAll(remSong);
    emit elementRemoved(remSong, fromPlaylist);
}
void MMediaLibrary::remove(MPlaylist *remPlaylist, MFolder *fromFolder){
    fromFolder->m_playlists.removeAll(remPlaylist);
    emit elementRemoved(remPlaylist, fromFolder);
}
void MMediaLibrary::remove(MFolder *remFolder, MFolder *fromFolder){
    fromFolder->m_folders.removeAll(remFolder);
    emit elementRemoved(remFolder, fromFolder);
}

void MMediaLibrary::move(MSong* moveSong, MPlaylist *parentPlaylist, int toPosition){

    int fromPosition = parentPlaylist->m_songs.indexOf(moveSong);
    int pos = qBound(0, toPosition, parentPlaylist->m_songs.size());

    parentPlaylist->m_songs.move(fromPosition, pos);
    emit elementMoved(moveSong, parentPlaylist);
}
void MMediaLibrary::move(MPlaylist *movePlaylist, MFolder *toFolder, int toPosition){

    if(movePlaylist->m_parent){
         movePlaylist->m_parent->m_playlists.removeAll(movePlaylist);
    }

    if(toFolder){
        int pos = qBound(0, toPosition, toFolder->m_playlists.size());
        toFolder->m_playlists.insert(pos, movePlaylist);
    }

    movePlaylist->m_parent = toFolder;
    emit elementMoved(movePlaylist, toFolder ? toFolder : static_cast<MMediaContainer*>(this));
}
void MMediaLibrary::move(MFolder *moveFolder, MFolder *toFolder, int toPosition){

    if(moveFolder->m_parent){
        moveFolder->m_parent->m_folders.removeAll(moveFolder);
    }

    if(toFolder){
        int pos = qBound(0, toPosition, toFolder->m_folders.size());
        toFolder->m_folders.insert(pos, moveFolder);
    }

    moveFolder->m_parent = toFolder;
    emit elementMoved(moveFolder, toFolder ? toFolder : static_cast<MMediaContainer*>(this));
}

void MMediaLibrary::discard(MSong *remSong){

    foreach(MPlaylist *playlist, m_libPlaylists){
        if(playlist->m_songs.contains(remSong)){
            playlist->m_songs.removeAll(remSong);
        }
    }

    m_libSongs.removeAll(remSong);
    emit elementDeleted(remSong);
}
void MMediaLibrary::discard(MPlaylist *remPlaylist){

    if(remPlaylist->m_parent){
        remPlaylist->m_parent->m_playlists.removeAll(remPlaylist);
    }

    m_libPlaylists.removeAll(remPlaylist);
    emit elementDeleted(remPlaylist);
}
void MMediaLibrary::discard(MFolder *remFolder){

    if(remFolder->m_parent){
        remFolder->m_parent->m_folders.removeAll(remFolder);
    }

    foreach(MMediaContainer *child, remFolder->childContainers()){

        if(child->type() == Mpi3::FolderElement){
            m_libFolders.removeAll(static_cast<MFolder*>(child));
        }
        else if(child->type() == Mpi3::PlaylistElement){
            m_libPlaylists.removeAll(static_cast<MPlaylist*>(child));
        }
    }

    m_libFolders.removeAll(remFolder);
    emit elementDeleted(remFolder);
}
