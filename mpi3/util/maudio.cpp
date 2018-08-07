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


MAudioEngine::MAudioEngine(QObject *parent) : QObject(parent){
    m_loaded = false;
    m_active = false;
    m_paused = false;
    m_volume = 0.0f;
    m_decibels = -30.0f;
    m_position = 0.0;

    av_register_all();
    avformat_network_init();
    ao_initialize();

    av_log_set_level(AV_LOG_VERBOSE); // AV_LOG_QUIET
}
MAudioEngine::~MAudioEngine(){

    m_active = false;

    if(m_decodeThread.joinable()){
        m_decodeThread.join();
    }

    if(m_formatCtx){
        avformat_close_input(&m_formatCtx);
        avformat_free_context(m_formatCtx);
    }

    if(m_codecCtx){
        avcodec_close(m_codecCtx);
        avcodec_free_context(&m_codecCtx);
    }

    avformat_network_deinit();
    ao_shutdown();

    m_loaded = false;
}

bool MAudioEngine::load(std::string filepath){

    m_loaded = false;

    if(m_active){
        stop();
    }


    int error;


    //---------------------------------------------------------------------------
    if(m_formatCtx){
        avformat_close_input(&m_formatCtx);
    }

    if(m_codecCtx){
        avcodec_close(m_codecCtx);
    }

    error = avformat_open_input(&m_formatCtx, filepath.c_str(), nullptr, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening input file";
        avformat_close_input(&m_formatCtx);
        return false;
    }


    //---------------------------------------------------------------------------
    error = avformat_find_stream_info(m_formatCtx, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error finding stream info" << std::endl;
        avformat_close_input(&m_formatCtx);
        return false;
    }


    //---------------------------------------------------------------------------
    AVCodec *codec;
    m_codecCtx = avcodec_alloc_context3(codec);
    if(!m_codecCtx){
        std::cerr << "ERROR: error allocating codec context";
        avformat_close_input(&m_formatCtx);
        return false;
    }


    //---------------------------------------------------------------------------
    m_streamIndex = av_find_best_stream(
        m_formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);

    if(m_streamIndex == AVERROR_STREAM_NOT_FOUND){
        std::cerr << "ERROR: stream not found" << std::endl;
        return false;
    }
    else if(m_streamIndex == AVERROR_DECODER_NOT_FOUND){
        std::cerr << "ERROR: decoder not found" << std::endl;
        return false;
    }


    //---------------------------------------------------------------------------
    error = avcodec_open2(m_codecCtx, codec, nullptr);
    if(error < 0){
        std::cerr << "ERROR: error opening codec";
        avformat_close_input(&m_formatCtx);
        return false;
    }


    //---------------------------------------------------------------------------
    for(unsigned int i = 0; i < m_formatCtx->nb_streams; i++){
        if(m_formatCtx->streams[i]->index != m_streamIndex){
            m_formatCtx->streams[i]->discard = AVDISCARD_ALL;
        }
    }

    m_formatCtx->streams[m_streamIndex]->discard = AVDISCARD_DEFAULT;

    m_loaded = true;
    return true;
}

void MAudioEngine::start(){
    if(!m_loaded){
        std::cerr << "ERROR: file or codec not loaded" << std::endl;
        return;
    }

    if(m_active){
        std::cerr << "ERROR: already reading a file" << std::endl;
        return;
    }

    if(!m_formatCtx){
        std::cerr << "ERROR: no format context" << std::endl;
        return;
    }

    if(!m_codecCtx){
        std::cerr << "ERROR: no codec context" << std::endl;
        return;
    }

    if(!m_active && m_decodeThread.joinable()){
        m_decodeThread.join();
    }

    m_decodeThread = std::thread(&MAudioEngine::beginDecoding, this);
}
void MAudioEngine::stop(){
    m_loaded = false;
    m_active = false;

    if(m_decodeThread.joinable()){
        m_decodeThread.join();
    }
}

void MAudioEngine::play(){
    m_paused = false;
}
void MAudioEngine::pause(){
    m_paused = true;
}

void MAudioEngine::setVolume(float vol){
    m_volume = av_clipf_c(vol, 0.0f, 1.0f);
    m_decibels = av_clipf_c(10.0f * log(m_volume), VOL_DB_FLOOR, VOL_DB_CIEL);
}

bool MAudioEngine::loaded(){
    return m_loaded;
}
bool MAudioEngine::active(){
    return m_active;
}
bool MAudioEngine::paused(){
    return m_paused;
}
float MAudioEngine::volume(){
    return m_volume;
}
double MAudioEngine::position(){
    return m_position;
}

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

void MAudioEngine::beginDecoding(){

    m_active = true;
    m_position = 0.0;

    int finished = 0;
    int error = 0;


    //---------------------------------------------------------------------------
    avformat_seek_file(m_formatCtx, 0, 0, 0, 0, 0);

    AVStream *stream = m_formatCtx->streams[m_streamIndex];
    AVFrame *frame = av_frame_alloc();


    //---------------------------------------------------------------------------
    int sample_rate = -1;
    while((!finished && !error) && sample_rate <= 0){
        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
        sample_rate = frame->sample_rate;
    }

    std::cout << "duration: " << stream->duration * av_q2d(stream->time_base) << std::endl;
    std::cout << "sample rate: " << sample_rate << std::endl;
    std::cout << "bit rate: " << m_formatCtx->bit_rate << std::endl;

    AVDictionaryEntry *tag;
    while ((tag = av_dict_get(m_formatCtx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX))){
        std::cout << tag->key << " = " << tag->value << std::endl;
    }

    std::cout << std::endl;


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

    ao_device *ao_device;
    ao_device = ao_open_live(default_driver, &output_format, nullptr);
    if(!ao_device){
        std::cerr << "ERROR: error opening output device" << std::endl;
        error = -1;
    }


    //---------------------------------------------------------------------------
    int bps = av_get_bytes_per_sample(m_codecCtx->sample_fmt);

    while((!finished && !error) && m_active) {

        m_position = frame->pkt_dts * av_q2d(stream->time_base);
        emit updatePosition(m_position);

        while(m_paused && m_active){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if(!m_active || error){
            break;
        }

        for (int i = 0; i < frame->nb_samples; i++){

            for (int ch = 0; ch < m_codecCtx->channels; ch++){

                short *data_raw = reinterpret_cast<short*>(frame->data[ch] + bps*i);
                float sample_raw = static_cast<float>(*data_raw) / static_cast<float>(BIT_RANGE);

                sample_raw *= m_volume;
                sample_raw *= BIT_RANGE;

                float sample_processed = av_clipf_c(sample_raw, -BIT_RANGE, BIT_RANGE-1);
                short data_processed = static_cast<short>(sample_processed);

                ao_play(ao_device, reinterpret_cast<char*>(&data_processed), static_cast<uint32_t>(bps));

            }
        }

        error = decode_audio_frame(frame, m_formatCtx, m_codecCtx, &finished);
    }


    //---------------------------------------------------------------------------
    avcodec_send_packet(m_codecCtx, nullptr);

    error = 0;
    while(error != AVERROR_EOF){
        error = avcodec_receive_frame(m_codecCtx, frame);
    }

    avcodec_flush_buffers(m_codecCtx);
    avcodec_close(m_codecCtx);

    av_frame_free(&frame);
    avformat_close_input(&m_formatCtx);

    ao_close(ao_device);

    m_loaded = false;
    m_active = false;
    m_position = 0.0;
}






