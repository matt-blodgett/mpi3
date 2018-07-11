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

};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Folder(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Playlist*> playlists;
    QVector<Mpi3Folder*> folders;

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
    QList<Mpi3Folder*> childFolders(Mpi3Folder *parent);
    QList<Mpi3Playlist*> childPlaylists(Mpi3Folder *parent);

    Mpi3Song *getSong(const QString &pid);
    Mpi3Playlist *getPlaylist(const QString &pid);
    Mpi3Folder *getFolder(const QString &pid);

//public:
//    Mpi3Song* newSong();
//    Mpi3Playlist* newPlaylist(Mpi3Folder *parent = nullptr);
//    Mpi3Folder* newFolder(Mpi3Folder *parent = nullptr);

//    void update(Mpi3Song *song);
//    void update(Mpi3Playlist *playlist);
//    void update(Mpi3Folder *folder);

//    void insert(Mpi3Song *inSong, Mpi3Playlist *toPlaylist, int position);
//    void insert(Mpi3Playlist *inPlaylist, Mpi3Folder *toFolder, int position);
//    void insert(Mpi3Folder *inFolder, Mpi3Folder *toFolder, int position);

//    void move(Mpi3Song *moveSong, Mpi3Playlist *inPlaylist, int position);
//    void move(Mpi3Playlist *movePlaylist, Mpi3Folder *inFolder, int position);
//    void move(Mpi3Folder *moveFolder, Mpi3Folder *inFolder, int position);

//    void remove(Mpi3Song *remSong, Mpi3Playlist *fromPlaylist);
//    void remove(Mpi3Playlist *remPlaylist, Mpi3Folder *fromFolder);
//    void remove(Mpi3Folder *remFolder, Mpi3Folder *fromFolder);

//    void remove(Mpi3Song *remSong);
//    void remove(Mpi3Playlist *remPlaylist);
//    void remove(Mpi3Folder *remFolder);


//signals:
//    void songUpdated(Mpi3Song *song);
//    void playlistUpdated(Mpi3Playlist *playlist);
//    void folderUpdated(Mpi3Folder *folder);

//    void songInserted(Mpi3Song *song, Mpi3Playlist *parent, int position);
//    void songMoved(int position, int destination);
//    void songRemoved(int position);

//    void playlistInserted(Mpi3Playlist *playlist);
//    void playlistMoved();
//    void playlistRemoved();

//    void folderInserted(Mpi3Folder *folder);
//    void folderMoved();
//    void folderRemoved();

};


#endif // MEDIALIB_H
