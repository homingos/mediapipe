package com.google.mediapipe.apps.basic;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.Size;
import android.view.Surface;
import android.util.DisplayMetrics;

import androidx.camera.core.Camera;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.Preview;
import androidx.camera.core.SurfaceRequest;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.lifecycle.LifecycleOwner;

import com.google.common.util.concurrent.ListenableFuture;

import java.util.concurrent.ExecutionException;

public class CameraXTextureRenderer {

    private final Context context;
    private final SurfaceTexture surfaceTexture;
    private final int textureId;
    private Preview preview;
    private DisplayMetrics displayMetrics;

    private int deviceWidth;
    private int deviceHeight;

    public CameraXTextureRenderer(Context context, SurfaceTexture surfaceTexture, int textureId) {
        this.context = context;
        this.surfaceTexture = surfaceTexture;
        this.textureId = textureId;
    }

    public void startCamera() {
        ListenableFuture<ProcessCameraProvider> cameraProviderFuture = ProcessCameraProvider.getInstance(context);
        cameraProviderFuture.addListener(() -> {
            try {
                ProcessCameraProvider cameraProvider = cameraProviderFuture.get();                

                DisplayMetrics displayMetrics = context.getResources().getDisplayMetrics();
                deviceWidth = displayMetrics.widthPixels;
                deviceHeight = displayMetrics.heightPixels;
            
                preview = new Preview.Builder()
                        .setTargetResolution(new Size(1920, 1080)) // Set your desired resolution here
                        .build();

                preview.setSurfaceProvider(request -> {
                    Surface surface = new Surface(surfaceTexture);
                    request.provideSurface(surface, context.getMainExecutor(), result -> {
                        // Handle the surface result here if needed
                    });
                });

                CameraSelector cameraSelector = new CameraSelector.Builder()
                        .requireLensFacing(CameraSelector.LENS_FACING_BACK)
                        .build();

                cameraProvider.unbindAll();
                cameraProvider.bindToLifecycle((LifecycleOwner) context, cameraSelector, preview);

            } catch (ExecutionException | InterruptedException e) {
                e.printStackTrace();
            }
        }, context.getMainExecutor());
    }
}
