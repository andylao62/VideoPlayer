/**
 * 媒体解加载器：
 * 1、或加载本地媒体文件；
 * 2、或加载远程媒体文件；
 * 3、或读取当前设备麦克风、摄像头
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include <ErrorCode.h>
#include "Log.h"
#include "MediaLoader.h"

MediaLoader::MediaLoader(PlayStatus *playStatus, Media *media) {
    this->playStatus = playStatus;
    this->media = media;
}

MediaLoader::~MediaLoader() {
    playStatus = NULL;
    media = NULL;
}

int avFormatCallback(void *ctx) {
    MediaLoader *decoder = (MediaLoader*) ctx;
    if (decoder->playStatus == NULL || decoder->playStatus->isExit()) {
        return AVERROR_EOF;
    }
    return 0;
}

int MediaLoader::load() {
    av_register_all();
    avformat_network_init();
    media->avFormatContext = avformat_alloc_context();
    media->avFormatContext->interrupt_callback.callback = avFormatCallback;
    media->avFormatContext->interrupt_callback.opaque = this;
    if (avformat_open_input(&media->avFormatContext, media->source, NULL, NULL) != 0) {
        if (LOG_DEBUG) {
            LOGE("MediaLoader#load, avformat_open_input failure");
        }
        return ERROR_AV_FORMAT_OPEN_INPUT;
    }
    if (avformat_find_stream_info(media->avFormatContext, NULL) < 0) {
        if (LOG_DEBUG) {
            LOGE("MediaLoader#load, avformat_find_stream_info failure");
        }
        return ERROR_AV_STREAM_FORMAT_NOT_FOUND;
    }
    return 0;
}
