package com.kejunyao.media;

import android.os.Looper;
import android.view.ViewGroup;

import com.kejunyao.media.widget.MediaSurfaceView;

/**
 * 多媒体播放Controller
 *
 * @author kejunyao
 * @since 2020年05月30日
 */
public class MediaPlayer {

    static {
        System.loadLibrary("MediaPlayer");

        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
    }

    private EventHandler mEventHandler;

    public MediaPlayer() {
        Looper looper;
        if ((looper = Looper.myLooper()) != null) {
            mEventHandler = new EventHandler(this, looper);
        } else if ((looper = Looper.getMainLooper()) != null) {
            mEventHandler = new EventHandler(this, looper);
        } else {
            mEventHandler = null;
        }
    }

    private boolean mAutoFitSurfaceViewSize;
    public void setAutoFitSurfaceViewSize(boolean autoFit) {
        mAutoFitSurfaceViewSize = autoFit;
    }

    void autoFitSurfaceViewSize(int width, int height) {
        if (!mAutoFitSurfaceViewSize) {
            return;
        }
        if (mSurfaceView != null && width > 0 && height > 0) {
            ViewGroup.LayoutParams layoutParams = mSurfaceView.getLayoutParams();
            if (layoutParams != null) {
                int surfaceWidth = mSurfaceView.getResources().getDisplayMetrics().widthPixels;
                int surfaceHeight = surfaceWidth * height / width;
                layoutParams.width = surfaceWidth;
                layoutParams.height = surfaceHeight;
                mSurfaceView.requestLayout();
            }

        }
    }

    OnPreparedListener mOnPreparedListener;
    public void setOnPreparedListener(OnPreparedListener listener) {
        mOnPreparedListener = listener;
    }

    OnLoadListener mOnLoadListener;
    public void setOnLoadListener(OnLoadListener listener) {
        mOnLoadListener = listener;
    }

    OnPauseResumeListener mOnPauseResumeListener;
    public void setOnPauseResumeListener(OnPauseResumeListener listener) {
        mOnPauseResumeListener = listener;
    }

    OnTimeInfoListener mOnTimeInfoListener;
    public void setOnTimeInfoListener(OnTimeInfoListener listener) {
        mOnTimeInfoListener = listener;
    }

    OnErrorListener mOnErrorListener;
    public void setOnErrorListener(OnErrorListener listener) {
        mOnErrorListener = listener;
    }

    OnCompleteListener mOnCompleteListener;
    public void setOnCompleteListener(OnCompleteListener listener) {
        mOnCompleteListener = listener;
    }

    private MediaSurfaceView mSurfaceView;
    public void setSurfaceView(MediaSurfaceView surfaceView) {
        mSurfaceView = surfaceView;
    }

    private String mSource;
    public void setDataSource(String source) {
        mSource = source;
    }

    public void prepare() {
        _prepare(mSource);
    }

    public void start() {
        setVolume(sVolumePercent);
        _start();
    }

    public void seek(float percent) {
        _seekByPercent(percent);
    }

    public void seek(int second) {
        _seek(second);
    }

    private static float sVolumePercent = 1.0f;
    public void setVolume(float percent) {
        int value = (int) (100 * percent);
        if (value < 0 || value > 100) {
            return;
        }
        sVolumePercent = percent;
        _setVolume(percent);
    }

    public float getVolumePercent() {
        return sVolumePercent;
    }

    public void resume() {
        _resume();
        if (mEventHandler != null) {
            mEventHandler.sendMessage(EventHandler.EVENT_RESUME);
        }
    }

    public void pause() {
        _pause();
        if (mEventHandler != null) {
            mEventHandler.sendMessage(EventHandler.EVENT_PAUSE);
        }
    }

    public void stop() {
        _stop();
        _release();
    }

    private void release() {
        mOnPreparedListener = null;
        mOnLoadListener = null;
        mOnPauseResumeListener = null;
        _release();
    }

    private void postEventFromNative(int what, int arg1, int arg2) {
        if (mEventHandler != null) {
            mEventHandler.sendMessage(what, arg1, arg2);
        }
    }

    private void renderYUV(int width, int height, byte[] y, byte[] u, byte[] v) {
        if (mSurfaceView != null) {
            mSurfaceView.setYUVData(width, height, y, u, v);
        }
    }

    private boolean isSupportMediaCodec(String codecName) {
        return VideoSupportUtils.isSupport(codecName);
    }

    private MediaCoder mMediaCoder;
    private void initMediaCoder(String codecName, int width, int height, byte[] csd0, byte[] csd1) {
        if (mSurfaceView != null && mSurfaceView.getSurface() != null) {
            mMediaCoder = new MediaCoder(mSurfaceView.getSurface(), codecName, width, height, csd0, csd1);
        }
    }

    private void mediaDecode(int size, byte[] data) {
        if (mSurfaceView != null) {
            mSurfaceView.useMediaCodecRender();
        }
        if (mMediaCoder != null) {
            mMediaCoder.decode(size, data);
        }
    }

    private native void _prepare(String source);
    private native void _start();
    private native void _seekByPercent(float percent);
    private native void _seek(int second);
    private native void _setVolume(float percent);
    private native void _pause();
    private native void _resume();
    private native void _stop();
    private native void _release();

    public native boolean isPlaying();

}
