package com.kejunyao.video.player;

import android.os.Bundle;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;
import com.kejunyao.media.MediaLog;
import com.kejunyao.media.MediaPlayer;
import com.kejunyao.media.OnCompleteListener;
import com.kejunyao.media.OnErrorListener;
import com.kejunyao.media.OnLoadListener;
import com.kejunyao.media.OnPauseResumeListener;
import com.kejunyao.media.OnPreparedListener;
import com.kejunyao.media.OnTimeInfoListener;
import com.kejunyao.media.opengl.MediaSurfaceView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MediaPlayer_MainActivity";

    final MediaPlayer mMediaPlayer = new MediaPlayer();
    private TextView mTimeInfoView;
    private SeekBar mSeekBar;
    private TextView mVolumeTextView;
    private SeekBar mVolumeSeekBar;
    private MediaSurfaceView mSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mSeekBar = findViewById(R.id.seek_bar);
        mVolumeTextView = findViewById(R.id.volume_text);
        mVolumeSeekBar = findViewById(R.id.seek_volume);
        mSurfaceView = findViewById(R.id.media_surface_view);
        mMediaPlayer.setSurfaceView(mSurfaceView);
        mVolumeSeekBar.setProgress((int) (mMediaPlayer.getVolumePercent() * 100));
        setVolumeProgress(mMediaPlayer.getVolumePercent());

        // MediaPlayer mediaPlayer = new MediaPlayer();
        // mediaPlayer.start();
        // mediaPlayer.release();
        // mediaPlayer.pause();
        // mediaPlayer.prepare();
        // mediaPlayer.isPlaying();
        // mediaPlayer.stop();
        // mediaPlayer.release();
        // mediaPlayer.setDataSource("");
        mTimeInfoView = findViewById(R.id.time_info);
        mMediaPlayer.setOnPreparedListener(new OnPreparedListener() {
            @Override
            public void onPrepared() {
                MediaLog.d(TAG, "onPrepared");
                mMediaPlayer.start();
            }
        });
        mMediaPlayer.setOnLoadListener(new OnLoadListener() {
            @Override
            public void onLoad(boolean isLoading) {
                MediaLog.d(TAG, "onLoad, ", (isLoading ? "加载中..." : "加载完成。"));
            }
        });
        mMediaPlayer.setOnPauseResumeListener(new OnPauseResumeListener() {
            @Override
            public void onPause(boolean pause) {
                MediaLog.d(TAG, "onLoad, ", (pause ? "暂停..." : "播放..."));
            }
        });
        mMediaPlayer.setOnTimeInfoListener(new OnTimeInfoListener() {
            @Override
            public void onTimeInfo(int currentTime, int totalTime) {
                mTimeInfoView.setText(TimeUtils.toTimeText(currentTime, totalTime));
                MediaLog.d(TAG, "onTimeInfo, currentTime: ", currentTime, ", totalTime: ", totalTime);
            }
        });
        mMediaPlayer.setOnErrorListener(new OnErrorListener() {
            @Override
            public void onError(int errCode) {
                MediaLog.d(TAG, "错误码：", errCode);
            }
        });

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

        mVolumeSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                float percent = (float) mVolumeSeekBar.getProgress() / (float) mVolumeSeekBar.getMax();
                mMediaPlayer.setVolume(percent);
                setVolumeProgress(percent);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });

        mMediaPlayer.setOnCompleteListener(new OnCompleteListener() {
            @Override
            public void onComplete() {
                MediaLog.d(TAG, "播放完成。");
            }
        });


    }

    private void setVolumeProgress(float percent) {
        int progress = (int) (100 * percent);
        String text = "音量：" + progress + "%";
        mVolumeTextView.setText(text);
    }

    public void start(View view) {
        mMediaPlayer.setDataSource("http://mpge.5nd.com/2015/2015-11-26/69708/1.mp3");
        mMediaPlayer.prepare();
    }

    public void pause(View view) {
        mMediaPlayer.pause();
    }

    public void play(View view) {
        mMediaPlayer.resume();
    }

    public void stop(View view) {
        mMediaPlayer.stop();
    }

    public void playLocalFile(View view) {
        mMediaPlayer.setDataSource("/sdcard/jwm1972.mp4");
        mMediaPlayer.prepare();
    }
}
