#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <QString>
#include <QMap>


QString generatePID();


class Mpi3Element
{

public:
    Mpi3Element(const QString &pid = NULL);

public:
    QString pid();
    QString name = NULL;
    QString added = NULL;

private:
    QString mPID = NULL;

};


class Mpi3Song : public Mpi3Element
{

public:
    explicit Mpi3Song(const QString &pid = NULL);

public:

    QString artist;
//    QString album;
//    QString time;
    QString path;
//    QString kind;

//    int size;
//    int bitRate;
//    int sampleRate;

};


class Mpi3Playlist : public Mpi3Element
{

public:
    explicit Mpi3Playlist(const QString &pid = NULL);

//public:
//    QString folder;
//    QMap<QString> songs;

};


class Mpi3Folder : public Mpi3Element
{

public:
    explicit Mpi3Folder(const QString &pid = NULL);

//public:
//    QString folder;
//    <QString> playlists;

};


class Mpi3Library : public Mpi3Element
{

public:
    explicit Mpi3Library(const QString &pid = NULL);

public:
    QString& filepath();
    QMap<QString, Mpi3Song *> songs;
    QMap<QString, Mpi3Playlist *> playlists;
    QMap<QString, Mpi3Folder *> folders;

public:
    void open(const QString &path = NULL);
    void load(const QString &path = NULL);
    void save(const QString &path = NULL);

    Mpi3Song* addSong();
    Mpi3Playlist* addPlaylist();
    Mpi3Folder* addFolder();

    void removeSong(const QString &pid);
    void removePlaylist(const QString &pid);
    void removeFolder(const QString &pid);

    QStringList children(const QString &parent = NULL);

private:
    QString mPath;

};


#endif // MEDIALIB_H
