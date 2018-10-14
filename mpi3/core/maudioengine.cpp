#include "maudioengine.h"

#include <QFileInfo>
#include <QWaitCondition>
#include <QThread>
#include <QMutex>

#include <iostream>


extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <ao/ao.h>

#define BIT_DEPTH 16
#define BIT_RANGE (1<<(BIT_DEPTH - 1))


namespace Mpi3
{
    void external_libs_init() {        
        av_log_set_level(AV_LOG_VERBOSE); // AV_LOG_QUIET
        av_register_all();
        ao_initialize();
    }
    void external_libs_deinit() {
        ao_shutdown();
    }

};


Q_DECL_UNUSED static void list_ao_drivers(){
    ao_info *info = nullptr;
    for(int i = 0; i < 20; i++){
        info = ao_driver_info(i);
        if(info){
            std::cout << info->name << std::endl;
        }
    }
}
Q_DECL_UNUSED static int flush_codec(AVCodecContext *codec_ctx){

    AVFrame *frame;
    frame = av_frame_alloc();

    avcodec_send_packet(codec_ctx, nullptr);

    int eof = 0;
    while(eof != AVERROR_EOF){
        eof = avcodec_receive_frame(codec_ctx, frame);
    }

    av_frame_free(&frame);
    avcodec_flush_buffers(codec_ctx);

    return 0;
}

static int decode_audio_frame(AVFrame *frame, AVFormatContext *format_ctx, AVCodecContext *codec_ctx, int *finished) {

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
static int load_contexts(std::string filepath, AVFormatContext **formatCtx, AVCodecContext **codecCtx, int *streamIdx){

    int error = 0;

    error = avformat_open_input(formatCtx, filepath.c_str(), nullptr, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening input file" << std::endl;
        return error;
    }

    error = avformat_find_stream_info(*formatCtx, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error finding stream info" << std::endl;
        return error;
    }

    AVCodec *codec;
    *streamIdx = av_find_best_stream(
        *formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);

    if(*streamIdx == AVERROR_STREAM_NOT_FOUND){
        std::cerr << "ERROR: error finding audio stream" << std::endl;
        return error;
    }
    else if(*streamIdx == AVERROR_DECODER_NOT_FOUND){
        std::cerr << "ERROR: error finding decoder" << std::endl;
        return error;
    }
    else if(!codec){
        std::cerr << "ERROR: error loading codec" << std::endl;
        return error;
    }

    if(strncmp(codec->name, "mp3float", 8) == 0){
        codec = avcodec_find_decoder_by_name("mp3");
    }

    *codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx){
        std::cerr << "ERROR: error allocating codec context";
        return error;
    }

    error = avcodec_open2(*codecCtx, codec, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening codec";
        return error;
    }

    return error;
}


void MSongInfo::load(const QString &path){

    AVFormatContext *formatCtx = nullptr;
    AVCodecContext *codecCtx = nullptr;

    int stream_index = -1;

    int error = load_contexts(path.toStdString(), &formatCtx, &codecCtx, &stream_index);
    if(error < 0 || stream_index < 0){
        loaded = false;
        return;
    }

    avformat_seek_file(formatCtx, 0, 0, 0, 0, 0);

    error = 0;
    int finished = 0;
    int sample_rate = -1;
    AVFrame *frame = av_frame_alloc();
    while((!finished && !error) && sample_rate <= 0){
        error = decode_audio_frame(frame, formatCtx, codecCtx, &finished);
        sample_rate = frame->sample_rate;
    }

    AVStream *stream = formatCtx->streams[stream_index];
    time = stream->duration * av_q2d(stream->time_base);
    size = QFileInfo(path).size();
    bitRate = static_cast<int>(formatCtx->bit_rate);
    sampleRate = sample_rate;
    kind = codecCtx->codec->name;

    AVDictionary *meta = formatCtx->metadata;
    AVDictionaryEntry *tag = nullptr;
    while ((tag = av_dict_get(meta, "", tag, AV_DICT_IGNORE_SUFFIX))){

        if(strncmp(tag->key, "title", 5) == 0){
            title = tag->value;
        }
        else if(strncmp(tag->key, "artist", 6) == 0){
            artist = tag->value;
        }
        else if(strncmp(tag->key, "album", 5) == 0){
            album = tag->value;
        }
        else if(strncmp(tag->key, "major_brand", 11) == 0){
            majorBrand = tag->value;
        }
        else if(strncmp(tag->key, "minor_version", 13) == 0){
            minorVersion = tag->value;
        }
        else if(strncmp(tag->key, "compatible_brands", 7) == 0){
            compatibleBrands = tag->value;
        }
        else if(strncmp(tag->key, "encoder", 7) == 0){
            encoder = tag->value;
        }

    }

    av_frame_free(&frame);
    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
    avcodec_close(codecCtx);
    avcodec_free_context(&codecCtx);

    loaded = true;
}


