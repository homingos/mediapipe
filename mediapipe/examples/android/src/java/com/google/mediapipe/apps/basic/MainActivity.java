// Copyright 2024 Flamingos Technologies Inc. All Rights Reserved.
//
// PROPRIETARY AND CONFIDENTIAL
//
// This software and its documentation are the confidential and proprietary
// information of Flamingos Technologies Inc. ("Confidential Information").
// You shall not disclose such Confidential Information and shall use it
// only in accordance with the terms of the license agreement you entered
// into with Flamingos Technologies Inc. Unauthorized copying, redistribution,
// or use of this software in source or binary forms is strictly prohibited.
package com.google.mediapipe.apps.basic;

import android.graphics.Color;

import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;

import com.google.mediapipe.components.PermissionHelper;

import com.google.mediapipe.components.CloudScanner;
import com.google.mediapipe.components.CloudScanConfig;
import com.google.mediapipe.components.CloudScanDetectionListener;
import com.google.mediapipe.components.CloudScanSDK;

import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;
import android.widget.FrameLayout;

/**
 * Main activity of MediaPipe basic app.
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private static final String SERVER_ENDPOINT = "https://zingcam.dev.flamapp.com/cloud-scan-svc/api/v1/targets/app?clientID=672e043cf9ebc60bb50e13b8";
    
    static {
        System.loadLibrary("mediapipe_jni");
        try {
            System.loadLibrary("opencv_java3");
        } catch (java.lang.UnsatisfiedLinkError e) {
            System.loadLibrary("opencv_java4");
        }
    }

    private TextView statusTextView;
    private Button restartButton;
    private CloudScanSDK mediaPipeSDK;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        initializeViews();
        initializeSDK();
        
        PermissionHelper.checkAndRequestCameraPermissions(this);
    }

    private void initializeViews() {
        statusTextView = findViewById(R.id.status_text);
        restartButton = findViewById(R.id.restart_button);
        restartButton.setOnClickListener(v -> restartDetection());
    }

    private void initializeSDK() {
        CloudScanConfig config = new CloudScanConfig.Builder(this)
            .setBinaryGraphName("feature_extraction_desktop.binarypb")
            .setInputStreamName("input_video_gpu")
            .setServerEndpoint(SERVER_ENDPOINT)
            .setFlipFramesVertically(true)
            .setNumBuffers(20)
            .build();

        mediaPipeSDK = new CloudScanner(config);
        mediaPipeSDK.setDetectionListener(new CloudScanDetectionListener() {
            @Override
            public void onDetecionAccquired(String url) {
                runOnUiThread(() -> {
                    statusTextView.setText(url);
                });
            }

            // @Override
            // public void onTrackingUpdate(boolean isLost) {
            //     runOnUiThread(() -> {
            //         String status = isLost ? "Tracking Lost" : "Tracking Active";
            //         statusTextView.setText(status);
            //     });
            // }

            // @Override
            // public void onCoordinatesUpdate(float[] coordinates) {
            //     runOnUiThread(() -> {
            //         StringBuilder coordText = new StringBuilder("Coordinates:\n");
            //         for (int i = 0; i < coordinates.length; i += 2) {
            //             coordText.append(String.format("Point %d: (%.2f, %.2f)\n", 
            //                 i/2 + 1, coordinates[i], coordinates[i+1]));
            //         }
            //         statusTextView.setText(coordText.toString());
            //     });
            // }

            @Override
            public void onError(String error) {
                Log.e(TAG, "MediaPipe error: " + error);
                runOnUiThread(() -> {
                    Toast.makeText(MainActivity.this, "Error: " + error, Toast.LENGTH_SHORT).show();
                    statusTextView.setText("Error: " + error);
                });
            }
        });

        mediaPipeSDK.initialize(this);
    }

    private void restartDetection() {
        if (mediaPipeSDK != null) {
            mediaPipeSDK.updateTrackStatus(1);
            statusTextView.setText("Detection Restarted");
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (PermissionHelper.cameraPermissionsGranted(this)) {
            mediaPipeSDK.startDetection(this);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        mediaPipeSDK.updateTrackStatus(0);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mediaPipeSDK != null) {
            mediaPipeSDK.release();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (PermissionHelper.cameraPermissionsGranted(this)) {
            // mediaPipeSDK.startDetection(this);
        }
    }
}