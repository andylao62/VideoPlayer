/**
 * 媒体输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "MediaOutput.h"

MediaOutput::MediaOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->media = media;
}

MediaOutput::~MediaOutput() {
    javaCaller = NULL;
    playStatus = NULL;
    media = NULL;
}
