package com.kejunyao.media;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.view.Surface;

import java.nio.ByteBuffer;

/**
 * 硬解码
 *
 * @author kejunyao
 * @since 2020年05月30日
 */
class MediaCoder {

    private static final String TAG = "MediaCoder";

    private MediaFormat mediaFormat;
    private MediaCodec mediaCodec;
    private final Surface surface;
    private MediaCodec.BufferInfo info;
    private boolean mInit;

    MediaCoder(Surface surface, String codecName, int width, int height, byte[] csd0, byte[] csd1) {
        this.surface = surface;
        try {
            String mime = VideoSupportUtils.findCodecName(codecName);
            mediaFormat = MediaFormat.createVideoFormat(mime, width, height);
            mediaFormat.setInteger(MediaFormat.KEY_MAX_INPUT_SIZE, width * height);
            mediaFormat.setByteBuffer("csd-0", ByteBuffer.wrap(csd0));
            mediaFormat.setByteBuffer("csd-1", ByteBuffer.wrap(csd1));
            mediaCodec = MediaCodec.createDecoderByType(mime);
            info = new MediaCodec.BufferInfo();
            mediaCodec.configure(mediaFormat, surface, null, 0);
            mediaCodec.start();
            mInit = true;
        } catch (Exception e) {
            mInit = false;
        }
    }

    void decode(int size, byte[] data) {
        if (!mInit) {
            MediaLog.e(TAG, "MediaCoder init failure.");
            return;
        }
        if (size <= 0 || data == null || data.length == 0) {
            MediaLog.e(TAG, "MediaCoder#decode data is illegal.");
            return;
        }
        if (surface == null || mediaCodec == null || info == null) {
            if (surface == null) {
                MediaLog.e(TAG, "MediaCoder#decode, surface is null.");
            }
            if (mediaCodec == null) {
                MediaLog.e(TAG, "MediaCoder#decode, mediaCodec is null.");
            }
            if (info == null) {
                MediaLog.e(TAG, "MediaCoder#decode, info is null.");
            }
            return;
        }
        try {
            int inputBufferIndex = mediaCodec.dequeueInputBuffer(10);
            if (inputBufferIndex >= 0) {
                ByteBuffer byteBuffer = mediaCodec.getInputBuffers()[inputBufferIndex];
                byteBuffer.clear();
                byteBuffer.put(data);
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, size, 0, 0);
            }
            int outputBufferIndex = mediaCodec.dequeueOutputBuffer(info, 10);
            while (outputBufferIndex >= 0) {
                mediaCodec.releaseOutputBuffer(outputBufferIndex, true);
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(info, 10);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    void release() {
        if (mediaCodec != null) {
            try {
                mediaCodec.flush();
                mediaCodec.stop();
                mediaCodec.release();
            } catch (Exception e) {
            } finally {
                mediaCodec = null;
                mediaFormat = null;
                info = null;
            }
        }
    }

}
