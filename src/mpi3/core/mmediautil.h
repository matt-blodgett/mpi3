#pragma once

#ifndef MMEDIAUTIL_H
#define MMEDIAUTIL_H


#include "mglobal.h"


namespace Mpi3
{

    namespace Core
    {

    #ifdef MPI3_BUILD_SHARED
        MPI3_EXPORT_CORE Q_NAMESPACE

        MPI3_EXPORT_CORE QByteArray songsToBytes(MSongList songlist);
        MPI3_EXPORT_CORE QList<QUrl> songsToPaths(MSongList songlist);

        MPI3_EXPORT_CORE bool validMediaFiles(QUrl mediaUrl);
        MPI3_EXPORT_CORE bool validMediaFiles(QList<QUrl> mediaUrls);

        MPI3_EXPORT_CORE QStringList bytesToSongs(QByteArray pidBytes);

    #else
        Q_NAMESPACE

        QByteArray songsToBytes(MSongList songlist);
        QList<QUrl> songsToPaths(MSongList songlist);

        bool validMediaFiles(QUrl mediaUrl);
        bool validMediaFiles(QList<QUrl> mediaUrls);

        QStringList bytesToSongs(QByteArray pidBytes);

    #endif

    };

};



#endif
