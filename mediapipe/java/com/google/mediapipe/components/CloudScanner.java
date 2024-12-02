package com.google.mediapipe.components;
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

import android.graphics.Color;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Size;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.os.SystemClock;

import com.google.mediapipe.components.CameraHelper;
import com.google.mediapipe.components.CameraXPreviewHelper;
import com.google.mediapipe.components.ExternalTextureConverter;
import com.google.mediapipe.components.FrameProcessor;
import com.google.mediapipe.components.PermissionHelper;
import com.google.mediapipe.framework.AndroidAssetUtil;
import com.google.mediapipe.framework.PacketGetter;
import com.google.mediapipe.glutil.EglManager;
import android.widget.TextView;

import com.google.mediapipe.components.CloudScanConfig;
import com.google.mediapipe.components.CloudScanDetectionListener;
import com.google.mediapipe.components.CloudScanSDK;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.sql.Time;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import com.google.mediapipe.framework.Packet;

import java.util.concurrent.locks.ReentrantLock;

import android.widget.Button;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.ProtocolException;
import java.net.URL;
import java.util.Arrays;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.List;
import android.os.Looper;
import android.os.Handler;
import android.widget.FrameLayout;
import java.util.concurrent.atomic.AtomicReference;
import android.app.Activity;

public class CloudScanner implements CloudScanSDK {
    private static final String TAG = "CloudScanner";
    private static final boolean FLIP_FRAMES_VERTICALLY = true;
    private static final int NUM_BUFFERS = 2;
    
    private final CloudScanConfig config;
    private FrameProcessor processor;
    private SurfaceTexture previewFrameTexture;
    private ExternalTextureConverter converter;
    private CameraXPreviewHelper cameraHelper;
    private EglManager eglManager;
    private CloudScanDetectionListener listener;
    private final AtomicReference<float[]> targetCoordinates;
    private final ExecutorService executorService;
    private final OkHttpClient httpClient;
    private final ReentrantLock processingLock;
    private boolean isInitialized = false;
    private final AtomicReference<Integer> status = new AtomicReference<>();
    private MediaPipePacketManager mediaPipePacketManager;
    private final AtomicReference<JSONArray> features  = new AtomicReference<>();
    private long currentFeatsTs;
    private final AtomicReference<JSONArray> imgIdx = new AtomicReference<>();
    private final AtomicReference<JSONArray>  imgUrls = new AtomicReference<>();

    public CloudScanner(CloudScanConfig config) {
        this.config = config;
        this.status.set(1);
        this.executorService = Executors.newSingleThreadExecutor();
        this.httpClient = new OkHttpClient.Builder()
            .connectTimeout(100, TimeUnit.MILLISECONDS)
            .readTimeout(500, TimeUnit.MILLISECONDS)
            .build();
        this.processingLock = new ReentrantLock();
        this.targetCoordinates = new AtomicReference<>();
    }

    @Override
    public void initialize(Context context) {
        if (isInitialized) {
            return;
        }

        // Initialize MediaPipe
        AndroidAssetUtil.initializeNativeAssetManager(context);
        eglManager = new EglManager(null);

        // Initialize frame processor
        processor = new FrameProcessor(
            context,
            eglManager.getNativeContext(),
            config.getBinaryGraphName(),
            config.getInputStreamName(),
            null
        );

        mediaPipePacketManager = new MediaPipePacketManager();
        processor.setOnWillAddFrameListener(mediaPipePacketManager);
        setupPacketCallbacks();
        
        // Initialize camera helper
        cameraHelper = new CameraXPreviewHelper();
        
        isInitialized = true;
    }

    private void setupPacketCallbacks() {
        // Box coordinates callback
        // processor.addPacketCallback(
        //     "box_floats",
        //     (packet) -> {
        //         try {
        //             float[] coordinates = PacketGetter.getFloat32Vector(packet);
        //             targetCoordinates.set(coordinates);
        //             if (listener != null) {
        //                 listener.onCoordinatesUpdate(coordinates);
        //             }
        //         } catch (Exception e) {
        //             onError("Error getting coordinates: " + e.getMessage());
        //         }
        //     }
        // );

        processor.addPacketCallback(
                "rr_index",
                (packet) -> {
                    int index = PacketGetter.getInt32(packet);
                    try {
                        if ( imgIdx.get() != null && index < imgIdx.get().length() && index >= 0) {
                                status.setOpaque(0);
                                try {
                                    // Download and process image
                                    URL url = new URL(imgUrls.get().optString(index));
                                    listener.onDetecionAccquired(url.toString());
                                } catch (Exception e) {
                                    Log.e(TAG, "Error in reranking: " + e.getMessage());
                                }
                        }
                    } catch (Exception e) {
                        Log.e(TAG, "Error accessing image index: " + e.getMessage());
                    }
                });
        
        // Feature embedding callback
        processor.addPacketCallback(
            "output_tensor_floats",
            (packet) -> {
                if(status.getOpaque()==1){
                    float[] embedding = PacketGetter.getFloat32Vector(packet);
                    sendEmbeddingToServer(embedding);
                }
            }
        );
    }

