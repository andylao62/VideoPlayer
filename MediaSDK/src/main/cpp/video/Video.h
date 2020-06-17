/**
 * 视频信息
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_VIDEO_H
#define VIDEOPLAYER_VIDEO_H
#include "AVPacketBlockQueue.h"

class Video {
    public:
        int streamIndex = -1;
        AVCodecParameters *codecPar = NULL;
        AVCodecContext *avCodecContext = NULL;
        AVPacketBlockQueue *queue = NULL;
        AVRational timeBase;
    public:
        Video(PlayStatus *playStatus);
        ~Video();

        void release();
};


#endif //VIDEOPLAYER_VIDEO_H
