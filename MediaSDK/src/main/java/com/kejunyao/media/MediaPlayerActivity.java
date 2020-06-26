package com.kejunyao.media;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.Toast;
import com.kejunyao.media.util.FileUtils;
import com.kejunyao.media.util.PermissionUtils;
import com.kejunyao.media.widget.MediaPlayerView;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MediaPlayerActivity extends AppCompatActivity {

    private static final String TAG = "MediaPlayerActivity";

    private static final String PARAM_SOURCE = "media_source";

    public static void startActivity(Context context, String source) {
        Intent intent = new Intent(context, MediaPlayerActivity.class);
        if (!(context instanceof Activity)) {
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        }
        intent.putExtra(PARAM_SOURCE, source);
        context.startActivity(intent);
    }

    private MediaPlayerView mMediaPlayerView;
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
        mMediaPlayerView = findViewById(R.id.media_player_view);
        mMediaPlayerView.setResizeSurfaceViewEnabled(true);
        mSource = getIntent().getStringExtra(PARAM_SOURCE);
        if (TextUtils.isEmpty(mSource)) {
            Toast.makeText(getApplicationContext(), R.string.no_medial_source, Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        if (FileUtils.isLocalFile(mSource) && !PermissionUtils.hasReadExternalStoragePermission(this)) {
            PermissionUtils.requestReadExternalStoragePermission(this);
        } else {
            play();
        }
    }

    private void play() {
        mMediaPlayerView.prepare(mSource);
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        refreshStatusBarShown();
        mMediaPlayerView.resizeMediaView(
                getResources().getDisplayMetrics().widthPixels,
                FrameLayout.LayoutParams.WRAP_CONTENT
        );
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        mMediaPlayerView.resume();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mMediaPlayerView.pause();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (PermissionUtils.isRequestReadExternalStoragePermissionSuccess(this, requestCode)) {
            play();
        } else {
            Toast.makeText(this, R.string.not_read_storage_permission, Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onDestroy() {
        mMediaPlayerView.stop();
        mMediaPlayerView.release();
        super.onDestroy();
    }

    /**
     * 刷新系统状态栏的显示/隐藏
     */
    private void refreshStatusBarShown() {
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
