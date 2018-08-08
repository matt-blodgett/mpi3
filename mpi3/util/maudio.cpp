#include "maudio.h"

#include <iostream>

#include <QThread>

#include <lib/libao/ao/ao.h>


extern "C" {
#include <lib/libav/libavcodec/avcodec.h>
#include <lib/libav/libavformat/avformat.h>
}


#define BIT_DEPTH 16
#define BIT_RANGE (1<<(BIT_DEPTH - 1))

#define VOL_DB_CIEL 0.0f
#define VOL_DB_FLOOR -30.0f


using namespace Mpi3;


static int decode_audio_frame(
        AVFrame *frame, AVFormatContext *format_ctx,
        AVCodecContext *codec_ctx, int *finished) {

    AVPacket pckt;
    av_init_packet(&pckt);

    int error;
    if((error = av_read_frame(format_ctx, &pckt)) < 0) {

        if(error == AVERROR_EOF){
            *finished = 1;
        }
        else {
            std::cerr << "ERROR: could not read frame";
            std::cerr << "(error " << error << ")" << std::endl;
            return error;
        }
    }

    if((error = avcodec_send_packet(codec_ctx, &pckt)) < 0) {
        std::cerr << "ERROR: could not send packet for decoding";
        std::cerr << "(error " << error << ")" << std::endl;
        return error;
    }

    error = avcodec_receive_frame(codec_ctx, frame);

    if(error == AVERROR(EAGAIN)) {
        error = 0;
    }
    else if(error == AVERROR_EOF) {
        *finished = 1;
        error = 0;
    }
    else if(error < 0) {
        std::cerr << "ERROR: could not decode frame";
        std::cerr << "(error " << error << ")" << std::endl;
    }

    av_packet_unref(&pckt);
    return error;
}


MAudioEngine::MAudioEngine(QObject *parent) : QObject(parent){
    QMutexLocker lock(&m_mutex);

    m_filepath = "";
    m_loaded = false;
    m_active = false;
    m_paused = false;
    m_volume = 0.0f;
    m_decibels = -30.0f;
    m_position = 0.0;

    m_volume = 0.5f;


    av_log_set_level(AV_LOG_VERBOSE); // AV_LOG_QUIET
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    ao_initialize();
}
MAudioEngine::~MAudioEngine(){
    QMutexLocker lock(&m_mutex);

    engine_dealloc();
    avformat_network_deinit();
    ao_shutdown();

    std::cout << "ENGINE DESTROYED" << std::endl;
}

void MAudioEngine::engine_dealloc(){
    avformat_close_input(&m_formatCtx);
    avcodec_close(m_codecCtx);

    avformat_free_context(m_formatCtx);
    avcodec_free_context(&m_codecCtx);

    ao_close(m_aoDevice);

    m_active = false;
    m_loaded = false;
    m_position = 0.0;
    m_filepath = "";
}
bool MAudioEngine::engine_alloc(){


    int error = 0;


    //---------------------------------------------------------------------------
    error = avformat_open_input(&m_formatCtx, m_filepath.c_str(), nullptr, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening input file";
        return false;
    }


    //---------------------------------------------------------------------------
    error = avformat_find_stream_info(m_formatCtx, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error finding stream info" << std::endl;
        return false;
    }


    //---------------------------------------------------------------------------
    AVCodec *codec;
    m_streamIdx = av_find_best_stream(
        m_formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);

    if(m_streamIdx == AVERROR_STREAM_NOT_FOUND){
        std::cerr << "ERROR: stream not found" << std::endl;
        return false;
    }
    else if(m_streamIdx == AVERROR_DECODER_NOT_FOUND){
        std::cerr << "ERROR: decoder not found" << std::endl;
        return false;
    }
    else if(!codec){
        std::cerr << "ERROR: no codec" << std::endl;
        return false;
    }


    //---------------------------------------------------------------------------
    m_codecCtx = avcodec_alloc_context3(codec);
    if(!m_codecCtx){
        std::cerr << "ERROR: error allocating codec context";
        return false;
    }


    //---------------------------------------------------------------------------
    error = avcodec_open2(m_codecCtx, codec, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening codec";
        return false;
    }


    //---------------------------------------------------------------------------
    for(uint32_t i = 0; i < m_formatCtx->nb_streams; i++){
        if(m_formatCtx->streams[i]->index != m_streamIdx){
            m_formatCtx->streams[i]->discard = AVDISCARD_ALL;
        }
    }

    AVStream *stream = m_formatCtx->streams[m_streamIdx];
    stream->discard = AVDISCARD_DEFAULT;

    AVFrame *frame;
    frame = av_frame_alloc();
    if(!frame){
        std::cerr << "ERROR: error allocating frame" << std::endl;
        return false;
    }


    //---------------------------------------------------------------------------
    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);

    error = 0;
    int finished = 0;
    int sample_rate = -1;
    while((!finished && !error) && sample_rate <= 0){
        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        sample_rate = frame->sample_rate;
    }

    av_frame_free(&frame);

