package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.logging.LogManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLRenderer implements GLSurfaceView.Renderer {

    private static final String TAG = "Aman GLRenderer";

    private FloatBuffer vertexBuffer;
    private FloatBuffer textureBuffer;
    private int[] textures = new int[1];

    private int mProgram;
    private int mGrayscaleProgram;
    private int mBlurProgram;
    private int positionHandle;
    private int textureCoordHandle;
    private int textureHandle;
    private int colorHandle;

    private final int vertexStride = 5 * 4; // 4 bytes per vertex (3 for position, 2 for texture coordinates)

    private Bitmap bitmap;
    private Context context;
    private int screenWidth;
    private int screenHeight;

    private final String vertexShaderCode = ShaderManager.VERTEX_SHADER_CODE;
    private final String fragmentShaderCode = ShaderManager.FRAGMENT_SHADER_CODE;

    public PlaneRenderer planeRenderer;

    // Coordinates for vertices
    private float triangleCoords[] = {
            0.0f, 0.622008459f, 0.0f, // top
            -0.5f, -0.311004243f, 0.0f, // bottom left
            0.5f, -0.311004243f, 0.0f // bottom right
    };
    float color[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // Color

    public void setContext(Context context) {
        this.context = context;
    }

    public void setBitmap(int resourceId) {
        if (this.context != null) {
            this.bitmap = BitmapFactory.decodeResource(this.context.getResources(), resourceId);
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
            bitmap.recycle();
        } else {
            Log.e("GLRenderer", "Context not set before calling setBitmap.");
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Initialize vertex byte buffer for shape coordinates
        ByteBuffer bb = ByteBuffer.allocateDirect(triangleCoords.length * 4);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();
        vertexBuffer.put(triangleCoords);
        vertexBuffer.position(0);

        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        mProgram = GLES20.glCreateProgram(); // Create empty OpenGL Program
        GLES20.glAttachShader(mProgram, vertexShader); // Add the vertex shader
        GLES20.glAttachShader(mProgram, fragmentShader); // Add the fragment shader
        GLES20.glLinkProgram(mProgram); // Link the program
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        GLES20.glUseProgram(mProgram); // Use the program

        positionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        GLES20.glEnableVertexAttribArray(positionHandle);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

        colorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
        GLES20.glUniform4fv(colorHandle, 1, color, 0);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 4); // Draw the triangle

        GLES20.glDisableVertexAttribArray(positionHandle);
        Log.d(TAG, "onDrawFrame: ");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        GLES20.glViewport(0, 0, width, height);
        screenWidth = width;
        screenHeight = height;
    }

    private void setupShaders() {
        // Load and compile shaders, then link programs
        mProgram = createProgram(vertexShaderCode, fragmentShaderCode);
    }

    private int createProgram(String vertexShaderCode, String fragmentShaderCode) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        int program = GLES20.glCreateProgram();
        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);

        return program;
    }

    private void setupBuffers() {
        // Initialize vertex byte buffer for shape coordinates
        ByteBuffer bb = ByteBuffer.allocateDirect(PlaneData.PLANE_VERTEX.length * 4);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();
        vertexBuffer.put(PlaneData.PLANE_VERTEX);
        vertexBuffer.position(0);
    }

    private void setupTextures() {
        // Load the texture
        GLES20.glGenTextures(1, textures, 0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);

        // Set filtering
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
    }

    private void renderImage(int offset) {
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }
}