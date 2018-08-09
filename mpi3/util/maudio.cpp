#include "maudio.h"

#include <iostream>


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
    QMutexLocker mlock(&m_accessMutex);

    m_loaded = false;
    m_active = false;
    m_paused = false;
    m_volume = 0.0f;
    m_decibels = -30.0f;
    m_position = 0.0;
    m_filepath.clear();

    m_volume = 0.5f;

    av_log_set_level(AV_LOG_VERBOSE); // AV_LOG_QUIET
    av_register_all();
    avcodec_register_all();
    avformat_network_init();
    ao_initialize();
}
MAudioEngine::~MAudioEngine(){
//    m_active = false;
//    m_processThread->wait();

    engine_dealloc();
    avformat_network_deinit();
    ao_shutdown();

    std::cout << "ENGINE DESTROYED" << std::endl;
}

void MAudioEngine::engine_dealloc(){
    m_active = false;
    m_loaded = false;
    m_position = 0.0;
    m_filepath.clear();

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

}
bool MAudioEngine::engine_alloc(){


    int error = 0;


    //---------------------------------------------------------------------------
    std::string spath = m_filepath.toStdString();
    error = avformat_open_input(&m_formatCtx, spath.c_str(), nullptr, nullptr);
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
        std::cerr << "ERROR: error finding audio stream" << std::endl;
        return false;
    }
    else if(m_streamIdx == AVERROR_DECODER_NOT_FOUND){
        std::cerr << "ERROR: error finding decoder" << std::endl;
        return false;
    }
    else if(!codec){
        std::cerr << "ERROR: error loading codec" << std::endl;
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


    return true;
}
void MAudioEngine::engine_process(){

    m_accessMutex.lock();

    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);

    AVStream *stream = m_formatCtx->streams[m_streamIdx];
    AVFrame *frame = av_frame_alloc();

    int error = 0;
    int finished = 0;
    int interrupt = 0;
    int bps = av_get_bytes_per_sample(m_codecCtx->sample_fmt);
    error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);

    if(!error){
        m_active = true;
        emit notifyActive(true);
    }

    m_accessMutex.unlock();


    m_processMutex.lock();
    m_processCondition.wait(&m_processMutex);
    m_processMutex.unlock();

    emit notifyPlayback(true);

    while(!finished && !error && active()) {


        if(++interrupt > 400){break;}



        emit notifyPosition(position());

        if(paused()){
            emit notifyPlayback(false);

            m_processMutex.lock();
            m_processCondition.wait(&m_processMutex);

            if(active()){
                emit notifyPlayback(true);
            }

            m_processMutex.unlock();
        }

        if(error || !active()){
            break;
        }

        m_accessMutex.lock();
        m_position = frame->pkt_dts * av_q2d(stream->time_base);

        for (int i = 0; i < frame->nb_samples; i++){

            for (int c = 0; c < m_codecCtx->channels; c++){

                short *data_raw = reinterpret_cast<short*>(frame->data[c] + bps*i);
                float sample_raw = static_cast<float>(*data_raw) / static_cast<float>(BIT_RANGE);

                sample_raw *= m_volume;
                sample_raw *= BIT_RANGE;

                float sample_processed = av_clipf_c(sample_raw, -BIT_RANGE, BIT_RANGE-1);
                short data_processed = static_cast<short>(sample_processed);

                ao_play(m_aoDevice, reinterpret_cast<char*>(&data_processed), static_cast<uint32_t>(bps));
            }

        }

        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        m_accessMutex.unlock();
    }


    av_frame_free(&frame);

    std::cout << "loop ended" << std::endl;

    emit notifyActive(false);
    QThread::currentThread()->quit();
}
void MAudioEngine::engine_finished(){
    QMutexLocker mlock(&m_accessMutex);

    if(m_processThread){
        if(m_processThread->isFinished()){
            std::cout << "thread finished" << std::endl;
            delete m_processThread;
        }
    }
}

bool MAudioEngine::open(const QString &path){
    QMutexLocker mlock(&m_accessMutex);

    engine_dealloc();
    m_filepath = path;
    m_loaded = engine_alloc();

    return m_loaded;
}

void MAudioEngine::start(){
    QMutexLocker mlock(&m_accessMutex);

    if(!m_processThread && m_loaded){
        m_processThread = QThread::create([this](){engine_process();});
        connect(m_processThread, &QThread::finished, this, [this](){engine_finished();});

        m_processThread->start();
        m_processThread->setPriority(QThread::HighestPriority);
    }
}
void MAudioEngine::stop(){
    m_accessMutex.lock();
    m_active = false;
    m_accessMutex.unlock();

    if(m_processThread){
        m_processThread->wait();
    }
}
void MAudioEngine::play(){
    QMutexLocker mlock(&m_accessMutex);
    m_paused = false;
    m_processCondition.notify_all();
}
void MAudioEngine::pause(){
    QMutexLocker mlock(&m_accessMutex);
    m_paused = true;
}
void MAudioEngine::seek(double pos){
    Q_UNUSED(pos);
}
void MAudioEngine::gain(float vol){
    QMutexLocker mlock(&m_accessMutex);
    m_volume = av_clipf_c(vol, 0.0f, 1.0f);
    m_decibels = av_clipf_c(10.0f * logf(m_volume), VOL_DB_FLOOR, VOL_DB_CIEL);

    emit notifyVolume(vol);
}

bool MAudioEngine::loaded() const{
    QMutexLocker mlock(&m_accessMutex);
    return m_loaded;
}
bool MAudioEngine::active() const{
    QMutexLocker mlock(&m_accessMutex);
    return m_active;
}
bool MAudioEngine::paused() const{
    QMutexLocker mlock(&m_accessMutex);
    return m_paused;
}
float MAudioEngine::volume() const{
    QMutexLocker mlock(&m_accessMutex);
    return m_volume;
}
double MAudioEngine::position() const{
    QMutexLocker mlock(&m_accessMutex);
    return m_position;
}





















