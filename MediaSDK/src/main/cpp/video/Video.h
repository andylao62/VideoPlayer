/**
 * 视频信息
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_VIDEO_H
#define VIDEOPLAYER_VIDEO_H

#include "Audio.h"
#include "AVPacketBlockQueue.h"

class Video {
    public:
        int streamIndex = -1;
        AVCodecParameters *codecPar = NULL;
        AVCodecContext *avCodecContext = NULL;
        AVPacketBlockQueue *queue = NULL;
        AVRational timeBase;
        double clock = 0;
        double delayTime = 0;
        double defaultDelayTime = 0.04;
        Audio *audio = NULL;
    public:
        Video(PlayStatus *playStatus);
        ~Video();

        void release();
};


#endif //VIDEOPLAYER_VIDEO_H
