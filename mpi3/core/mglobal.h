#pragma once

#ifndef MGLOBAL_H
#define MGLOBAL_H

#include <QMetaType>

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

    void register_global_metatypes();
};


#endif
