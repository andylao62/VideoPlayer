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

//    AVDictionary *optionsDict = NULL;
//    if(Rtsp_Protocol == TCP) {
//        av_dict_set(&optionsDict, "rtsp_transport", "tcp", 0); // 采用tcp传输
//    }
//    av_dict_set(&optionsDict, "stimeout", "5000000", 0); // 设置rtsp超时5s，单位微妙


    int ret = avformat_open_input(&media->avFormatContext, media->source, NULL, NULL);
    if (ret != 0) {
        if (LOG_DEBUG) {
            char msg[512];
            av_make_error_string(msg, 512, ret);
            LOGE("MediaLoader#load, avformat_open_input failure, ret: %d, msg: %s, source: %s", ret, msg, media->source)
        }
        return ERROR_AV_FORMAT_OPEN_INPUT;
    }
    ret = avformat_find_stream_info(media->avFormatContext, NULL);
    if (ret < 0) {
        if (LOG_DEBUG) {
            LOGE("MediaLoader#load, avformat_find_stream_info failure, source: %s, ret: %d", media->source, ret)
        }
        return ERROR_AV_STREAM_FORMAT_NOT_FOUND;
    }
    return 0;
}
