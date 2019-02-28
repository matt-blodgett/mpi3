#pragma once

#ifndef MPLAYBACKQUEUE_H
#define MPLAYBACKQUEUE_H


#include "mglobal.h"
#include "maudioengine.h"
#include "mmodelsonglist.h"


class MPlaybackQueue : public QObject
{
    Q_OBJECT

public:
    explicit MPlaybackQueue(QObject *parent = nullptr);

public:
    void setModel(MModelSonglist *model);
    void setEngine(MAudioEngine *engine);

    void playNext();
    void playPrev();

private:
    QVector<MSong*> m_queue;
    MAudioEngine *m_engine;

private:
    void handleMediaStatus(Mpi3::MediaState state);
    void handleEngineStatus(Mpi3::EngineState state);
    void handleErrorStatus(Mpi3::ErrorState state);
    void handleRequestStatus(Mpi3::EngineState state);

signals:
    void currentPlayingChanged(MSong *song);
};


#endif
