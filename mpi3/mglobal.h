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

    Q_NAMESPACE

    enum ElementType {
        BaseElement,
        SongElement,
        ContainerElement,
        PlaylistElement,
        FolderElement,
        LibraryElement
    };

    Q_ENUM_NS(ElementType)

    enum MediaState {
        MediaEmpty,
        MediaReady,
        MediaBusy
    };

    Q_ENUM_NS(MediaState)

    enum EngineState {
        EngineStopped,
        EngineActive,
        EngineIdle
    };

    Q_ENUM_NS(EngineState)

    enum ErrorState {
        NoError,
        ResourceError,
        FileFormatError,
        FileAccessError,
        FileInvalidError
    };

    Q_ENUM_NS(ErrorState)

    void register_global_metatypes();
};


#endif
