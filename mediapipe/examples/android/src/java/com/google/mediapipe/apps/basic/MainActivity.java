// Copyright 2019 The MediaPipe Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
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

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.util.List;
import android.os.Looper;
import android.os.Handler;  

/**
 * Main activity of MediaPipe basic app.
 */
public class MainActivity extends AppCompatActivity {

    private static final String TAG = "Aman Mediapipe";
    private static final String ASSET_URL_BASE = "https://storage.googleapis.com/avatar-system/test/assets/";


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

    private volatile boolean matchFound = false;
    private volatile boolean processing = false;
    private JSONArray imgIdx;
    // private Packet featuresPacket;
    private long currentFeatsTs;

    private static final String SERVER_ENDPOINT = "https://us-central1-development-382019.cloudfunctions.net/cloudscan";
    private final ExecutorService executorService = Executors.newFixedThreadPool(4);
    private final ReentrantLock processingLock = new ReentrantLock();
    private OkHttpClient httpClient = new OkHttpClient.Builder().connectTimeout(1, TimeUnit.SECONDS).readTimeout(1, TimeUnit.SECONDS).build();
    private Button restartButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getContentViewLayoutResId());

        try {
            applicationInfo = getPackageManager().getApplicationInfo(getPackageName(), PackageManager.GET_META_DATA);
        } catch (NameNotFoundException e) {
            Log.e(TAG, "Cannot find application info: " + e);
        }


		final String assetDir = getFilesDir().getAbsolutePath() + "/";
		downloadAssets();
		Assets.copyFiles(getAssets(), assetDir + "/", true);

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
                applicationInfo.metaData.getString("outputVideoStreamName"));
        processor
                .getVideoSurfaceOutput()
                .setFlipY(
                        applicationInfo.metaData.getBoolean("flipFramesVertically", FLIP_FRAMES_VERTICALLY));

        PermissionHelper.checkAndRequestCameraPermissions(this);
        processor.getGraph().addPacketToInputStream("match_image", processor.getPacketCreator().createRgbImageFrame(
                Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888)), System.currentTimeMillis());
        processor.getGraph().addPacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(true), System.currentTimeMillis());
        // Add packet callbacks for new outputs
        processor.addPacketCallback("box_floats",
                (packet) -> {
                    try {
                        float[] boxFloats = PacketGetter.getFloat32Vector(packet);
                        updateView(Arrays.toString(boxFloats));
                    } catch (Exception e) {
                        Log.e(TAG, "Error getting box floats: " + e.getMessage());
                    }

                }
        );
        processor.addPacketCallback(
                "output_tensor_floats",
                (packet) -> {
                    if (!matchFound && !processing) {
                        float[] embeddingBytes = PacketGetter.getFloat32Vector(packet);
                        sendEmbeddingToServer(embeddingBytes);
                        processingLock.tryLock();
                        currentFeatsTs = packet.getTimestamp();
                        processingLock.unlock();
                    }
                });

        processor.addPacketCallback(
                "rr_index",
                (packet) -> {
                    int index = PacketGetter.getInt32(packet);
                    try {
                        if (index < imgIdx.length() && !matchFound && imgIdx != null && index >= 0) {
                            executorService.submit(() -> {
                                processingLock.lock();
                                try {
                                    matchFound = true;
                                    URL url = new URL(imgIdx.optString(index));
                                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                                    connection.setRequestMethod("GET");
                                    connection.setDoInput(true);
                                    connection.connect();

                                    InputStream inputStream = connection.getInputStream();
                                    Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
                                    updateView("Image downloaded");
                                    Packet imagePacket = processor.getPacketCreator().createRgbImageFrame(bitmap);
                                    processor.getGraph().addPacketToInputStream("match_image", imagePacket,
                                            System.currentTimeMillis());
                                    processor.getGraph().addPacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(false), System.currentTimeMillis());
                                    bitmap.recycle();
                                } catch (ProtocolException ex) {
                                } catch (IOException ex) {
                                } finally {
                                    processingLock.unlock();
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
                applicationInfo.metaData.getInt("converterNumBuffers", NUM_BUFFERS));
        converter.setFlipY(
                applicationInfo.metaData.getBoolean("flipFramesVertically", FLIP_FRAMES_VERTICALLY));
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
                        processor.getVideoSurfaceOutput().setSurface(holder.getSurface());
                    }

                    @Override
                    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                        onPreviewDisplaySurfaceChanged(holder, format, width, height);
                    }

                    @Override
                    public void surfaceDestroyed(SurfaceHolder holder) {
                        processor.getVideoSurfaceOutput().setSurface(null);
                    }
                });
    }

    // Send the embedding to the server
    private void sendEmbeddingToServer(float[] embeddingBytes) {
        executorService.submit(() -> {
            try {
                processingLock.tryLock();
                if (processing) {
                    updateView("Processing...");
                    processingLock.unlock();
                    return;
                }
                processing = true;
                processingLock.unlock();
                // Convert float array to byte array
                ByteBuffer byteBuffer = ByteBuffer.allocate(embeddingBytes.length * 4); // 4 bytes per float
                byteBuffer.order(ByteOrder.nativeOrder()); // Use the native byte order
                for (float value : embeddingBytes) {
                    byteBuffer.putFloat(value);
                }
                byte[] embeddingByteArray = byteBuffer.array();

                // Create the request
                RequestBody body = RequestBody.create(MediaType.parse("application/octet-stream"), embeddingByteArray);
                Request request = new Request.Builder()
                        .url(SERVER_ENDPOINT)
                        .post(body)
                        .build();

                // Send the request
                updateView("Detecting...");
                httpClient.newCall(request).enqueue(new Callback() {
                    @Override
                    public void onFailure(Call call, IOException e) {
                        processingLock.tryLock();
                        processing = false;
                        processingLock.unlock();
                        Log.e(TAG, "Error sending embedding to server: " + e.getMessage());
                    }

                    @Override
                    public void onResponse(Call call, Response response) {
                        JSONArray images = null;
                        try {
                            if (response.isSuccessful()) {
                                String responseBody = response.body().string();
                                JSONObject Jobject = new JSONObject(responseBody);
                                images = Jobject.getJSONArray("images");
                                if (images.length() == 0) {
                                    Log.e(TAG, "No images returned from server");
                                    updateView("No detections");
                                    return;
                                } else {
                                    updateView("Detection : " + images.length());
                                }

                                JSONArray features = Jobject.getJSONArray("features");

                                if (features.length() == 0) {
                                    Log.e(TAG, "No features returned from server");
                                } else {
                                    processor.getGraph().addPacketToInputStream("query_feats", createQueryFeaturesPacket(features), currentFeatsTs);
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
                                processingLock.tryLock();
                                processing = false;
                                imgIdx = images;
                                processingLock.unlock();
                                response.close();
                            } catch (Exception e) {
                                response.close();
                            }
                        }
                    }
                });
            } catch (Exception e) {
                Log.e(TAG, "Error creating JSON payload: " + e.getMessage());
            }
        });
    }

    private Packet createQueryFeaturesPacket(JSONArray queryFeatures) {
        if (queryFeatures == null || queryFeatures.length() == 0) {
            return null; // No features to send
        }
        // Join the array of strings into a single string, separated by a delimiter

        try {
            String joinedFeatures = queryFeatures.join("|");
            return processor.getPacketCreator().createString(joinedFeatures);
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
        processingLock.tryLock();
        matchFound = false;
        processing = false;
        // imgIdx = null; 
        processor.getGraph().addPacketToInputStream("enable_scanning", processor.getPacketCreator().createBool(true), System.currentTimeMillis());
        processingLock.unlock();
        updateView("Detection restarted"); // Update the UI
    }

     private void downloadAssets() {
		 final String assetDir = getFilesDir().getAbsolutePath() + "/";
		 List<String> files = Arrays.asList("testcinema.ox3dv", "cinema.jpeg",
				 "trex-attribution.txt", "trex.mtl", "trex.obj", "trex.png");
		 for (String file : files) {
			 String fileURL = ASSET_URL_BASE + file;
			 downloadFile(fileURL, assetDir, file);
		 }
		 Assets.copyFiles(getAssets(), assetDir, true);
	 }
 
	 private void downloadFile(final String fileURL, final String dirPath, final String fileName) {
		 ExecutorService executor = Executors.newSingleThreadExecutor();
		 Handler handler = new Handler(Looper.getMainLooper());
 
		 executor.execute(() -> {
			 String result = performDownload(fileURL, dirPath, fileName);
			 handler.post(() -> {
				 // Update UI with result, e.g., display a message or update a view
				 if (result != null) {
					 Log.d(TAG, "Download successful: " + result);
				 } else {
					 Log.e(TAG, "Download failed");
				 }
			 });
		 });
	 }
 
	 private String performDownload(String fileURL, String dirPath, String fileName) {
		 InputStream input = null;
		 OutputStream output = null;
		 HttpURLConnection urlConnection = null;
		 try {
			 File dir = new File(dirPath);
			 if (!dir.exists() && !dir.mkdirs()) {
				 Log.e(TAG, "Failed to create directory: " + dirPath);
				 return null;
			 }
 
			 URL url = new URL(fileURL);
			 urlConnection = (HttpURLConnection) url.openConnection();
			 urlConnection.setRequestMethod("GET");
			 urlConnection.connect();
 
			 File file = new File(dir, fileName);
			 output = new FileOutputStream(file);
 
			 input = urlConnection.getInputStream();
			 byte[] buffer = new byte[4096];
			 int byteCount;
			 while ((byteCount = input.read(buffer)) != -1) {
				 output.write(buffer, 0, byteCount);
			 }
 
			 return file.getAbsolutePath();
		 } catch (Exception e) {
			 Log.e(TAG, "Error downloading file: " + e.getMessage(), e);
			 return null;
		 } finally {
			 try {
				 if (input != null) input.close();
				 if (output != null) output.close();
			 } catch (Exception e) {
				 Log.e(TAG, "Error closing streams: " + e.getMessage(), e);
			 }
			 if (urlConnection != null) urlConnection.disconnect();
		 }
	 }
}
