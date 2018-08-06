#pragma once

#ifndef MAUDIO_H
#define MAUDIO_H

#include <QObject>

#include <atomic>
#include <thread>

struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;


namespace Mpi3 {

class MAudioEngine : public QObject
{
    Q_OBJECT

public:
    explicit MAudioEngine(QObject *parent = nullptr);
    ~MAudioEngine();

public:
    bool load(std::string filepath);

    void start();
    void stop();

    void pause();
    void pause(bool paus);

    void setVolume(float vol);

public:
    bool paused();
    float volume();
    double position();

private:
    std::atomic<bool> m_paused;
    std::atomic<float> m_volume;
    std::atomic<float> m_decibels;
    std::atomic<double> m_position;

    AVFormatContext *m_formatCtx = nullptr;
    AVCodecContext *m_codecCtx = nullptr;
    int m_streamIndex;

private:
    void beginDecoding();
    std::thread m_decodeThread;
    std::atomic<bool> m_continue;

};

};

#endif // MAUDIO_H












