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

// Consider this file an implementation detail. None of this is part of the
// public API.

#ifndef MEDIAPIPE_GPU_GL_TEXTURE_BUFFER_POOL_H_
#define MEDIAPIPE_GPU_GL_TEXTURE_BUFFER_POOL_H_

#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/synchronization/mutex.h"
#include "mediapipe/gpu/gl_texture_buffer.h"
#include "mediapipe/gpu/multi_pool.h"
#include "mediapipe/gpu/reusable_pool.h"

namespace mediapipe {

class GlTextureBufferPool : public ReusablePool<GlTextureBuffer> {
 public:
  // Creates a pool. This pool will manage buffers of the specified dimensions,
  // and will keep keep_count buffers around for reuse.
  // We enforce creation as a shared_ptr so that we can use a weak reference in
  // the buffers' deleters.
  static std::shared_ptr<GlTextureBufferPool> Create(int width, int height,
                                                     GpuBufferFormat format,
                                                     int keep_count) {
    return Create({width, height, format}, {.keep_count = keep_count});
  }

  static std::shared_ptr<GlTextureBufferPool> Create(
      const internal::GpuBufferSpec& spec, const MultiPoolOptions& options) {
    return std::shared_ptr<GlTextureBufferPool>(
        new GlTextureBufferPool(spec, options));
  }

  int width() const { return spec_.width; }
  int height() const { return spec_.height; }
  GpuBufferFormat format() const { return spec_.format; }

  static absl::StatusOr<GlTextureBufferSharedPtr> CreateBufferWithoutPool(
      const internal::GpuBufferSpec& spec) {
    return GlTextureBuffer::Create(spec);
  }

 protected:
  GlTextureBufferPool(const internal::GpuBufferSpec& spec,
                      const MultiPoolOptions& options)
      : ReusablePool<GlTextureBuffer>(
            [this] { return GlTextureBuffer::Create(spec_); }, options),
        spec_(spec) {}

  const internal::GpuBufferSpec spec_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_GL_TEXTURE_BUFFER_POOL_H_
