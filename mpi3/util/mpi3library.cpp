#include "mpi3library.h"

#include <QDomDocument>
#include <QTextStream>
#include <QFile>

#include <QDebug>


Mpi3Element::Mpi3Element() : QObject(nullptr){}

QString Mpi3Element::pid(){
    return m_pid;
}

QString Mpi3Element::name(){
    return m_name;
}

QString Mpi3Element::added(){
    return m_added;
}


Mpi3Song::Mpi3Song() : Mpi3Element(){}
Mpi3Playlist::Mpi3Playlist() : Mpi3Element(){}

Mpi3Song* Mpi3Playlist::getSong(const QString &pid){
    for(int i = 0; i < songs.size(); i++){
        Mpi3Song *song = songs.at(i);
        if(song->pid() == pid){
            return song;
        }
    }

    return nullptr;
}

Mpi3Folder::Mpi3Folder() : Mpi3Element(){}

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

        QDomNodeList songs = root.namedItem("songs").toElement().childNodes();
        QDomNodeList playlists = root.namedItem("playlists").toElement().childNodes();
        QDomNodeList folders = root.namedItem("folders").toElement().childNodes();

        for(int i = 0; i < songs.length(); i++){
            Mpi3Song *s = new Mpi3Song();
            s->m_pid = songs.at(i).namedItem("pid").toElement().text();
            s->m_name = songs.at(i).namedItem("name").toElement().text();
            s->m_added = songs.at(i).namedItem("added").toElement().text();

            s->artist = songs.at(i).namedItem("artist").toElement().text();
            s->album = songs.at(i).namedItem("album").toElement().text();
            s->time = songs.at(i).namedItem("time").toElement().text();
            s->path = songs.at(i).namedItem("path").toElement().text();
            s->kind = songs.at(i).namedItem("kind").toElement().text();

            s->size = songs.at(i).namedItem("size").toElement().text().toInt();
            s->bitRate = songs.at(i).namedItem("bitRate").toElement().text().toInt();
            s->sampleRate = songs.at(i).namedItem("sampleRate").toElement().text().toInt();

            libSongs->push_back(s);
        }

        for(int i = 0; i < playlists.length(); i++){
            Mpi3Playlist *p = new Mpi3Playlist();
            p->m_pid = playlists.at(i).namedItem("pid").toElement().text();
            p->m_name = playlists.at(i).namedItem("name").toElement().text();
            p->m_added = playlists.at(i).namedItem("added").toElement().text();

            libPlaylists->push_back(p);
        }

        for(int i = 0; i < folders.length(); i++){
            Mpi3Folder *f = new Mpi3Folder();
            f->m_pid = folders.at(i).namedItem("pid").toElement().text();
            f->m_name = folders.at(i).namedItem("name").toElement().text();
            f->m_added = folders.at(i).namedItem("added").toElement().text();

            libFolders->push_back(f);
        }
    }
    else {
        m_pid = generatePID();
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

        QDomElement songs = xml.createElement("songs");
        QDomElement playlists = xml.createElement("playlists");
        QDomElement folders = xml.createElement("folders");

        root.appendChild(songs);
        root.appendChild(playlists);
        root.appendChild(folders);

        for(int i = 0; i < libSongs->size(); i++){
            Mpi3Song *s = libSongs->at(i);
            QDomElement song = xml.createElement("song");

            xmlWriteElement(xml, song, "pid", s->pid());
            xmlWriteElement(xml, song, "name", s->name());
            xmlWriteElement(xml, song, "added", s->added());

            xmlWriteElement(xml, song, "artist", s->artist);
            xmlWriteElement(xml, song, "album", s->album);
            xmlWriteElement(xml, song, "time", s->time);
            xmlWriteElement(xml, song, "path", s->path);
            xmlWriteElement(xml, song, "kind", s->kind);

            xmlWriteElement(xml, song, "size", QString::number(s->size));
            xmlWriteElement(xml, song, "bitRate", QString::number(s->bitRate));
            xmlWriteElement(xml, song, "sampleRate", QString::number(s->sampleRate));

            songs.appendChild(song);
        }

        for(int i = 0; i < libPlaylists->size(); i++){
            Mpi3Playlist *p = libPlaylists->at(i);
            QDomElement playlist = xml.createElement("playlist");

            xmlWriteElement(xml, playlist, "pid", p->pid());
            xmlWriteElement(xml, playlist, "name", p->name());
            xmlWriteElement(xml, playlist, "added", p->added());

            if(p->parent){
                xmlWriteElement(xml, playlist, "parent", p->parent->pid());
            }

            playlists.appendChild(playlist);
        }

        for(int i = 0; i < libFolders->size(); i++){
            Mpi3Folder *f = libFolders->at(i);
            QDomElement folder = xml.createElement("folder");

            xmlWriteElement(xml, folder, "pid", f->pid());
            xmlWriteElement(xml, folder, "name", f->name());
            xmlWriteElement(xml, folder, "added", f->added());

            if(f->parent){
                xmlWriteElement(xml, folder, "parent", f->parent->pid());
            }

            folders.appendChild(folder);
        }

        QTextStream xmlStream(&saveFile);
        xmlStream << xml.toString();
    }
}

