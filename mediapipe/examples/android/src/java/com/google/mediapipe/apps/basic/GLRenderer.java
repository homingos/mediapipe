package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLRenderer implements GLSurfaceView.Renderer {

    private static final String TAG = "Aman GLRenderer";

    private FloatBuffer vertexBuffer;
    private int mProgram;
    private int positionHandle;
    private int colorHandle;

    private final int vertexStride = 3 * 4; // 4 bytes per vertex (3 for position)

    // Coordinates for the vertices of the triangle
    private float triangleCoords[] = {
            0.0f,  0.622008459f, 0.0f,   // top
           -0.5f, -0.311004243f, 0.0f,   // bottom left
            0.5f, -0.311004243f, 0.0f    // bottom right
    };

    // Color for the triangle (red, green, blue, alpha)
    private float color[] = { 0.63671875f, 0.76953125f, 0.22265625f, 1.0f };

    private final String vertexShaderCode =
            "attribute vec4 vPosition;" +
            "void main() {" +
            "  gl_Position = vPosition;" +
            "}";

    private final String fragmentShaderCode =
            "precision mediump float;" +
            "uniform vec4 vColor;" +
            "void main() {" +
            "  gl_FragColor = vColor;" +
            "}";

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

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT); // Clear the screen
        GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 3); // Draw the triangle

        GLES20.glDisableVertexAttribArray(positionHandle);
        Log.d(TAG, "onDrawFrame: Triangle rendered");
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
}
