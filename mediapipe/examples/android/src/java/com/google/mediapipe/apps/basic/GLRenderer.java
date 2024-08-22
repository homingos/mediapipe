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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public abstract class GLRenderer implements GLSurfaceView.Renderer {

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

    private int bitMapTextureId = R.drawable.ic_refresh;

    private final int vertexStride = 5 * 4; // 4 bytes per vertex (3 for position, 2 for texture coordinates)

    private Bitmap bitmap;
    private Context context;
    private int screenWidth;
    private int screenHeight;

    private final String vertexShaderCode = ShaderManager.VERTEX_SHADER_CODE;
    private final String fragmentShaderCode = ShaderManager.FRAGMENT_SHADER_CODE;

    // Coordinates for vertices
    private float triangleCoords[] = {
        0.0f,  0.622008459f, 0.0f, // top
       -0.5f, -0.311004243f, 0.0f, // bottom left
        0.5f, -0.311004243f, 0.0f  // bottom right
    };
    float color[] = { 0.63671875f, 0.76953125f, 0.22265625f, 1.0f }; // Color

    private static final String TAG = "Aman GLRenderer";

    public void setContext(Context context){
        this.context = context;
    }

    public void setBitmap(int resourceId) {
        if(this.context != null) {
            this.bitmap = BitmapFactory.decodeResource(this.context.getResources(), resourceId);
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);
            bitmap.recycle();
        } else {
            Log.e("GLRenderer", "Context not set before calling setBitmap.");
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // setupShaders();
        // setupBuffers();
        // setupTextures();
        // setBitmap(bitMapTextureId);
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
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        GLES20.glUseProgram(mProgram);

        int positionHandle = GLES20.glGetAttribLocation(mProgram, "vPosition");
        GLES20.glEnableVertexAttribArray(positionHandle);
        vertexBuffer.position(0);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 0, vertexBuffer);
        
        colorHandle = GLES20.glGetUniformLocation(mProgram, "vColor");
        GLES20.glUniform4fv(colorHandle, 1, color, 0);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);

        GLES20.glDisableVertexAttribArray(positionHandle);
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
        IntBuffer intBuffer = IntBuffer.allocate(1);
        GLES20.glGetIntegerv(GLES20.GL_CURRENT_PROGRAM, intBuffer.array(), 0);
        int program = intBuffer.get(0);

        positionHandle = GLES20.glGetAttribLocation(program, "vPosition");
        GLES20.glEnableVertexAttribArray(positionHandle);
        vertexBuffer.position(offset * 20);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

        textureCoordHandle = GLES20.glGetAttribLocation(program, "a_TexCoordinate");
        GLES20.glEnableVertexAttribArray(textureCoordHandle);
        vertexBuffer.position(3 + offset * 20);
        GLES20.glVertexAttribPointer(textureCoordHandle, 2, GLES20.GL_FLOAT, false, vertexStride, vertexBuffer);

        textureHandle = GLES20.glGetUniformLocation(program, "u_Texture");
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textures[0]);
        GLES20.glUniform1i(textureHandle, 0);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);

        GLES20.glDisableVertexAttribArray(positionHandle);
        GLES20.glDisableVertexAttribArray(textureCoordHandle);
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);
        return shader;
    }
    
    public void updateBoxVertices(float[] boxFloats) {
        Log.d(TAG, "updateBoxVertices: " + Arrays.toString(boxFloats));
    
        // Convert screen space coordinates to normalized device coordinates (NDC)
        float[] ndcCoordinates = new float[8];
        for (int i = 0; i < 4; i++) {
            ndcCoordinates[2 * i] = (boxFloats[2 * i] / screenWidth) * 2.0f - 1.0f;
            ndcCoordinates[2 * i + 1] = 1.0f - (boxFloats[2 * i + 1] / screenHeight) * 2.0f;
        }
    
        // Define vertices for the rectangle (assuming the points are in order)
        // float[] vertices = new float[] {
        //     ndcCoordinates[0], ndcCoordinates[1], 0.0f, // Top-Left
        //     ndcCoordinates[4], ndcCoordinates[5], 0.0f,  // Top-right
        //     ndcCoordinates[2], ndcCoordinates[3], 0.0f, // Bottom-left
        //     ndcCoordinates[6], ndcCoordinates[7], 0.0f // Bottom-right
        // };
        float[] vertices = {
            -0.5f, -0.5f, 0.0f,  // Bottom-left
             0.5f, -0.5f, 0.0f,  // Bottom-right
            -0.5f,  0.5f, 0.0f,  // Top-left
             0.5f,  0.5f, 0.0f   // Top-right
        };
        
        vertexBuffer.clear();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);
    }
    
}