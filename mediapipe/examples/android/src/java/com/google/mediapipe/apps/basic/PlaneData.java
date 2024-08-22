package com.google.mediapipe.apps.basic;

public class PlaneData {
    public static final float[] PLANE_VERTEX = {
        -1.0f,  1.0f, 0.0f,  0.0f, 0.0f,  // top left
         0.0f,  1.0f, 0.0f,  1.0f, 0.0f,  // top right
        -1.0f, -1.0f, 0.0f,  0.0f, 1.0f,  // bottom left
         0.0f, -1.0f, 0.0f,  1.0f, 1.0f,  // bottom right
    };

    public static final int[] PLANE_INDICES = {
        0, 1, 2,
        0, 2, 3
    };
}
