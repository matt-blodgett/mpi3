#include "maudioengine.h"

#include <QThread>
#include <QMutex>

#include <QReadWriteLock>
#include <QWriteLocker>
#include <QReadLocker>
#include <QWaitCondition>


extern "C"
{
#include <lib/libav/libavcodec/avcodec.h>
#include <lib/libav/libavformat/avformat.h>
}

#include <lib/libao/ao/ao.h>


#define BIT_DEPTH 16
#define BIT_RANGE (1<<(BIT_DEPTH - 1))


#include <iostream>


static int decode_audio_frame(
        AVFrame *frame, AVFormatContext *format_ctx,
        AVCodecContext *codec_ctx, int *finished) {

    //static int flush_codec(AVCodecContext *codec_ctx){

    //    AVFrame *frame;
    //    frame = av_frame_alloc();

    //    if(!frame){
    //        return -1;
    //    }

    //    avcodec_send_packet(codec_ctx, nullptr);

    //    int eof = 0;
    //    while(eof != AVERROR_EOF){
    //        eof = avcodec_receive_frame(codec_ctx, frame);
    //    }

    //    av_frame_free(&frame);
    //    avcodec_flush_buffers(codec_ctx);

    //    return 0;
    //}

    AVPacket pckt;
    av_init_packet(&pckt);

    int error = 0;
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
    m_processThread = nullptr;
    m_processMutex = new QMutex();
    m_processCondition = new QWaitCondition();
    m_attribMtx = new QReadWriteLock();

    m_mediaStatus = Mpi3::MediaEmpty;
    m_engineStatus = Mpi3::EngineStopped;
    m_errorStatus = Mpi3::NoError;
    m_requestStatus = Mpi3::EngineStopped;

    m_vol_percent = 0.0f;
    m_vol_dbratio = 0.0f;
    m_position = 0.0;
    m_filepath = QString();

    m_formatCtx = nullptr;
    m_codecCtx = nullptr;
    m_aoDevice = nullptr;
    m_streamIdx = -1;

    av_log_set_level(AV_LOG_VERBOSE); // AV_LOG_QUIET
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    ao_initialize();
}
MAudioEngine::~MAudioEngine(){
    stop();

    media_dealloc();
    avformat_network_deinit();
    ao_shutdown();

    delete m_processMutex;
    delete m_processCondition;
    delete m_attribMtx;

    std::cout << "ENGINE DESTROYED" << std::endl;
}

void MAudioEngine::media_dealloc(){

    if(m_formatCtx){
        avformat_close_input(&m_formatCtx);
        avformat_free_context(m_formatCtx);
    }

    if(m_codecCtx){
        avcodec_close(m_codecCtx);
        avcodec_free_context(&m_codecCtx);
    }

    if(m_aoDevice){
        ao_close(m_aoDevice);
    }

    m_filepath.clear();
    updateStatus(Mpi3::MediaEmpty);
}
void MAudioEngine::media_alloc(){


    int error = 0;


    //---------------------------------------------------------------------------
    std::string spath = m_filepath.toStdString();
    error = avformat_open_input(&m_formatCtx, spath.c_str(), nullptr, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening input file";
        return;
    }


    //---------------------------------------------------------------------------
    error = avformat_find_stream_info(m_formatCtx, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error finding stream info" << std::endl;
        return;
    }


    //---------------------------------------------------------------------------
    AVCodec *codec;
    m_streamIdx = av_find_best_stream(
        m_formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);

    if(m_streamIdx == AVERROR_STREAM_NOT_FOUND){
        std::cerr << "ERROR: error finding audio stream" << std::endl;
        return;
    }
    else if(m_streamIdx == AVERROR_DECODER_NOT_FOUND){
        std::cerr << "ERROR: error finding decoder" << std::endl;
        return;
    }
    else if(!codec){
        std::cerr << "ERROR: error loading codec" << std::endl;
        return;
    }


    //---------------------------------------------------------------------------
    m_codecCtx = avcodec_alloc_context3(codec);
    if(!m_codecCtx){
        std::cerr << "ERROR: error allocating codec context";
        return;
    }


    //---------------------------------------------------------------------------
    error = avcodec_open2(m_codecCtx, codec, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening codec";
        return;
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
        return;
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
        return;
    }

    updateStatus(Mpi3::MediaReady);
}
void MAudioEngine::engine_process(){

    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);

    AVStream *stream = m_formatCtx->streams[m_streamIdx];
    AVFrame *frame = av_frame_alloc();

    int error = 0;
    int finished = 0;
    int interrupt = 0;
    int bps = av_get_bytes_per_sample(m_codecCtx->sample_fmt);
    error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);

    while(!finished && !error) {

        if(++interrupt > 1200){break;}

        for(int i = 0; i < frame->nb_samples; i++){

            if(requestedStatus() == Mpi3::EngineIdle){

                updateStatus(Mpi3::EngineIdle);

                m_processMutex->lock();
                m_processCondition->wait(m_processMutex);
                m_processMutex->unlock();

                switch(requestedStatus()){
                    case Mpi3::EngineActive:
                        updateStatus(Mpi3::EngineActive);
                        break;
                    case Mpi3::EngineStopped:
                        goto halt;
                    case Mpi3::EngineIdle:
                        continue;
                }
            }
            else if(requestedStatus() == Mpi3::EngineStopped){
                goto halt;
            }

            for(int c = 0; c < m_codecCtx->channels; c++){

                short *data_raw = reinterpret_cast<short*>(frame->data[c] + bps*i);
                float sample_raw = static_cast<float>(*data_raw) / static_cast<float>(BIT_RANGE);

//                sample_raw *= 0.0f;
//                sample_raw *= BIT_RANGE;

                m_attribMtx->lockForRead();
                sample_raw *= m_vol_dbratio;
                sample_raw *= BIT_RANGE;
                m_attribMtx->unlock();

                float sample_processed = av_clipf_c(sample_raw, -BIT_RANGE, BIT_RANGE-1);
                short data_processed = static_cast<short>(sample_processed);

                ao_play(m_aoDevice, reinterpret_cast<char*>(&data_processed), static_cast<uint32_t>(bps));
            }
        }

        m_attribMtx->lockForWrite();
        m_position = frame->pkt_dts * av_q2d(stream->time_base);
        emit notifyPosition(m_position);
        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        m_attribMtx->unlock();
    }

