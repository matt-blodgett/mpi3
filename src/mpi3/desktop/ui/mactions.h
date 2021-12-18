#pragma once

#ifndef MACTIONS_H
#define MACTIONS_H


#include "mpi3/core/mglobal.h"


// TODO: Use namespace Mpi3::Actions {...}


namespace MActions
{
    Q_NAMESPACE

    QString pathAppData();
    QString pathDesktop();

    QString pathProfile();
    QString pathLibraryDefault();
    QString pathLibraryBackups();

    void openFileLocation(const QString &path);
};


#endif
