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

package com.google.mediapipe.components;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

/** Manages camera permission request and handling. */
public class PermissionHelper {
  private static final String TAG = "PermissionHelper";

  private static final String AUDIO_PERMISSION = Manifest.permission.RECORD_AUDIO;

  private static final String CAMERA_PERMISSION = Manifest.permission.CAMERA;

  private static final String READ_EXTERNAL_STORAGE_PERMISSION =
      Manifest.permission.READ_EXTERNAL_STORAGE;

  private static final int REQUEST_CODE = 0;

  public static boolean permissionsGranted(Activity context, String[] permissions) {
    for (String permission : permissions) {
      int permissionStatus = ContextCompat.checkSelfPermission(context, permission);
      if (permissionStatus != PackageManager.PERMISSION_GRANTED) {
        return false;
      }
    }
    return true;
  }

  public static void checkAndRequestPermissions(Activity context, String[] permissions) {
    if (!permissionsGranted(context, permissions)) {
      ActivityCompat.requestPermissions(context, permissions, REQUEST_CODE);
    }
  }

  /** Called by context to check if camera permissions have been granted. */
  public static boolean cameraPermissionsGranted(Activity context) {
    return permissionsGranted(context, new String[] {CAMERA_PERMISSION});
  }

  /**
   * Called by context to check if camera permissions have been granted and if not, request them.
   */
  public static void checkAndRequestCameraPermissions(Activity context) {
    Log.d(TAG, "checkAndRequestCameraPermissions");
    checkAndRequestPermissions(context, new String[] {CAMERA_PERMISSION});
  }

  /** Called by context to check if audio permissions have been granted. */
  public static boolean audioPermissionsGranted(Activity context) {
    return permissionsGranted(context, new String[] {AUDIO_PERMISSION});
  }

  /** Called by context to check if audio permissions have been granted and if not, request them. */
  public static void checkAndRequestAudioPermissions(Activity context) {
    Log.d(TAG, "checkAndRequestAudioPermissions");
    checkAndRequestPermissions(context, new String[] {AUDIO_PERMISSION});
  }

  /** Called by context to check if read external storage permissions have been granted. */
  public static boolean readExternalStoragePermissionsGranted(Activity context) {
    if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN) {
      return true;
    }
    return permissionsGranted(context, new String[] {READ_EXTERNAL_STORAGE_PERMISSION});
  }

  /**
   * Called by context to check if read external storage permissions have been granted and if not,
   * request them.
   */
  public static void checkAndRequestReadExternalStoragePermissions(Activity context) {
    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
      Log.d(TAG, "checkAndRequestReadExternalStoragePermissions");
      checkAndRequestPermissions(context, new String[] {READ_EXTERNAL_STORAGE_PERMISSION});
    }
  }

  /** Called by context when permissions request has been completed. */
  public static void onRequestPermissionsResult(
      int requestCode, String[] permissions, int[] grantResults) {
    Log.d(TAG, "onRequestPermissionsResult");
    if (permissions.length > 0 && grantResults.length != permissions.length) {
      Log.d(TAG, "Permission denied.");
      return;
    }
    for (int i = 0; i < grantResults.length; ++i) {
      if (grantResults[i] == PackageManager.PERMISSION_GRANTED) {
        Log.d(TAG, permissions[i] + " permission granted.");
      }
    }
    // Note: We don't need any special callbacks when permissions are ready because activities
    // using this helper class can have code in onResume() which is called after the
    // permissions dialog box closes. The code can be branched depending on if permissions are
    // available via permissionsGranted(Activity).
    return;
  }
}
