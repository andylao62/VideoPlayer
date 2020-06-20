/**
 * 视频输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include <EventCode.h>

#include "VideoOutput.h"

VideoOutput::VideoOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->video = video;
}

VideoOutput::~VideoOutput() {
    this->javaCaller = NULL;
    this->playStatus = NULL;
    this->video = NULL;
}

void *playVideoCallback(void *data) {
    VideoOutput *out = (VideoOutput *)(data);
    while (out->playStatus != NULL && !out->playStatus->isExit()) {
        if (out->playStatus->isSeek()) {
            av_usleep(1000 * 100);
            continue;
        }
        if(out->video->queue->size() == 0) { // 加载中
            if(!out->playStatus->isLoad()) {
                out->playStatus->setLoad(true);
                out->javaCaller->callJavaMethod(true, EVENT_LOADING, 0, 0);
            }
            av_usleep(1000 * 100);
            continue;
        } else { // 加载完成
            if(out->playStatus->isLoad()) {
                out->playStatus->setLoad(false);
                out->javaCaller->callJavaMethod(true, EVENT_LOADING, 1, 0);
            }
        }
        AVPacket *avPacket = av_packet_alloc();
        if (out->video->queue->pop(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        if (avcodec_send_packet(out->video->avCodecContext, avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        AVFrame *avFrame = av_frame_alloc();
        if (avcodec_receive_frame(out->video->avCodecContext, avFrame) != 0) {
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            continue;
        }
        if(avFrame->format == AV_PIX_FMT_YUV420P) {
            out->javaCaller->callRenderYUV(out->video->avCodecContext->width,
                                           out->video->avCodecContext->height,
                                           avFrame->data[0],
                                           avFrame->data[1],
                                           avFrame->data[2]
                                           );
        } else {
            AVFrame *frameYUV420P = out->swsToYUV420P(avFrame);
            if (frameYUV420P != NULL) {
                out->javaCaller->callRenderYUV(out->video->avCodecContext->width,
                                               out->video->avCodecContext->height,
                                               frameYUV420P->data[0],
                                               frameYUV420P->data[1],
                                               frameYUV420P->data[2]
                );
                av_frame_free(&frameYUV420P);
                av_free(frameYUV420P);
                frameYUV420P = NULL;
            }
        }

        av_frame_free(&avFrame);
        av_free(avFrame);
        avFrame = NULL;
        av_packet_free(&avPacket);
        av_free(avPacket);
        avPacket = NULL;
        LOGE("子线程视频解码成功.");
    }
    pthread_exit(&out->threadPlay);
}

AVFrame *VideoOutput::swsToYUV420P(AVFrame *avFrame) {
    AVFrame *pFrameYUV420P = av_frame_alloc();
    int num = av_image_get_buffer_size(
            AV_PIX_FMT_YUV420P,
            video->avCodecContext->width,
            video->avCodecContext->height,
            1
            );
    uint8_t *buffer = static_cast<uint8_t *>(av_malloc(num * sizeof(uint8_t)));
    av_image_fill_arrays(
            pFrameYUV420P->data,
            pFrameYUV420P->linesize,
            buffer,
            AV_PIX_FMT_YUV420P,
            video->avCodecContext->width,
            video->avCodecContext->height,
            1
            );
    SwsContext *sws_ctx = sws_getContext(
            video->avCodecContext->width,
            video->avCodecContext->height,
            video->avCodecContext->pix_fmt,
            video->avCodecContext->width,
            video->avCodecContext->height,
            AV_PIX_FMT_YUV420P,
            SWS_BICUBIC,
            NULL,
            NULL,
            NULL
            );
    if (!sws_ctx) {
        av_frame_free(&pFrameYUV420P);
        av_free(pFrameYUV420P);
        pFrameYUV420P = NULL;
        av_free(buffer);
        buffer = NULL;
        return NULL;
    }
    sws_scale(
            sws_ctx,
            reinterpret_cast<const uint8_t *const *>(avFrame->data),
            avFrame->linesize,
            0,
            avFrame->height,
            pFrameYUV420P->data,
            pFrameYUV420P->linesize);
    av_free(buffer);
    buffer = NULL;
    sws_freeContext(sws_ctx);
    return pFrameYUV420P;
}

void VideoOutput::play() {
    pthread_create(&threadPlay, NULL, playVideoCallback, this);
}

void VideoOutput::release() {
}

