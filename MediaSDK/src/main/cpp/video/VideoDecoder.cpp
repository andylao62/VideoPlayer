/**
 * 视频解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->video = video;
}

VideoDecoder::~VideoDecoder() {
    this->javaCaller = NULL;
    this->playStatus = NULL;
    this->video = NULL;
}

void VideoDecoder::fill(int streamIndex, AVStream *streams, int64_t duration) {
}
