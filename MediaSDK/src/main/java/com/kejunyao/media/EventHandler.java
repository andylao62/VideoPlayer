package com.kejunyao.media;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import java.lang.ref.WeakReference;

import androidx.annotation.NonNull;

import com.kejunyao.media.util.MediaLog;

/**
 * 音频播放器事件处理Handler
 *
 * @author kejunyao
 * @since 2020年05月31日
 */
class EventHandler extends Handler {

    private static final String TAG = "EventHandler";

    static final int EVENT_NOP       = 0;
    static final int EVENT_PREPARED  = 1;
    static final int EVENT_LOADING   = 2;
    static final int EVENT_PAUSE     = 3;
    static final int EVENT_RESUME    = 4;
    static final int EVENT_TIME_INFO = 5;
    static final int EVENT_ERROR     = 6;
    static final int EVENT_COMPLETE  = 7;


    private final WeakReference<MediaPlayer> mPlayerRef;
    public EventHandler(MediaPlayer player, Looper looper) {
        super(looper);
        mPlayerRef = new WeakReference<>(player);
    }

    @Override
    public void handleMessage(@NonNull Message msg) {
        switch (msg.what) {
            case EVENT_PREPARED:
                onPrepared(msg.arg1, msg.arg2);
                break;
            case EVENT_LOADING:
                onLoad(msg.arg1 == 0);
                break;
            case EVENT_PAUSE:
            case EVENT_RESUME:
                onPauseResume(msg.what == EVENT_PAUSE);
                break;
            case EVENT_TIME_INFO:
                onTimeInfo(msg.arg1, msg.arg2);
                break;
            case EVENT_COMPLETE:
                onComplete();
                break;
            case EVENT_ERROR:
                onError(msg.arg1);
                break;
            case EVENT_NOP:
                break;
            default:
                MediaLog.e(TAG, "Unknown message type ", msg.what);
                break;
        }
    }

    void sendMessage(int what, int arg1, int arg2) {
        Message m = obtainMessage(what, arg1, arg2, null);
        sendMessage(m);
    }

    void sendMessage(int what) {
        sendMessage(what, 0, 0);
    }

    private void onPrepared(int mediaWidth, int mediaHeight) {
        MediaPlayer player = mPlayerRef.get();
        if (player != null && player.mOnPreparedListener != null) {
            player.mOnPreparedListener.onPrepared(mediaWidth, mediaHeight);
        }
    }

    private void onLoad(boolean isLoading) {
        MediaPlayer player = mPlayerRef.get();
        if (player != null && player.mOnLoadListener != null) {
            player.mOnLoadListener.onLoad(isLoading);
        }
    }

    private void onPauseResume(boolean pause) {
        MediaPlayer player = mPlayerRef.get();
        if (player != null && player.mOnPauseResumeListener != null) {
            player.mOnPauseResumeListener.onPause(pause);
        }
    }

    private void onTimeInfo(int currentTime, int totalTime) {
        MediaPlayer player = mPlayerRef.get();
        if (player != null && player.mOnTimeInfoListener != null) {
            player.mOnTimeInfoListener.onTimeInfo(currentTime, totalTime);
        }
    }

    private void onError(int errCode) {
        MediaPlayer player = mPlayerRef.get();
        if (player != null) {
            player.stop();
            if (player.mOnErrorListener != null) {
                player.mOnErrorListener.onError(errCode);
            }
        }
    }

    private void onComplete() {
        MediaPlayer player = mPlayerRef.get();
        if (player != null) {
            player.stop();
            if (player.mOnCompleteListener != null) {
                player.mOnCompleteListener.onComplete();
            }
        }
    }
}