MAudioEngine::MAudioEngine(QObject *parent) : QObject(parent){
    m_attribMtx = new QMutex();
    m_processMutex = new QMutex();
    m_processCondition = new QWaitCondition();
    m_processThread = nullptr;

    m_mediaStatus = Mpi3::MediaEmpty;
    m_engineStatus = Mpi3::EngineStopped;
    m_errorStatus = Mpi3::NoError;
    m_requestStatus = Mpi3::EngineStopped;
    m_playpauseStatus = Mpi3::EngineActive;

    m_vol_percent = 0.0f;
    m_vol_dbratio = 0.0f;
    m_position = 0.0;
    m_filepath = QString();

    m_formatCtx = nullptr;
    m_codecCtx = nullptr;
    m_aoDevice = nullptr;
    m_streamIdx = -1;
}
MAudioEngine::~MAudioEngine(){
    stop();
    media_dealloc();

    delete m_attribMtx;
    delete m_processMutex;
    delete m_processCondition;
    delete m_processThread;

    delete m_formatCtx;
    delete m_codecCtx;

    std::cout << "THREAD: ENGINE DESTROYED" << std::endl;
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

    m_filepath.clear();
    updateStatus(Mpi3::MediaEmpty);
}
void MAudioEngine::media_alloc(){

    m_streamIdx = -1;

    int error = load_contexts(m_filepath.toStdString(), &m_formatCtx, &m_codecCtx, &m_streamIdx);
    if(error < 0 || m_streamIdx < 0){return;}

    for(uint32_t i = 0; i < m_formatCtx->nb_streams; i++){
        if(m_formatCtx->streams[i]->index != m_streamIdx){
            m_formatCtx->streams[i]->discard = AVDISCARD_ALL;
        }
    }

    m_formatCtx->streams[m_streamIdx]->discard = AVDISCARD_DEFAULT;
    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);

    error = 0;
    int finished = 0;
    int sample_rate = -1;

    AVFrame *frame = av_frame_alloc();
    while((!finished && !error) && sample_rate <= 0){
        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        sample_rate = frame->sample_rate;
    }

    av_frame_free(&frame);

    int default_driver = ao_default_driver_id();
    if(default_driver < 0){
        std::cerr << "ERROR: error finding default driver" << std::endl;
        return;
    }

    ao_info *info = ao_driver_info(default_driver);
    ao_sample_format ao_format;
    memset(&ao_format, 0, sizeof(ao_format));

    ao_format.bits = BIT_DEPTH;
    ao_format.channels = m_codecCtx->channels;
    ao_format.rate = sample_rate;
    ao_format.byte_format = info->preferred_byte_format;
    ao_format.matrix = nullptr;

    m_aoDevice = ao_open_live(default_driver, &ao_format, nullptr);
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
    int bps = av_get_bytes_per_sample(m_codecCtx->sample_fmt);
    error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);

    std::cout << av_get_sample_fmt_name(m_codecCtx->sample_fmt) << std::endl;
    std::cout << av_get_sample_fmt_name(AV_SAMPLE_FMT_S16) << std::endl;


    std::cout << "THREAD: begin loop" << std::endl;
    while(!finished && !error) {

//        if(m_position > 10.00){break;}

        for(int i = 0; i < frame->nb_samples; i++){

            char *buffer = (char*)(frame->data[0] + bps * i);
            ao_play(m_aoDevice, buffer, 4);


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

//            for(int c = 0; c < m_codecCtx->channels; c++){

//                short *data_raw = reinterpret_cast<short*>(frame->data[c] + bps*i);
//                float sample_raw = static_cast<float>(*data_raw) / static_cast<float>(BIT_RANGE);

//                sample_raw *= m_vol_dbratio;
//                sample_raw *= BIT_RANGE;

//                float sample_processed = av_clipf_c(sample_raw, -BIT_RANGE, BIT_RANGE-1);
//                short data_processed = static_cast<short>(sample_processed);

////                int p = ao_play(m_aoDevice, (char*)(frame->data[c] + bps*i), static_cast<uint32_t>(bps));

//                int p = ao_play(m_aoDevice, reinterpret_cast<char*>(&data_processed), static_cast<uint32_t>(bps));

//                if(p==0 && t-- == 0){
//                    std::cerr << "ao_play error" << std::endl;
//                    goto halt;

//                }
//            }
        }

        m_position = frame->pkt_dts * av_q2d(stream->time_base);
        emit notifyPosition(m_position);
        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
    }

