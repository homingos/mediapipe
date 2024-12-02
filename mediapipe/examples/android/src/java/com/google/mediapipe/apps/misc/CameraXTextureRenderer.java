package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.Size;
import android.view.Surface;
import android.util.DisplayMetrics;
import android.util.Log;

import androidx.camera.core.Camera;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.Preview;
import androidx.camera.core.SurfaceRequest;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.lifecycle.LifecycleOwner;
import androidx.core.content.ContextCompat;

import com.google.common.util.concurrent.ListenableFuture;

import java.util.concurrent.Executor;
import java.util.concurrent.atomic.AtomicBoolean;

public class CameraXTextureRenderer {

    private static final String TAG = "CameraXTextureRenderer";

    // Constants for optimal camera preview resolution
    private static final int TARGET_PREVIEW_WIDTH = 1280;
    private static final int TARGET_PREVIEW_HEIGHT = 720;
    private static final float ASPECT_RATIO_TOLERANCE = 0.1f;

    private final Context context;
    private final SurfaceTexture surfaceTexture;
    private final int textureId;
    private final AtomicBoolean isCameraStarted = new AtomicBoolean(false);
    private final Size screenSize;
    private final Executor mainExecutor;

    private Camera camera;
    private Preview preview;
    private ProcessCameraProvider cameraProvider;
    private Surface previewSurface;

    public CameraXTextureRenderer(Context context,
            SurfaceTexture surfaceTexture,
            int textureId) {
        this.context = context.getApplicationContext();
        this.surfaceTexture = surfaceTexture;
        this.textureId = textureId;
        this.mainExecutor = ContextCompat.getMainExecutor(context);
        this.screenSize = calculateScreenSize(context);
    }

    public void startCamera() {
        if (!isCameraStarted.compareAndSet(false, true)) {
            Log.w(TAG, "Camera already started");
            return;
        }

        ListenableFuture<ProcessCameraProvider> providerFuture = ProcessCameraProvider.getInstance(context);
        providerFuture.addListener(() -> initializeCamera(providerFuture), mainExecutor);
    }

    public void release() {
        if (isCameraStarted.compareAndSet(true, false)) {
            mainExecutor.execute(() -> {
                if (cameraProvider != null) {
                    cameraProvider.unbindAll();
                }
                if (previewSurface != null) {
                    previewSurface.release();
                    previewSurface = null;
                }
                camera = null;
                preview = null;
                cameraProvider = null;
            });
        }
    }

    private void initializeCamera(ListenableFuture<ProcessCameraProvider> providerFuture) {
        try {
            cameraProvider = providerFuture.get();
            setupPreview();
            bindCameraUseCases();
        } catch (Exception e) {
            Log.e(TAG, "Camera initialization failed", e);
            isCameraStarted.set(false);
        }
    }

    private void setupPreview() {
        Size optimalPreviewSize = calculateOptimalPreviewSize();

        Preview.Builder previewBuilder = new Preview.Builder()
                .setTargetResolution(optimalPreviewSize);

        preview = previewBuilder.build();
        preview.setSurfaceProvider(request -> provideSurface(request));
    }

    private void provideSurface(SurfaceRequest request) {
        if (previewSurface != null) {
            previewSurface.release();
        }

        previewSurface = new Surface(surfaceTexture);

        Size resolution = request.getResolution();
        surfaceTexture.setDefaultBufferSize(
                resolution.getWidth(),
                resolution.getHeight()
        );

        request.provideSurface(previewSurface, mainExecutor, result -> {
        });
    }

    private void bindCameraUseCases() {
        CameraSelector cameraSelector = new CameraSelector.Builder()
                .requireLensFacing(CameraSelector.LENS_FACING_BACK)
                .build();

        try {
            cameraProvider.unbindAll();
            camera = cameraProvider.bindToLifecycle(
                    (LifecycleOwner) context,
                    cameraSelector,
                    preview
            );
        } catch (Exception e) {
            Log.e(TAG, "Use case binding failed", e);
        }
    }

    private Size calculateOptimalPreviewSize() {
        float targetAspectRatio = (float) TARGET_PREVIEW_WIDTH / TARGET_PREVIEW_HEIGHT;
        float screenAspectRatio = (float) screenSize.getWidth() / screenSize.getHeight();

        if (Math.abs(screenAspectRatio - targetAspectRatio) < ASPECT_RATIO_TOLERANCE) {
            return screenSize;
        }

        return new Size(TARGET_PREVIEW_WIDTH, TARGET_PREVIEW_HEIGHT);
    }

    private static Size calculateScreenSize(Context context) {
        DisplayMetrics metrics = context.getResources().getDisplayMetrics();
        return new Size(metrics.widthPixels, metrics.heightPixels);
    }

    public boolean isRunning() {
        return isCameraStarted.get();
    }

    public Camera getCamera() {
        return camera;
    }
}
