#pragma once

#ifndef MMEDIALIBRARY_H
#define MMEDIALIBRARY_H

#include <QObject>
#include <QVector>


#include "mglobal.h"


class MMediaElement : public QObject
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MMediaElement(QObject *parent = nullptr);
    virtual Mpi3::ElementType type() const;

public:
    QString pid() const;
    QString name() const;
    QString added() const;

protected:
    QString m_pid;
    QString m_name;
    QString m_added;
};


class MSong : public MMediaElement
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MSong();
    Mpi3::ElementType type() const override;

    enum MutableProperty {
        SongName,
        SongArtist,
        SongAlbum,
    };

public:
    QString artist() const;
    QString album() const;
    QString kind() const;

    QString path() const;
    QString url() const;

    double time() const;
    double size() const;

    int bitRate() const;
    int sampleRate() const;

    QString majorBrand() const;
    QString minorVersion() const;
    QString compatibleBrands() const;
    QString encoder() const;

private:
    QString m_artist;
    QString m_album;
    QString m_kind;
    QString m_path;

    int m_bitRate = -1;
    double m_size = -1;
    double m_time = 0.0;
    int m_sampleRate = -1;

    QString m_majorBrand;
    QString m_minorVersion;
    QString m_compatibleBrands;
    QString m_encoder;
};


class MMediaContainer : public MMediaElement
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MMediaContainer();
    Mpi3::ElementType type() const override;

    virtual QVector<MSong*> songs() const;
    virtual QVector<MPlaylist*> playlists() const;
    virtual QVector<MFolder*> folders() const;

    MFolder *parentFolder() const;

    QVector<MMediaElement*> childElements(Mpi3::ElementType elemType = Mpi3::BaseElement) const;
    MMediaElement *getElement(const QString &pid, Mpi3::ElementType elemType = Mpi3::BaseElement) const;

    MMediaContainer *getContainer(const QString &pid) const;
    MPlaylist *getPlaylist(const QString &pid) const;
    MFolder *getFolder(const QString &pid) const;
    MSong *getSong(const QString &pid) const;

private:
    MFolder *m_parent = nullptr;
};


class MPlaylist : public MMediaContainer
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MPlaylist();
    Mpi3::ElementType type() const override;
    QVector<MSong*> songs() const override;

private:
    QVector<MSong*> m_songs;

};


class MFolder : public MMediaContainer
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MFolder();
    Mpi3::ElementType type() const override;

    QVector<MSong*> songs() const override;
    QVector<MPlaylist*> playlists() const override;
    QVector<MFolder*> folders() const override;
    QVector<MMediaContainer*> childContainers() const;

private:
    QVector<MPlaylist*> m_playlists;
    QVector<MFolder*> m_folders;
};


class MMediaLibrary : public MMediaContainer
{
    Q_OBJECT
    Q_DISABLE_COPY(MMediaLibrary)

public:    
    explicit MMediaLibrary();
    Mpi3::ElementType type() const override;

    QString filepath() const;
    void load(const QString &path = QString());
    void save(const QString &path = QString());
    void reset();

public:
    QVector<MSong*> songs() const override;
    QVector<MPlaylist*> playlists() const override;
    QVector<MFolder*> folders() const override;

    QVector<MFolder*> rootFolders() const;
    QVector<MPlaylist*> rootPlaylists() const;

    MSong *newSong(const QString &path = QString()) const;
    MPlaylist *newPlaylist(bool named = false) const;
    MFolder *newFolder(bool named = false) const;
    QString generatePID(Mpi3::ElementType elemType) const;

public:
    void importItunesPlist(const QString &path, MFolder *parentFolder = nullptr);

    QVector<MSong*> songsFromBytes(QByteArray pidBytes) const;
    static QByteArray songsToBytes(QVector<MSong*> songObjects);
    static QList<QUrl> songsToPaths(QVector<MSong*> songObjects);

    static bool validMediaFiles(QUrl mediaUrl);
    static bool validMediaFiles(QList<QUrl> mediaUrls);

private:
    QString m_filepath;
    QVector<MSong*> m_libSongs;
    QVector<MPlaylist*> m_libPlaylists;
    QVector<MFolder*> m_libFolders;

public:
    void modify(const QString &pid, const QString &value);
    void modify(MSong *song, const QString &value, MSong::MutableProperty songProperty);

    void insert(MSong *inSong, MPlaylist *toPlaylist = nullptr, int atPosition = -1);
    void insert(MPlaylist *inPlaylist, MFolder *toFolder = nullptr, int atPosition = -1);
    void insert(MFolder *inFolder, MFolder *toFolder = nullptr, int atPosition = -1);

    void remove(MSong *remSong, MPlaylist *fromPlaylist);
    void remove(MPlaylist *remPlaylist, MFolder *fromFolder);
    void remove(MFolder *remFolder, MFolder *fromFolder);

    void move(MSong *moveSong, MPlaylist *parentPlaylist, int toPosition = -1);
    void move(MPlaylist *movePlaylist, MFolder *toFolder, int toPosition = -1);
    void move(MFolder *moveFolder, MFolder *toFolder, int toPosition = -1);

    void discard(MSong *remSong);
    void discard(MPlaylist *remPlaylist);
    void discard(MFolder *remFolder);

signals:
    void elementModified(MMediaElement *elemModified);
    void elementInserted(MMediaElement *elemInserted, MMediaContainer *elemParent);
    void elementRemoved(MMediaElement *elemRemoved, MMediaContainer *elemParent);
    void elementMoved(MMediaElement *elemMoved, MMediaContainer *elemParent);
    void elementDeleted(MMediaElement *elemDeleted);
};

#endif
