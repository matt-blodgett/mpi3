#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H


#include <QMetaType>
typedef QColor QColour; // Canadian (Sorry)


#define MPI3_LIBRARY_FILE_EXT ".mpi3lib"

#define MPI3_PID_STRING_LENGTH      18
#define MPI3_PID_PREFIX_LIBRARY     "L:"
#define MPI3_PID_PREFIX_ELEMENT     "E:"
#define MPI3_PID_PREFIX_CONTAINER   "C:"
#define MPI3_PID_PREFIX_PLAYLIST    "P:"
#define MPI3_PID_PREFIX_FOLDER      "F:"
#define MPI3_PID_PREFIX_SONG        "S:"


class MMediaLibrary;
class MMediaElement;
class MContainer;
class MPlaylist;
class MFolder;
class MSong;

typedef QVector<MMediaElement*> MElementList;
typedef QVector<MContainer*> MContainerList;
typedef QVector<MPlaylist*> MPlaylistList;
typedef QVector<MFolder*> MFolderList;
typedef QVector<MSong*> MSongList;


#include "mcore.h"
namespace Mpi3
{

#ifdef MPI3_BUILD_SHARED
    MPI3_EXPORT_CORE Q_NAMESPACE
#else
    Q_NAMESPACE
#endif

    void initialize();

    enum ElementType {
        BaseElement,
        SongElement,
        ContainerElement,
        PlaylistElement,
        FolderElement,
        LibraryElement
    }; Q_ENUM_NS(ElementType)

    enum MediaState {
        MediaEmpty,
        MediaReady,
        MediaBusy
    }; Q_ENUM_NS(MediaState)

    enum EngineState {
        EngineStopped,
        EngineActive,
        EngineIdle
    }; Q_ENUM_NS(EngineState)

    enum ErrorState {
        NoError,
        ResourceError,
        FileFormatError,
        FileAccessError,
        FileInvalidError
    }; Q_ENUM_NS(ErrorState)
};


#endif
