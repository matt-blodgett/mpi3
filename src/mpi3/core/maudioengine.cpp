// MOVE FROM CORE TO UTILS

#include "mpi3/core/maudioengine.h"
#include <iostream>

#include <QFileInfo>
#include <QMap>
#include <QVariant>


extern "C"
{
#include <libavformat/avformat.h>
}


static int decode_audio_frame(AVFrame *frame, AVFormatContext *format_ctx, AVCodecContext *codec_ctx, int *finished)
{
    int error = 0;

    AVPacket pckt;
    av_init_packet(&pckt);

    error = av_read_frame(format_ctx, &pckt);
    if(error < 0) {

        if(error == AVERROR_EOF) {
            *finished = 1;
        }
        else {
            std::cerr << "ERROR: could not read frame" << std::endl;
            return error;
        }
    }

    error = avcodec_send_packet(codec_ctx, &pckt);
    if(error < 0) {
        std::cerr << "ERROR: could not send packet for decoding" << std::endl;
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
        std::cerr << "ERROR: could not decode frame" << std::endl;
    }

    av_packet_unref(&pckt);
    return error;
}
static int load_contexts(std::string filepath, AVFormatContext **formatCtx, AVCodecContext **codecCtx, int *streamIdx)
{
    int error = 0;

    error = avformat_open_input(formatCtx, filepath.c_str(), nullptr, nullptr);
    if(error < 0) {
        std::cerr << "ERROR: error opening input file" << std::endl;
        return error;
    }

    error = avformat_find_stream_info(*formatCtx, nullptr);
    if(error < 0) {
        std::cerr << "ERROR: error finding stream info" << std::endl;
        return error;
    }

    AVCodec *codec;
    *streamIdx = av_find_best_stream(
        *formatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, &codec, 0);

    if(*streamIdx == AVERROR_STREAM_NOT_FOUND) {
        std::cerr << "ERROR: error finding audio stream" << std::endl;
        return error;
    }
    else if(*streamIdx == AVERROR_DECODER_NOT_FOUND) {
        std::cerr << "ERROR: error finding decoder" << std::endl;
        return error;
    }
    else if(!codec) {
        std::cerr << "ERROR: error loading codec" << std::endl;
        return error;
    }

    if(strncmp(codec->name, "mp3float", 8) == 0) {
        codec = avcodec_find_decoder_by_name("mp3");
    }

    *codecCtx = avcodec_alloc_context3(codec);
    if(!codecCtx) {
        std::cerr << "ERROR: error allocating codec context";
        return error;
    }

    error = avcodec_open2(*codecCtx, codec, nullptr);
    if(error < 0) {
        std::cerr << "ERROR: error opening codec";
        return error;
    }

    return error;
}


bool MSongInfo::load(const QString &path)
{
    this->path = path;

    AVFormatContext *formatCtx = nullptr;
    AVCodecContext *codecCtx = nullptr;

    int stream_index = -1;
    int error = load_contexts(path.toStdString(), &formatCtx, &codecCtx, &stream_index);
    if(error < 0 || stream_index < 0) {
        return false;
    }

    AVFrame *frame = av_frame_alloc();

    error = 0;
    int finished = 0;
    int sample_rate = -1;
    while((!finished && !error) && sample_rate <= 0) {
        error = decode_audio_frame(frame, formatCtx, codecCtx, &finished);
        sample_rate = frame->sample_rate;
    }

    AVStream *stream = formatCtx->streams[stream_index];

    time = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
    size = static_cast<double>(QFileInfo(path).size());
    bitRate = static_cast<int>(formatCtx->bit_rate);
    sampleRate = sample_rate;
    kind = codecCtx->codec->name;

    AVDictionary *meta = formatCtx->metadata;
    AVDictionaryEntry *tag = nullptr;

    while ((tag = av_dict_get(meta, "", tag, AV_DICT_IGNORE_SUFFIX))) {
        if(strncmp(tag->key, "title", 5) == 0) {title = tag->value;}
        else if(strncmp(tag->key, "artist", 6) == 0) {artist = tag->value;}
        else if(strncmp(tag->key, "album", 5) == 0) {album = tag->value;}
        else if(strncmp(tag->key, "major_brand", 11) == 0) {majorBrand = tag->value;}
        else if(strncmp(tag->key, "minor_version", 13) == 0) {minorVersion = tag->value;}
        else if(strncmp(tag->key, "compatible_brands", 7) == 0) {compatibleBrands = tag->value;}
        else if(strncmp(tag->key, "encoder", 7) == 0) {encoder = tag->value;}
    }

    av_frame_free(&frame);
    avformat_close_input(&formatCtx);
    avformat_free_context(formatCtx);
    avcodec_close(codecCtx);
    avcodec_free_context(&codecCtx);

    return true;
}
QMap<QString, QVariant> MSongInfo::songInfoMap()
{
    QMap<QString, QVariant> map;
    map["path"] = QVariant(path);
    map["title"] = QVariant(title);
    map["artist"] = QVariant(artist);
    map["album"] = QVariant(album);
    map["kind"] = QVariant(kind);
    map["time"] = QVariant(time);
    map["size"] = QVariant(size);
    map["bitRate"] = QVariant(bitRate);
    map["sampleRate"] = QVariant(sampleRate);
    map["majorBrand"] = QVariant(majorBrand);
    map["minorVersion"] = QVariant(minorVersion);
    map["compatibleBrands"] = QVariant(compatibleBrands);
    map["encoder"] = QVariant(encoder);
    return map;
}
