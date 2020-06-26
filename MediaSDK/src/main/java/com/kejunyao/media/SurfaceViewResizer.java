package com.kejunyao.media;

import android.view.View;
import android.view.ViewGroup;

/**
 * 媒体View宽高及方向调整器
 *
 * @author kejunyao
 * @since 2020年05月31日
 */
public class SurfaceViewResizer {

    private static final String TAG = "SurfaceViewResizer";

    /** 媒体宽度 */
    private int mWidth;
    /** 媒体高度 */
    private int mHeight;
    boolean mResizeEnabled;

    void resizeSurfaceView(View surfaceView, int expectedSurfaceWidth, int sourceSurfaceHeight) {
        if (!mResizeEnabled) {
            return;
        }
        if (surfaceView != null && mWidth > 0 && mHeight > 0) {
            ViewGroup.LayoutParams layoutParams = surfaceView.getLayoutParams();
            int targetSurfaceHeight;
            if (layoutParams instanceof ViewGroup.MarginLayoutParams) {
                ViewGroup.MarginLayoutParams mlp = (ViewGroup.MarginLayoutParams) layoutParams;
                if (mWidth >= mHeight) {
                    // 横屏视频
                    targetSurfaceHeight = expectedSurfaceWidth * mHeight / mWidth;
                } else {
                    // 竖屏视频
                    if (sourceSurfaceHeight > 0) {
                        targetSurfaceHeight = mHeight <= sourceSurfaceHeight ? mHeight : sourceSurfaceHeight;
                    } else {
                        targetSurfaceHeight = expectedSurfaceWidth * mWidth / mHeight;
                    }
                }
                if (targetSurfaceHeight < sourceSurfaceHeight) {
                    mlp.topMargin = (sourceSurfaceHeight - targetSurfaceHeight) / 2;
                    mlp.bottomMargin = mlp.topMargin;
                }
            } else {
                targetSurfaceHeight = expectedSurfaceWidth * mWidth / mHeight;
            }
            layoutParams.width = expectedSurfaceWidth;
            layoutParams.height = targetSurfaceHeight;
            surfaceView.requestLayout();
            MediaLog.d(TAG,
                    "targetSurfaceWidth: ", expectedSurfaceWidth,
                    ", targetSurfaceHeight: ", targetSurfaceHeight
            );
        }
    }

    void setMediaSize(int width, int height) {
        mWidth = width;
        mHeight = height;
    }
}
