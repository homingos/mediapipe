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

#import <UIKit/UIKit.h>

#import "mediapipe/objc/MPPCameraInputSource.h"
#import "mediapipe/objc/MPPGraph.h"
#import "mediapipe/objc/MPPLayerRenderer.h"
#import "mediapipe/objc/MPPPlayerInputSource.h"
#import "mediapipe/objc/MPPTimestampConverter.h"

typedef NS_ENUM(NSInteger, MediaPipeDemoSourceMode) {
  MediaPipeDemoSourceCamera,
  MediaPipeDemoSourceVideo
};

@interface CommonViewController : UIViewController <MPPGraphDelegate, MPPInputSourceDelegate>

// The MediaPipe graph currently in use. Initialized in viewDidLoad, started in
// viewWillAppear: and sent video frames on videoQueue.
@property(nonatomic) MPPGraph* mediapipeGraph;

// Handles camera access via AVCaptureSession library.
@property(nonatomic) MPPCameraInputSource* cameraSource;

// Provides data from a video.
@property(nonatomic) MPPPlayerInputSource* videoSource;

// Helps to convert timestamp.
@property(nonatomic) MPPTimestampConverter* timestampConverter;

// The data source for the demo.
@property(nonatomic) MediaPipeDemoSourceMode sourceMode;

// Inform the user when camera is unavailable.
@property(nonatomic) IBOutlet UILabel* noCameraLabel;

// Display the camera preview frames.
@property(strong, nonatomic) IBOutlet UIView* liveView;

// Render frames in a layer.
@property(nonatomic) MPPLayerRenderer* renderer;

// Process camera frames on this queue.
@property(nonatomic) dispatch_queue_t videoQueue;

// Graph name.
@property(nonatomic) NSString* graphName;

// Graph input stream.
@property(nonatomic) const char* graphInputStream;

// Graph output stream.
@property(nonatomic) const char* graphOutputStream;

@end
