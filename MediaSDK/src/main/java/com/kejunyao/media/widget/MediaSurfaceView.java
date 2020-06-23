package com.kejunyao.media.widget;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.Surface;

/**
 * 多媒体SurfaceView
 *
 * @author kejunyao
 * @since 2020年06月19日
 */
public class MediaSurfaceView extends GLSurfaceView {

    private static int EGL_CONTEXT_CLIENT_VERSION = 2;

    private final SurfaceRender mSurfaceRender;

    public MediaSurfaceView(Context context) {
        this(context, null);
    }

    public MediaSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mSurfaceRender = new SurfaceRender(context);
        init();
    }

    public void setRenderer(Renderer renderer) {
    }

    private void init() {
        setEGLContextClientVersion(EGL_CONTEXT_CLIENT_VERSION);
        super.setRenderer(mSurfaceRender);
    }

    public void setYUVData(int width, int height, byte[] y, byte[] u, byte[] v) {
        mSurfaceRender.setRenderMediaCodec(false);
        mSurfaceRender.setYUVRenderData(width, height, y, u, v);
        requestRender();
    }

    public void useMediaCodecRender() {
        mSurfaceRender.setRenderMediaCodec(true);
    }

    public Surface getSurface() {
        return mSurfaceRender.getSurface();
    }
}
