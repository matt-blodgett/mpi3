#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <QObject>
#include <QMap>

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


// ----------------------------------------------------------------------------------------------------
// * Mpi3Element *
// ----------------------------------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------------------------------
// * Mpi3Song *
// ----------------------------------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------------------------------
// * Mpi3Playlist *
// ----------------------------------------------------------------------------------------------------
class Mpi3Playlist : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Playlist(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
//    QMap<QString> songs;

};


// ----------------------------------------------------------------------------------------------------
// * Mpi3Folder *
// ----------------------------------------------------------------------------------------------------
class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Folder(const bool &newpid = false);

public:
    Mpi3Folder *parent = nullptr;
//    <QString> playlists;

};


// ----------------------------------------------------------------------------------------------------
// * Mpi3Library *
// ----------------------------------------------------------------------------------------------------
class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:
    explicit Mpi3Library(const bool &newpid = false);
    static Mpi3Library* load(const QString &path);
    void save(const QString &path);

public:
    QString filepath;

    QMap<QString, Mpi3Song*> *songs = nullptr;
    QMap<QString, Mpi3Playlist*> *playlists = nullptr;
    QMap<QString, Mpi3Folder*> *folders = nullptr;

public:
    Mpi3Song* addSong();
    Mpi3Playlist* addPlaylist(Mpi3Folder *parent = nullptr);
    Mpi3Folder* addFolder(Mpi3Folder *parent = nullptr);

    void removeSong(const QString &pid);
    void removePlaylist(const QString &pid);
    void removeFolder(const QString &pid);

    QStringList children(const QString &parent);

    void update();

public:
    int mediaCount() const;

signals:
    void mediaInserted(int position, int rows);
    void mediaChanged(int index, Mpi3Song *s);

    void mediaRemoved();

};


#endif // MEDIALIB_H




















