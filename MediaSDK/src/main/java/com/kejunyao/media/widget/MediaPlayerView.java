package com.kejunyao.media.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import com.kejunyao.arch.util.ActivityUtils;
import com.kejunyao.media.MediaPlayer;
import com.kejunyao.media.R;
import com.kejunyao.media.callback.OnPauseResumeListener;
import com.kejunyao.media.callback.OnPreparedListener;
import com.kejunyao.media.callback.OnTimeInfoListener;
import com.kejunyao.media.util.TimeUtils;
import java.lang.ref.WeakReference;

/**
 * 播放多媒体View
 *
 * @author kejunyao
 * @since 2020年06月19日
 */
public class MediaPlayerView extends FrameLayout {
    /** 按钮工具栏显示时间 */
    private static final int DURATION_TOOL_BAR_SHOW = 3500;

    private final MediaPlayer mMediaPlayer = new MediaPlayer();
    /** 播放器标题栏 */
    private PlayerTitleBar mTitleBar;
    /** 媒体视图播放View */
    private MediaSurfaceView mMediaSurfaceView;
    /** 按钮工具类 */
    private View mToolbar;
    /** 媒体文件播放进度条 */
    private SeekBar mPlayProgressSeekBar;
    /** 显示当前播放时间View */
    private TextView mCurrentTimeView;
    /** 显示媒体总共时间View */
    private TextView mTotalTimeView;
    /** "播放/暂停"按钮 */
    private TextView mPlayView;

    public MediaPlayerView(@NonNull Context context) {
        this(context, null);
    }

