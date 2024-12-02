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

// This class lets calculators allocate GpuBuffers of various sizes, caching
// and reusing them as needed. It does so by automatically creating and using
// platform-specific buffer pools for the requested sizes.
//
// This class is not meant to be used directly by calculators, but is instead
// used by GlCalculatorHelper to allocate buffers.

#ifndef MEDIAPIPE_GPU_GPU_BUFFER_MULTI_POOL_H_
#define MEDIAPIPE_GPU_GPU_BUFFER_MULTI_POOL_H_

#include "absl/status/statusor.h"
#include "absl/synchronization/mutex.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/multi_pool.h"

#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
#include "mediapipe/gpu/cv_pixel_buffer_pool_wrapper.h"
#else
#include "mediapipe/gpu/gl_texture_buffer_pool.h"
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER

namespace mediapipe {

class CvPixelBufferPoolWrapper;

class GpuBufferMultiPool : public MultiPool<
#if MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
                               CvPixelBufferPoolWrapper,
#else
                               GlTextureBufferPool,
#endif  // MEDIAPIPE_GPU_BUFFER_USE_CV_PIXEL_BUFFER
                               internal::GpuBufferSpec, GpuBuffer> {
 public:
  using MultiPool::MultiPool;

  absl::StatusOr<GpuBuffer> GetBuffer(
      int width, int height,
      GpuBufferFormat format = GpuBufferFormat::kBGRA32) {
    return Get(internal::GpuBufferSpec(width, height, format));
  }
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GPU_BUFFER_MULTI_POOL_H_