halt:
    std::cout << "loop ended" << std::endl;
    av_frame_free(&frame);
    QThread::currentThread()->quit();
}
void MAudioEngine::engine_finished(){
    delete m_processThread;
    media_dealloc();
    updateStatus(Mpi3::EngineStopped);
}

void MAudioEngine::open(const QString &path){

    if(empty()){

        media_dealloc();
        m_filepath = path;
        media_alloc();
    }
}

void MAudioEngine::start(){

    if(ready()){

        if(!m_processThread){
            m_processThread = QThread::create([this](){engine_process();});
            connect(m_processThread, &QThread::finished, this, [this](){engine_finished();});

            updateStatus(Mpi3::MediaBusy);
            updateStatus(Mpi3::EngineActive);
            m_requestStatus = Mpi3::EngineActive;

            m_processThread->start();
            m_processThread->setPriority(QThread::HighestPriority);
        }
    }
}
void MAudioEngine::stop(){

    if(busy()){

        updateRequest(Mpi3::EngineStopped);

        if(m_processThread){
            m_processThread->wait();
        }
    }
}

void MAudioEngine::play(){
    updateRequest(Mpi3::EngineActive);
}
void MAudioEngine::pause(){
    updateRequest(Mpi3::EngineIdle);
}

void MAudioEngine::seek(double pos){
    QWriteLocker lock_write(m_attribMtx);
    Q_UNUSED(pos);
}
void MAudioEngine::gain(int vol){
    QWriteLocker lock_write(m_attribMtx);

    m_vol_percent = av_clipf_c(static_cast<float>(vol)/100.0f, 0.0f, 1.0f);
    m_vol_dbratio = av_clipf_c(log10f(1 - m_vol_percent) / -2.0f, 0.0f, 1.0f);

    emit notifyVolume(vol);
}

QString MAudioEngine::filepath() const {
    QReadLocker lock_read(m_attribMtx);
    return m_filepath;
}

bool MAudioEngine::empty() const {
    QReadLocker lock_read(m_attribMtx);
    return m_mediaStatus == Mpi3::MediaEmpty;
}
bool MAudioEngine::ready() const{
    QReadLocker lock_read(m_attribMtx);
    return m_mediaStatus == Mpi3::MediaReady;
}
bool MAudioEngine::busy() const {
    QReadLocker lock_read(m_attribMtx);
    return m_mediaStatus == Mpi3::MediaBusy;
}

bool MAudioEngine::stopped() const {
    QReadLocker lock_read(m_attribMtx);
    return m_engineStatus == Mpi3::EngineStopped;
}
bool MAudioEngine::playing() const{
    QReadLocker lock_read(m_attribMtx);
    return m_engineStatus == Mpi3::EngineActive;
}
bool MAudioEngine::paused() const{
    QReadLocker lock_read(m_attribMtx);
    return m_engineStatus == Mpi3::EngineIdle;;
}

float MAudioEngine::volume() const{
    QReadLocker lock_read(m_attribMtx);
    return m_vol_percent;
}
double MAudioEngine::position() const{
    QReadLocker lock_read(m_attribMtx);
    return m_position;
}

Mpi3::MediaState MAudioEngine::mediaStatus() const {
    QReadLocker lock_read(m_attribMtx);
    return m_mediaStatus;
}
Mpi3::EngineState MAudioEngine::engineStatus() const {
    QReadLocker lock_read(m_attribMtx);
    return m_engineStatus;
}
Mpi3::ErrorState MAudioEngine::errorStatus() const {
    QReadLocker lock_read(m_attribMtx);
    return m_errorStatus;
}
Mpi3::EngineState MAudioEngine::requestedStatus() const {
    QReadLocker lock_read(m_attribMtx);
    return m_requestStatus;
}

void MAudioEngine::updateStatus(Mpi3::MediaState state){
    QWriteLocker lock_write(m_attribMtx);
    m_mediaStatus = state;
    emit notifyMediaStatus(state);
}
void MAudioEngine::updateStatus(Mpi3::EngineState state){
    QWriteLocker lock_write(m_attribMtx);
    m_engineStatus = state;
    emit notifyEngineStatus(state);
}
void MAudioEngine::updateStatus(Mpi3::ErrorState state){
    QWriteLocker lock_write(m_attribMtx);
    m_errorStatus = state;
    emit notifyErrorStatus(state);
}
void MAudioEngine::updateRequest(Mpi3::EngineState state){
    QWriteLocker lock_write(m_attribMtx);
    m_requestStatus = state;

    if(state != Mpi3::EngineIdle){
        m_processCondition->notify_all();
    }

    emit notifyRequestStatus(state);
}











