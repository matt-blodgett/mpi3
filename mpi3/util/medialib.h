#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <QObject>
#include <QVector>

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
//    QMap<QString> songs;

};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Folder(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
//    <QString> playlists;

};


class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Library(const bool &newpid = false);
    static Mpi3Library* load(const QString &path);
    ~Mpi3Library();

    void save(const QString &path);

public:
    QString filepath;

    QVector<Mpi3Song*> *songs = nullptr;
    QVector<Mpi3Playlist*> *playlists = nullptr;
    QVector<Mpi3Folder*> *folders = nullptr;

public:
    Mpi3Song* addSong();
    Mpi3Playlist* addPlaylist(Mpi3Folder *parent = nullptr);
    Mpi3Folder* addFolder(Mpi3Folder *parent = nullptr);

    void removeSong(Mpi3Song *s);
    void removePlaylist(Mpi3Playlist *p);
    void removeFolder(Mpi3Folder *f);

    void update(Mpi3Song *song);

signals:
    void mediaInserted(int position, int rows);
    void mediaChanged(int index, Mpi3Song *s);

    void mediaRemoved();


    void songDeleted();
    void playlistDeleted();
    void folderDeleted();

    void songAdded();
    void playlistAdded();
    void folderAdded();

    void songInsertedInto();
    void playlistInsertedInto();
    void folderInsertedInto();

    void songRemovedFrom();
    void playlistRemovedFrom();
    void folderRemovedFrom();


};


#endif // MEDIALIB_H




















