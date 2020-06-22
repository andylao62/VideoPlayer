/**
 * 视频解码器
 *
 * @author kejunyao
 * @since 2020年06月17日
 */

#include "VideoDecoder.h"

VideoDecoder::VideoDecoder(JavaCaller *javaCaller, PlayStatus *playStatus, Video *video) {
    this->javaCaller = javaCaller;
    this->playStatus = playStatus;
    this->video = video;
}

VideoDecoder::~VideoDecoder() {
    this->javaCaller = NULL;
    this->playStatus = NULL;
    this->video = NULL;
}

void VideoDecoder::fill(int streamIndex, AVStream *streams, int64_t duration) {
    if (video->streamIndex == -1) {
        video->streamIndex = streamIndex;
        video->codecPar = streams->codecpar;
        video->timeBase = streams->time_base;
    }
}

void VideoDecoder::tryInitMediaCoder() {
    const char *codecName = ((const AVCodec *) video->avCodecContext->codec)->name;
    bool supportMediaCodec = false;
    const AVBitStreamFilter *bsFilter = NULL;
    if (supportMediaCodec = javaCaller->isSupportMediaCodec(codecName)) {
        LOGE("当前设备支持硬解码当前视频");
        if (strcasecmp(codecName, "h264") == 0) {
            bsFilter = av_bsf_get_by_name("h264_mp4toannexb");
        } else if (strcasecmp(codecName, "h265") == 0) {
            bsFilter = av_bsf_get_by_name("hevc_mp4toannexb");
        }
        if (bsFilter == NULL) {
            supportMediaCodec = false;
            goto end;
        }
        if (av_bsf_alloc(bsFilter, &video->abs_ctx) != 0) {
            supportMediaCodec = false;
            goto end;
        }
        if (avcodec_parameters_copy(video->abs_ctx->par_in, video->codecPar) < 0) {
            supportMediaCodec = false;
            av_bsf_free(&video->abs_ctx);
            video->abs_ctx = NULL;
            goto end;
        }
        if (av_bsf_init(video->abs_ctx) != 0) {
            supportMediaCodec = false;
            av_bsf_free(&video->abs_ctx);
            video->abs_ctx = NULL;
            goto end;
        }
        video->abs_ctx->time_base_in = video->timeBase;
    }
    end:
    if (supportMediaCodec) {
        video->codecType = CODEC_MEDIACODEC;
        javaCaller->initMediaCodec(
                codecName,
                video->avCodecContext->width,
                video->avCodecContext->height,
                video->avCodecContext->extradata_size,
                video->avCodecContext->extradata_size,
                video->avCodecContext->extradata,
                video->avCodecContext->extradata
        );
    }
}
