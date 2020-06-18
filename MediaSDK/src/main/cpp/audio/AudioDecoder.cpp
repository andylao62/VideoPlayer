/**
 * 音频解码器
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#include "AudioDecoder.h"

AudioDecoder::AudioDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Audio *audio) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->audio = audio;
    pthread_mutex_init(&mutexSeek, NULL);
}

AudioDecoder::~AudioDecoder() {
    pthread_mutex_destroy(&mutexSeek);
}

void AudioDecoder::release() {
}

void AudioDecoder::seekByPercent(float percent, AVFormatContext *avFormatContext) {
    if (audio == NULL) {
        return;
    }
    seek(percent * audio->durationInSecond(), avFormatContext);
}

void AudioDecoder::seek(int64_t second, AVFormatContext *avFormatContext) {
    if (audio == NULL) {
        return;
    }
    if (playStatus == NULL) {
        return;
    }
    if (avFormatContext == NULL) {
        return;
    }
    if (second < 0 && second > audio->duration) {
        return;
    }
    playStatus->setSeek(true);
    audio->queue->clear();
    audio->reset();
    pthread_mutex_lock(&mutexSeek);
    int64_t rel = second * AV_TIME_BASE;
    avcodec_flush_buffers(audio->avCodecContext);
    avformat_seek_file(avFormatContext, -1, INT64_MIN, rel, INT64_MAX, 0);
    pthread_mutex_unlock(&mutexSeek);
    playStatus->setSeek(false);
}

void AudioDecoder::fill(int streamIndex, AVStream *streams, int64_t duration) {
    if (audio->streamIndex == -1) {
        audio->streamIndex = streamIndex;
        audio->setSampleRate(streams->codecpar->sample_rate);
        audio->duration = duration;
        audio->timeBase = streams->time_base;
        if (LOG_DEBUG) {
            LOGD("AudioDecoder::fill, find AVCodecParameters");
        }
    }
}




