#pragma once

#ifndef MAUDIOENGINE_H
#define MAUDIOENGINE_H


#include <QObject>


#include "mpi3/core/mglobal.h"
#ifdef MPI3_BUILD_SHARED
struct MPI3_EXPORT_CORE MSongInfo;
#endif


struct MSongInfo
{
    bool load(const QString &path);

    QString path;
    QString title;
    QString artist;
    QString album;
    QString kind;

    double time;
    double size;
    int bitRate;
    int sampleRate;

    QString majorBrand;
    QString minorVersion;
    QString compatibleBrands;
    QString encoder;

    QMap<QString, QVariant> songInfoMap();
};

#endif
