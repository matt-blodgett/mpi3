#include "medialib.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QFile>

#include <random>


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
Mpi3Element::Mpi3Element(const QString &pid)
{
    if(pid != NULL){mPID = pid;}
    else {mPID = generatePID();}
}

QString Mpi3Element::pid()
{
    return mPID;
}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Song *
// ----------------------------------------------------------------------------------------------------
Mpi3Song::Mpi3Song(const QString &pid) : Mpi3Element(pid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Playlist *
// ----------------------------------------------------------------------------------------------------
Mpi3Playlist::Mpi3Playlist(const QString &pid) : Mpi3Element(pid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Folder *
// ----------------------------------------------------------------------------------------------------
Mpi3Folder::Mpi3Folder(const QString &pid) : Mpi3Element(pid)
{

}


// ----------------------------------------------------------------------------------------------------
// * Mpi3Library *
// ----------------------------------------------------------------------------------------------------
Mpi3Library::Mpi3Library(const QString &pid) : Mpi3Element(pid)
{

}

QString& Mpi3Library::filepath()
{
    return mPath;
}

void Mpi3Library::load(const QString &path)
{
    Q_UNUSED(path);
}

void Mpi3Library::save(const QString &path)
{
    if(path != NULL){mPath = path;}

    QFile saveFile(mPath);

    if(saveFile.open(QIODevice::ReadWrite))
    {
        QJsonObject jsonLibrary;
        QJsonObject jsonSongs;

        foreach(Mpi3Song *song, songs.values()){
            QJsonObject jsonSong;

            jsonSong["name"] = song->name;
            jsonSong["artist"] = song->artist;

            jsonSongs[song->pid()] = jsonSong;
        }

        jsonLibrary["songs"] = jsonSongs;

        QJsonDocument jsonDoc;
        jsonDoc.setObject(jsonLibrary);

        QTextStream st(&saveFile);
        st << jsonDoc.toJson(QJsonDocument::Compact);
    }
}

Mpi3Song* Mpi3Library::addSong()
{
    Mpi3Song *song = new Mpi3Song;
    songs[song->pid()] = song;
    return song;
}

Mpi3Playlist* Mpi3Library::addPlaylist()
{
    Mpi3Playlist *playlist = new Mpi3Playlist;
    playlists[playlist->pid()] = playlist;
    return playlist;
}

Mpi3Folder* Mpi3Library::addFolder()
{
    Mpi3Folder *folder = new Mpi3Folder;
    folders[folder->pid()] = folder;
    return folder;
}

void Mpi3Library::removeSong(const QString &pid)
{
    songs.remove(pid);
}

void Mpi3Library::removePlaylist(const QString &pid)
{
    playlists.remove(pid);
}

void Mpi3Library::removeFolder(const QString &pid)
{
    folders.remove(pid);
}


QStringList children(const QString &parent)
{
    Q_UNUSED(parent);
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
