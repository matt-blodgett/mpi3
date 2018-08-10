#pragma once

#ifndef MAUDIO_H
#define MAUDIO_H

#include "mglobal.h"


#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTime>

#include <QReadWriteLock>


struct AVFormatContext;
struct AVCodecContext;
struct ao_device;


class MAudioEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MAudioEngine)

//    Q_PROPERTY

public:
    explicit MAudioEngine(QObject *parent = nullptr);
    ~MAudioEngine();

private:
    void media_alloc();
    void media_dealloc();

    void engine_process();
    void engine_finished();

public:
    void open(const QString &path);

    void start();
    void stop();

    void play();
    void pause();

    void seek(double pos);
    void gain(float vol);

public:
    QString filepath() const;

    bool empty() const;
    bool ready() const;
    bool busy() const;

    bool stopped() const;
    bool playing() const;
    bool paused() const;

    float volume() const;
    double position() const;

    Mpi3::MediaState mediaStatus() const;
    Mpi3::EngineState engineStatus() const;
    Mpi3::ErrorState errorStatus() const;
    Mpi3::EngineState requestedStatus() const;

private:
    void updateStatus(Mpi3::MediaState state);
    void updateStatus(Mpi3::EngineState state);
    void updateStatus(Mpi3::ErrorState state);
    void updateRequest(Mpi3::EngineState state);

private:
    mutable QReadWriteLock m_attribMtx;
    mutable QMutex m_processMutex;
    QWaitCondition m_processCondition;
    QThread *m_processThread = nullptr;

    Mpi3::MediaState m_mediaStatus;
    Mpi3::EngineState m_engineStatus;
    Mpi3::ErrorState m_errorStatus;
    Mpi3::EngineState m_requestStatus;

    float m_volume;
    float m_decibels;
    double m_position;
    QString m_filepath;

private:
    AVFormatContext *m_formatCtx = nullptr;
    AVCodecContext *m_codecCtx = nullptr;
    ao_device *m_aoDevice = nullptr;
    int m_streamIdx;

signals:
    void notifyVolume(float vol);
    void notifyPosition(double pos);
    void notifyMediaStatus(Mpi3::MediaState state);
    void notifyEngineStatus(Mpi3::EngineState state);
    void notifyErrorStatus(Mpi3::ErrorState state);

};

#endif












