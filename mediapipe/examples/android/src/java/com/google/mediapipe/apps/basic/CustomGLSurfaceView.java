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
    private static final String TAG = "Aman CustomGLSurfaceView";

    public CustomGLSurfaceView(Context context) {
        super(context);

        setEGLContextClientVersion(2);
        mGLRenderer = new GLRenderer();
        Log.d(TAG, "CustomGLSurfaceView: GLRenderer created");
        mGLRenderer.setContext(context);
        setRenderer(mGLRenderer);
    }
    
    public GLRenderer getRenderer() {
        if(mGLRenderer == null) {
            Log.d(TAG, "getRenderer: GLRenderer is null");
        }
        else {
            Log.d(TAG, "getRenderer: GLRenderer is not null");
        }
        return mGLRenderer;
    }
}
