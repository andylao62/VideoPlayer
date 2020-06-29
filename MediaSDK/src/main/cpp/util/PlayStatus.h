/**
 * 播放器状态
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#ifndef AUDIOPLAYER_PLAYSTATUS_H
#define AUDIOPLAYER_PLAYSTATUS_H


#include <pthread.h>

class PlayStatus {
    private:
        pthread_mutex_t mutexStatus;
        pthread_mutex_t mutexPause;
        pthread_mutex_t mutexSeek;
        bool seek = false;
    public:
        bool exit = false;
        bool load = true;
        bool pause = false;

    public:
        PlayStatus();
        ~PlayStatus();
        bool isExit();
        bool isLoad();
        bool isSeek();
        bool isPause();
        void setExit(bool exit);
        void setLoad(bool load);
        void setSeek(bool seek);
        void setPause(bool pause);
};


#endif //AUDIOPLAYER_PLAYSTATUS_H
