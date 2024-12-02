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

#ifndef MEDIAPIPE_GPU_EGL_SURFACE_HOLDER_H_
#define MEDIAPIPE_GPU_EGL_SURFACE_HOLDER_H_

#include "absl/base/thread_annotations.h"
#include "absl/synchronization/mutex.h"
#include "mediapipe/gpu/gl_base.h"

#if HAS_EGL

namespace mediapipe {

// This is used to pass an EGLSurface to a GlSurfaceSinkCalculator.
struct EglSurfaceHolder {
  // Access to the surface needs to be protected by a mutex to ensure that the
  // application does not destroy the surface while MediaPipe is using it.
  // NOTE: Code that needs to grab the GlContext mutex should always do so
  // before grabbing this one. For example, do not call GlContext::Run or
  // GlCalculatorHelper::RunInGlContext while holding this mutex, but instead
  // grab this inside the callable passed to them.
  absl::Mutex mutex;
  EGLSurface surface ABSL_GUARDED_BY(mutex) = EGL_NO_SURFACE;
  // True if MediaPipe created the surface and is responsible for destroying it.
  bool owned ABSL_GUARDED_BY(mutex) = false;
  // Vertical flip of the surface, useful for conversion between coordinate
  // systems with top-left v.s. bottom-left origins.
  bool flip_y = false;
  // If true, update the surface presentation timestamp from the MediaPipe
  // packet on Android. It is set to 1000 times the packet timestamp to convert
  // from microseconds (packet) to nanoseconds (surface).
  //
  // This enables consumers to control the presentation time on a SurfaceView or
  // to recover the timestamp with ImageReader or SurfaceTexture.
  //
  // See
  // https://registry.khronos.org/EGL/extensions/ANDROID/EGL_ANDROID_presentation_time.txt
  // for details about the meaning of the presentation time.
  //
  // See also
  //  * https://developer.android.com/reference/android/media/Image#getTimestamp()
  //  * https://developer.android.com/reference/android/graphics/SurfaceTexture#getTimestamp()
  bool update_presentation_time = false;
};

}  // namespace mediapipe

#endif  // HAS_EGL
#endif  // MEDIAPIPE_GPU_EGL_SURFACE_HOLDER_H_
