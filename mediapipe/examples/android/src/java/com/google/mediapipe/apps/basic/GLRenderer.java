package com.google.mediapipe.apps.basic;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.graphics.SurfaceTexture;
import android.opengl.GLUtils;
import android.media.MediaPlayer;
import android.content.Context;
import android.view.Surface;
import android.util.Log;
import android.opengl.GLES11Ext;

import androidx.camera.core.Camera;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.Preview;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.core.content.ContextCompat;
import androidx.lifecycle.LifecycleOwner;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.concurrent.ExecutionException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public abstract class GLRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {

    private static final String TAG = "Aman GLRenderer";

    private FloatBuffer vertexBuffer;
    private FloatBuffer bgVertexBuffer;
    private FloatBuffer textureBuffer;
    private int mProgram;
    private int bgProgram;
    private int positionHandle;
    private int textureHandle;
    private int textureId;
    private int bgTextureId;
    private SurfaceTexture surfaceTexture;
    private SurfaceTexture bgSurfaceTexture;
    private MediaPlayer mediaPlayer;

    private final int vertexCount = 4;
    private final int vertexStride = 3 * 4;

    private boolean firstFrameReceived = false;

    private final String vertexShaderCode =
        "attribute vec4 vPosition;" +
        "attribute vec2 aTexCoord;" +
        "varying vec2 vTexCoord;" +
        "void main() {" +
        "  gl_Position = vPosition;" +
        "  vTexCoord = aTexCoord;" +
        "}";
    private final String fragmentShaderCode =
        "#extension GL_OES_EGL_image_external : require \n" +
        "precision mediump float;" +
        "uniform samplerExternalOES uTexture;" +
        "varying vec2 vTexCoord;" +
        "void main() {" +
        "  gl_FragColor = texture2D(uTexture, vTexCoord);" +
        "}";

    private final String bgVertexShaderCode =
        "attribute vec4 vPosition;" +
        "attribute vec2 aTexCoord;" +
        "varying vec2 vTexCoord;" +
        "void main() {" +
        "  gl_Position = vPosition;" +
        "  vTexCoord = aTexCoord;" +
        "}";
    
    // Fragment Shader for background
    private final String bgFragmentShaderCode =
    "#extension GL_OES_EGL_image_external : require \n" +
    "precision mediump float;" +
    "uniform samplerExternalOES bgTexture;" +
    "varying vec2 vTexCoord;" +
    "void main() {" +
    "  gl_FragColor = texture2D(bgTexture, vTexCoord);" +
    "}";

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

    private float fullScreenVertexCoordinates[] = {
        -1.0f,  1.0f, 0.0f,   // top left
         1.0f,  1.0f, 0.0f,   // top right
        -1.0f, -1.0f, 0.0f,   // bottom left
         1.0f, -1.0f, 0.0f    // bottom right
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

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        setupBuffers();
        compileAndLinkShaders();
        setupTextures();
        setupCameraX();
        initializeMediaPlayer();
    }

    // Initialize and set up vertex and texture buffers
    private void setupBuffers() {
        bgVertexBuffer = createFloatBuffer(fullScreenVertexCoordinates);
        vertexBuffer = createFloatBuffer(vertexCoordinates);
        textureBuffer = createFloatBuffer(textureCoordinates);
    }

    private FloatBuffer createFloatBuffer(float[] coordinates) {
        ByteBuffer bb = ByteBuffer.allocateDirect(coordinates.length * 4);
        bb.order(ByteOrder.nativeOrder());
        FloatBuffer buffer = bb.asFloatBuffer();
        buffer.put(coordinates);
        buffer.position(0);
        return buffer;
    }

    // Compile and link shaders
    private void compileAndLinkShaders() {
        bgProgram = createProgram(bgVertexShaderCode, bgFragmentShaderCode);
        mProgram = createProgram(vertexShaderCode, fragmentShaderCode);
    }

    private int createProgram(String vertexCode, String fragmentCode) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentCode);

        int program = GLES20.glCreateProgram();
        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);

        checkLinkStatus(program);
        return program;
    }

    private void checkLinkStatus(int program) {
        int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
        if (linkStatus[0] != GLES20.GL_TRUE) {
            String errorMsg = GLES20.glGetProgramInfoLog(program);
            Log.e(TAG, "Error linking program: " + errorMsg);
        }
    }

    private void setupTextures() {
        int[] textures = new int[2];
        GLES20.glGenTextures(2, textures, 0);
        textureId = textures[0];
        bgTextureId = textures[1];

        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, bgTextureId);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

        bgSurfaceTexture = new SurfaceTexture(bgTextureId);
        bgSurfaceTexture.setOnFrameAvailableListener(this);

        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);

        surfaceTexture = new SurfaceTexture(textureId);
        surfaceTexture.setOnFrameAvailableListener(this);
    }

    private void setupCameraX() {
        ProcessCameraProvider cameraProvider;
        try {
            cameraProvider = ProcessCameraProvider.getInstance(getContext()).get();
        } catch (ExecutionException | InterruptedException e) {
            Log.e(TAG, "Error setting up CameraX", e);
            return;
        }

        Preview preview = new Preview.Builder().build();
        preview.setSurfaceProvider(request -> {
            Surface surface = new Surface(bgSurfaceTexture);
            request.provideSurface(surface, ContextCompat.getMainExecutor(getContext()), result -> {
                Log.d(TAG, "Surface provided for CameraX preview");
            });
        });

        CameraSelector cameraSelector = new CameraSelector.Builder()
            .requireLensFacing(CameraSelector.LENS_FACING_BACK)
            .build();

        cameraProvider.bindToLifecycle((LifecycleOwner) getContext(), cameraSelector, preview);
    }

    private void initializeMediaPlayer() {
        mediaPlayer = MediaPlayer.create(getContext(), R.raw.test);
        mediaPlayer.setSurface(new Surface(surfaceTexture));
        mediaPlayer.setLooping(true);
        mediaPlayer.start();
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        clearScreen();
        drawBackground();
        drawVideoFrame();
        checkOpenGLErrors();
    }

    private void clearScreen() {
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
    }

    private void drawBackground() {
        GLES20.glUseProgram(bgProgram);
        bgSurfaceTexture.updateTexImage();

        // Bind the background texture (camera texture)
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, bgTextureId);

        int bgTextureUniformHandle = GLES20.glGetUniformLocation(bgProgram, "bgTexture");
        GLES20.glUniform1i(bgTextureUniformHandle, 0);

        int bgPositionHandle = GLES20.glGetAttribLocation(bgProgram, "vPosition");
        int bgTextureCoordHandle = GLES20.glGetAttribLocation(bgProgram, "aTexCoord");

        // Set the vertex attribute pointers
        GLES20.glEnableVertexAttribArray(bgPositionHandle);
        GLES20.glVertexAttribPointer(bgPositionHandle, 3, GLES20.GL_FLOAT, false, 0, bgVertexBuffer);

        GLES20.glEnableVertexAttribArray(bgTextureCoordHandle);
        GLES20.glVertexAttribPointer(bgTextureCoordHandle, 2, GLES20.GL_FLOAT, false, 0, textureBuffer);

        // Draw the background as a full-screen quad
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);

        // Disable the vertex attribute arrays
        GLES20.glDisableVertexAttribArray(bgPositionHandle);
        GLES20.glDisableVertexAttribArray(bgTextureCoordHandle);
    }

    private void drawVideoFrame() {
        GLES20.glUseProgram(mProgram);
        surfaceTexture.updateTexImage();

        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, textureId);

        int textureUniformHandle = GLES20.glGetUniformLocation(mProgram, "uTexture");
        GLES20.glUniform1i(textureUniformHandle, 0);

        int positionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        int textureHandle = GLES20.glGetAttribLocation(mProgram, "aTexCoord");

        GLES20.glEnableVertexAttribArray(positionHandle);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

        GLES20.glEnableVertexAttribArray(textureHandle);
        GLES20.glVertexAttribPointer(textureHandle, 2, GLES20.GL_FLOAT, false, 2 * 4, textureBuffer);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, vertexCount);

        GLES20.glDisableVertexAttribArray(positionHandle);
        GLES20.glDisableVertexAttribArray(textureHandle);
    }

    private void checkOpenGLErrors() {
        int error = GLES20.glGetError();
        if (error != GLES20.GL_NO_ERROR) {
            Log.e("GL_ERROR", "OpenGL Error: " + error);
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        int[] compileStatus = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);

        if (compileStatus[0] == 0) {
            Log.e(TAG, "Shader Compilation Error: " + GLES20.glGetShaderInfoLog(shader));
            GLES20.glDeleteShader(shader);
            shader = 0;
        }

        return shader;
    }

    @Override
    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
        firstFrameReceived = true;
    }

    protected abstract Context getContext();
}