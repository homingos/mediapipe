package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.graphics.SurfaceTexture;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.media.MediaPlayer;
import android.view.Surface;
import android.util.Log;

public class CustomGLSurfaceView extends GLSurfaceView {

    private final GLRenderer mGLRenderer;
    private static final String TAG = "Aman CustomGLSurfaceView";
    private MediaPlayer mediaPlayer;
    // private String videoPath;

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
        setRenderer(mGLRenderer);
    }

    public void initMediaplayer(String videoPath) {
        SurfaceTexture surfaceTexture = mGLRenderer.getSurfaceTexture();
        mediaPlayer = new MediaPlayer(); // Create the MediaPlayer instance
        try {
            Log.d(TAG, "Setting data source: " + videoPath);
            mediaPlayer.setDataSource(videoPath); // Set the data source
            mediaPlayer.setSurface(new Surface(surfaceTexture));
            mediaPlayer.setLooping(true);
            mediaPlayer.prepare(); // Prepare the media player
            mediaPlayer.start(); // Start playback
        } catch (Exception e) {
            e.printStackTrace(); // Handle exceptions
        }
    }

    public void setPlaneCoordinates(float[] coordinates) {
        mGLRenderer.setPlaneCoordinates(coordinates);
    }
}
