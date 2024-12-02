package com.google.mediapipe.components;

public interface CloudScanDetectionListener {
    // void onTrackingUpdate(boolean isLost);
    // void onCoordinatesUpdate(float[] coordinates);
    void onDetecionAccquired(String url);
    void onError(String error);
}