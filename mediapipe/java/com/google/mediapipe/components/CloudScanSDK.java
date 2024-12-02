package com.google.mediapipe.components;

import android.content.Context;
import com.google.mediapipe.components.CloudScanDetectionListener;
import android.app.Activity;

public interface CloudScanSDK {
    void initialize(Context context);
    void startDetection(Activity activity);
    void stopDetection();
    void updateTrackStatus(int status);
    void release();
    void setDetectionListener(CloudScanDetectionListener listener);
}