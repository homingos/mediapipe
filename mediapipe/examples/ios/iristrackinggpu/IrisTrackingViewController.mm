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

#import "IrisTrackingViewController.h"

#include "mediapipe/framework/formats/landmark.pb.h"

static const char* kLandmarksOutputStream = "iris_landmarks";

@implementation IrisTrackingViewController {
  /// Input side packet for focal length parameter.
  std::map<std::string, mediapipe::Packet> _input_side_packets;
  mediapipe::Packet _focal_length_side_packet;
}

#pragma mark - UIViewController methods

- (void)viewDidLoad {
  [super viewDidLoad];

  [self.mediapipeGraph addFrameOutputStream:kLandmarksOutputStream
                           outputPacketType:MPPPacketTypeRaw];
  _focal_length_side_packet =
      mediapipe::MakePacket<std::unique_ptr<float>>(absl::make_unique<float>(0.0));
  _input_side_packets = {
      {"focal_length_pixel", _focal_length_side_packet},
  };
  [self.mediapipeGraph addSidePackets:_input_side_packets];
}

#pragma mark - MPPGraphDelegate methods

// Receives a raw packet from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
     didOutputPacket:(const ::mediapipe::Packet&)packet
          fromStream:(const std::string&)streamName {
  if (streamName == kLandmarksOutputStream) {
    if (packet.IsEmpty()) {
      NSLog(@"[TS:%lld] No iris landmarks", packet.Timestamp().Value());
      return;
    }
    const auto& landmarks = packet.Get<::mediapipe::NormalizedLandmarkList>();
    NSLog(@"[TS:%lld] Number of landmarks on iris: %d", packet.Timestamp().Value(),
          landmarks.landmark_size());
    for (int i = 0; i < landmarks.landmark_size(); ++i) {
      NSLog(@"\tLandmark[%d]: (%f, %f, %f)", i, landmarks.landmark(i).x(),
            landmarks.landmark(i).y(), landmarks.landmark(i).z());
    }
  }
}

#pragma mark - MPPInputSourceDelegate methods

// Must be invoked on _videoQueue.
- (void)processVideoFrame:(CVPixelBufferRef)imageBuffer
                timestamp:(CMTime)timestamp
               fromSource:(MPPInputSource*)source {
  if (source != self.cameraSource) {
    NSLog(@"Unknown source: %@", source);
    return;
  }

  // TODO: This is a temporary solution. Need to verify whether the focal length is
  // constant. In that case, we need to use input stream instead of using side packet.
  *(_input_side_packets["focal_length_pixel"].Get<std::unique_ptr<float>>()) =
      self.cameraSource.cameraIntrinsicMatrix.columns[0][0];
  [self.mediapipeGraph sendPixelBuffer:imageBuffer
                            intoStream:self.graphInputStream
                            packetType:MPPPacketTypePixelBuffer];
}

@end
