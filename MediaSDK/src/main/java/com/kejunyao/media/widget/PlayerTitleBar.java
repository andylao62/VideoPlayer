package com.kejunyao.media.widget;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import androidx.annotation.Nullable;

import com.kejunyao.arch.util.ActivityUtils;
import com.kejunyao.arch.widget.BaseLinearLayout;
import com.kejunyao.media.R;

/**
 * 播放器标题View
 *
 * @author kejunyao
 * @since 2020年09月24日
 */
public class PlayerTitleBar extends BaseLinearLayout {

    private ImageView mBackIcon;
    private TextView mTitleView;
    private ImageView mBatteryView;
    private TextView mSystemTimeView;

    private BatteryReceiver mBatteryReceiver;
    /** 是否注册BatteryReceiver */
    private boolean mIsRegister;

    public PlayerTitleBar(Context context) {
        this(context, null);
    }

    public PlayerTitleBar(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public PlayerTitleBar(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public PlayerTitleBar(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        this(context, attrs, defStyleAttr);
    }

    @Override
    protected int layoutId() {
        return R.layout.player_title_bar_layout;
    }

    @Override
    protected void findViews() {
        super.setOrientation(HORIZONTAL);
        mBackIcon = findViewById(R.id.back);
        mTitleView = findViewById(R.id.title);
        mBatteryView = findViewById(R.id.battery);
        mSystemTimeView = findViewById(R.id.system_time);
    }

    @Override
    protected void setAttrs(AttributeSet attrs) {
        mBackIcon.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                Context context = v.getContext();
                Activity activity = ActivityUtils.getActivity(context);
                if (activity != null) {
                    activity.onBackPressed();
                }
            }
        });
    }

    public void setOrientation(int orientation) {
    }

    public void setTitle(CharSequence title) {
        mTitleView.setText(title);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        if (!mIsRegister) {
            getContext().registerReceiver(mBatteryReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
            mIsRegister = true;
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        if (mIsRegister) {
            getContext().unregisterReceiver(mBatteryReceiver);
            mIsRegister = false;
        }
    }

    private static class BatteryReceiver extends BroadcastReceiver {

        private ImageView pow;

        public BatteryReceiver(ImageView pow) {
            this.pow = pow;
        }

        @Override
        public void onReceive(Context context, Intent intent) {
            Bundle extras = intent.getExtras();
            if (extras == null) return;
            int current = extras.getInt("level"); // 获得当前电量
            int total = extras.getInt("scale"); // 获得总电量
            int percent = current * 100 / total;
            pow.getDrawable().setLevel(percent);
        }
    }
}
