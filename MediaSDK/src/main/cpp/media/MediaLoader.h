/**
 * 媒体解加载器：
 * 1、或加载本地媒体文件；
 * 2、或加载远程媒体文件；
 * 3、或读取当前设备麦克风、摄像头
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#ifndef VIDEOPLAYER_MEDIALOADER_H
#define VIDEOPLAYER_MEDIALOADER_H


#include "PlayStatus.h"
#include "Media.h"

class MediaLoader {
    private:
        Media *media = NULL;

    public:
        PlayStatus *playStatus = NULL;

    public:
        MediaLoader(PlayStatus *playStatus, Media *media);
        ~MediaLoader();

        /**
         * 加载媒体文件
         * @return 返回1，加载成功；返回其他值，加载失败
         */
        int load();
};


#endif //VIDEOPLAYER_MEDIALOADER_H
