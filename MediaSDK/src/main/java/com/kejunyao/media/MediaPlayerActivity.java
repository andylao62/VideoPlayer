package com.kejunyao.media;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.kejunyao.media.widget.MediaSurfaceView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MediaPlayerActivity extends AppCompatActivity {

    private static final String PARAM_SOURCE = "media_source";

    public static void startActivity(Context context, String source) {
        Intent intent = new Intent(context, MediaPlayerActivity.class);
        if (!(context instanceof Activity)) {
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        }
        intent.putExtra(PARAM_SOURCE, source);
        context.startActivity(intent);
    }

    private final MediaPlayer mMediaPlayer = new MediaPlayer();

    private View mSurfaceLayout;
    private MediaSurfaceView mMediaSurfaceView;
    private SeekBar mSeekBar;
    private TextView mCurrentTimeView;
    private TextView mTotalTimeView;
    private TextView mPlayView;

    private String mSource;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON
        );
        setContentView(R.layout.activity_media_player);
        mSource = getIntent().getStringExtra(PARAM_SOURCE);
        if (TextUtils.isEmpty(mSource)) {
            Toast.makeText(getApplicationContext(), "请传入媒体文件路径source", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        mSurfaceLayout = findViewById(R.id.surface_layout);
        mMediaSurfaceView = findViewById(R.id.media_surface_view);
        mMediaPlayer.setSurfaceView(mMediaSurfaceView);
        mMediaPlayer.setResizeSurfaceViewEnabled(true);
        mSeekBar = findViewById(R.id.seek_bar);
        mCurrentTimeView = findViewById(R.id.media_current_time);
        mTotalTimeView = findViewById(R.id.media_total_time);
        mPlayView = findViewById(R.id.play);

        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                float percent = (float) seekBar.getProgress() / (float) seekBar.getMax();
                mMediaPlayer.seek(percent);
            }
        });
        mMediaPlayer.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared(int mediaWidth, int mediaHeight) {
                mMediaPlayer.resizeSurfaceView(
                        mSurfaceLayout,
                        getResources().getDisplayMetrics().widthPixels,
                        FrameLayout.LayoutParams.WRAP_CONTENT
                );
                mMediaPlayer.start();
            }
        });
        mMediaPlayer.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                mPlayView.setSelected(pause);
                mPlayView.setText(pause ? R.string.play : R.string.pause);
            }
        });
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
        mMediaPlayer.setOnTimeInfoListener(new OnTimeInfoListener() {
            @Override
            public void onTimeInfo(int currentTime, int totalTime) {
                if (totalTime < 0) {
                    mCurrentTimeView.setVisibility(View.INVISIBLE);
                    mTotalTimeView.setText(R.string.living);
                } else {
                    mCurrentTimeView.setVisibility(View.VISIBLE);
                    mCurrentTimeView.setText(TimeUtils.timeText(currentTime));
                    mTotalTimeView.setText(TimeUtils.timeText(totalTime));
                }
            }
        });
        mMediaPlayer.setDataSource(mSource);
        mMediaPlayer.prepare();
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        refreshFullScreen();
        mMediaPlayer.resizeSurfaceView(
                mSurfaceLayout,
                getResources().getDisplayMetrics().widthPixels,
                FrameLayout.LayoutParams.WRAP_CONTENT
        );
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        mMediaPlayer.resume();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mMediaPlayer.pause();
    }

    @Override
    protected void onDestroy() {
        mMediaPlayer.stop();
        mMediaPlayer.release();
        super.onDestroy();
    }

    private void refreshFullScreen() {
        if (getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
            getWindow().setFlags(
                    WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN
            );
        } else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }

    }
}
