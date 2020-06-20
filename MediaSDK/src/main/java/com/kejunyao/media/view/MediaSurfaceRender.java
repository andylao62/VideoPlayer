package com.kejunyao.media.view;

import android.content.Context;
import android.icu.lang.UProperty;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import com.kejunyao.media.R;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * 多媒体渲染器
 *
 * @author kejunyao
 * @since 2020年06月19日
 */
public class MediaSurfaceRender implements GLSurfaceView.Renderer {

    private final float[] mVertexData = {
            -1f, 0f,
            0f, 1f,
            1f, 0f
    };

    private Context mContext;
    private final FloatBuffer mVertexBuffer;
    private int mProgram;
    private int mAvPosition;
    private int mAfColor;

    public MediaSurfaceRender(Context context) {
        mContext = context;
        mVertexBuffer = ByteBuffer.allocateDirect(mVertexData.length * 4)
                .order(ByteOrder.nativeOrder())
                .asFloatBuffer()
                .put(mVertexData);
        mVertexBuffer.position(0);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String vertexSource = RenderShaderUtils.readRaw(mContext, R.raw.vertex_shader);
        String fragmentSource = RenderShaderUtils.readRaw(mContext, R.raw.fragment_shader);
        mProgram = RenderShaderUtils.createProgram(vertexSource, fragmentSource);
        if (mProgram > 0) {
            mAvPosition = GLES20.glGetAttribLocation(mProgram, "av_Position");
            mAfColor = GLES20.glGetUniformLocation(mProgram, "af_Color");
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
//        GLES20.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        GLES20.glUseProgram(mProgram);
        GLES20.glUniform4f(mAfColor, 1f, 0f, 0f, 1f);
        GLES20.glEnableVertexAttribArray(mAvPosition);
        GLES20.glVertexAttribPointer(mAvPosition, 2, GLES20.GL_FLOAT, false, 8, mVertexBuffer);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3);
    }
}
