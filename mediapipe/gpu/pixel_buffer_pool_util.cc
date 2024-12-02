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

#include "mediapipe/gpu/pixel_buffer_pool_util.h"

#import <Foundation/Foundation.h>

#include "mediapipe/objc/util.h"

#if !defined(ENABLE_MEDIAPIPE_GPU_BUFFER_THRESHOLD_CHECK) && !defined(NDEBUG)
#define ENABLE_MEDIAPIPE_GPU_BUFFER_THRESHOLD_CHECK 1
#endif  // defined(ENABLE_MEDIAPIPE_GPU_BUFFER_THRESHOLD_CHECK)

namespace mediapipe {

CVPixelBufferPoolRef CreateCVPixelBufferPool(int width, int height,
                                             OSType pixelFormat, int keepCount,
                                             CFTimeInterval maxAge) {
  CVPixelBufferPoolRef pool = NULL;

  NSMutableDictionary *sourcePixelBufferOptions =
      [(__bridge NSDictionary *)GetCVPixelBufferAttributesForGlCompatibility()
          mutableCopy];
  [sourcePixelBufferOptions addEntriesFromDictionary:@{
    (id)kCVPixelBufferPixelFormatTypeKey : @(pixelFormat),
    (id)kCVPixelBufferWidthKey : @(width),
    (id)kCVPixelBufferHeightKey : @(height),
  }];

  NSMutableDictionary *pixelBufferPoolOptions =
      [[NSMutableDictionary alloc] init];
  pixelBufferPoolOptions[(id)kCVPixelBufferPoolMinimumBufferCountKey] =
      @(keepCount);
  if (maxAge > 0) {
    pixelBufferPoolOptions[(id)kCVPixelBufferPoolMaximumBufferAgeKey] =
        @(maxAge);
  }

  CVPixelBufferPoolCreate(
      kCFAllocatorDefault, (__bridge CFDictionaryRef)pixelBufferPoolOptions,
      (__bridge CFDictionaryRef)sourcePixelBufferOptions, &pool);

  return pool;
}

OSStatus PreallocateCVPixelBufferPoolBuffers(CVPixelBufferPoolRef pool,
                                             int count,
                                             CFDictionaryRef auxAttributes) {
  CVReturn err = kCVReturnSuccess;
  NSMutableArray *pixelBuffers = [[NSMutableArray alloc] init];
  for (int i = 0; i < count && err == kCVReturnSuccess; i++) {
    CVPixelBufferRef pixelBuffer = NULL;
    err = CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(
        kCFAllocatorDefault, pool, auxAttributes, &pixelBuffer);
    if (err != kCVReturnSuccess) {
      break;
    }

    [pixelBuffers addObject:(__bridge id)pixelBuffer];
    CFRelease(pixelBuffer);
  }
  return err;
}

CFDictionaryRef CreateCVPixelBufferPoolAuxiliaryAttributesForThreshold(
    int allocationThreshold) {
  if (allocationThreshold > 0) {
    return (CFDictionaryRef)CFBridgingRetain(@{
      (id)kCVPixelBufferPoolAllocationThresholdKey : @(allocationThreshold)
    });
  } else {
    return nil;
  }
}

CVReturn CreateCVPixelBufferWithPool(CVPixelBufferPoolRef pool,
                                     CFDictionaryRef auxAttributes,
                                     CVTextureCacheType textureCache,
                                     CVPixelBufferRef *outBuffer) {
  return CreateCVPixelBufferWithPool(
      pool, auxAttributes,
      [textureCache]() {
#if TARGET_OS_OSX
        CVOpenGLTextureCacheFlush(textureCache, 0);
#else
        CVOpenGLESTextureCacheFlush(textureCache, 0);
#endif  // TARGET_OS_OSX
      },
      outBuffer);
}

CVReturn CreateCVPixelBufferWithPool(CVPixelBufferPoolRef pool,
                                     CFDictionaryRef auxAttributes,
                                     std::function<void(void)> flush,
                                     CVPixelBufferRef *outBuffer) {
  CVReturn err = CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(
      kCFAllocatorDefault, pool, auxAttributes, outBuffer);
  if (err == kCVReturnWouldExceedAllocationThreshold) {
    if (flush) {
      // Call the flush function to potentially release the retained buffers
      // and try again to create a pixel buffer.
      flush();
      err = CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(
          kCFAllocatorDefault, pool, auxAttributes, outBuffer);
    }
    if (err == kCVReturnWouldExceedAllocationThreshold) {
      // TODO: allow the application to set the threshold. For now, disable it
      // by default, since the threshold we are using is arbitrary and some
      // graphs routinely cross it.
#ifdef ENABLE_MEDIAPIPE_GPU_BUFFER_THRESHOLD_CHECK
      NSLog(
          @"Using more buffers than expected! This is a debug-only warning, "
           "you can ignore it if your app works fine otherwise.");
#ifdef DEBUG
      NSLog(@"Pool status: %@", ((__bridge NSObject *)pool).description);
#endif  // DEBUG
#endif  // defined(ENABLE_MEDIAPIPE_GPU_BUFFER_THRESHOLD_CHECK)
      // Try again and ignore threshold.
      // TODO drop a frame instead?
      err = CVPixelBufferPoolCreatePixelBufferWithAuxAttributes(
          kCFAllocatorDefault, pool, NULL, outBuffer);
    }
  }
  return err;
}

}  // namespace mediapipe
