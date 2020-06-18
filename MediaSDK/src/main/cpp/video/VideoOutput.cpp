/**
 * 视频输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "VideoOutput.h"

VideoOutput::VideoOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->video = video;
}

VideoOutput::~VideoOutput() {
    this->javaCaller = NULL;
    this->playStatus = NULL;
    this->video = NULL;
}
