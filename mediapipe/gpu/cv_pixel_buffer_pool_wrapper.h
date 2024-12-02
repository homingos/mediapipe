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

#ifndef MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_
#define MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_

#include "CoreFoundation/CFBase.h"
#include "absl/status/statusor.h"
#include "mediapipe/gpu/cv_texture_cache_manager.h"
#include "mediapipe/gpu/gpu_buffer_format.h"
#include "mediapipe/gpu/multi_pool.h"
#include "mediapipe/gpu/pixel_buffer_pool_util.h"
#include "mediapipe/objc/CFHolder.h"

namespace mediapipe {

class CvPixelBufferPoolWrapper {
 public:
  CvPixelBufferPoolWrapper(int width, int height, GpuBufferFormat format,
                           CFTimeInterval maxAge,
                           CvTextureCacheManager* texture_caches);

  static std::shared_ptr<CvPixelBufferPoolWrapper> Create(
      const internal::GpuBufferSpec& spec, const MultiPoolOptions& options,
      CvTextureCacheManager* texture_caches = nullptr) {
    return std::make_shared<CvPixelBufferPoolWrapper>(
        spec.width, spec.height, spec.format, options.max_inactive_buffer_age,
        texture_caches);
  }

  absl::StatusOr<CFHolder<CVPixelBufferRef>> GetBuffer();

  int GetBufferCount() const { return count_; }
  std::string GetDebugString() const;

  void Flush();

  static absl::StatusOr<CFHolder<CVPixelBufferRef>> CreateBufferWithoutPool(
      const internal::GpuBufferSpec& spec);

 private:
  CFHolder<CVPixelBufferPoolRef> pool_;
  int count_ = 0;
  CvTextureCacheManager* texture_caches_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_CV_PIXEL_BUFFER_POOL_WRAPPER_H_
