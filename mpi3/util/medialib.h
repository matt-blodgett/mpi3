#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <QObject>
#include <QVector>
#include <QList>

QT_BEGIN_NAMESPACE
class QDomDocument;
class QDomElement;
QT_END_NAMESPACE

class Mpi3Element;
class Mpi3Song;
class Mpi3Playlist;
class Mpi3Folder;
class Mpi3Library;


QString generatePID();
void xmlWriteElement(QDomDocument xml, QDomElement elem, QString tagname, QString text);


class Mpi3Element : public QObject
{
    Q_OBJECT

public:
    Mpi3Element(const bool &newpid = false);

public:
    QString pid;
    QString name;
    QString added;

};


class Mpi3Song : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Song(const bool &newpid = false);

public:
    QString artist;
    QString album;
    QString time;
    QString path;
    QString kind;

    int size = 0;
    int bitRate = 0;
    int sampleRate = 0;

};


class Mpi3Playlist : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Playlist(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Song*> songs;

public:
    void appendSong(Mpi3Song *song);
    void removeSong(Mpi3Song *song);
    void insertSong(Mpi3Song *song, int position);
    void moveSong(Mpi3Song *song, int position);

};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Folder(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Playlist*> playlists;

public:
    void appendPlaylist(Mpi3Playlist *playlist);
    void removePlaylist(Mpi3Playlist *playlist);
    void insertPlaylist(Mpi3Playlist *playlist, int position);
    void movePlaylist(Mpi3Playlist *playlist, int position);

    void appendFolder(Mpi3Folder *folder);
    void removeFolder(Mpi3Folder *folder);
    void insertFolder(Mpi3Folder *folder, int position);
    void moveFolder(Mpi3Folder *folder, int position);

};


class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Library(const bool &newpid = false);
    static Mpi3Library* load(const QString &path);
    ~Mpi3Library();

public:
    QString filepath;
    void save(const QString &path);

    QVector<Mpi3Song*> *libSongs = nullptr;
    QVector<Mpi3Playlist*> *libPlaylists = nullptr;
    QVector<Mpi3Folder*> *libFolders = nullptr;

public:
    Mpi3Song* newSong();
    Mpi3Playlist* newPlaylist(Mpi3Folder *parent = nullptr);
    Mpi3Folder* newFolder(Mpi3Folder *parent = nullptr);

    void deleteSong(Mpi3Song *s);
    void deletePlaylist(Mpi3Playlist *p);
    void deleteFolder(Mpi3Folder *f);

    QList<Mpi3Folder*> childFolders(Mpi3Folder *parent);
    QList<Mpi3Playlist*> childPlaylists(Mpi3Folder *parent);

    Mpi3Song *getSong(const QString &pid);
    Mpi3Playlist *getPlaylist(const QString &pid);
    Mpi3Folder *getFolder(const QString &pid);

    void update(Mpi3Song *song);


//signals:
//    void mediaInserted(int position, int rows);
//    void mediaChanged(int index, Mpi3Song *s);

//    void mediaRemoved();


//    void songDeleted();
//    void playlistDeleted();
//    void folderDeleted();

//    void songAdded();
//    void playlistAdded();
//    void folderAdded();

//    void songInsertedInto();
//    void playlistInsertedInto();
//    void folderInsertedInto();

//    void songRemovedFrom();
//    void playlistRemovedFrom();
//    void folderRemovedFrom();


};


#endif // MEDIALIB_H
