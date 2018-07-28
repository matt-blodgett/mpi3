#ifndef MPI3LIBRARY_H
#define MPI3LIBRARY_H

#include <QList>

QT_BEGIN_NAMESPACE
class QDomDocument;
class QDomElement;
class QDomNode;
QT_END_NAMESPACE

class Mpi3Element;
class Mpi3Library;
class Mpi3Folder;
class Mpi3Playlist;
class Mpi3Song;


class Mpi3Element : public QObject
{
    Q_OBJECT

public:
    explicit Mpi3Element();

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

friend class Mpi3Library;

public:
    explicit Mpi3Song();

    enum MutableProperty{
        SongName,
        SongArtist,
        SongAlbum,
    };

public:
    QString artist() const;
    QString album() const;
    QString time() const;
    QString path() const;
    QString kind() const;

    int size() const;
    int bitRate() const;
    int sampleRate() const;

private:
    QString m_artist;
    QString m_album;
    QString m_time;
    QString m_path;
    QString m_kind;

    int m_size = 0;
    int m_bitRate = 0;
    int m_sampleRate = 0;
};


class Mpi3Playlist : public Mpi3Element
{
    Q_OBJECT

friend class Mpi3Library;

public:
    explicit Mpi3Playlist();

public:
    Mpi3Folder *parent = nullptr;
    QList<Mpi3Song*> songs;

public:
    Mpi3Song *getSong(const QString &pid);
};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

friend class Mpi3Library;

public:
    explicit Mpi3Folder();

public:
    Mpi3Folder *parent = nullptr;
    QList<Mpi3Folder*> folders;
    QList<Mpi3Playlist*> playlists;
};


class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:    
    explicit Mpi3Library();
    ~Mpi3Library();

    void load(const QString &path = QString());
    void save(const QString &path = QString());

private:
    QString m_filepath;

public:
    QList<Mpi3Song*> *libSongs = nullptr;
    QList<Mpi3Playlist*> *libPlaylists = nullptr;
    QList<Mpi3Folder*> *libFolders = nullptr;
    QString filepath() const;

private:
    QString generatePID();
    void xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text);
    QMap<QString, QVariant> plistDict(const QDomNode &parentNode);

public:
    void importItunesPlist(const QString &path);

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
    void modify(const QString &pid, const QString &value);
    void modify(Mpi3Song *song, const QString &value, Mpi3Song::MutableProperty songProperty);

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


#endif
