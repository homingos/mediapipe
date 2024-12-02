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

#import "FaceMeshGpuViewController.h"

#include "mediapipe/framework/formats/landmark.pb.h"

static NSString* const kGraphName = @"face_mesh_mobile_gpu";

static const char* kNumFacesInputSidePacket = "num_faces";
static const char* kLandmarksOutputStream = "multi_face_landmarks";

// Max number of faces to detect/process.
static const int kNumFaces = 1;

@implementation FaceMeshGpuViewController

#pragma mark - UIViewController methods

- (void)viewDidLoad {
  [super viewDidLoad];

  [self.mediapipeGraph setSidePacket:(mediapipe::MakePacket<int>(kNumFaces))
                               named:kNumFacesInputSidePacket];
  [self.mediapipeGraph addFrameOutputStream:kLandmarksOutputStream
                           outputPacketType:MPPPacketTypeRaw];
}

#pragma mark - MPPGraphDelegate methods

// Receives a raw packet from the MediaPipe graph. Invoked on a MediaPipe worker thread.
- (void)mediapipeGraph:(MPPGraph*)graph
     didOutputPacket:(const ::mediapipe::Packet&)packet
          fromStream:(const std::string&)streamName {
  if (streamName == kLandmarksOutputStream) {
    if (packet.IsEmpty()) {
      NSLog(@"[TS:%lld] No face landmarks", packet.Timestamp().Value());
      return;
    }
    const auto& multi_face_landmarks = packet.Get<std::vector<::mediapipe::NormalizedLandmarkList>>();
    NSLog(@"[TS:%lld] Number of face instances with landmarks: %lu", packet.Timestamp().Value(),
          multi_face_landmarks.size());
    for (int face_index = 0; face_index < multi_face_landmarks.size(); ++face_index) {
      const auto& landmarks = multi_face_landmarks[face_index];
      NSLog(@"\tNumber of landmarks for face[%d]: %d", face_index, landmarks.landmark_size());
      for (int i = 0; i < landmarks.landmark_size(); ++i) {
        NSLog(@"\t\tLandmark[%d]: (%f, %f, %f)", i, landmarks.landmark(i).x(),
              landmarks.landmark(i).y(), landmarks.landmark(i).z());
      }
    }
  }
}

@end
