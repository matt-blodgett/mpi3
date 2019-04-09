#pragma once

#ifndef MMEDIALIBRARY_H
#define MMEDIALIBRARY_H


#include <QObject>
#include <QVector>


#include "mglobal.h"
#ifdef MPI3_BUILD_SHARED
class MPI3_EXPORT_CORE MMediaLibrary;
class MPI3_EXPORT_CORE MMediaElement;
class MPI3_EXPORT_CORE MContainer;
class MPI3_EXPORT_CORE MPlaylist;
class MPI3_EXPORT_CORE MFolder;
class MPI3_EXPORT_CORE MSong;
#endif


class MMediaElement : public QObject
{
    Q_GADGET
    Q_PROPERTY(Mpi3::ElementType type READ type)
    Q_PROPERTY(QString pid READ pid)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString added READ added)

protected:
    explicit MMediaElement(QObject *parent = nullptr);

public:
    virtual Mpi3::ElementType type() const;
    friend class MMediaLibrary;

public:
    QString pid() const;
    QString name() const;
    QString added() const;

    void setName(const QString &value);

private:
    QString m_pid;
    QString m_name;
    QString m_added;

protected:
    void notifyPropertyChanged(
        const QString &propertyName,
        const QVariant &oldPropertyValue);
};


class MSong : public MMediaElement
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist WRITE setArtist)
    Q_PROPERTY(QString album READ album WRITE setAlbum)
    Q_PROPERTY(QString kind READ kind)
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(double time READ time)
    Q_PROPERTY(double size READ size)
    Q_PROPERTY(QString timeString READ timeString)
    Q_PROPERTY(QString sizeString READ sizeString)
    Q_PROPERTY(int bitRate READ bitRate)
    Q_PROPERTY(int sampleRate READ sampleRate)

private:
    explicit MSong(MMediaLibrary *parent);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    QString artist() const;
    QString album() const;
    QString kind() const;
    QString path() const;

    double time() const;
    double size() const;

    QString timeString() const;
    QString sizeString() const;

    int bitRate() const;
    int sampleRate() const;

    void setArtist(const QString &value);
    void setAlbum(const QString &value);

private:
    QString m_artist;
    QString m_album;
    QString m_kind;
    QString m_path;

    double m_time = 0.0;
    double m_size = 0.0;

    int m_bitRate = 0;
    int m_sampleRate = 0;
};


class MContainer : public MMediaElement
{
    Q_GADGET

protected:
    explicit MContainer(MMediaLibrary *parent);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MFolder *parentFolder() const;
    void setParentFolder(MFolder *folder);

private:
    MFolder *m_parentFolder = nullptr;
};


class MPlaylist : public MContainer
{
    Q_OBJECT

private:
    explicit MPlaylist(MMediaLibrary *parent);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MSongList songs() const;

    void insert(int index, MSong *song);
    void insert(int index, const MSongList &songlist);

    void append(MSong *song);
    void append(const MSongList &songlist);

    void prepend(MSong *song);
    void prepend(const MSongList &songlist);

    void move(int from, int to);
    void move(QList<int> indexes, int to);

    void remove(int index);
    void remove(QList<int> indexes);

    void clear();

private:
    MSongList m_songs;
    void notifyContentsChanged();
};


class MFolder : public MContainer
{
    Q_OBJECT

private:
    explicit MFolder(MMediaLibrary *parent);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MSongList childSongs(bool recursive = false) const;
    MFolderList childFolders(bool recursive = false) const;
    MPlaylistList childPlaylists(bool recursive = false) const;
    MContainerList childContainers(bool recursive = false) const;
};


class MMediaLibrary : public MMediaElement
{
    Q_OBJECT
    Q_PROPERTY(QString savePath READ savePath)
    Q_PROPERTY(QString mediaPath READ mediaPath WRITE setMediaPath)
    Q_PROPERTY(QString backupPath READ backupPath WRITE setBackupPath)
    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath)

