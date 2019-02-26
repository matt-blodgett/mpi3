#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H


#define MPI3_LIBRARY_FILE_EXT ".mpi3lib"


class MMediaElement;
class MMediaContainer;
class MMediaLibrary;
class MPlaylist;
class MFolder;
class MSong;


#include <QMetaType>
typedef QColor QColour; // Canadian (Sorry)


#include "mcore.h"


namespace Mpi3
{

#ifdef MPI3_BUILD_SHARED
    MPI3_EXPORT_CORE Q_NAMESPACE
#else
    Q_NAMESPACE
#endif

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

    enum EngineState
    {
        EngineStopped,
        EngineActive,
        EngineIdle
    }; Q_ENUM_NS(EngineState)

    enum ErrorState
    {
        NoError,
        ResourceError,
        FileFormatError,
        FileAccessError,
        FileInvalidError
    }; Q_ENUM_NS(ErrorState)

    void initialize();

//#ifdef MPI3_BUILD_SHARED
//   MPI3_EXPORT_CORE void register_global_metatypes();
//#else
//    void register_global_metatypes();
//#endif
};


#endif
