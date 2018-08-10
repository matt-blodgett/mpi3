#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H

#include <QMetaType>

#include <QMetaEnum>

namespace Mpi3 {

    Q_NAMESPACE

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

};

Q_DECLARE_METATYPE(Mpi3::MediaState)
Q_DECLARE_METATYPE(Mpi3::EngineState)
Q_DECLARE_METATYPE(Mpi3::ErrorState)


#endif
