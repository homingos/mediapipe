package com.google.mediapipe.apps.basic;

import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.hardware.Camera;
import android.opengl.GLUtils;
import android.util.Log;
import android.opengl.GLES11Ext;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class CameraRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private SurfaceTexture surfaceTexture;
    private int textureId;
    private float[] textureMatrix = new float[16];
    private GLRenderer mGLRenderer;
    private Camera camera;

    public CameraRenderer(GLRenderer glRenderer) {
        this.mGLRenderer = glRenderer;
    }

    @Override
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

        // Initialize GLRenderer's surface
        mGLRenderer.onSurfaceCreated(gl, config);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

        synchronized (this) {
            try {
                surfaceTexture.updateTexImage();
            } catch (RuntimeException e) {
                Log.e("CameraRenderer", "Error during updateTexImage: " + e.getMessage());
                return;
            }
        }

        surfaceTexture.getTransformMatrix(textureMatrix);

        // Draw the camera view as background
        drawCameraView(textureId, textureMatrix);

        // Now draw the GLRenderer content on top
        mGLRenderer.onDrawFrame(gl);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        mGLRenderer.onSurfaceChanged(gl, width, height);
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        // Request rendering when a new frame is available
        mGLRenderer.onFrameAvailable(surfaceTexture);
    }

    private int createTexture() {
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textures[0]); // Use GL_TEXTURE_EXTERNAL_OES
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        return textures[0];
    }

    public void drawCameraView(int textureId, float[] textureMatrix) {
        // Implement camera view drawing here if needed
    }
}