QString Mpi3Library::filepath() const {
    return m_filepath;
}

QString Mpi3Library::generatePID(){

    QString clist [36] = {"A", "B", "C", "D", "E", "F", "G",
                          "H", "I", "J", "K", "L", "M", "N",
                          "O", "P", "Q", "R", "S", "T", "U",
                          "V", "W", "X", "Y", "Z", "0", "1",
                          "2", "3", "4", "5", "6", "7", "8", "9"};
    QString pid;
    for (int i = 0; i < 16; i++){
        pid += clist[rand() % 36];
    }

    return pid;
}
void Mpi3Library::xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text){
    QDomElement e = xml.createElement(tagname);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
}
QMap<QString, QVariant> Mpi3Library::plistDict(const QDomNode &parentNode){

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

void Mpi3Library::importItunesPlist(const QString &path){
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
            song->artist = track["Artist"].toString();
            song->album = track["Album"].toString();
            song->time = track["Total Time"].toString();
            song->path = track["Location"].toString();
            song->kind = track["Kind"].toString();
            song->size = track["Size"].toInt();
            song->bitRate = track["Bit Rate"].toInt();
            song->sampleRate = track["Sample Rate"].toInt();
            insert(song);

            songs[key] = song;
        }

        QList<QVariant> plistPlaylists = rootDict["Playlists"].toList();
        for(int i = 0; i < plistPlaylists.size(); i++){
            QMap<QString, QVariant> itunesPlaylist = plistPlaylists.at(i).toMap();

            Mpi3Playlist *playlist = newPlaylist();
            playlist->m_name = itunesPlaylist["Name"].toString();
            playlist->m_added = "";
            insert(playlist);

            QList<QVariant> playlistTrackIDs = itunesPlaylist["Playlist Items"].toList();
            for(int j = 0; j < playlistTrackIDs.size(); j++){
                QMap<QString, QVariant> trackID = playlistTrackIDs.at(j).toMap();
                insert(songs[trackID["Track ID"].toString()], playlist, j);
            }

        }
    }
}

QList<Mpi3Folder*> Mpi3Library::childFolders(Mpi3Folder *parent){
    QList<Mpi3Folder*> folders;
    for(int i = 0; i < libFolders->size(); i++){
        Mpi3Folder *f = libFolders->at(i);
        if(f->parent == parent){
            folders.append(f);
        }
    }

    return folders;
}
QList<Mpi3Playlist*> Mpi3Library::childPlaylists(Mpi3Folder *parent){
    QList<Mpi3Playlist*> playlists;
    for(int i = 0; i < libPlaylists->size(); i++){
        Mpi3Playlist *p = libPlaylists->at(i);
        if(p->parent == parent){
            playlists.append(p);
        }
    }

    return playlists;
}

