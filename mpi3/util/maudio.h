#pragma once

#ifndef MAUDIO_H
#define MAUDIO_H

#include <QObject>
#include <QMutex>

struct AVFormatContext;
struct AVCodecContext;
struct ao_device;


namespace Mpi3 {

class MAudioEngine : public QObject
{
    Q_OBJECT

public:
    explicit MAudioEngine(QObject *parent = nullptr);
    ~MAudioEngine();

private:
    bool engine_alloc();
    void engine_dealloc();
    void engine_dispatch();

public:
    void open(const std::string &path);
    void stop();

public slots:
    void play();
    void pause();
    void seek(double pos);
    void setVolume(float vol);

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
    mutable QMutex m_mutex;

    bool m_loaded;
    bool m_active;
    bool m_paused;
    float m_volume;
    float m_decibels;
    double m_position;
    std::string m_filepath;

private:
    AVFormatContext *m_formatCtx = nullptr;
    AVCodecContext *m_codecCtx = nullptr;
    int m_streamIdx;

    ao_device *m_aoDevice;

};

};

#endif












