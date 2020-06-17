/**
 * 播放控制器
 * 1、协调解码器解码；
 * 2、媒体信息存储；
 * 3、协调输出设备重采样及播放
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#include "MediaPlayerController.h"

MediaPlayerController::MediaPlayerController(JavaVM *javaVM, JNIEnv *env, jobject *instance) {
    this->javaCaller = new JavaCaller(javaVM, env, instance);
    pthread_mutex_init(&mutexReleasing, NULL);
    pthread_mutex_init(&mutexWorking, NULL);
    pthread_mutex_init(&mutexDecode, NULL);
}

MediaPlayerController::~MediaPlayerController() {
    pthread_mutex_destroy(&mutexReleasing);
    pthread_mutex_destroy(&mutexWorking);
    pthread_mutex_destroy(&mutexDecode);
}

bool MediaPlayerController::isReleasing() {
    bool result;
    pthread_mutex_lock(&mutexReleasing);
    result = releasing;
    pthread_mutex_unlock(&mutexReleasing);
    return result;
}

void MediaPlayerController::setReleasing(bool isReleasing) {
    pthread_mutex_lock(&mutexReleasing);
    releasing = isReleasing;
    pthread_mutex_unlock(&mutexReleasing);
}


bool MediaPlayerController::isWorking() {
    bool result;
    pthread_mutex_lock(&mutexWorking);
    result = working;
    pthread_mutex_unlock(&mutexWorking);
    return result;
}

void MediaPlayerController::setWorking(bool working) {
    pthread_mutex_lock(&mutexWorking);
    this->working = working;
    pthread_mutex_unlock(&mutexWorking);
}

void *prepareCallback(void *data) {
    MediaPlayerController *ctrl = (MediaPlayerController *) data;
    pthread_mutex_lock(&ctrl->mutexDecode);
    int result = ctrl->loader->load();
    if (result == 0) {

    } else {
        ctrl->javaCaller->callJavaMethod(true, EVENT_ERROR, result, 0);
        ctrl->exit = true;
        pthread_mutex_unlock(&ctrl->mutexDecode);
    }
    pthread_exit(&ctrl->threadDecode);
}

void MediaPlayerController::prepare(const char *source) {
    if (isReleasing()) {
        return;
    }
    if (isWorking()) {
        return;
    }
    setWorking(true);
    playStatus = new PlayStatus();
    playStatus->setExit(false);
    media = new Media(source, playStatus);
    decoder = new MediaDecoder(javaCaller, playStatus, media);
    output = new MediaOutput(javaCaller, playStatus, media);
    loader = new MediaLoader(playStatus, media);
    pthread_create(&threadDecode, NULL, prepareCallback, this);
}

void MediaPlayerController::start() {
    if (isReleasing()) {
        return;
    }
    if (audio == NULL) {
        return;
    }
    if (audioOutput == NULL) {
        return;
    }
    audioOutput->play();
    audioDecoder->decodeAsync();
}


void MediaPlayerController::seekByPercent(float percent) {
    if (audioDecoder == NULL) {
        return;
    }
    audioDecoder->seekByPercent(percent);
}


void MediaPlayerController::seek(int second) {
    if (audioDecoder == NULL) {
        return;
    }
    audioDecoder->seek(second);
}

void MediaPlayerController::resume() {
    if (isReleasing()) {
        return;
    }
    if (audioOutput == NULL) {
        return;
    }
    audioOutput->resume();
}

void MediaPlayerController::pause() {
    if (isReleasing()) {
        return;
    }
    if (audioOutput == NULL) {
        return;
    }
    audioOutput->pause();
}

void MediaPlayerController::stop() {
    if (isReleasing()) {
        return;
    }
    if (!isWorking()) {
        return;
    }
    if (audioOutput == NULL) {
        return;
    }
    audioOutput->stop();
}

void MediaPlayerController::setVolume(float percent) {
    if (isReleasing()) {
        return;
    }
    if (!isWorking()) {
        return;
    }
    if (audioOutput == NULL) {
        return;
    }
    audioOutput->setVolume(percent);
}

void MediaPlayerController::release() {
    if (isReleasing()) {
        return;
    }
    if (!isWorking()) {
        return;
    }
    setReleasing(true);
    playStatus->setExit(true);
    if (audioOutput != NULL) {
        audioOutput->release();
    }
    if (audioDecoder != NULL) {
        audioDecoder->release();
    }
    if (audio != NULL) {
        audio->release();
    }
    if (audioOutput != NULL) {
        delete(audioOutput);
        audioOutput = NULL;
    }
    if (audioDecoder != NULL) {
        delete(audioDecoder);
        audioDecoder = NULL;
    }
    if (audio != NULL) {
        delete(audio);
        audio = NULL;
    }
    if (playStatus != NULL) {
        delete(playStatus);
        playStatus = NULL;
    }
    setReleasing(false);
    setWorking(false);
    if (LOG_DEBUG) {
        LOGD("MediaPlayerController::release() 完成。");
    }
}




