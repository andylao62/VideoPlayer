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
}

AudioDecoder::~AudioDecoder() {
    javaCaller = NULL;
    playStatus = NULL;
    audio = NULL;
}

void AudioDecoder::fill(int streamIndex, AVStream *streams, int64_t duration) {
    if (audio->streamIndex == -1) {
        audio->streamIndex = streamIndex;
        audio->codecPar = streams->codecpar;
        audio->setSampleRate(streams->codecpar->sample_rate);
        audio->duration = duration;
        audio->timeBase = streams->time_base;
        if (LOG_DEBUG) {
            LOGD("AudioDecoder::fill, find AVCodecParameters");
        }
    }
}




