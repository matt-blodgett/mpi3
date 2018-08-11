#pragma once

#ifndef MCONTENTDELEGATE_H
#define MCONTENTDELEGATE_H

#include <QObject>


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
    void setContent(QVector<MSong*> content);
    void setIndex(int idx);

public:
    MAudioEngine *engine() const;
    QStringList content() const;
    QString pathAt(int idx) const;
    int index() const;

private:
    MAudioEngine *m_audioEngine = nullptr;
    QStringList m_mediaContent;
    int m_index;

private slots:
    void processMediaStatus(Mpi3::MediaState state);
    void processEngineStatus(Mpi3::EngineState state);
    void processErrorStatus(Mpi3::ErrorState state);
    void processRequestStatus(Mpi3::EngineState state);

signals:
    void notifyIndex(int idx);

};


#endif
