/**
 * 媒体输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIAOUTPUT_H
#define VIDEOPLAYER_MEDIAOUTPUT_H

#include "JavaCaller.h"
#include "PlayStatus.h"
#include "Media.h"

class MediaOutput {
    private:
        const JavaCaller *javaCaller = NULL;
        const PlayStatus *playStatus = NULL;
        const Media *media = NULL;

    public:
        MediaOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Media *media);
        ~MediaOutput();
};


#endif //VIDEOPLAYER_MEDIAOUTPUT_H
