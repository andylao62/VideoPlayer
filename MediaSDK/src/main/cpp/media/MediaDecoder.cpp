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
            LOGE("MediaDecoder#openAVCodec, find AVCodecParameters failure");
        }
        // 既没有发现音频流，也没发现视频流，执行失败
        return ERROR_AV_MEDIA_TYPE_NOT_FOUND;
    }

    return 0;
}
