/**
 * 播放器状态
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#include "PlayStatus.h"

PlayStatus::PlayStatus() {
    pthread_mutex_init(&mutexStatus, NULL);
    pthread_mutex_init(&mutexSeek, NULL);
    pthread_mutex_init(&mutexPause, NULL);
}

PlayStatus::~PlayStatus() {
    pthread_mutex_destroy(&mutexSeek);
    pthread_mutex_destroy(&mutexStatus);
    pthread_mutex_destroy(&mutexPause);
}

bool PlayStatus::isExit() {
    bool result;
    pthread_mutex_lock(&mutexStatus);
    result = exit;
    pthread_mutex_unlock(&mutexStatus);
    return result;
}

bool PlayStatus::isLoad() {
    bool result;
    pthread_mutex_lock(&mutexStatus);
    result = load;
    pthread_mutex_unlock(&mutexStatus);
    return result;
}

void PlayStatus::setExit(bool exit) {
    pthread_mutex_lock(&mutexStatus);
    this->exit = exit;
    pthread_mutex_unlock(&mutexStatus);
}

void PlayStatus::setLoad(bool load) {
    pthread_mutex_lock(&mutexStatus);
    this->load = load;
    pthread_mutex_unlock(&mutexStatus);
}

bool PlayStatus::isSeek() {
    bool result;
    pthread_mutex_lock(&mutexSeek);
    result = seek;
    pthread_mutex_unlock(&mutexSeek);
    return result;
}

void PlayStatus::setSeek(bool seek) {
    pthread_mutex_lock(&mutexSeek);
    this->seek = seek;
    pthread_mutex_unlock(&mutexSeek);
}

bool PlayStatus::isPause() {
    bool result;
    pthread_mutex_lock(&mutexPause);
    result = pause;
    pthread_mutex_unlock(&mutexPause);
    return result;
}

void PlayStatus::setPause(bool pause) {
    pthread_mutex_lock(&mutexPause);
    this->pause = pause;
    pthread_mutex_unlock(&mutexPause);
}
