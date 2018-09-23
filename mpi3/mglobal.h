#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H


#include <QMetaType>


typedef QColor QColour;


class MMediaElement;
class MMediaContainer;
class MMediaLibrary;
class MPlaylist;
class MFolder;
class MSong;


namespace Mpi3 {

#ifdef Q_NAMESPACE
    Q_NAMESPACE
#endif

    enum ElementType {
        BaseElement,
        SongElement,
        ContainerElement,
        PlaylistElement,
        FolderElement,
        LibraryElement
    };

#ifdef Q_NAMESPACE
    Q_ENUM_NS(ElementType)
#endif

    enum MediaState {
        MediaEmpty,
        MediaReady,
        MediaBusy
    };

#ifdef Q_NAMESPACE
    Q_ENUM_NS(MediaState)
#endif

    enum EngineState {
        EngineStopped,
        EngineActive,
        EngineIdle
    };

#ifdef Q_NAMESPACE
    Q_ENUM_NS(EngineState)
#endif

    enum ErrorState {
        NoError,
        ResourceError,
        FileFormatError,
        FileAccessError,
        FileInvalidError
    };

#ifdef Q_NAMESPACE
    Q_ENUM_NS(ErrorState)
#endif

    void register_global_metatypes();
};


#endif
