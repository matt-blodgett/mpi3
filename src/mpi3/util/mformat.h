#pragma once

#ifndef MFORMAT_H
#define MFORMAT_H


#include <QMetaType>


#include "mpi3/util/mutil.h"


namespace Mpi3
{

#ifdef MPI3_BUILD_SHARED
    MPI3_EXPORT_UTIL Q_NAMESPACE
#else
    Q_NAMESPACE
#endif

    namespace Util
    {

    #ifdef MPI3_BUILD_SHARED
        MPI3_EXPORT_UTIL Q_NAMESPACE
        MPI3_EXPORT_UTIL QString timeToString(double time);
        MPI3_EXPORT_UTIL QString sizeToString(double size, int prec = 2);
        MPI3_EXPORT_UTIL QString percentToString(double percent, int prec = 2);
    #else
        Q_NAMESPACE
        QString timeToString(double time);
        QString sizeToString(double size, int prec = 2);
        QString percentToString(double percent, int prec = 2);
    #endif

    };

};




#endif
