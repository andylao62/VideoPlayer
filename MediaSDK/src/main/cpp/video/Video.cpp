/**
 * 视频信息
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "Video.h"

Video::Video(PlayStatus *playStatus) {
    queue = new AVPacketBlockQueue(playStatus);
}

Video::~Video() {
    codecPar = NULL;
    audio = NULL;
}

void Video::release() {
    if (queue != NULL) {
        queue->clear();
        delete(queue);
        queue = NULL;
    }
    if (avCodecContext != NULL) {
        avcodec_close(avCodecContext);
        avcodec_free_context(&avCodecContext);
        av_free(avCodecContext);
        avCodecContext = NULL;
    }
}
