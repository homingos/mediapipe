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

#ifndef MEDIAPIPE_GPU_MPP_METAL_UTIL_H_
#define MEDIAPIPE_GPU_MPP_METAL_UTIL_H_

#import <CoreVideo/CVMetalTextureCache.h>
#import <CoreVideo/CoreVideo.h>
#import <Metal/Metal.h>

NS_ASSUME_NONNULL_BEGIN

@interface MPPMetalUtil : NSObject {
}

/// Copies a Metal Buffer from source to destination.
/// Uses blitCommandEncoder and assumes offset of 0.
+ (void)blitMetalBufferTo:(id<MTLBuffer>)destination
                     from:(id<MTLBuffer>)source
                 blocking:(bool)blocking
            commandBuffer:(id<MTLCommandBuffer>)commandBuffer;

/// Copies a Metal Buffer from source to destination.
/// Simple wrapper for blitCommandEncoder.
/// Optionally block until operation is completed.
+ (void)blitMetalBufferTo:(id<MTLBuffer>)destination
        destinationOffset:(int)destinationOffset
                     from:(id<MTLBuffer>)source
             sourceOffset:(int)sourceOffset
                    bytes:(size_t)bytes
                 blocking:(bool)blocking
            commandBuffer:(id<MTLCommandBuffer>)commandBuffer;

/// Commits the command buffer and waits until execution completes. This is
/// functionally equivalent to calling commit and waitUntilCompleted on the
/// command buffer, but may use different synchronization strategies, such as
/// active wait.
+ (void)commitCommandBufferAndWait:(id<MTLCommandBuffer>)commandBuffer;

@end

NS_ASSUME_NONNULL_END

#endif  // MEDIAPIPE_GPU_MPP_METAL_UTIL_H_
