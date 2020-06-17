/**
 * 媒体解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIADECODER_H
#define VIDEOPLAYER_MEDIADECODER_H


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
        const JavaCaller *javaCaller = NULL;
        const PlayStatus *playStatus = NULL;
        const Media *media = NULL;

    public:
        MediaDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media);
        ~MediaDecoder();

        int openAVCodec();
};


#endif //VIDEOPLAYER_MEDIADECODER_H
