#pragma once

#ifndef MCONTENTDELEGATE_H
#define MCONTENTDELEGATE_H

#include <QObject>
#include <QVector>


#include "mglobal.h"


class MAudioEngine;


class MContentDelegate : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MContentDelegate)

public:
    explicit MContentDelegate(QObject *parent = nullptr);
    ~MContentDelegate();

public:
    void setEngine(MAudioEngine *engine);
    void setContent(MMediaContainer *container, MSong *song);
    void setIndex(int idx);

    void next();
    void prev();

public:
    MAudioEngine *engine() const;
    MMediaContainer *container() const;
    MSong *song() const;
    MSong *songAt(int idx) const;
    int index() const;

private:
    MAudioEngine *m_audioEngine = nullptr;
    MMediaContainer *m_container = nullptr;
    MSong *m_song = nullptr;

private slots:
    void processMediaStatus(Mpi3::MediaState state);
    void processEngineStatus(Mpi3::EngineState state);
    void processErrorStatus(Mpi3::ErrorState state);
    void processRequestStatus(Mpi3::EngineState state);

signals:
    void notifySongChanged(MSong *song, MMediaContainer *container);

};


#endif
