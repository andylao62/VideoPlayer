/**
 * 媒体信息
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "Media.h"

Media::Media(const char *source, PlayStatus *playStatus) {
    this->source = source;
    this->audio = new Audio(playStatus);
    this->video = new Video(playStatus);
}

Media::~Media() {
    delete(audio);
    audio = NULL;
    delete(video);
    video = NULL;
}

void Media::release() {
    if (audio != NULL) {
        audio->release();
    }
    if (video != NULL) {
        video->release();
    }
    if (avFormatContext != NULL) {
        avformat_close_input(&avFormatContext);
        avformat_free_context(avFormatContext);
        av_free(avFormatContext);
        avFormatContext = NULL;
    }
}
