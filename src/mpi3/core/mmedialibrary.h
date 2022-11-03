#pragma once

#ifndef MMEDIALIBRARY_H
#define MMEDIALIBRARY_H


#include <QObject>
#include <QVector>


#include "mpi3/core/mglobal.h"
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
    Q_PROPERTY(QString name READ name)
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

private:
    QString m_pid;
    QString m_name;
    QString m_added;
};


class MChildElement : public MMediaElement
{
    Q_OBJECT

protected:
    explicit MChildElement(MMediaLibrary *parentLibrary);
    MMediaLibrary *m_parentLibrary = nullptr;
};


class MSong : public MChildElement
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(QString kind READ kind)
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(double time READ time)
    Q_PROPERTY(double size READ size)
    Q_PROPERTY(int bitRate READ bitRate)
    Q_PROPERTY(int sampleRate READ sampleRate)

private:
    explicit MSong(MMediaLibrary *parentLibrary);

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

    int bitRate() const;
    int sampleRate() const;

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


class MContainer : public MChildElement
{
    Q_GADGET

protected:
    explicit MContainer(MMediaLibrary *parentLibrary);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MFolder *parentFolder() const;

private:
    MFolder *m_parentFolder = nullptr;
};


class MPlaylist : public MContainer
{
    Q_OBJECT

private:
    explicit MPlaylist(MMediaLibrary *parentLibrary);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MSongList songs() const;
    QStringList songsPidList() const;

private:
    QStringList m_songsPidList;
};


class MFolder : public MContainer
{
    Q_OBJECT

private:
    explicit MFolder(MMediaLibrary *parentLibrary);

public:
    Mpi3::ElementType type() const override;
    friend class MMediaLibrary;

public:
    MFolderList childFolders() const;
    MPlaylistList childPlaylists() const;
    MContainerList childContainers() const;
};


class MMediaLibrary : public MMediaElement
{
    Q_OBJECT
    Q_DISABLE_COPY(MMediaLibrary)
    Q_PROPERTY(QString savePath READ savePath)
    Q_PROPERTY(QString localMediaPath READ localMediaPath WRITE setLocalMediaPath)

public:    
    explicit MMediaLibrary(QObject *parent = nullptr);
    Mpi3::ElementType type() const override;

public:
    bool load(const QString &filePath);
    bool save(const QString &filePath = QString());
    void reset();

public:
    QString savePath() const;
    QString localMediaPath() const;

    void setLocalMediaPath(const QString &path);

private:
    QString m_savePath;
    QString m_localMediaPath;

public:
    MSongList songs() const;
    MFolderList folders() const;
    MPlaylistList playlists() const;
    MContainerList containers() const;

    template<typename I, class E>
    static I rootSearch(I iterable) {
        I ret;
        for(E *e : iterable) {
            if(!e->parentFolder()) {
                ret.append(e);
            }
        }
        return ret;
    }

    MFolderList rootFolders() const;
    MPlaylistList rootPlaylists() const;
    MContainerList rootContainers() const;

    template<typename I, class E>
    static E *pidSearch(I iterable, const QString &pid) {
        for(E *element : iterable) {
            if(element->pid() == pid) {
                return element;
            }
        }
        return nullptr;
    }

    MSong *getSong(const QString &pid) const;
    MFolder *getFolder(const QString &pid) const;
    MPlaylist *getPlaylist(const QString &pid) const;
    MContainer *getContainer(const QString &pid) const;

    template<typename I, class E>
    static I pidSearchList(I iterable, const QStringList &pids) {
        I ret;
        for(const QString &pid : pids) {
            E *element = MMediaLibrary::pidSearch<I, E>(iterable, pid);
            ret.append(element);
        }
        return ret;
    }

    MSongList getSongList(const QStringList &pidList) const;
    MFolderList getFolderList(const QStringList &pidList) const;
    MPlaylistList getPlaylistList(const QStringList &pidList) const;
    MContainerList getContainerList(const QStringList &pidList) const;

private:
    static QString generatePID(Mpi3::ElementType elementType);

public:
    MSong *newSong(const QUrl &url);
    MFolder *newFolder(MFolder *parentFolder = nullptr, const QString &name = QString());
    MPlaylist *newPlaylist(MFolder *parentFolder = nullptr, const QString &name = QString());

    bool edit(MSong *song, const QString &property, const QVariant &value);
    bool edit(MFolder *folder, const QString &property, const QVariant &value);
    bool edit(MPlaylist *playlist, const QString &property, const QVariant &value);
    bool edit(MContainer *container, const QString &property, const QVariant &value);

    bool remove(MSong *song);
    bool remove(MFolder *folder);
    bool remove(MPlaylist *playlist);
    bool remove(MContainer *container);
    bool remove(const QString &pid);

private:
    MSongList m_songs;
    MFolderList m_folders;
    MPlaylistList m_playlists;

signals:
    void libraryLoading();
    void librarySaving();
    void libraryResetting();

    void libraryLoaded();
    void librarySaved();
    void libraryReset();

    void songCreated(MSong *song);
    void folderCreated(MFolder *folder);
    void playlistCreated(MPlaylist *playlist);

    void songChanged(MSong *song);
    void folderChanged(MFolder *folder);
    void playlistChanged(MPlaylist *playlist);
    void libraryChanged(MMediaLibrary *library);

    void songDeleted(MSong *song);
    void folderDeleted(MFolder *folder);
    void playlistDeleted(MPlaylist *playlist);

    void playlistSongsChanged(MPlaylist *playlist);
    void parentFolderChanged(MContainer *container);
};


#endif
