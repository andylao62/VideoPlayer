/**
 * 视频输出模块
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "EventCode.h"
#include "VideoOutput.h"

VideoOutput::VideoOutput(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->video = video;
    pthread_mutex_init(&mutexDecode, NULL);
}

VideoOutput::~VideoOutput() {
    this->javaCaller = NULL;
    this->playStatus = NULL;
    this->video = NULL;
    pthread_mutex_destroy(&mutexDecode);
}

void *playVideoCallback(void *data) {
    VideoOutput *out = (VideoOutput *) (data);
    while (out->playStatus != NULL && !out->playStatus->isExit()) {
        if (out->playStatus->seek) {
            av_usleep(MILLI_SECOND * 100);
            continue;
        }
        if (out->playStatus->isPause()) {
            av_usleep(MILLI_SECOND * 100);
            continue;
        }
        if (out->video->queue->size() == 0) { // 加载中
            if (!out->playStatus->isLoad()) {
                out->playStatus->setLoad(true);
                out->javaCaller->callJavaMethod(true, EVENT_LOADING, 0, 0);
            }
            av_usleep(MILLI_SECOND * 100);
            continue;
        } else { // 加载完成
            if (out->playStatus->isLoad()) {
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
        if (out->video->codecType == CODEC_MEDIACODEC) {
            if (av_bsf_send_packet(out->video->abs_ctx, avPacket) != 0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                continue;
            }
            while (av_bsf_receive_packet(out->video->abs_ctx, avPacket) == 0) {
//                if (LOG_DEBUG) {
//                    LOGD("开始硬件解码....");
//                }
                double diff = out->getFrameDiffTime(avPacket->pts);
                av_usleep(out->getDelayTime(diff) * AV_TIME_BASE);
                out->javaCaller->mediaDecode(avPacket->size, avPacket->data);
                av_packet_free(&avPacket);
                av_free(avPacket);
                continue;
            }
            avPacket = NULL;
        } else if (out->video->codecType == CODEC_YUV) {
            pthread_mutex_lock(&out->mutexDecode);
            if (avcodec_send_packet(out->video->avCodecContext, avPacket) != 0) {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
                pthread_mutex_unlock(&out->mutexDecode);
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
                pthread_mutex_unlock(&out->mutexDecode);
                continue;
            }
            if (avFrame->format == AV_PIX_FMT_YUV420P) {
                double diff = out->getFrameDiffTime(av_frame_get_best_effort_timestamp(avFrame));
                av_usleep(out->getDelayTime(diff) * AV_TIME_BASE);
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
            pthread_mutex_unlock(&out->mutexDecode);
            LOGE("子线程视频解码成功.");
        }
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

double VideoOutput::getFrameDiffTime(double pts) {
    if (pts == AV_NOPTS_VALUE) {
        pts = 0;
    }
    pts *= av_q2d(video->timeBase);
    if (pts > 0) {
        video->clock = pts;
    }
    double diff = video->audio->clock - video->clock;
    return diff;
}

double VideoOutput::getDelayTime(double diff) {
    if (diff > 0.003) {
        video->delayTime = video->delayTime * 2 / 3;
        if (video->delayTime < video->defaultDelayTime / 2) {
            video->delayTime = video->defaultDelayTime * 2 / 3;
        } else if (video->delayTime > video->defaultDelayTime * 2) {
            video->delayTime = video->defaultDelayTime * 2;
        }
    } else if (diff < -0.003) {
        video->delayTime = video->delayTime * 3 / 2;
        if (video->delayTime < video->defaultDelayTime / 2) {
            video->delayTime = video->defaultDelayTime * 2 / 3;
        } else if (video->delayTime > video->defaultDelayTime * 2) {
            video->delayTime = video->defaultDelayTime * 2;
        }
    } else if (diff == 0.003) {
    }
    if (diff >= 0.5) {
        video->delayTime = 0;
    } else if (diff <= -0.5) {
        video->delayTime = video->defaultDelayTime * 2;
    }
    if (fabs(diff) >= 10) {
        video->delayTime = video->defaultDelayTime;
    }
    return video->delayTime;
}