private:
    Q_DISABLE_COPY(MMediaLibrary)

public:    
    explicit MMediaLibrary(QObject *parent = nullptr);
    Mpi3::ElementType type() const override;

public:
    MSongList songs() const;
    MFolderList folders() const;
    MPlaylistList playlists() const;
    MContainerList containers() const;
    MElementList elements(Mpi3::ElementType filterType = Mpi3::BaseElement) const;

    template<typename I, class E>
    static I rootSearch(I iterable){
        I ret;
        for(E *e : iterable){
            if(!e->parentFolder()){
                ret.append(e);
            }
        }
        return ret;
    }

    MFolderList rootFolders() const;
    MPlaylistList rootPlaylists() const;
    MContainerList rootContainers() const;

    template<typename I, class E>
    static E *pidSearch(I iterable, const QString &pid){
        for(E *element : iterable){
            if(element->pid() == pid){
                return element;
            }
        }
        return nullptr;
    }

    MSong *getSong(const QString &pid) const;
    MFolder *getFolder(const QString &pid) const;
    MPlaylist *getPlaylist(const QString &pid) const;
    MContainer *getContainer(const QString &pid) const;
    MMediaElement *getElement(const QString &pid) const;

private:
    MSongList m_songs;
    MFolderList m_folders;
    MPlaylistList m_playlists;

public:
    QString savePath() const;
    QString mediaPath() const;
    QString backupPath() const;
    QString downloadPath() const;

    void setMediaPath(const QString &dirPath);
    void setBackupPath(const QString &dirPath);
    void setDownloadPath(const QString &dirPath);

private:
    QString m_savePath;
    QString m_mediaPath;
    QString m_backupPath;
    QString m_downloadPath;

public:
    bool load(const QString &filePath);
    bool save(const QString &filePath = QString());
    void reset();

public:
    bool importItunesPlist(const QString &filePath, MFolder *parentFolder = nullptr);

    MSongList songsFromBytes(QByteArray pidBytes) const;
    static QByteArray songsToBytes(MSongList songlist);
    static QList<QUrl> songsToPaths(MSongList songlist);

    // move below functions to utility module
    static bool validMediaFiles(QUrl mediaUrl);
    static bool validMediaFiles(QList<QUrl> mediaUrls);

    static QString timeToString(double time);
    static QString sizeToString(double size, int prec = 2);
    static QString percentToString(double percent, int prec = 2);
    // -----------------------------------------

public:
    MMediaLibrary *createRaspiVolume(const QString &rootPath);
    static MMediaLibrary *loadRaspiVolume(const QString &rootPath);
    static bool detectRaspiVolume(const QString &rootPath);

private:
    QString generatePID(Mpi3::ElementType elementType) const;

public:
    MSong *newSong(const QString &filePath);
    MFolder *newFolder(MFolder *parentFolder = nullptr, const QString &name = QString());
    MPlaylist *newPlaylist(MFolder *parentFolder = nullptr, const QString &name = QString());

    void remove(MSong *childSong);
    void remove(MFolder *childFolder);
    void remove(MPlaylist *childPlaylist);
    void remove(MContainer *childContainer);
    void remove(const QString &pid);

signals:
    void aboutToLoad();
    void aboutToSave();
    void aboutToReset();

    void libraryLoaded();
    void librarySaved();
    void libraryReset();

    void songCreated(MSong *childSong);
    void folderCreated(MFolder *childFolder);
    void playlistCreated(MPlaylist *childPlaylist);

    void songDeleted(MSong *childSong);
    void folderDeleted(MFolder *childFolder);
    void playlistDeleted(MPlaylist *childPlaylist);

    void songChanged(MSong *childSong);
    void folderChanged(MFolder *childFolder);
    void playlistChanged(MPlaylist *childPlaylist);
    void libraryChanged(MMediaLibrary *library);

    void playlistContentsChanged(MPlaylist *childPlaylist);
    void parentFolderChanged(MContainer *childContainer);

};


#endif
