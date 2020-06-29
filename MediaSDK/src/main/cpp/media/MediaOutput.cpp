/**
 * 媒体输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "MediaOutput.h"

MediaOutput::MediaOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->media = media;
    pthread_mutex_init(&mutexSeek, NULL);
    audioOutput = new AudioOutput(javaCaller, playStatus, media->audio);
    videoOutput = new VideoOutput(javaCaller, playStatus, media->video);
}

MediaOutput::~MediaOutput() {
    javaCaller = NULL;
    playStatus = NULL;
    media = NULL;
    audioOutput = NULL;
    videoOutput = NULL;
    pthread_mutex_destroy(&mutexSeek);
}

void MediaOutput::play() {
    if (audioOutput != NULL) {
        audioOutput->play();
    }
    if (videoOutput != NULL) {
        videoOutput->play();
    }
}

void MediaOutput::pause() {
    if (playStatus != NULL) {
        playStatus->setPause(true);
    }
    if (audioOutput != NULL) {
        audioOutput->pause();
    }
}

void MediaOutput::resume() {
    if (playStatus != NULL) {
        playStatus->setPause(false);
    }
    if (audioOutput != NULL) {
        audioOutput->resume();
    }
}

void MediaOutput::stop() {
    if (audioOutput != NULL) {
        audioOutput->stop();
    }
}

void MediaOutput::setVolume(float percent) {
    if (audioOutput != NULL) {
        audioOutput->setVolume(percent);
    }
}

void MediaOutput::release() {
    if (videoOutput != NULL) {
        videoOutput->release();
    }
    if (audioOutput != NULL) {
        audioOutput->release();
    }
}

void MediaOutput::seekByPercent(float percent) {
    if (media != NULL && media->audio != NULL) {
        if (LOG_DEBUG) {
            LOGD("MediaOutput::seekByPercent, percent: %f", percent);
        }
        seek(percent * media->audio->durationInMills);
    }
}

void MediaOutput::seek(int64_t second) {
    if (LOG_DEBUG) {
        LOGD("MediaOutput::seek, second: %d", second, ", duration: %d", media->audio->durationInMills);
    }
    if (media == NULL) {
        return;
    }
    if (media->audio == NULL) {
        return;
    }
    if (playStatus == NULL) {
        return;
    }
    if (media->avFormatContext == NULL) {
        return;
    }
    if (second < 0 && second > media->audio->durationInMills) {
        return;
    }
    playStatus->setSeek(true);
    pthread_mutex_lock(&mutexSeek);
    int64_t rel = second * AV_TIME_BASE;
    avformat_seek_file(media->avFormatContext, -1, INT64_MIN, rel, INT64_MAX, 0);
    if (media->audio != NULL) {
        media->audio->queue->clear();
        media->audio->reset();
        pthread_mutex_lock(&audioOutput->mutexDecode);
        avcodec_flush_buffers(media->audio->avCodecContext);
        pthread_mutex_unlock(&audioOutput->mutexDecode);
    }
    if (media->video != NULL) {
        media->video->queue->clear();
        media->video->clock = 0;
        pthread_mutex_lock(&videoOutput->mutexDecode);
        avcodec_flush_buffers(media->video->avCodecContext);
        pthread_mutex_unlock(&videoOutput->mutexDecode);
    }
    pthread_mutex_unlock(&mutexSeek);
    playStatus->setSeek(false);
}

