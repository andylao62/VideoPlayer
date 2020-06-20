package com.kejunyao.media.view;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/**
 * 多媒体SurfaceView
 *
 * @author kejunyao
 * @since 2020年06月19日
 */
public class MediaSurfaceView extends GLSurfaceView {

    private static int EGL_CONTEXT_CLIENT_VERSION = 2;

    public MediaSurfaceView(Context context) {
        this(context, null);
    }

    public MediaSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public void setRenderer(Renderer renderer) {
    }

    private void init() {
        setEGLContextClientVersion(EGL_CONTEXT_CLIENT_VERSION);
        super.setRenderer(new MediaSurfaceRender(getContext()));
    }
}
