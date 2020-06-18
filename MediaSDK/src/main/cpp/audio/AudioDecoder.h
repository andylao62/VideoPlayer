/**
 * 音频解码器
 *
 * @author kejunyao
 * @since 2020年05月30日
 */

#ifndef AUDIOPLAYER_AUDIODECODER_H
#define AUDIOPLAYER_AUDIODECODER_H

#include "AVPacketBlockQueue.h"
#include "PlayStatus.h"
#include "Log.h"
#include "Audio.h"
#include "JavaCaller.h"
#include "EventCode.h"
#include "ErrorCode.h"

extern "C" {
    #include <libavformat/avformat.h>
}

class AudioDecoder {
private:
    pthread_mutex_t mutexSeek;
    Audio *audio = NULL;
    JavaCaller *javaCaller;

public:
    PlayStatus *playStatus;

public:
    AudioDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Audio *audio);
    ~AudioDecoder();

    void seekByPercent(float percent, AVFormatContext *avFormatContext);

    void seek(int64_t second, AVFormatContext *avFormatContext);

    void decode();

    /**
     * 填充音频数据
     * @param streamIndex 音频流index
     * @param streams 音频流
     * @param duration 音频时长
     */
    void fill(int streamIndex, AVStream *streams, int64_t duration);

    void release();

};


#endif //AUDIOPLAYER_AUDIODECODER_H