Mpi3Song* Mpi3Library::getSong(const QString &pid){
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
Mpi3Playlist* Mpi3Library::getPlaylist(const QString &pid){
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
Mpi3Folder* Mpi3Library::getFolder(const QString &pid){
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

Mpi3Song* Mpi3Library::newSong(){
    Mpi3Song *song = new Mpi3Song();
    song->m_pid = generatePID();

    return song;
}
Mpi3Playlist* Mpi3Library::newPlaylist(bool named){
    Mpi3Playlist *playlist = new Mpi3Playlist();
    playlist->m_pid = generatePID();

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
Mpi3Folder* Mpi3Library::newFolder(bool named){
    Mpi3Folder *folder = new Mpi3Folder();
    folder->m_pid = generatePID();

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

void Mpi3Library::modify(Mpi3Library::Property property, const QString &value){
    switch(property) {
        case Mpi3Library::Name:
            m_name = value;
            break;
        case Mpi3Library::Added:
            m_added = value;
            break;
    }

    emit elementModified(pid());
}
void Mpi3Library::modify(Mpi3Song *song, Mpi3Song::Property property, const QString &value){
    switch(property) {
        case Mpi3Song::Name:
            song->m_name = value;
            break;
        case Mpi3Song::Added:
            song->m_added = value;
            break;
    }

    emit elementModified(song->pid());
}
void Mpi3Library::modify(Mpi3Playlist *playlist, Mpi3Playlist::Property property, const QString &value){
    switch(property) {
        case Mpi3Playlist::Name:
            playlist->m_name = value;
            break;
        case Mpi3Playlist::Added:
            playlist->m_added = value;
            break;
    }

    emit elementModified(playlist->pid());
}
void Mpi3Library::modify(Mpi3Folder *folder, Mpi3Folder::Property property, const QString &value){
    switch(property) {
        case Mpi3Folder::Name:
            folder->m_name = value;
            break;
        case Mpi3Folder::Added:
            folder->m_added = value;
            break;
    }

    emit elementModified(folder->pid());
}

void Mpi3Library::insert(Mpi3Song *inSong, Mpi3Playlist *toPlaylist, int position){
    if(toPlaylist){
        toPlaylist->songs.insert(position, inSong);
    }

    if(!libSongs->contains(inSong)){
        libSongs->push_back(inSong);
    }

    emit elementInserted(inSong->pid(), toPlaylist ? toPlaylist->pid() : QString());
}
void Mpi3Library::insert(Mpi3Playlist *inPlaylist, Mpi3Folder *toFolder, int position){
    if(toFolder){
        toFolder->playlists.insert(position, inPlaylist);
        inPlaylist->parent = toFolder;
    }

    if(!libPlaylists->contains(inPlaylist)){
        libPlaylists->push_back(inPlaylist);
    }

    emit elementInserted(inPlaylist->pid(), toFolder ? toFolder->pid() : QString());
}
void Mpi3Library::insert(Mpi3Folder *inFolder, Mpi3Folder *toFolder, int position){
    if(toFolder){
        toFolder->folders.insert(position, inFolder);
        inFolder->parent = toFolder;
    }

    if(!libFolders->contains(inFolder)){
        libFolders->push_back(inFolder);
    }

    emit elementInserted(inFolder->pid(), toFolder ? toFolder->pid() : QString());
}

void Mpi3Library::move(Mpi3Song *moveSong, Mpi3Playlist *inPlaylist, int position){
    Q_UNUSED(moveSong);
    Q_UNUSED(inPlaylist);
    Q_UNUSED(position);
}
void Mpi3Library::move(Mpi3Playlist *movePlaylist, Mpi3Folder *inFolder, int position){
    Q_UNUSED(movePlaylist);
    Q_UNUSED(inFolder);
    Q_UNUSED(position);
}
void Mpi3Library::move(Mpi3Folder *moveFolder, Mpi3Folder *inFolder, int position){
    Q_UNUSED(moveFolder);
    Q_UNUSED(inFolder);
    Q_UNUSED(position);
}

void Mpi3Library::remove(Mpi3Song *remSong, Mpi3Playlist *fromPlaylist){
    fromPlaylist->songs.removeAll(remSong);
    emit elementRemoved(remSong->pid(), fromPlaylist->pid());
}
void Mpi3Library::remove(Mpi3Playlist *remPlaylist, Mpi3Folder *fromFolder){
    fromFolder->playlists.removeAll(remPlaylist);
    emit elementRemoved(remPlaylist->pid(), fromFolder->pid());
}
void Mpi3Library::remove(Mpi3Folder *remFolder, Mpi3Folder *fromFolder){
    fromFolder->folders.removeAll(remFolder);
    emit elementRemoved(remFolder->pid(), fromFolder->pid());
}

void Mpi3Library::remove(Mpi3Song *remSong){
    libSongs->removeAll(remSong);

    for(int i = 0; i < libPlaylists->size(); i++){
        Mpi3Playlist *playlist = libPlaylists->at(i);
        if(playlist->songs.contains(remSong)){
            playlist->songs.removeAll(remSong);
            emit elementRemoved(remSong->pid(), playlist->pid());
        }
    }

    emit elementRemoved(remSong->pid(), QString());
    delete remSong;
}
void Mpi3Library::remove(Mpi3Playlist *remPlaylist){
    libPlaylists->removeAll(remPlaylist);

    Mpi3Folder *parentFolder = remPlaylist->parent;
    if(parentFolder){
        parentFolder->playlists.removeAll(remPlaylist);
    }

    emit elementRemoved(remPlaylist->pid(), parentFolder ? parentFolder->pid() : QString());
    delete remPlaylist;
}
void Mpi3Library::remove(Mpi3Folder *remFolder){
    libFolders->removeAll(remFolder);

    Mpi3Folder *parentFolder = remFolder->parent;
    if(parentFolder){
        parentFolder->folders.removeAll(remFolder);
    }

    emit elementRemoved(remFolder->pid(), parentFolder ? parentFolder->pid() : QString());
    delete remFolder;
}



























