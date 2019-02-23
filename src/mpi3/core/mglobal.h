#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H


#include <QMetaType>


// Canadian (Sorry)
typedef QColor QColour;


#include "mcore.h"


class MMediaElement;
class MMediaContainer;
class MMediaLibrary;
class MPlaylist;
class MFolder;
class MSong;


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


#ifdef MPI3_BUILD_SHARED
   MPI3_EXPORT_CORE void register_global_metatypes();
#else
    void register_global_metatypes();
#endif


};


#endif
