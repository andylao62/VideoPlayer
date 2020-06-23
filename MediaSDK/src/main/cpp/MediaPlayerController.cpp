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
        result = ctrl->decoder->openAVCodec();
    }
    if (result == 0) {
        ctrl->javaCaller->callJavaMethod(
                true,
                EVENT_PREPARED,
                ctrl->media->video->avCodecContext->width,
                ctrl->media->video->avCodecContext->height
                );
        if (ctrl->playStatus == NULL || ctrl->playStatus->isExit()) {
            ctrl->exit = true;
        }
    } else {
        ctrl->javaCaller->callJavaMethod(true, EVENT_ERROR, result, 0);
        ctrl->exit = true;
    }
    pthread_mutex_unlock(&ctrl->mutexDecode);
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

void *startCallback(void *data) {
    MediaPlayerController *ctrl = (MediaPlayerController*) data;
    ctrl->decoder->tryInitMediaCoder();
    ctrl->output->play();
    ctrl->decode();
    return 0;
}

void MediaPlayerController::start() {
    if (isReleasing()) {
        return;
    }
    if (media == NULL) {
        return;
    }
    if (output == NULL) {
        return;
    }
    if (decoder == NULL) {
        return;
    }
    pthread_create(&threadStart, NULL, startCallback, this);
}



void MediaPlayerController::seekByPercent(float percent) {
    if (output == NULL) {
        return;
    }
    output->seekByPercent(percent);
}


void MediaPlayerController::seek(int second) {
    if (output == NULL) {
        return;
    }
    output->seek(second);
}

void MediaPlayerController::resume() {
    if (isReleasing()) {
        return;
    }
    if (output == NULL) {
        return;
    }
    output->resume();
}

void MediaPlayerController::pause() {
    if (isReleasing()) {
        return;
    }
    if (output == NULL) {
        return;
    }
    output->pause();
}

void MediaPlayerController::stop() {
    if (isReleasing()) {
        return;
    }
    if (!isWorking()) {
        return;
    }
    if (output == NULL) {
        return;
    }
    output->stop();
}

void MediaPlayerController::setVolume(float percent) {
    if (isReleasing()) {
        return;
    }
    if (!isWorking()) {
        return;
    }
    if (output == NULL) {
        return;
    }
    output->setVolume(percent);
}

void *decodeCallback(void *data) {
    MediaPlayerController *ctrl = (MediaPlayerController *) data;
    ctrl->decoder->decode();
    if (ctrl->playStatus != NULL && !ctrl->playStatus->isExit()) {
        if (ctrl->javaCaller != NULL) {
            ctrl->javaCaller->callJavaMethod(true, EVENT_COMPLETE, 0, 0);
        }
    }
    ctrl->exit = true;
    pthread_exit(&ctrl->threadDecode);
}

void MediaPlayerController::decode() {
    pthread_create(&threadDecode, NULL, decodeCallback, this);
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
    pthread_mutex_lock(&mutexDecode);
    if (output != NULL) {
        output->release();
    }
    int sleepCount = 0;
    while (!exit) {
        if(sleepCount > 1000) {
            exit = true;
        }
        if(LOG_DEBUG) {
            LOGE("wait ffmpeg  exit %d", sleepCount);
        }
        sleepCount++;
        av_usleep(1000 * 10); // 暂停10毫秒
    }
    if (decoder != NULL) {
        decoder->release();
    }
    if (media != NULL) {
        media->release();
    }
    if (output != NULL) {
        delete(output);
        output = NULL;
    }
    if (decoder != NULL) {
        delete(decoder);
        decoder = NULL;
    }
    if (media != NULL) {
        delete(media);
        media = NULL;
    }
    if (playStatus != NULL) {
        delete(playStatus);
        playStatus = NULL;
    }
    pthread_mutex_unlock(&mutexDecode);
    setReleasing(false);
    setWorking(false);
    if (LOG_DEBUG) {
        LOGD("MediaPlayerController::release() 完成。");
    }
}




