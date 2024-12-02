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

#ifndef MEDIAPIPE_GPU_PIXEL_BUFFER_POOL_UTIL_H_
#define MEDIAPIPE_GPU_PIXEL_BUFFER_POOL_UTIL_H_

#include <CoreVideo/CoreVideo.h>
#include <TargetConditionals.h>

#include <functional>

#ifndef __APPLE__
#error gpu_pixel_buffer_pool_util is only for use on Apple platforms.
#endif  // !defined(__APPLE__)

namespace mediapipe {

#if TARGET_OS_OSX
typedef CVOpenGLTextureCacheRef CVTextureCacheType;
#else
typedef CVOpenGLESTextureCacheRef CVTextureCacheType;
#endif  // TARGET_OS_OSX

// Create a CVPixelBufferPool.
CVPixelBufferPoolRef CreateCVPixelBufferPool(int width, int height,
                                             OSType pixel_format,
                                             int keep_count,
                                             CFTimeInterval maxAge);

// Preallocate the given number of pixel buffers.
OSStatus PreallocateCVPixelBufferPoolBuffers(CVPixelBufferPoolRef pool,
                                             int count,
                                             CFDictionaryRef auxAttributes);

// Create a CVPixelBuffer using a pool.
// If the pool is full, will flush the provided texture cache before trying
// again.
CVReturn CreateCVPixelBufferWithPool(CVPixelBufferPoolRef pool,
                                     CFDictionaryRef auxAttributes,
                                     CVTextureCacheType textureCache,
                                     CVPixelBufferRef* outBuffer);

// Create a CVPixelBuffer using a pool.
// If the pool is full, will call the provided function before trying again.
CVReturn CreateCVPixelBufferWithPool(CVPixelBufferPoolRef pool,
                                     CFDictionaryRef auxAttributes,
                                     std::function<void(void)> flush,
                                     CVPixelBufferRef* outBuffer);

// Create an auxiliary attribute dictionary, which can be used with
// CVPixelBufferPool, specifying the given allocation threshold.
CFDictionaryRef CreateCVPixelBufferPoolAuxiliaryAttributesForThreshold(
    int allocationThreshold);

}  // namespace mediapipe

#endif  // MEDIAPIPE_GPU_PIXEL_BUFFER_POOL_UTIL_H_
