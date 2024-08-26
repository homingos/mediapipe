package com.google.mediapipe.apps.basic;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.graphics.SurfaceTexture;
import android.opengl.GLUtils;
import android.media.MediaPlayer;
import android.content.Context;
import android.view.Surface;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public abstract class GLRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;
    private final String vertexShaderCode =
        "attribute vec4 vPosition;" +
        "attribute vec2 aTexCoord;" +
        "varying vec2 vTexCoord;" +
        "void main() {" +
        "  gl_Position = vPosition;" +
        "  vTexCoord = aTexCoord;" +
        "}";
    private final String fragmentShaderCode =
        "precision mediump float;" +
        "uniform sampler2D uTexture;" +
        "varying vec2 vTexCoord;" +
        "void main() {" +
        "  gl_FragColor = texture2D(uTexture, vTexCoord);" +
        "}";
    private int mProgram;
    private int positionHandle;
    private int textureHandle;
    private int textureId;
    private SurfaceTexture surfaceTexture;
    private MediaPlayer mediaPlayer;
    private final int vertexCount = 4;
    private final int vertexStride = 0;

    private float vertexCoordinates[] = {
        -0.5f,  0.25f, 0.0f,   // top left
         0.5f,  0.25f, 0.0f,   // top right
        -0.5f, -0.25f, 0.0f,   // bottom left
         0.5f, -0.25f, 0.0f    // bottom right
    };

    private float[] textureCoordinates = {
        0.0f, 0.0f,  // top left
        1.0f, 0.0f,  // top right
        0.0f, 1.0f,  // bottom left
        1.0f, 1.0f   // bottom right
    };

    float color[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Red color

    public void setPlaneCoordinates(float[] coordinates) {
        if (coordinates.length == 12) {
            vertexCoordinates = coordinates;
            if (vertexBuffer != null) {
                vertexBuffer.clear();
                vertexBuffer.put(vertexCoordinates);
                vertexBuffer.position(0);
            }
        }
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        ByteBuffer bb = ByteBuffer.allocateDirect(vertexCoordinates.length * 4);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();
        vertexBuffer.put(vertexCoordinates);
        vertexBuffer.position(0);

        // Setup texture Buffer
        ByteBuffer tb = ByteBuffer.allocateDirect(textureCoordinates.length * 4);
        tb.order(ByteOrder.nativeOrder());
        textureBuffer = tb.asFloatBuffer();
        textureBuffer.put(textureCoordinates);
        textureBuffer.position(0);

        // Complie the shader and link program
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        mProgram = GLES20.glCreateProgram();
        GLES20.glAttachShader(mProgram, vertexShader);
        GLES20.glAttachShader(mProgram, fragmentShader);
        GLES20.glLinkProgram(mProgram);

        // Set up texture
        int[] textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        textureId = textures[0];

        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

        surfaceTexture = new SurfaceTexture(textureId);
        surfaceTexture.setOnFrameAvailableListener(this);

        // Set up MediaPlayer
        mediaPlayer = MediaPlayer.create(getContext(), R.raw.test);
        mediaPlayer.setSurface(new Surface(surfaceTexture));
        mediaPlayer.setLooping(true);
        mediaPlayer.start();
    }

    public void onDrawFrame(GL10 gl) {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
        GLES20.glUseProgram(mProgram);

        // Update Texture with the video frame
        surfaceTexture.updateTexImage();

        //Pass vertex data to shader
        positionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        GLES20.glEnableVertexAttribArray(positionHandle);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 0, vertexBuffer);

        // Pass texture coordinates data to shader
        textureHandle = GLES20.glGetAttribLocation(mProgram, "aTexCoord");
        GLES20.glEnableVertexAttribArray(textureHandle);
        GLES20.glVertexAttribPointer(textureHandle, 2, GLES20.GL_FLOAT, false, 0, textureBuffer);

        // Bind the texture
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);

        // Draw the plane
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, vertexCount);

        // disable attribute arrays
        GLES20.glDisableVertexAttribArray(positionHandle);
        GLES20.glDisableVertexAttribArray(textureHandle);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }

    protected abstract Context getContext();
}