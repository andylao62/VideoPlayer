/**
 * 媒体解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIADECODER_H
#define VIDEOPLAYER_MEDIADECODER_H
extern "C" {
    #include <libavutil/time.h>
}

#include "VideoDecoder.h"
#include "AudioDecoder.h"
#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Media.h"

class MediaDecoder {
    private:
        AudioDecoder *audioDecoder = NULL;
        VideoDecoder *videoDecoder = NULL;

    public:
        JavaCaller *javaCaller = NULL;
        PlayStatus *playStatus = NULL;
        Media *media = NULL;

private:
    int getAVCodecContext(AVCodecParameters *codecPar, AVCodecContext **avCodecContext);

    public:
        MediaDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media);
        ~MediaDecoder();

        int openAVCodec();

        void tryInitMediaCoder();

        void decode();

        void release();
};


#endif //VIDEOPLAYER_MEDIADECODER_H
