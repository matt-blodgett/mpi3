#pragma once

#ifndef MLIBRARY_H
#define MLIBRARY_H

#include <QObject>
#include <QVector>

QT_BEGIN_NAMESPACE
class QDomDocument;
class QDomElement;
class QDomNode;
QT_END_NAMESPACE


#include "mglobal.h"


class MMediaElement : public QObject
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MMediaElement();
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
    enum MutableProperty {
        SongName,
        SongArtist,
        SongAlbum,
    };

    explicit MSong();
    Mpi3::ElementType type() const override;

public:
    QString artist() const;
    QString album() const;
    QString path() const;
    QString kind() const;

    int time() const;
    int size() const;
    int bitRate() const;
    int sampleRate() const;

private:
    QString m_artist;
    QString m_album;
    QString m_path;
    QString m_kind;

    int m_time = -1;
    int m_size = -1;
    int m_bitRate = -1;
    int m_sampleRate = -1;
};


class MPlaylist : public MMediaElement
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MPlaylist();
    Mpi3::ElementType type() const override;

public:
    MFolder *parent = nullptr;
    QVector<MSong*> songs;

public:
    MSong *getSong(const QString &pid) const;
};


class MFolder : public MMediaElement
{
    Q_OBJECT

friend class MMediaLibrary;

public:
    explicit MFolder();
    Mpi3::ElementType type() const override;

public:
    MFolder *parent = nullptr;
    QVector<MFolder*> folders;
    QVector<MPlaylist*> playlists;

public:
    MPlaylist *getPlaylist(const QString &pid) const;
    MFolder *getFolder(const QString &pid) const;
};


class MMediaLibrary : public MMediaElement
{
    Q_OBJECT
    Q_DISABLE_COPY(MMediaLibrary)

public:    
    explicit MMediaLibrary();
    ~MMediaLibrary() override;
    Mpi3::ElementType type() const override;

    void load(const QString &path = QString());
    void save(const QString &path = QString());

public:
    QString filepath() const;

private:
    QString m_filepath;

private:
    QString generatePID(Mpi3::ElementType elemType) const;
    void xmlWriteElement(QDomDocument &xml, QDomElement &elem, const QString &tagname, const QString &text) const;
    QMap<QString, QVariant> plistDict(const QDomNode &parentNode) const;

public:
    void importItunesPlist(const QString &path, MFolder *parentFolder = nullptr);

    bool validMediaFiles(QUrl mediaUrl) const;
    bool validMediaFiles(QList<QUrl> mediaUrls) const;

    QList<QUrl> songsToPaths(QVector<MSong*> songObjects) const;
    QVector<MSong*> songsFromData(QByteArray pidBytes) const;
    QByteArray songsToData(QVector<MSong*> songObjects) const;

public:
    QVector<MSong*> *libSongs = nullptr;
    QVector<MPlaylist*> *libPlaylists = nullptr;
    QVector<MFolder*> *libFolders = nullptr;

public:
//    QVector<Mpi3Folder*> rootFolders;
//    QVector<Mpi3Playlist*> rootPlaylist;

    QVector<MFolder*> childFolders() const;
    QVector<MPlaylist*> childPlaylists() const;

    QVector<MSong*> allChildSongs(MFolder *parentFolder = nullptr) const;
    QVector<MPlaylist*> allChildPlaylists(MFolder *parentFolder = nullptr) const;
    QVector<MFolder*> allChildFolders(MFolder *parentFolder = nullptr) const;

public:
    Q_REQUIRED_RESULT MMediaElement *getElement(const QString &pid);

    Q_REQUIRED_RESULT MSong *getSong(const QString &pid) const;
    Q_REQUIRED_RESULT MPlaylist *getPlaylist(const QString &pid) const;
    Q_REQUIRED_RESULT MFolder *getFolder(const QString &pid) const;

    Q_REQUIRED_RESULT MSong* newSong(const QString &path = QString()) const;
    Q_REQUIRED_RESULT MPlaylist* newPlaylist(bool named = false) const;
    Q_REQUIRED_RESULT MFolder* newFolder(bool named = false) const;

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
    void elementInserted(MMediaElement *elemInserted, MMediaElement *elemParent);
    void elementRemoved(MMediaElement *elemRemoved, MMediaElement *elemParent);
    void elementMoved(MMediaElement *elemMoved, MMediaElement *elemParent);
    void elementDeleted(const QString &pidDeleted, Mpi3::ElementType elemType, QVector<QString> pidChildren = QVector<QString>());
};


#endif
