#include "medialib.h"

#include <QDomDocument>
#include <QTextStream>
#include <QFile>

#include <random>
#include <time.h>

#include <QDebug>


QString generatePID()
{

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


Mpi3Element::Mpi3Element(const bool &newpid) : QObject(NULL)
{
    if(newpid){
        this->pid = generatePID();
    }
}


Mpi3Song::Mpi3Song(const bool &newpid) : Mpi3Element(newpid)
{

}


Mpi3Playlist::Mpi3Playlist(const bool &newpid) : Mpi3Element(newpid)
{

}


Mpi3Folder::Mpi3Folder(const bool &newpid) : Mpi3Element(newpid)
{

}


Mpi3Library::Mpi3Library(const bool &newpid) : Mpi3Element(newpid)
{
    songs = new QVector<Mpi3Song*>;
    playlists = new QVector<Mpi3Playlist*>;
    folders = new QVector<Mpi3Folder*>;
}

Mpi3Library* Mpi3Library::load(const QString &path)
{
    Mpi3Library *mpi3Lib = new Mpi3Library;
    QFile loadFile(path);

    if (loadFile.open(QIODevice::ReadOnly)){
        QDomDocument xml;
        xml.setContent(loadFile.readAll());

        QDomElement root = xml.documentElement();
        mpi3Lib->pid = root.namedItem("pid").toElement().text();
        mpi3Lib->name = root.namedItem("name").toElement().text();
        mpi3Lib->added = root.namedItem("added").toElement().text();
        mpi3Lib->filepath = path;

        QDomNodeList songs = root.namedItem("songs").toElement().childNodes();
        QDomNodeList playlists = root.namedItem("playlists").toElement().childNodes();
        QDomNodeList folders = root.namedItem("folders").toElement().childNodes();

        for(int i = 0; i < songs.length(); i++){
            Mpi3Song *s = new Mpi3Song;
            s->pid = songs.at(i).namedItem("pid").toElement().text();
            s->name = songs.at(i).namedItem("name").toElement().text();
            s->added = songs.at(i).namedItem("added").toElement().text();

            s->artist = songs.at(i).namedItem("artist").toElement().text();
            s->album = songs.at(i).namedItem("album").toElement().text();
            s->time = songs.at(i).namedItem("time").toElement().text();
            s->path = songs.at(i).namedItem("path").toElement().text();
            s->kind = songs.at(i).namedItem("kind").toElement().text();
            s->added = songs.at(i).namedItem("added").toElement().text();

            s->size = songs.at(i).namedItem("size").toElement().text().toInt();
            s->bitRate = songs.at(i).namedItem("bitRate").toElement().text().toInt();
            s->sampleRate = songs.at(i).namedItem("sampleRate").toElement().text().toInt();

            mpi3Lib->songs->push_back(s);
        }

        for(int i = 0; i < playlists.length(); i++){
            Mpi3Playlist *p = new Mpi3Playlist;
            p->pid = playlists.at(i).namedItem("pid").toElement().text();
            p->name = playlists.at(i).namedItem("name").toElement().text();
            p->added = playlists.at(i).namedItem("added").toElement().text();

            mpi3Lib->playlists->push_back(p);
        }

        for(int i = 0; i < folders.length(); i++){
            Mpi3Folder *f = new Mpi3Folder;
            f->pid = folders.at(i).namedItem("pid").toElement().text();
            f->name = folders.at(i).namedItem("name").toElement().text();
            f->added = folders.at(i).namedItem("added").toElement().text();

            mpi3Lib->folders->push_back(f);
        }
    }

    return mpi3Lib;
}

void xmlWriteElement(QDomDocument xml, QDomElement elem, QString tagname, QString text)
{
    QDomElement e = xml.createElement(tagname);
    QDomText t = xml.createTextNode(text);
    e.appendChild(t);
    elem.appendChild(e);
}

void Mpi3Library::save(const QString &path)
{
    if(path != NULL){
        this->filepath = path;
    }

    QFile saveFile(this->filepath);
    if(saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QDomDocument xml("Mpi3Library");
        QDomElement root = xml.createElement("library");
        xml.appendChild(root);

        xmlWriteElement(xml, root, "pid", this->pid);
        xmlWriteElement(xml, root, "name", this->name);
        xmlWriteElement(xml, root, "added", this->added);

        QDomElement songs = xml.createElement("songs");
        QDomElement playlists = xml.createElement("playlists");
        QDomElement folders = xml.createElement("folders");

        root.appendChild(songs);
        root.appendChild(playlists);
        root.appendChild(folders);

        for(int i = 0; i < this->songs->size(); i++){
            Mpi3Song *s = this->songs->at(i);
            QDomElement song = xml.createElement("song");

            xmlWriteElement(xml, song, "pid", s->pid);
            xmlWriteElement(xml, song, "name", s->name);
            xmlWriteElement(xml, song, "added", s->added);

            xmlWriteElement(xml, song, "artist", s->artist);
            xmlWriteElement(xml, song, "album", s->album);
            xmlWriteElement(xml, song, "time", s->time);
            xmlWriteElement(xml, song, "path", s->path);
            xmlWriteElement(xml, song, "kind", s->kind);

            xmlWriteElement(xml, song, "size", QString(s->size));
            xmlWriteElement(xml, song, "bitRate", QString(s->bitRate));
            xmlWriteElement(xml, song, "sampleRate", QString(s->sampleRate));

            songs.appendChild(song);
        }

        for(int i = 0; i < this->playlists->size(); i++){
            Mpi3Playlist *p = this->playlists->at(i);
            QDomElement playlist = xml.createElement("song");

            xmlWriteElement(xml, playlist, "pid", p->pid);
            xmlWriteElement(xml, playlist, "name", p->name);
            xmlWriteElement(xml, playlist, "added", p->added);

            playlists.appendChild(playlist);
        }

        for(int i = 0; i < this->folders->size(); i++){
            Mpi3Folder *f = this->folders->at(i);
            QDomElement folder = xml.createElement("song");

            xmlWriteElement(xml, folder, "pid", f->pid);
            xmlWriteElement(xml, folder, "name", f->name);
            xmlWriteElement(xml, folder, "added", f->added);

            folders.appendChild(folder);
        }

        QTextStream xmlStream(&saveFile);
        xmlStream << xml.toString();
    }
}

Mpi3Song* Mpi3Library::addSong()
{
    Mpi3Song *song = new Mpi3Song(true);
    songs->push_back(song);

    int index = songs->size() - 1;
    emit mediaInserted(index, 1);

    return song;
}

Mpi3Playlist* Mpi3Library::addPlaylist(Mpi3Folder *parent)
{
    Mpi3Playlist *playlist = new Mpi3Playlist(true);

    playlists->push_back(playlist);

    if(parent){
        playlist->parent = parent;
    }

    return playlist;
}

Mpi3Folder* Mpi3Library::addFolder(Mpi3Folder *parent)
{
    Mpi3Folder *folder = new Mpi3Folder(true);
    folders->push_back(folder);

    if(parent){
        folder->parent = parent;
    }

    return folder;
}

void Mpi3Library::removeSong(Mpi3Song *s)
{
    qDebug() << s->pid;
}

void Mpi3Library::removePlaylist(Mpi3Playlist *p)
{
    qDebug() << p->pid;
}

void Mpi3Library::removeFolder(Mpi3Folder *f)
{
    qDebug() << f->pid;
}


void Mpi3Library::update(Mpi3Song *song)
{
    int index = songs->indexOf(song);
    emit mediaChanged(index, song);
}


























