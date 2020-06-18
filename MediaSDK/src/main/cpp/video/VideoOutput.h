/**
 * 视频输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_VIDEOOUTPUT_H
#define VIDEOPLAYER_VIDEOOUTPUT_H


#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Video.h"

class VideoOutput {
    public:
        JavaCaller *javaCaller = NULL;
        PlayStatus *playStatus = NULL;
        Video *video = NULL;
    public:
        VideoOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video);
        ~VideoOutput();
};


#endif //VIDEOPLAYER_VIDEOOUTPUT_H
