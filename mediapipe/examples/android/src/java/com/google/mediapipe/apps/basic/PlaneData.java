package com.google.mediapipe.apps.basic;

public class PlaneData {
    public static float vertexCoordinates[] = {
        -1.0f, 1.0f, 0.0f, // top left
        1.0f, 1.0f, 0.0f, // top right
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f // bottom right
    };

    public static float[] textureCoordinates = {
        0.0f, 0.0f, // top left
        1.0f, 0.0f, // top right
        0.0f, 1.0f, // bottom left
        1.0f, 1.0f // bottom right
    };

    public static float fullScreenVertexCoordinates[] = {
        -1.0f, 1.0f, 0.0f, // top left
        1.0f, 1.0f, 0.0f, // top right
        -1.0f, -1.0f, 0.0f, // bottom left
        1.0f, -1.0f, 0.0f // bottom right
    };
}