//    std::cout << "duration: " << stream->duration * av_q2d(stream->time_base) << std::endl;
//    std::cout << "sample rate: " << sample_rate << std::endl;
//    std::cout << "bit rate: " << m_formatCtx->bit_rate << std::endl;

//    AVDictionaryEntry *tag;
//    while ((tag = av_dict_get(m_formatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))){
//        std::cout << tag->key << " = " << tag->value << std::endl;
//    }

//    std::cout << std::endl;


    //---------------------------------------------------------------------------
    int default_driver = ao_default_driver_id();
    ao_info *info = ao_driver_info(default_driver);

    ao_sample_format output_format;
    memset(&output_format, 0, sizeof(output_format));

    output_format.bits = BIT_DEPTH;
    output_format.channels = m_codecCtx->channels;
    output_format.rate = sample_rate;
    output_format.byte_format = info->preferred_byte_format;
    output_format.matrix = nullptr;

    m_aoDevice = ao_open_live(default_driver, &output_format, nullptr);
    if(!m_aoDevice){
        std::cerr << "ERROR: error opening output device" << std::endl;
        return false;
    }

    m_loaded = true;
    return true;
}
void MAudioEngine::engine_dispatch(){


    int error = 0;


    //---------------------------------------------------------------------------
    AVStream *stream = m_formatCtx->streams[m_streamIdx];
    AVFrame *frame = av_frame_alloc();

    error = 0;
    avcodec_send_packet(m_codecCtx, nullptr);
    while(error != AVERROR_EOF){
        error = avcodec_receive_frame(m_codecCtx, frame);
    }

    avcodec_flush_buffers(m_codecCtx);
    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);


    //---------------------------------------------------------------------------
    int btps = av_get_bytes_per_sample(m_codecCtx->sample_fmt);

    int finished = 0;
    int interrupt = 0;

    while(!finished && !error && active()) {

        emit notifyPosition(position());

        if(++interrupt > 220){
            break;
        }

        while(paused() && active()){
            std::cout << "loop paused" << std::endl;
            QThread::msleep(100);
        }

        if(error || !active()){
            break;
        }

        m_mutex.lock();
        m_position = frame->pkt_dts * av_q2d(stream->time_base);
        std::cout << m_position << std::endl;

        for (int i = 0; i < frame->nb_samples; i++){

            for (int c = 0; c < m_codecCtx->channels; c++){

                short *data_raw = reinterpret_cast<short*>(frame->data[c] + btps*i);
                float sample_raw = static_cast<float>(*data_raw) / static_cast<float>(BIT_RANGE);

                sample_raw *= m_volume;
                sample_raw *= BIT_RANGE;

                float sample_processed = av_clipf_c(sample_raw, -BIT_RANGE, BIT_RANGE-1);
                short data_processed = static_cast<short>(sample_processed);

                ao_play(m_aoDevice, reinterpret_cast<char*>(&data_processed), static_cast<uint32_t>(btps));

            }
        }

        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        m_mutex.unlock();
    }


    av_frame_free(&frame);
}

void MAudioEngine::open(const std::string &path){
    m_mutex.lock();

    engine_dealloc();
    m_filepath = path;
    engine_alloc();

    if(m_loaded){
        m_active = true;
        m_mutex.unlock();
        engine_dispatch();
    }
    else{
        m_active = false;
        m_mutex.unlock();
    }
}
void MAudioEngine::stop(){
    QMutexLocker lock(&m_mutex);
    m_loaded = false;
    m_active = false;
    emit notifyActive(false);
}

void MAudioEngine::play(){
    QMutexLocker lock(&m_mutex);
    m_paused = false;
    emit notifyPlayback(true);
}
void MAudioEngine::pause(){
    QMutexLocker lock(&m_mutex);
    m_paused = true;
    emit notifyPlayback(false);
}
void MAudioEngine::seek(double pos){
    Q_UNUSED(pos);
}
void MAudioEngine::setVolume(float vol){
    QMutexLocker lock(&m_mutex);
    m_volume = av_clipf_c(vol, 0.0f, 1.0f);
    m_decibels = av_clipf_c(10.0f * logf(m_volume), VOL_DB_FLOOR, VOL_DB_CIEL);
    emit notifyVolume(vol);
}

bool MAudioEngine::loaded() const{
    QMutexLocker lock(&m_mutex);
    return m_loaded;
}
bool MAudioEngine::active() const{
    QMutexLocker lock(&m_mutex);
    return m_active;
}
bool MAudioEngine::paused() const{
    QMutexLocker lock(&m_mutex);
    return m_paused;
}
float MAudioEngine::volume() const{
    QMutexLocker lock(&m_mutex);
    return m_volume;
}
double MAudioEngine::position() const{
    QMutexLocker lock(&m_mutex);
    return m_position;
}





