halt:
    std::cout << "THREAD: loop ended" << std::endl;
    av_frame_free(&frame);

    std::cout << "THREAD: media_dealloc" << std::endl;
    media_dealloc();

    QThread::currentThread()->quit();
}

void MAudioEngine::open(const QString &path){

    if(empty()){

        std::cout << "OPEN: media_dealloc" << std::endl;
        media_dealloc();

        std::cout << "OPEN: m_filepath" << std::endl;

        m_filepath = path;

        std::cout << "OPEN: media_alloc" << std::endl;
        media_alloc();

    }
}

void MAudioEngine::start(){

    if(ready()){

        if(m_processThread){
            std::cout << "START: delete thread" << std::endl;
            delete m_processThread;
            m_processThread = nullptr;
        }

        if(!m_processThread){
            m_processThread = QThread::create([this](){engine_process();});

            std::cout << "START: update status" << std::endl;
            updateStatus(Mpi3::MediaBusy);
            updateStatus(m_playpauseStatus);
            m_requestStatus = m_playpauseStatus;

            std::cout << "START: start thread" << std::endl;
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
            updateStatus(Mpi3::EngineStopped);
        }
    }
}

void MAudioEngine::play(){
    m_playpauseStatus = Mpi3::EngineActive;
    updateRequest(Mpi3::EngineActive);
}
void MAudioEngine::pause(){
    m_playpauseStatus = Mpi3::EngineIdle;
    updateRequest(Mpi3::EngineIdle);
}

void MAudioEngine::seek(double pos){
    Q_UNUSED(pos);
}
void MAudioEngine::gain(int vol){
    m_vol_percent = av_clipf_c(static_cast<float>(vol)/100.0f, 0.0f, 1.0f);
    m_vol_dbratio = av_clipf_c(log10f(1 - m_vol_percent) / -2.0f, 0.0f, 1.0f);

    emit notifyVolume(vol);
}

QString MAudioEngine::filepath() const {
    return m_filepath;
}

bool MAudioEngine::empty() const {
    return m_mediaStatus == Mpi3::MediaEmpty;
}
bool MAudioEngine::ready() const{
    return m_mediaStatus == Mpi3::MediaReady;
}
bool MAudioEngine::busy() const {
    return m_mediaStatus == Mpi3::MediaBusy;
}

bool MAudioEngine::stopped() const {
    return m_engineStatus == Mpi3::EngineStopped;
}
bool MAudioEngine::playing() const{
    return m_engineStatus == Mpi3::EngineActive;
}
bool MAudioEngine::paused() const{
    return m_engineStatus == Mpi3::EngineIdle;;
}

float MAudioEngine::volume() const{
    return m_vol_percent;
}
double MAudioEngine::position() const{
    return m_position;
}

Mpi3::MediaState MAudioEngine::mediaStatus() const {
    return m_mediaStatus;
}
Mpi3::EngineState MAudioEngine::engineStatus() const {
    return m_engineStatus;
}
Mpi3::ErrorState MAudioEngine::errorStatus() const {
    return m_errorStatus;
}
Mpi3::EngineState MAudioEngine::requestedStatus() const {
    return m_requestStatus;
}

void MAudioEngine::updateStatus(Mpi3::MediaState state){
    m_mediaStatus = state;
    emit notifyMediaStatus(state);
}
void MAudioEngine::updateStatus(Mpi3::EngineState state){
    m_engineStatus = state;
    emit notifyEngineStatus(state);
}
void MAudioEngine::updateStatus(Mpi3::ErrorState state){
    m_errorStatus = state;
    emit notifyErrorStatus(state);
}
void MAudioEngine::updateRequest(Mpi3::EngineState state){
    m_requestStatus = state;

    if(state != Mpi3::EngineIdle){
        m_processCondition->notify_all();
    }

    emit notifyRequestStatus(state);
}
