#pragma once

#ifndef MAUDIO_H
#define MAUDIO_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QTime>

struct AVFormatContext;
struct AVCodecContext;
struct ao_device;


namespace Mpi3 {

class MAudioEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MAudioEngine)

public:
    explicit MAudioEngine(QObject *parent = nullptr);
    ~MAudioEngine();

private:
    bool engine_alloc();
    void engine_dealloc();
    void engine_process();
    void engine_finished();

public:
    bool open(const QString &path);

    void start();
    void stop();
    void play();
    void pause();
    void seek(double pos);
    void gain(float vol);

public:
    bool loaded() const;
    bool active() const;
    bool paused() const;
    float volume() const;
    double position() const;

signals:
    void notifyVolume(float vol);
    void notifyPosition(double pos);
    void notifyPlayback(bool state);
    void notifyActive(bool act);

private:
    QThread *m_processThread = nullptr;
    QWaitCondition m_processCondition;

    mutable QMutex m_processMutex;
    mutable QMutex m_accessMutex;

    bool m_loaded;
    bool m_active;
    bool m_paused;
    float m_volume;
    float m_decibels;
    double m_position;
    QString m_filepath;

private:
    AVFormatContext *m_formatCtx = nullptr;
    AVCodecContext *m_codecCtx = nullptr;
    int m_streamIdx;

    ao_device *m_aoDevice = nullptr;

};

};

#endif












