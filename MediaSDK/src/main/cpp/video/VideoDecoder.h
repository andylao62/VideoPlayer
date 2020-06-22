/**
 * 视频解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_VIDEODECODER_H
#define VIDEOPLAYER_VIDEODECODER_H
extern "C" {
    #include <libavformat/avformat.h>
}
#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Video.h"

class VideoDecoder {
    private:
        JavaCaller *javaCaller = NULL;
        PlayStatus *playStatus = NULL;
        Video *video = NULL;

    public:
        VideoDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video);
        ~VideoDecoder();
        /**
         * 填充音频数据
         * @param streamIndex 视频流index
         * @param streams 音频流
         * @param duration 音频时长
         */
        void fill(int streamIndex, AVStream *streams, int64_t duration);

        void tryInitMediaCoder();
};


#endif //VIDEOPLAYER_VIDEODECODER_H
