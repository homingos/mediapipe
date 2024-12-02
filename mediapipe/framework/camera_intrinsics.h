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

#ifndef MEDIAPIPE_FRAMEWORK_CAMERA_INTRINSICS_H_
#define MEDIAPIPE_FRAMEWORK_CAMERA_INTRINSICS_H_

class CameraIntrinsics {
 public:
  CameraIntrinsics(float fx, float fy, float cx, float cy, float width,
                   float height)
      : fx_(fx), fy_(fy), cx_(cx), cy_(cy), width_(width), height_(height) {}
  CameraIntrinsics(float fx, float fy, float cx, float cy)
      : CameraIntrinsics(fx, fy, cx, cy, -1, -1) {}

  float fx() const { return fx_; }
  float fy() const { return fy_; }
  float cx() const { return cx_; }
  float cy() const { return cy_; }
  float width() const { return width_; }
  float height() const { return height_; }

 private:
  // Lens focal length along the x-axis, in pixels.
  const float fx_;

  // Lens focal length along the y-axis, in pixels.
  const float fy_;

  // Principal point, x-coordinate on the image, in pixels.
  const float cx_;

  // Principal point, y-coordinate on the image, in pixels.
  const float cy_;

  // Image width, in pixels.
  const float width_;

  // Image height, in pixels.
  const float height_;
};

#endif  // MEDIAPIPE_FRAMEWORK_CAMERA_INTRINSICS_H_
