/**
 * 媒体信息
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIA_H
#define VIDEOPLAYER_MEDIA_H
extern "C" {
    #include <libavformat/avformat.h>
}
#include <cwchar>
#include "Video.h"
#include "Audio.h"

class Media {
    public:
        /** 媒体文件来源，本地文件、服务器文件、服务器流 */
        const char* source = NULL;
        AVFormatContext *avFormatContext = NULL;
        /** 媒体文件中的视频信息 */
        Audio *audio = NULL;
        /** 媒体文件中的音频信息 */
        Video *video = NULL;
    public:
        Media(const char* source, PlayStatus *playStatus);
        ~Media();

        void release();
};


#endif //VIDEOPLAYER_MEDIA_H
