#ifndef MPI3LIBRARY_H
#define MPI3LIBRARY_H

#include <QObject>
#include <QVector>

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

friend class Mpi3Library;

public:
    enum ElementType {
        BaseElement,
        SongElement,
        PlaylistElement,
        FolderElement,
        LibraryElement
    };

    explicit Mpi3Element();
    virtual Mpi3Element::ElementType type() const;

private:
    static const int PersistentIDLength;
    static const QString BasePrefix;
    static const QString SongPrefix;
    static const QString PlaylistPrefix;
    static const QString FolderPrefix;
    static const QString LibraryPrefix;

public:
    QString pid() const;
    QString name() const;
    QString added() const;

protected:
    QString m_pid;
    QString m_name;
    QString m_added;
};


class Mpi3Song : public Mpi3Element
{
    Q_OBJECT

friend class Mpi3Library;

public:
    enum MutableProperty {
        SongName,
        SongArtist,
        SongAlbum,
    };

    explicit Mpi3Song();
    Mpi3Element::ElementType type() const override;

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

    int m_size = -1;
    int m_bitRate = -1;
    int m_sampleRate = -1;
};


class Mpi3Playlist : public Mpi3Element
{
    Q_OBJECT

friend class Mpi3Library;

public:
    explicit Mpi3Playlist();
    Mpi3Element::ElementType type() const override;

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Song*> songs;

public:
    Mpi3Song *getSong(const QString &pid) const;
};


class Mpi3Folder : public Mpi3Element
{
    Q_OBJECT

friend class Mpi3Library;

public:
    explicit Mpi3Folder();
    Mpi3Element::ElementType type() const override;

public:
    Mpi3Folder *parent = nullptr;
    QVector<Mpi3Folder*> folders;
    QVector<Mpi3Playlist*> playlists;

public:
    Mpi3Playlist *getPlaylist(const QString &pid) const;
    Mpi3Folder *getFolder(const QString &pid) const;
};


class Mpi3Library : public Mpi3Element
{
    Q_OBJECT

public:    
    explicit Mpi3Library();
    ~Mpi3Library() override;
    Mpi3Element::ElementType type() const override;

    void load(const QString &path = QString());
    void save(const QString &path = QString());

private:
    QString m_filepath;

public:
    QString filepath() const;
    QVector<Mpi3Song*> *libSongs = nullptr;
    QVector<Mpi3Playlist*> *libPlaylists = nullptr;
    QVector<Mpi3Folder*> *libFolders = nullptr;

private:
    QString generatePID(Mpi3Element::ElementType elemType) const;
    void xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text) const;
    QMap<QString, QVariant> plistDict(const QDomNode &parentNode) const;

public:
    void importItunesPlist(const QString &path, Mpi3Folder *parentFolder = nullptr);

    bool validMediaFiles(QUrl mediaUrl);
    bool validMediaFiles(QList<QUrl> mediaUrls);

    QVector<Mpi3Song*> songsFromData(QByteArray pidBytes) const;

public:
    QVector<Mpi3Folder*> childFolders(Mpi3Folder *parentFolder = nullptr) const;
    QVector<Mpi3Playlist*> childPlaylists(Mpi3Folder *parentFolder = nullptr) const;

    QVector<Mpi3Song*> allChildSongs(Mpi3Folder *parentFolder = nullptr) const;
    QVector<Mpi3Playlist*> allChildPlaylists(Mpi3Folder *parentFolder = nullptr) const;
    QVector<Mpi3Folder*> allChildFolders(Mpi3Folder *parentFolder = nullptr) const;

public:
    Mpi3Element *getElement(const QString &pid) const;
    Mpi3Song *getSong(const QString &pid) const;
    Mpi3Playlist *getPlaylist(const QString &pid) const;
    Mpi3Folder *getFolder(const QString &pid) const;

    Mpi3Song* newSong(const QString &path = QString()) const;
    Mpi3Playlist* newPlaylist(bool named = false) const;
    Mpi3Folder* newFolder(bool named = false) const;

public:
    void modify(const QString &pid, const QString &value);
    void modify(Mpi3Song *song, const QString &value, Mpi3Song::MutableProperty songProperty);

    void insert(Mpi3Song *inSong, Mpi3Playlist *toPlaylist = nullptr, int position = -1);
    void insert(Mpi3Playlist *inPlaylist, Mpi3Folder *toFolder = nullptr, int position = -1);
    void insert(Mpi3Folder *inFolder, Mpi3Folder *toFolder = nullptr, int position = -1);

    void remove(Mpi3Song *remSong, Mpi3Playlist *fromPlaylist);
    void remove(Mpi3Playlist *remPlaylist, Mpi3Folder *fromFolder);
    void remove(Mpi3Folder *remFolder, Mpi3Folder *fromFolder);

    void move(Mpi3Song *moveSong, Mpi3Playlist *inPlaylist, int position);
    void move(Mpi3Playlist *movePlaylist, Mpi3Folder *toFolder, int position);
    void move(Mpi3Folder *moveFolder, Mpi3Folder *toFolder, int position);

    void discard(Mpi3Song *remSong);
    void discard(Mpi3Playlist *remPlaylist);
    void discard(Mpi3Folder *remFolder);

signals:
    void elementModified(Mpi3Element *elemModified);
    void elementInserted(Mpi3Element *elemInserted, Mpi3Element *elemParent);
    void elementRemoved(Mpi3Element *elemRemoved, Mpi3Element *elemParent);
    void elementDeleted(const QString &pidDeleted, Mpi3Element::ElementType elemType, QVector<QString> pidChildren = QVector<QString>());
};


#endif
