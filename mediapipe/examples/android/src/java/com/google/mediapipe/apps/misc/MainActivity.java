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

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import androidx.appcompat.app.AppCompatActivity;
import android.util.Log;
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
import com.google.mediapipe.apps.basic.CustomGLSurfaceView;
import com.google.mediapipe.apps.basic.GLRenderer;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.List;
import android.os.Looper;
import android.os.Handler;
import android.widget.FrameLayout;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Main activity of MediaPipe basic app.
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "RtCS";

    // Flips the camera-preview frames vertically by default, before sending them
    // into FrameProcessor
    // to be processed in a MediaPipe graph, and flips the processed frames back
    // when they are
    // displayed. This maybe needed because OpenGL represents images assuming the
    // image origin is at
    // the bottom-left corner, whereas MediaPipe in general assumes the image origin
    // is at the
    // top-left corner.
    // NOTE: use "flipFramesVertically" in manifest metadata to override this
    // behavior.
    private static final boolean FLIP_FRAMES_VERTICALLY = true;

    // Number of output frames allocated in ExternalTextureConverter.
    // NOTE: use "converterNumBuffers" in manifest metadata to override number of
    // buffers. For
    // example, when there is a FlowLimiterCalculator in the graph, number of
    // buffers should be at
    // least `max_in_flight + max_in_queue + 1` (where max_in_flight and
    // max_in_queue are used in
    // FlowLimiterCalculator options). That's because we need buffers for all the
    // frames that are in
    // flight/queue plus one for the next frame from the camera.
    private static final int NUM_BUFFERS = 2;

    static {
        // Load all native libraries needed by the app.
        System.loadLibrary("mediapipe_jni");
        try {
            System.loadLibrary("opencv_java3");
        } catch (java.lang.UnsatisfiedLinkError e) {
            // Some example apps (e.g. template matching) require OpenCV 4.
            System.loadLibrary("opencv_java4");
        }
    }
    // Sends camera-preview frames into a MediaPipe graph for processing, and
    // displays the processed
    // frames onto a {@link Surface}.
    protected FrameProcessor processor;
    // Handles camera access via the {@link CameraX} Jetpack support library.
    protected CameraXPreviewHelper cameraHelper;

    // {@link SurfaceTexture} where the camera-preview frames can be accessed.
    private SurfaceTexture previewFrameTexture;
    // {@link SurfaceView} that displays the camera-preview frames processed by a
    // MediaPipe graph.
    private SurfaceView previewDisplayView;

    // Creates and manages an {@link EGLContext}.
    private EglManager eglManager;
    // Converts the GL_TEXTURE_EXTERNAL_OES texture from Android camera into a
    // regular texture to be
    // consumed by {@link FrameProcessor} and the underlying MediaPipe graph.
    private ExternalTextureConverter converter;

    // ApplicationInfo for retrieving metadata defined in the manifest.
    private ApplicationInfo applicationInfo;

    private TextView embeddingTextView;

    private final AtomicReference<JSONArray> imgIdx = new AtomicReference<>();
    private final AtomicReference<JSONArray>  imgUrls = new AtomicReference<>();
    private final AtomicReference<JSONArray>  vidIdx = new AtomicReference<>();
    // private Packet featuresPacket;
    private long currentFeatsTs;

    private static final String SERVER_ENDPOINT = "https://zingcam.dev.flamapp.com/cloud-scan-svc/api/v1/targets/app?clientID=672e043cf9ebc60bb50e13b8";
    private final ExecutorService executorService = Executors.newSingleThreadExecutor();
    private final ExecutorService rerankService = Executors.newSingleThreadExecutor();
    private final ReentrantLock processingLock = new ReentrantLock();
    private final ReentrantLock rerankingLock = new ReentrantLock();
    private final OkHttpClient httpClient = new OkHttpClient.Builder().connectTimeout(500, TimeUnit.MILLISECONDS).readTimeout(500, TimeUnit.MILLISECONDS).build();
    private Button restartButton;

    private CustomGLSurfaceView mGLSurfView;
    private GLRenderer mGLRenderer;
    private FrameLayout frameLayout;

    private float[] xyCoordinates;

    private int screenWidth;
    private int screenHeight;

    private long startTime = 0;
    private int tracking_lost = 1;
    private int last_tracking_lost = 1;
    private boolean matchFound = false;
    private MediaPipePacketManager mediaPipePacketManager;
    private boolean timingStarted = false;
    private boolean trackerRunning = false;
    private final AtomicReference<float[]> targetCoordinatesRef = new AtomicReference<>(); // target coordinates
    private final AtomicReference<float[]> currCoordinatesRef = new AtomicReference<>(); // interpolated coordinates
    private final AtomicReference<JSONArray> features  = new AtomicReference<>();
    public float[] getTargetCoordinates() {
        return targetCoordinatesRef.get();
    }

    public float[] getCurrCoordinates() {
        return currCoordinatesRef.get();
    }

    public void setCurrCoordinates(float[] currCoordinates) {
        currCoordinatesRef.set(currCoordinates);
    }

    public void initialize() {
        frameLayout = findViewById(R.id.preview_display_layout);
        mGLSurfView = new CustomGLSurfaceView(this);
        screenWidth = ScreenUtils.getScreenWidth(this);
        screenHeight = ScreenUtils.getScreenHeight(this);

        float[] initialCoordinates = new float[]{
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,};

        if (currCoordinatesRef.get() == null) {
            currCoordinatesRef.set(initialCoordinates.clone());
        }

        if (targetCoordinatesRef.get() == null) {
            targetCoordinatesRef.set(initialCoordinates.clone());
        }

        updateGLSurfaceViewCoordinates();
        frameLayout.addView(mGLSurfView);
    }

    public void updateGLSurfaceViewCoordinates() {
        mGLSurfView.setPlaneCoordinates(targetCoordinatesRef.get());
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getContentViewLayoutResId());
        initialize();

        try {
            applicationInfo = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
        } catch (NameNotFoundException e) {
            Log.e(TAG, "Cannot find application info: " + e);
        }

        previewDisplayView = new SurfaceView(this);
        setupPreviewDisplayView();

        restartButton = findViewById(R.id.restart_button); // Get a reference to the button
        restartButton.setOnClickListener(view -> restartDetection()); // Set click listener
        // Initialize asset manager so that MediaPipe native libraries can access the
        // app assets, e.g.,
        // binary graphs.
        embeddingTextView = findViewById(R.id.embedding_text);

        AndroidAssetUtil.initializeNativeAssetManager(this);
        eglManager = new EglManager(null);
        processor = new FrameProcessor(
                this,
                eglManager.getNativeContext(),
                applicationInfo.metaData.getString("binaryGraphName"),
                applicationInfo.metaData.getString("inputVideoStreamName"),
                null);
        // // processor
        // //         .getVideoSurfaceOutput()
        // //         .setFlipY(
        //                 applicationInfo.metaData.getBoolean("flipFramesVertically", FLIP_FRAMES_VERTICALLY));
        mediaPipePacketManager = new MediaPipePacketManager();
        processor.setOnWillAddFrameListener(mediaPipePacketManager);
        PermissionHelper.checkAndRequestCameraPermissions(this);
        // processor.getGraph().addPacketToInputStream("match_image", processor.getPacketCreator().createRgbImageFrame(
        //         Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888)), System.currentTimeMillis());
        // processor.getGraph().addPacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(true),
        //         System.currentTimeMillis());
        if (!timingStarted) {
            startTime = SystemClock.elapsedRealtime();
            timingStarted = true;
        }
        // Add packet callbacks for new outputs
        processor.addPacketCallback("box_floats",
                (packet) -> {
                    try {
                        float[] boxFloats = PacketGetter.getFloat32Vector(packet);
                        targetCoordinatesRef.set(boxFloats.clone());
                        updateGLSurfaceViewCoordinates();
                    } catch (Exception e) {
                        Log.e(TAG, "coordinates Error getting box floats: " + e.getMessage());
                    }
                }
        );
        processor.addPacketCallback(
                "output_tensor_floats",
                (packet) -> {
                        Log.d(TAG, "tensors");
                        float[] embeddingBytes = PacketGetter.getFloat32Vector(packet);
                        sendEmbeddingToServer(embeddingBytes);
                        // if(processingLock.tryLock()){
                        //     currentFeatsTs = packet.getTimestamp();
                        //     processingLock.unlock();
                        // }
                });

        processor.addPacketCallback(
            "tracking",
            (packet) -> {
                if (rerankingLock.tryLock()) {
                    int packetValue = PacketGetter.getInt32(packet);
                    tracking_lost = packetValue;
                    Log.d(TAG, "Tracking lost Callback: " + tracking_lost);
                    rerankingLock.unlock();
                }
                
            });

        processor.addPacketCallback(
                "rr_index",
                (packet) -> {
                    int index = PacketGetter.getInt32(packet);
                    try {
                        if ( imgIdx.get() != null && index < imgIdx.get().length() && index >= 0) {
                            rerankService.submit(() -> {
                                if (!rerankingLock.tryLock()) {
                                    tracking_lost = 0;
                                    Log.d(TAG, "Reranking already in progress");
                                    return;
                                }
                                tracking_lost = 0;
                                try {
                                    if (timingStarted) {
                                        long endTime = SystemClock.elapsedRealtime();
                                        final long elapsedTime = endTime - startTime;
                                        timingStarted = false;
                                        runOnUiThread(() -> {
                                            updateView("DetectionTime: " + elapsedTime + "ms");
                                            
                                        });
                                    }
                                    
                                    // Download and process image
                                    URL url = new URL(imgUrls.get().optString(index));
                                    String video_url = new URL("https://zingcam.cdn.flamapp.com/compressed/videos/playback/66d316a31753dedfff44e721_22061656.mp4").toString();
                                    
                                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                                    connection.setRequestMethod("GET");
                                    connection.setDoInput(true);
                                    connection.connect();
                            
                                    try (InputStream inputStream = connection.getInputStream()) {
                                        Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
                                        Packet imagePacket = processor.getPacketCreator().createRgbImageFrame(bitmap);
                                        processor.getGraph().addPacketToInputStream("match_image", imagePacket, currentFeatsTs);
                                        mGLSurfView.initMediaplayer(video_url);
                                        bitmap.recycle();
                                    }
                                } catch (Exception e) {
                                    Log.e(TAG, "Error in reranking: " + e.getMessage());
                                    updateView("Reranking error");
                                } finally {
                                    tracking_lost = 0;
                                    rerankingLock.unlock();
                                }
                            });
                        }
                    } catch (Exception e) {
                        Log.e(TAG, "Error accessing image index: " + e.getMessage());
                    }
                });
    }

    // Used to obtain the content view for this application. If you are extending
    // this class, and
    // have a custom layout, override this method and return the custom layout.
    protected int getContentViewLayoutResId() {
        return R.layout.activity_main;
    }

    @Override
    protected void onResume() {
        super.onResume();

        converter = new ExternalTextureConverter(
                eglManager.getContext(),
                applicationInfo.metaData.getInt("converterNumBuffers", NUM_BUFFERS)
        );
        converter.setFlipY(applicationInfo.metaData.getBoolean("flipFramesVertically", FLIP_FRAMES_VERTICALLY));

        converter.setConsumer(processor);
        if (PermissionHelper.cameraPermissionsGranted(this)) {
            startCamera();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        converter.close();
        // Hide preview display until we re-open the camera again.
        previewDisplayView.setVisibility(View.GONE);
    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        PermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    protected void onCameraStarted(SurfaceTexture surfaceTexture) {
        previewFrameTexture = surfaceTexture;
        // Make the display view visible to start showing the preview. This triggers the
        // SurfaceHolder.Callback added to (the holder of) previewDisplayView.
        previewDisplayView.setVisibility(View.VISIBLE);
    }

    protected Size cameraTargetResolution() {
        return null; // No preference and let the camera (helper) decide.
    }

    public void startCamera() {
        cameraHelper = new CameraXPreviewHelper();
        previewFrameTexture = converter.getSurfaceTexture();
        cameraHelper.setOnCameraStartedListener(
                surfaceTexture -> {
                    onCameraStarted(surfaceTexture);
                });
        CameraHelper.CameraFacing cameraFacing = applicationInfo.metaData.getBoolean("cameraFacingFront", false)
                ? CameraHelper.CameraFacing.FRONT
                : CameraHelper.CameraFacing.BACK;
        cameraHelper.startCamera(
                this, cameraFacing, previewFrameTexture, cameraTargetResolution());
    }

    protected Size computeViewSize(int width, int height) {
        return new Size(width, height);
    }

    protected void onPreviewDisplaySurfaceChanged(
        SurfaceHolder holder, int format, int width, int height) {
        // (Re-)Compute the ideal size of the camera-preview display (the area that the
        // camera-preview frames get rendered onto, potentially with scaling and
        // rotation)
        // based on the size of the SurfaceView that contains the display.
        Size viewSize = computeViewSize(width, height);
        Size displaySize = cameraHelper.computeDisplaySizeFromViewSize(viewSize);
        boolean isCameraRotated = cameraHelper.isCameraRotated();

        // Configure the output width and height as the computed display size.
        converter.setDestinationSize(
                isCameraRotated ? displaySize.getHeight() : displaySize.getWidth(),
                isCameraRotated ? displaySize.getWidth() : displaySize.getHeight());
    }

    private void setupPreviewDisplayView() {
        previewDisplayView.setVisibility(View.GONE);
        ViewGroup viewGroup = findViewById(R.id.preview_display_layout);
        viewGroup.addView(previewDisplayView);

        previewDisplayView
                .getHolder()
                .addCallback(
                        new SurfaceHolder.Callback() {
                    @Override
                    public void surfaceCreated(SurfaceHolder holder) {
                        // processor.getVideoSurfaceOutput().setSurface(holder.getSurface());
                    }

                    @Override
                    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                        onPreviewDisplaySurfaceChanged(holder, format, width, height);
                    }

                    @Override
                    public void surfaceDestroyed(SurfaceHolder holder) {
                        // processor.getVideoSurfaceOutput().setSurface(null);
                    }
                });
    }

    // Send the embedding to the server
    private void sendEmbeddingToServer(float[] embeddingBytes) {
        executorService.submit(() -> {
            if (!processingLock.tryLock()) {
                updateView("Processing...");
                return;  // Important: early return if lock cannot be acquired
            }
            
            try {
                // Move all processing logic inside try block
                ByteBuffer byteBuffer = ByteBuffer.allocate(embeddingBytes.length * 4);
                byteBuffer.order(ByteOrder.nativeOrder());
                for (float value : embeddingBytes) {
                    byteBuffer.putFloat(value);
                }
                byte[] embeddingByteArray = byteBuffer.array();
                
                // Create and send request
                RequestBody body = RequestBody.create(
                    MediaType.parse("application/octet-stream"), 
                    embeddingByteArray
                );
                Request request = new Request.Builder()
                    .url(SERVER_ENDPOINT)
                    .post(body)
                    .build();
                final long requestStartTime = SystemClock.elapsedRealtime();
                
                httpClient.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {
                        Log.e(TAG, "Error sending embedding to server: " + e.getMessage());
                    }

                    @Override
                    public void onResponse(Call call, Response response) {
                        long requestEndTime = SystemClock.elapsedRealtime();
                        long rtt = requestEndTime - requestStartTime;
                        Log.d(TAG, "Server Request RTT: " + rtt + "ms");
                        JSONArray images = null;
                        JSONArray videos = null;
                        JSONArray image_urls = null;
                        try {
                            if (response.isSuccessful()) {
                                String responseBody = response.body().string();
                                Log.d(TAG, "Received payload size: " + responseBody.length() + " bytes");
                                JSONObject Jobject = new JSONObject(responseBody);
                                JSONObject dataObject = Jobject.getJSONObject("data");
                                images = dataObject.getJSONArray("id");
                                image_urls = dataObject.getJSONArray("image_urls");
                                if (images.length() == 0) {
                                    Log.e(TAG, "No images returned from server");
                                    updateView("No detections");
                                    return;
                                }

                                features.set(dataObject.getJSONArray("features"));

                                if (features.get().length() == 0) {
                                    Log.e(TAG, "No features returned from server");
                                } else {
                                   
                                }
                            } else {
                                Log.e(TAG, "Server returned an error: " + response.code());
                            }
                        } catch (JSONException e) {
                            Log.e(TAG, "Error reading server response: " + e.getMessage());
                        } catch (IOException e) {
                            Log.e(TAG, "Error reading server response: " + e.getMessage());
                        } finally {
                            try {
                                imgIdx.set(images);
                                vidIdx.set(videos);
                                imgUrls.set(image_urls);
                            } finally {
                                response.close();
                            }
                        }
                    }
                });
            } catch (Exception e) {
                Log.e(TAG, "Error in embedding processing: " + e.getMessage());
                updateView("Processing error");
            } finally {
                processingLock.unlock();  // Always unlock in finally block
            }
        });
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

    private void updateView(final String text) {
        runOnUiThread(() -> {
            embeddingTextView.setText(text);
        });
    }

    private void restartDetection() {
        long ts = System.currentTimeMillis();
        updateView("Detection restarted"); // Update the UI
        startTime = SystemClock.elapsedRealtime();
        timingStarted = true;
    }

    private class MediaPipePacketManager implements FrameProcessor.OnWillAddFrameListener {
        @Override
        public void onWillAddFrame(long timestamp) {
            currentFeatsTs = timestamp;
            
            boolean locked = false;
            try {
                // Only try to acquire lock if conditions are met
                if (features.get() != null && features.get().length() != 0 && tracking_lost == 1) {
                    locked = rerankingLock.tryLock();
                    if (locked) {
                        processor.getGraph().addPacketToInputStream("query_feats", createQueryFeaturesPacket(features.get()), timestamp);
                        if(!timingStarted){
                            timingStarted = true;
                            startTime = SystemClock.elapsedRealtime();
                            updateView("Tracking lost");
                        }
                    }
                }
            } finally {
                if (locked) {
                    rerankingLock.unlock();
                }
            }
            
            processor.getGraph().addConsumablePacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(tracking_lost == 1), timestamp);
        }
    }
    
}
