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

#import <AVFoundation/AVFoundation.h>

#import "mediapipe/objc/MPPInputSource.h"

/// A source that obtains video frames from the camera.
@interface MPPCameraInputSource : MPPInputSource

/// Whether we are allowed to use the camera.
@property(nonatomic, getter=isAuthorized, readonly) BOOL authorized;

/// Session preset to use for capturing.
@property(nonatomic, nullable) NSString *sessionPreset;

/// Which camera on an iOS device to use, assuming iOS device with more than one camera.
@property(nonatomic) AVCaptureDevicePosition cameraPosition;

// Whether to use depth data or not
@property(nonatomic) BOOL useDepth;

/// Whether to rotate video buffers with device rotation.
@property(nonatomic) BOOL autoRotateBuffers;

/// Whether to mirror the video or not.
@property(nonatomic) BOOL videoMirrored;

/// The camera intrinsic matrix.
@property(nonatomic, readonly) matrix_float3x3 cameraIntrinsicMatrix;

/// The capture session.
@property(nonatomic, readonly) AVCaptureSession *session;

/// The capture video preview layer.
@property(nonatomic, readonly) AVCaptureVideoPreviewLayer *videoPreviewLayer;

/// The orientation of camera frame buffers.
@property(nonatomic) AVCaptureVideoOrientation orientation;

/// Prompts the user to grant camera access and provides the result as a BOOL to a completion
/// handler. Should be called after [MPPCameraInputSource init] and before
/// [MPPCameraInputSource start]. If the user has previously granted or denied permission, this
/// method simply returns the saved response to the permission request.
- (void)requestCameraAccessWithCompletionHandler:(void (^_Nullable)(BOOL granted))handler;

@end