    public MediaPlayerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public MediaPlayerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    public MediaPlayerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        this(context, attrs, defStyleAttr);
    }

    /**
     * 初始各个View及设置其监听Listener
     */
    private void init() {
        inflate(getContext(), R.layout.media_player_view, this);
        mTitleBar = findViewById(R.id.title_bar);
        mMediaSurfaceView = findViewById(R.id.media_surface_view);
        mMediaPlayer.setSurfaceView(mMediaSurfaceView);
        mToolbar = findViewById(R.id.media_tool_bar);
        mPlayProgressSeekBar = findViewById(R.id.play_progress_seek_bar);
        mCurrentTimeView = findViewById(R.id.media_current_time);
        mTotalTimeView = findViewById(R.id.media_total_time);
        mPlayView = findViewById(R.id.play);

        setTouchListener();
        setPlayProgressListener();
        setPreparedListener();
        setPauseResumeListener();
        setTimeInfoListener();

        mPlayView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (v.isSelected()) {
                    mMediaPlayer.resume();
                } else {
                    mMediaPlayer.pause();
                }
            }
        });
    }

    private OnTouchListener mOnTouchListener;
    @Override
    public void setOnTouchListener(OnTouchListener l) {
        mOnTouchListener = l;
    }

    /**
     * 设置当前View的触碰事件
     */
    private void setTouchListener() {
        super.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                showOrHideToolBarImmediately();
                return mOnTouchListener != null && mOnTouchListener.onTouch(v, event);
            }
        });
    }

    private ToolBarShownToggle mToolBarShownToggle;
    /**
     * 立即显示或隐藏按钮工具栏，若是显示则在3秒后自动隐藏
     */
    private void showOrHideToolBarImmediately() {
        if (mTitleBar.getVisibility() == VISIBLE) {
            mTitleBar.setVisibility(INVISIBLE);
        }
        if (mToolbar.getVisibility() == VISIBLE) {
            mToolbar.setVisibility(INVISIBLE);
            removeToolBarShownToggle();
            return;
        }
        mTitleBar.setVisibility(VISIBLE);
        mToolbar.setVisibility(VISIBLE);
        if (mToolBarShownToggle == null) {
            mToolBarShownToggle = new ToolBarShownToggle(this);
        } else {
            removeCallbacks(mToolBarShownToggle);
        }
        postDelayed(mToolBarShownToggle, DURATION_TOOL_BAR_SHOW);
    }

    private boolean mTrackingTouch = false;
    /**
     * 设置监听媒体播放进度Listener
     */
    private void setPlayProgressListener() {
        mPlayProgressSeekBar.setOnSeekBarChangeListener(new SeekBarChangeAdapter() {

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                mTrackingTouch = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                float percent = (float) seekBar.getProgress() / (float) seekBar.getMax();
                mMediaPlayer.seek(percent);
                mTrackingTouch = false;
            }
        });
    }

    /**
     * 设置监听媒体文件预加载Listener
     */
    private void setPreparedListener() {
        mMediaPlayer.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared(int mediaWidth, int mediaHeight) {
                mMediaPlayer.start();
                showOrHideToolBarImmediately();
            }
        });
    }

    /**
     * 设置"播放/暂停"监听Listener
     */
    private void setPauseResumeListener() {
        mMediaPlayer.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                mPlayView.setSelected(pause);
                mPlayView.setText(pause ? R.string.play : R.string.pause);
            }
        });
    }

    /**
     * 设置监听播放时间变化Listener
     */
    private void setTimeInfoListener() {
        mMediaPlayer.setOnTimeInfoListener(new OnTimeInfoListener() {
            @Override
            public void onTimeInfo(int currentTime, int totalTime) {
                if (mTrackingTouch) {
                    return;
                }
                if (totalTime <= 0) {
                    if (mPlayProgressSeekBar.isEnabled()) {
                        mPlayProgressSeekBar.setEnabled(false);
                    }
                    mCurrentTimeView.setVisibility(View.INVISIBLE);
                    mTotalTimeView.setText(R.string.living);
                } else {
                    if (!mPlayProgressSeekBar.isEnabled()) {
                        mPlayProgressSeekBar.setEnabled(true);
                    }
                    int progress = mPlayProgressSeekBar.getMax() * currentTime / totalTime;
                    mPlayProgressSeekBar.setProgress(progress);
                    mCurrentTimeView.setVisibility(View.VISIBLE);
                    mCurrentTimeView.setText(TimeUtils.timeText(currentTime));
                    mTotalTimeView.setText(TimeUtils.timeText(totalTime));
                }
            }
        });
    }

    /**
     * 预加载媒体文件
     * @param source 媒体文件来源（或本地文件、或远程文件）
     */
    public void prepare(String source) {
        mMediaPlayer.setDataSource(source);
        mMediaPlayer.prepare();
    }

    /**
     * 继续播放
     */
    public void resume() {
        mMediaPlayer.resume();
    }

    /**
     * 暂停播放
     */
    public void pause() {
        mMediaPlayer.pause();
    }

    /**
     * 停止播放器
     */
    public void stop() {
        mMediaPlayer.stop();
    }

    /**
     * 释放播放器资源
     */
    public void release() {
        removeToolBarShownToggle();
        mMediaPlayer.release();
    }

    /**
     * 取消按钮工具栏延时隐藏操作
     */
    private void removeToolBarShownToggle() {
        if (mToolBarShownToggle != null) {
            removeCallbacks(mToolBarShownToggle);
        }
    }

    /**
     * 按钮工具栏延时隐藏Runnable
     */
    private static class ToolBarShownToggle implements Runnable {

        private final WeakReference<MediaPlayerView> reference;

        ToolBarShownToggle(MediaPlayerView playerView) {
            reference = new WeakReference<>(playerView);
        }

        @Override
        public void run() {
            MediaPlayerView playerView = reference.get();
            if (playerView == null || ActivityUtils.isFinishing(playerView.getContext())) {
                return;
            }
            if (playerView.mTitleBar != null) {
                playerView.mTitleBar.setVisibility(INVISIBLE);
            }
            if (playerView.mToolbar != null) {
                playerView.mToolbar.setVisibility(INVISIBLE);
            }
        }
    }
}
