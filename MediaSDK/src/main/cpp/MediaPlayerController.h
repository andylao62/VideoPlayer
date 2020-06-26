/**
 * 播放控制器
 * 1、协调解码器解码；
 * 2、音频信息存储；
 * 3、协调输出设备重采样及播放
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#ifndef AUDIOPLAYER_AUDIOPLAYERCONTROLLER_H
#define AUDIOPLAYER_AUDIOPLAYERCONTROLLER_H


#include "MediaLoader.h"
#include "MediaDecoder.h"
#include "MediaOutput.h"
#include "Media.h"
#include "Audio.h"
#include "PlayStatus.h"
#include "AudioDecoder.h"
#include "AudioOutput.h"
#include "JavaCaller.h"

class MediaPlayerController {
    private:
        pthread_t threadStart;
        pthread_t threadDecode;
        pthread_mutex_t mutexReleasing;
        pthread_mutex_t mutexWorking;
        bool releasing = false;
        bool working = false;

    public:
        Media *media = NULL;
        PlayStatus *playStatus = NULL;
        JavaCaller *javaCaller = NULL;
        pthread_mutex_t mutexDecode;
        MediaLoader *loader = NULL;
        MediaDecoder *decoder = NULL;
        MediaOutput *output = NULL;
        bool exit = false;
    private:
        bool isReleasing();
        void setReleasing(bool releasing);
        bool isWorking();
        void setWorking(bool working);

    public:
        MediaPlayerController(JavaVM *javaVM, JNIEnv *env, jobject *instance);
        ~MediaPlayerController();
        void prepare(const char *source);
        void start();
        void seekByPercent(float percent);
        void seek(int second);
        void resume();
        void pause();
        void stop();
        void release();
        void decode();
        void setVolume(float percent);
};


#endif //AUDIOPLAYER_AUDIOPLAYERCONTROLLER_H
