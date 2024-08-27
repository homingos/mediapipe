package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.graphics.SurfaceTexture;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CustomGLSurfaceView extends GLSurfaceView {

    private GLRenderer mGLRenderer;

    public CustomGLSurfaceView(Context context) {
        super(context);

        setEGLContextClientVersion(2);
        mGLRenderer = new GLRenderer() {

            @Override
            protected Context getContext() {
                return context;
            }

            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                super.onSurfaceCreated(gl, config);
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                // Override this method if needed
            }
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                // Handle the frame available event here
            }
            
        };
        CameraRenderer cameraRenderer = new CameraRenderer(mGLRenderer) {
            @Override
            public void drawCameraView(int textureId, float[] textureMatrix) {
                // Implement camera view drawing here if needed
            }
        };
        setRenderer(cameraRenderer);
    }

    public void setPlaneCoordinates(float[] coordinates) {
        mGLRenderer.setPlaneCoordinates(coordinates);
    }
}
