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

package com.google.mediapipe.framework;

/**
 * Utilities for compatibility with old versions of Android.
 */
public class Compat {
  /**
   * Returns the native handle to the current EGL context. Can be used as a
   * replacement for EGL14.eglGetCurrentContext().getNativeHandle() before
   * API 17.
   */
  public static native long getCurrentNativeEGLContext();

  /**
   * Returns the native handle to the current EGL surface. Can be used as a
   * replacement for EGL14.eglGetCurrentSurface().getNativeHandle() before
   * API 17.
   */
  public static native long getCurrentNativeEGLSurface(int readdraw);
}
