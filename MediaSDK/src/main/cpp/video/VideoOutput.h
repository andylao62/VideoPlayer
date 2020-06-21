/**
 * 视频输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_VIDEOOUTPUT_H
#define VIDEOPLAYER_VIDEOOUTPUT_H

extern "C" {
    #include <libavutil/time.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
};

#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Video.h"

class VideoOutput {
    public:
        JavaCaller *javaCaller = NULL;
        PlayStatus *playStatus = NULL;
        Video *video = NULL;
        pthread_t threadPlay;
        pthread_mutex_t mutexDecode;
    public:
        VideoOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video);
        ~VideoOutput();

        /**
         * 将原AVFrame转换为YUV420PFrame
         * @param avFrame 原AVFrame
         * @return YUV420PFrame
         */
        AVFrame *swsToYUV420P(AVFrame *avFrame);
        double getFrameDiffTime(AVFrame *avFrame);
        double getDelayTime(double diff);
        void play();
        void release();
};


#endif //VIDEOPLAYER_VIDEOOUTPUT_H
