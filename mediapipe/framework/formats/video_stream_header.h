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
//
// The Video stream header.

#ifndef MEDIAPIPE_FRAMEWORK_FORMATS_VIDEO_STREAM_HEADER_H_
#define MEDIAPIPE_FRAMEWORK_FORMATS_VIDEO_STREAM_HEADER_H_

#include "mediapipe/framework/formats/image_format.pb.h"

namespace mediapipe {

// This defines the format of a video stream header.
struct VideoHeader {
  // Video frame format.
  ImageFormat::Format format = ImageFormat::UNKNOWN;

  // Dimensions of the video in pixels.
  int width = 0;
  int height = 0;

  // Video duration in seconds.
  // NOTE: This field was introduced after the others, so it is not widely
  // supported. If you use it, make sure that all intermediate calculators pass
  // it through.
  float duration = 0.0f;

  // The frame rate in Hz at which the video frames are output.
  double frame_rate = 0.0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_FRAMEWORK_FORMATS_VIDEO_STREAM_HEADER_H_
