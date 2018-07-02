#include "medialib.h"

#include <QDomDocument>
#include <QTextStream>
#include <QFile>

#include <random>
#include <time.h>

#include <QDebug>


// ----------------------------------------------------------------------------------------------------
// * Functions *
// ----------------------------------------------------------------------------------------------------
QString generatePID()
{

    QString clist [36] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R",
                              "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    QString pid;
    for (int i = 0; i < 16; i++)
    {
        pid += clist[rand() % 36];
    }

    return pid;
}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Element *
// ----------------------------------------------------------------------------------------------------
Mpi3Element::Mpi3Element(const bool &newpid) : QObject(NULL)
{
    if(newpid){this->pid = generatePID();}
}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Song *
// ----------------------------------------------------------------------------------------------------
Mpi3Song::Mpi3Song(const bool &newpid) : Mpi3Element(newpid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Playlist *
// ----------------------------------------------------------------------------------------------------
Mpi3Playlist::Mpi3Playlist(const bool &newpid) : Mpi3Element(newpid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Folder *
// ----------------------------------------------------------------------------------------------------
Mpi3Folder::Mpi3Folder(const bool &newpid) : Mpi3Element(newpid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Library *
// ----------------------------------------------------------------------------------------------------
Mpi3Library::Mpi3Library(const bool &newpid) : Mpi3Element(newpid)
{
    songs = new QMap<QString, Mpi3Song*>;
    playlists = new QMap<QString, Mpi3Playlist*>;
    folders = new QMap<QString, Mpi3Folder*>;
}

Mpi3Library* Mpi3Library::load(const QString &path)
{
    Mpi3Library *mpi3Lib = new Mpi3Library;
    QFile loadFile(path);

    if (loadFile.open(QIODevice::ReadOnly))
    {
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

        for(int i = 0; i < songs.length(); i++)
        {
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

            mpi3Lib->songs->insert(s->pid, s);
        }

        for(int i = 0; i < playlists.length(); i++)
        {
            Mpi3Playlist *p = new Mpi3Playlist;
            p->pid = playlists.at(i).namedItem("pid").toElement().text();
            p->name = playlists.at(i).namedItem("name").toElement().text();
            p->added = playlists.at(i).namedItem("added").toElement().text();

            mpi3Lib->playlists->insert(p->pid, p);
        }

        for(int i = 0; i < folders.length(); i++)
        {
            Mpi3Folder *f = new Mpi3Folder;
            f->pid = folders.at(i).namedItem("pid").toElement().text();
            f->name = folders.at(i).namedItem("name").toElement().text();
            f->added = folders.at(i).namedItem("added").toElement().text();

            mpi3Lib->folders->insert(f->pid, f);
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
    if(path != NULL){this->filepath = path;}
    QFile saveFile(this->filepath);

    if(saveFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
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

        foreach(Mpi3Song *s, this->songs->values())
        {
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

        foreach(Mpi3Playlist *p, this->playlists->values())
        {
            QDomElement playlist = xml.createElement("song");

            xmlWriteElement(xml, playlist, "pid", p->pid);
            xmlWriteElement(xml, playlist, "name", p->name);
            xmlWriteElement(xml, playlist, "added", p->added);

            playlists.appendChild(playlist);
        }

        foreach(Mpi3Folder *f, this->folders->values())
        {
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
    int s = songs->values().length();

    Mpi3Song *song = new Mpi3Song(true);
    songs->insert(song->pid, song);

    s = songs->values().length();

    emit mediaInserted(0, 1);

    return song;
}

Mpi3Playlist* Mpi3Library::addPlaylist(Mpi3Folder *parent)
{
    Mpi3Playlist *playlist = new Mpi3Playlist(true);

    playlists->insert(playlist->pid, playlist);

    if(parent){
        playlist->parent = parent;
    }

    return playlist;
}

Mpi3Folder* Mpi3Library::addFolder(Mpi3Folder *parent)
{
    Mpi3Folder *folder = new Mpi3Folder(true);

    folders->insert(folder->pid, folder);

    if(parent){
        folder->parent = parent;
    }

    return folder;
}

void Mpi3Library::removeSong(const QString &pid)
{
    songs->remove(pid);
}

void Mpi3Library::removePlaylist(const QString &pid)
{
    playlists->remove(pid);
}

void Mpi3Library::removeFolder(const QString &pid)
{
    folders->remove(pid);
}


QStringList children(const QString &parent)
{
    Q_UNUSED(parent);
    QStringList a;
    return a;
//    def children(self, parent=None):
//        if parent is None:
//            for f in self.folders.values():
//                if f.folder is None:
//                    yield f
//            for p in self.playlists.values():
//                if p.folder is None:
//                    yield p
//        elif isinstance(parent, Folder):
//            for f in self.folders.values():
//                if f.folder == parent.pid:
//                    yield f
//            for p in self.playlists.values():
//                if p.folder == parent.pid:
//                    yield p
//        elif isinstance(parent, Playlist):
//            for s in parent.songs:
//                yield self[s]
//        else:
//            err = 'Non-container element {}'.format(parent)
//            raise ValueError(err)
}



void Mpi3Library::update()
{
    emit mediaChanged(0, songs->values().at(0));
}

int Mpi3Library::mediaCount() const
{
    return songs->values().length();
}


























