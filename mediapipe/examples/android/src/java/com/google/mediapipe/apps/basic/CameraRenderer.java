package com.google.mediapipe.apps.basic;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public abstract class CameraRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
    
    private SurfaceTexture surfaceTexture;
    private int textureId;
    private Camera camera;
    private float[] textureMatrix = new float[16];
    
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        textureId = createTexture();
        surfaceTexture = new SurfaceTexture(textureId);
        surfaceTexture.setOnFrameAvailableListener(this);

        camera = Camera.open();
        try {
            camera.setPreviewTexture(surfaceTexture);
            camera.startPreview();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        surfaceTexture.updateTexImage();
        surfaceTexture.getTransformMatrix(textureMatrix);

        // Draw the camera view as background
        drawCameraView(textureId, textureMatrix);
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        // Request rendering when a new frame is available
    }

    private int createTexture() {
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        return textures[0];
    }

    protected abstract void drawCameraView(int textureId, float[] textureMatrix);
}