    private void sendEmbeddingToServer(float[] embedding) {
        executorService.submit(() -> {
            if (!processingLock.tryLock()) {
                return;
            }
            
            try {
                ByteBuffer buffer = ByteBuffer.allocate(embedding.length * 4);
                buffer.order(ByteOrder.nativeOrder());
                for (float value : embedding) {
                    buffer.putFloat(value);
                }
                
                RequestBody body = RequestBody.create(
                    MediaType.parse("application/octet-stream"),
                    buffer.array()
                );
                
                Request request = new Request.Builder()
                    .url(config.getServerEndpoint())
                    .post(body)
                    .build();
                    
                httpClient.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {
                        onError("Server request failed: " + e.getMessage());
                    }
                    
                    @Override
                    public void onResponse(Call call, Response response) {
                        try {
                            if (!response.isSuccessful()) {
                                onError("Server error: " + response.code());
                                return;
                            }
                            processServerResponse(response);
                        } finally {
                            response.close();
                        }
                    }
                });
            } finally {
                processingLock.unlock();
            }
        });
    }

    private class MediaPipePacketManager implements FrameProcessor.OnWillAddFrameListener {
        @Override
        public void onWillAddFrame(long timestamp) {
            currentFeatsTs = timestamp;
            if (features.get() != null && features.get().length() != 0 && status.get() == 1) {
                Log.d(TAG,"Seinding Features: " + features.get().toString());
                processor.getGraph().addPacketToInputStream("query_feats", createQueryFeaturesPacket(features.get()), timestamp);
            }
            processor.getGraph().addConsumablePacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(status.get() == 1), timestamp);
        }
    }

    private void processServerResponse(Response response) {
        try {
            String responseBody = response.body().string();
            JSONObject json = new JSONObject(responseBody);
            JSONObject data = json.getJSONObject("data");
            JSONArray images = data.getJSONArray("id");
            JSONArray image_urls = data.getJSONArray("image_urls");
            features.set(data.getJSONArray("features"));
            imgIdx.set(images);
            imgUrls.set(image_urls);
        } catch (Exception e) {
            onError("Error processing server response: " + e.getMessage());
        }
    }

    private Packet createQueryFeaturesPacket(JSONArray queryFeatures) {
        if (queryFeatures == null || queryFeatures.length() == 0) {
            return null; // No features to send
        }
        // Join the array of strings into a single string, separated by a delimiter

        try {
            String[] features = new String[queryFeatures.length()];
            for (int i = 0; i < queryFeatures.length(); i++) {
                features[i] = queryFeatures.getString(i);
            }
            return processor.getPacketCreator().createStringArray(features);
        } catch (JSONException e) {
            Log.e(TAG, "Error creating query features packet: " + e.getMessage());
            return null;
        }
    }

    @Override
    public void updateTrackStatus(int trackStatus) {
        status.set(trackStatus);
    }
    @Override
    public void startDetection(Activity activity) {
        if (!isInitialized) {
            onError("SDK not initialized");
            return;
        }
        converter = new ExternalTextureConverter(
            eglManager.getContext(),
            config.getNumBuffers()
        );

        // Configure the output width and height as the computed display size.
        converter.setFlipY(config.isFlipFramesVertically());
        converter.setConsumer(processor);
        
        // Start camera
        previewFrameTexture = converter.getSurfaceTexture();
        cameraHelper.setOnCameraStartedListener(
            surfaceTexture -> {
                onCameraStarted(surfaceTexture);
            }
        );
        cameraHelper.startCamera(
            activity,
            CameraHelper.CameraFacing.BACK,
            previewFrameTexture,
            null
        );
        
    }

    protected void onCameraStarted(SurfaceTexture surfaceTexture) {
        previewFrameTexture = surfaceTexture;
        Size displaySize = new Size(640,480);
        boolean isCameraRotated = cameraHelper.isCameraRotated();
        converter.setDestinationSize(
            isCameraRotated ? displaySize.getHeight() : displaySize.getWidth(),
            isCameraRotated ? displaySize.getWidth() : displaySize.getHeight()
        );
      }

    @Override 
    public void stopDetection() {
        if (converter != null) {
            converter.close();
        }
    }

    @Override
    public void release() {
        stopDetection();
        executorService.shutdown();
        isInitialized = false;
    }

    @Override
    public void setDetectionListener(CloudScanDetectionListener listener) {
        this.listener = listener;
    }

    private void onError(String error) {
        if (listener != null) {
            listener.onError(error);
        }
    }
}