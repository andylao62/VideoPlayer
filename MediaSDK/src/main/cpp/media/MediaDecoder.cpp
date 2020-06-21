/**
 * 媒体解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "MediaDecoder.h"

MediaDecoder::MediaDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->media = media;
    this->audioDecoder = new AudioDecoder(javaCaller, playStatus, media->audio);
    this->videoDecoder = new VideoDecoder(javaCaller, playStatus, media->video);
}

MediaDecoder::~MediaDecoder() {
    javaCaller = NULL;
    playStatus = NULL;
    media = NULL;
}

int MediaDecoder::openAVCodec() {
    for (int i = 0, size = media->avFormatContext->nb_streams; i < size; i++) {
        AVStream *streams = media->avFormatContext->streams[i];
        switch (streams->codecpar->codec_type) {
            case AVMEDIA_TYPE_AUDIO:
                // TODO 此处需要优化：有的视频，有国语、粤语之类的音频
                audioDecoder->fill(i, streams, media->avFormatContext->duration);
                break;
            case AVMEDIA_TYPE_VIDEO:
                videoDecoder->fill(i, streams, media->avFormatContext->duration);
                break;
            default:
                break;
        }
    }
    if (media->audio->streamIndex == -1 && media->video->streamIndex == -1) {
        if (LOG_DEBUG) {
            LOGE("MediaDecoder::openAVCodec, find AVCodecParameters failure");
        }
        // 既没有发现音频流，也没发现视频流，执行失败
        return ERROR_AV_MEDIA_TYPE_NOT_FOUND;
    }
    if (LOG_DEBUG) {
        LOGE("MediaDecoder::openAVCodec, get audio av codec context，id: %d", media->audio->codecPar->codec_id);
    }
    if (media->audio->streamIndex >= 0) {
        int errCode = getAVCodecContext(media->audio->codecPar, &media->audio->avCodecContext);
        if (errCode != 0) {
            return errCode;
        }
    }
    if (LOG_DEBUG) {
        LOGE("MediaDecoder::openAVCodec, get video av codec context");
    }
    if (media->video->streamIndex >= 0) {
        int errCode = getAVCodecContext(media->video->codecPar, &media->video->avCodecContext);
        if (errCode != 0) {
            return errCode;
        }
    }
    return 0;
}

int MediaDecoder::getAVCodecContext(AVCodecParameters *codecPar, AVCodecContext **avCodecContext) {
    AVCodec *decoder = avcodec_find_decoder(codecPar->codec_id);
    if (!decoder) {
        if (LOG_DEBUG) {
            LOGE("MediaDecoder::getAVCodecContext, avcodec_find_decoder failure, codec_id: %d", codecPar->codec_id);
        }
        return ERROR_AV_CODEC_DECODER_NOT_FOUND;
    }
    *avCodecContext = avcodec_alloc_context3(decoder);
    if (!*avCodecContext) {
        if (LOG_DEBUG) {
            LOGE("MediaDecoder::getAVCodecContext, avcodec_alloc_context3 failure, codec_id: %d", codecPar->codec_id);
        }
        return ERROR_AV_CODEC_ALLOC_CONTEXT3;
    }
    if (avcodec_parameters_to_context(*avCodecContext, codecPar) < 0) {
        if (LOG_DEBUG) {
            LOGE("MediaDecoder::getAVCodecContext, avcodec_parameters_to_context failure, codec_id: %d", codecPar->codec_id);
        }
        return ERROR_AV_CODEC_PARAMETERS_CONTEXT;
    }
    if (avcodec_open2(*avCodecContext, decoder, 0) != 0) {
        if (LOG_DEBUG) {
            LOGE("MediaDecoder::getAVCodecContext, avcodec_open2 failure, codec_id: %d", codecPar->codec_id);
        }
        return ERROR_AV_CODEC_OPEN2;
    }
    return 0;
}

void MediaDecoder::decode() {
    if (LOG_DEBUG) {
        LOGD("MediaDecoder::decode()， 开始解码");
    }
    while (playStatus != NULL && !playStatus->isExit()) {
        if (media->avFormatContext == NULL) {
            break;
        }
        if (playStatus->isSeek()) {
            av_usleep(1000 * 100);
            continue;
        }
        if (media->audio->queue->size() > 40) {
            av_usleep(1000 * 100);
            continue;
        }
        AVPacket *avPacket = av_packet_alloc();
        if(av_read_frame(media->avFormatContext, avPacket) == 0) {
            if (avPacket->stream_index == media->audio->streamIndex) {
                media->audio->queue->push(avPacket);
            } else if (avPacket->stream_index == media->video->streamIndex) {
                media->video->queue->push(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            while (playStatus != NULL && !playStatus->isExit()) {
                if (media->audio->queue->size() > 0) {
                    av_usleep(1000 * 100);
                    continue;
                }
                if (playStatus != NULL && !playStatus->isSeek()) {
                    av_usleep(1000 * 100);
                    playStatus->setExit(true);
                }
                break;
            }
        }
    }
    if (LOG_DEBUG) {
        LOGD("MediaDecoder::decode()，解码完成。");
    }
}

void MediaDecoder::release() {

}
