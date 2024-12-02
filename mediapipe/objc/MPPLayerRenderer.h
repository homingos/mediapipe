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

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>

#import "mediapipe/objc/MPPGLViewRenderer.h"

/// Renders frames in a Core Animation layer.
@interface MPPLayerRenderer : NSObject

@property(nonatomic, readonly) CAEAGLLayer *layer;

/// Updates the layer with a new pixel buffer.
- (void)renderPixelBuffer:(CVPixelBufferRef)pixelBuffer;

/// Sets which way to rotate input frames before rendering them.
/// Default value is MPPFrameRotationNone.
@property(nonatomic) MPPFrameRotation frameRotationMode;

/// Sets how to scale the frame within the layer.
/// Default value is MediaPipeFrameScaleScaleToFit.
@property(nonatomic) MPPFrameScaleMode frameScaleMode;

/// If YES, swap left and right. Useful for the front camera.
@property(nonatomic) BOOL mirrored;

@end
