package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;

import com.google.mediapipe.glutil.EglManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CustomGLSurfaceView extends GLSurfaceView {

    private GLRenderer mGLRenderer;

    public CustomGLSurfaceView(Context context) {
        super(context);

        setEGLContextClientVersion(2);
        mGLRenderer = new GLRenderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                super.onSurfaceCreated(gl, config);
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                // Override this method if needed
            }
        };
        mGLRenderer.setContext(context);
        setRenderer(mGLRenderer);
    }
    
    public GLRenderer getRenderer() {
        return mGLRenderer;
    }
}
