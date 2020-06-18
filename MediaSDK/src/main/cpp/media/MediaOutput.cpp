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
    audioOutput = new AudioOutput(javaCaller, playStatus, media->audio);
    videoOutput = new VideoOutput(javaCaller, playStatus, media->video);
}

MediaOutput::~MediaOutput() {
    javaCaller = NULL;
    playStatus = NULL;
    media = NULL;
}

void MediaOutput::play() {
    if (audioOutput != NULL) {
        audioOutput->play();
    }
}

void MediaOutput::pause() {
    if (audioOutput != NULL) {
        audioOutput->pause();
    }
}

void MediaOutput::resume() {
    if (audioOutput != NULL) {
        audioOutput->release();
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
}

