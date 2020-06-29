/**
 * 媒体输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIAOUTPUT_H
#define VIDEOPLAYER_MEDIAOUTPUT_H

#include "AudioOutput.h"
#include "VideoOutput.h"
#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Media.h"

class MediaOutput {
    private:
        JavaCaller *javaCaller = NULL;
        PlayStatus *playStatus = NULL;
        Media *media = NULL;
        AudioOutput *audioOutput = NULL;
        VideoOutput *videoOutput = NULL;
        pthread_mutex_t mutexSeek;

    public:
        MediaOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media);
        ~MediaOutput();

        void play();
        void pause();
        void resume();
        void stop();
        void setVolume(float percent);
        void seekByPercent(float percent);
        void seek(int64_t second);
        void release();
};


#endif //VIDEOPLAYER_MEDIAOUTPUT_H
