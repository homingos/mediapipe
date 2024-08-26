package com.google.mediapipe.apps.basic;

import android.opengl.GLES20;
import android.opengl.Matrix;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.List;

public class PlaneRenderer {

    private int shaderProgram;
    private int positionHandle;
    private int colorHandle;
    private int modelMatrixHandle;

    // Plane vertex coordinates
    private static final float[] PLANE_COORDS = {
            -0.1f, -0.1f, 0.0f,  // Bottom Left
            -0.1f, 0.1f, 0.0f,   // Top Left
            0.1f, -0.1f, 0.0f,   // Bottom Right
            0.1f, 0.1f, 0.0f     // Top Right
    };

    private FloatBuffer vertexBuffer;

    private final float[] color ={0.0f, 1.0f, 0.0f, 1.0f};

    public PlaneRenderer() {

        initializeBuffers();
        initializeShaderProgram();
    }

    private void initializeBuffers() {
        ByteBuffer bb = ByteBuffer.allocateDirect(PLANE_COORDS.length * 4);
        bb.order(ByteOrder.nativeOrder());
        vertexBuffer = bb.asFloatBuffer();
        vertexBuffer.put(PLANE_COORDS);
        vertexBuffer.position(0);
    }

    private void initializeShaderProgram() {
        String vertexShaderCode =
                "#version 300 es\n" +
                        "layout(location = 0) in vec4 vertexPosition;\n" +
                        "uniform mat4 modelMatrix;\n" +
                        "void main() {\n" +
                        "    gl_Position = modelMatrix * vertexPosition;\n" +
                        "}\n";

        String fragmentShaderCode =
                "#version 300 es\n" +
                        "precision mediump float;\n" +
                        "out vec4 outColor;\n" +
                        "uniform vec4 uColor;\n" +
                        "void main() {\n" +
                        "    outColor = uColor;\n" +
                        "}\n";

        shaderProgram = createProgram(vertexShaderCode, fragmentShaderCode);

        positionHandle = GLES20.glGetAttribLocation(shaderProgram, "vertexPosition");
        colorHandle = GLES20.glGetUniformLocation(shaderProgram, "uColor");
        modelMatrixHandle = GLES20.glGetUniformLocation(shaderProgram, "modelMatrix");
    }

    private int createProgram(String vertexShaderCode, String fragmentShaderCode) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
        int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

        int program = GLES20.glCreateProgram();
        GLES20.glAttachShader(program, vertexShader);
        GLES20.glAttachShader(program, fragmentShader);
        GLES20.glLinkProgram(program);

        int[] linkStatus = new int[1];
        GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
        if (linkStatus[0] == 0) {
            String errorMsg = GLES20.glGetProgramInfoLog(program);
            GLES20.glDeleteProgram(program);
            throw new RuntimeException("Error linking program: " + errorMsg);
        }

        return program;
    }

    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        GLES20.glShaderSource(shader, shaderCode);
        GLES20.glCompileShader(shader);

        int[] compileStatus = new int[1];
        GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);
        if (compileStatus[0] == 0) {
            String errorMsg = GLES20.glGetShaderInfoLog(shader);
            GLES20.glDeleteShader(shader);
            throw new RuntimeException("Error compiling shader: " + errorMsg);
        }

        return shader;
    }

    public void draw(float[] centerAndOrientation) {
        float[] modelMatrix = new float[16];
        Matrix.setIdentityM(modelMatrix, 0);

        // Translate to the center of the fist
        Matrix.translateM(modelMatrix, 0, centerAndOrientation[0], centerAndOrientation[1], centerAndOrientation[2]);

        // Calculate the rotation angle and axis from the orientation vector
        float angle = (float) Math.toDegrees(Math.atan2(centerAndOrientation[4], centerAndOrientation[3]));
        Matrix.rotateM(modelMatrix, 0, angle, 0.0f, 0.0f, 1.0f);

        GLES20.glUseProgram(shaderProgram);

        GLES20.glUniformMatrix4fv(modelMatrixHandle, 1, false, modelMatrix, 0);
        GLES20.glUniform4fv(colorHandle, 1, color, 0);

        GLES20.glEnableVertexAttribArray(positionHandle);
        GLES20.glVertexAttribPointer(positionHandle, 3, GLES20.GL_FLOAT, false, 3 * 4, vertexBuffer);

        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);

        GLES20.glDisableVertexAttribArray(positionHandle);
    }

    public float[] calculateFistCenterAndOrientation(List<float[]> landmarks) {
        float centerX = 0, centerY = 0, centerZ = 0;
        int[] fistLandmarks = {0, 5, 9, 13, 17}; // Wrist and base of each finger

        for (int i : fistLandmarks) {
            centerX += landmarks.get(i)[0];
            centerY += landmarks.get(i)[1];
            centerZ += landmarks.get(i)[2];
        }

        centerX /= fistLandmarks.length;
        centerY /= fistLandmarks.length;
        centerZ /= fistLandmarks.length;

        float[] wrist = landmarks.get(0);
        float[] middleFinger = landmarks.get(9);
        float[] orientation = new float[3];

        for (int i = 0; i < 3; i++) {
            orientation[i] = middleFinger[i] - wrist[i];
        }

        return new float[]{centerX, centerY, centerZ, orientation[0], orientation[1], orientation[2]};
    }
}