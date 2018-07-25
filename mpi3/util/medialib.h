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
class Mpi3Library;
class Mpi3Folder;
class Mpi3Playlist;
class Mpi3Song;


class Mpi3Element : public QObject
{
    Q_OBJECT

friend class Mpi3Library;

public:
    Mpi3Element();

public:
    QString pid();
    QString name();
    QString added();

private:
    QString m_pid;
    QString m_name;
    QString m_added;

};


class Mpi3Song : public Mpi3Element
{
    Q_OBJECT

public:
    enum Property{
        Name,
        Added
    };

    explicit Mpi3Song();

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
    enum Property{
        Name,
        Added,
    };

    explicit Mpi3Playlist();

public:
    Mpi3Song *getSong(const QString &pid);
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Song*> songs;

};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

public:
    enum Property{
        Name,
        Added,
    };

    explicit Mpi3Folder();

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Folder*> folders;
    QVector<Mpi3Playlist*> playlists;

};


class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:    
    enum Property{
        Name,
        Added,
    };

    explicit Mpi3Library();
    ~Mpi3Library();

    void load(const QString &path);
    void save(const QString &path);

private:
    QString generatePID();
    void xmlWriteElement(QDomDocument xml, QDomElement elem, QString tagname, QString text);

public:
    QString filepath;
    QVector<Mpi3Song*> *libSongs = nullptr;
    QVector<Mpi3Playlist*> *libPlaylists = nullptr;
    QVector<Mpi3Folder*> *libFolders = nullptr;

public:
    QList<Mpi3Folder*> childFolders(Mpi3Folder *parent = nullptr);
    QList<Mpi3Playlist*> childPlaylists(Mpi3Folder *parent = nullptr);

    Mpi3Song *getSong(const QString &pid);
    Mpi3Playlist *getPlaylist(const QString &pid);
    Mpi3Folder *getFolder(const QString &pid);

public:
    Mpi3Song* newSong();
    Mpi3Playlist* newPlaylist(bool named = false);
    Mpi3Folder* newFolder(bool named = false);

public:
    void modify(Mpi3Library::Property property, const QString &value);
    void modify(Mpi3Song *song, Mpi3Song::Property property, const QString &value);
    void modify(Mpi3Playlist *playlist, Mpi3Playlist::Property property, const QString &value);
    void modify(Mpi3Folder *folder, Mpi3Folder::Property property, const QString &value);

    void insert(Mpi3Song *inSong, Mpi3Playlist *toPlaylist = nullptr, int position = -1);
    void insert(Mpi3Playlist *inPlaylist, Mpi3Folder *toFolder = nullptr, int position = -1);
    void insert(Mpi3Folder *inFolder, Mpi3Folder *toFolder = nullptr, int position = -1);

    void remove(Mpi3Song *remSong, Mpi3Playlist *fromPlaylist);
    void remove(Mpi3Playlist *remPlaylist, Mpi3Folder *fromFolder);
    void remove(Mpi3Folder *remFolder, Mpi3Folder *fromFolder);

    void remove(Mpi3Song *remSong);
    void remove(Mpi3Playlist *remPlaylist);
    void remove(Mpi3Folder *remFolder);

    void move(Mpi3Song *moveSong, Mpi3Playlist *inPlaylist, int position);
    void move(Mpi3Playlist *movePlaylist, Mpi3Folder *inFolder, int position);
    void move(Mpi3Folder *moveFolder, Mpi3Folder *inFolder, int position);

signals:
    void elementModified(const QString &pidModified);
    void elementInserted(const QString &pidInserted, const QString &pidParent);
    void elementRemoved(const QString &pidRemoved, const QString &pidParent);
};


#endif // MEDIALIB_H
