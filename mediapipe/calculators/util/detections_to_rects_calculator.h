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
#ifndef MEDIAPIPE_CALCULATORS_UTIL_DETECTIONS_TO_RECTS_CALCULATOR_H_
#define MEDIAPIPE_CALCULATORS_UTIL_DETECTIONS_TO_RECTS_CALCULATOR_H_

#include <cmath>

#include "absl/types/optional.h"
#include "mediapipe/calculators/util/detections_to_rects_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_options.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/location_data.pb.h"
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// Dynamic options passed as calculator `input_stream` that can be used for
// calculation of rectangle or rotation for given detection. Does not include
// static calculator options which are available via private fields.
struct DetectionSpec {
  absl::optional<std::pair<int, int>> image_size;
};

// A calculator that converts Detection proto to Rect proto.
//
// Detection is the format for encoding one or more detections in an image.
// The input can be a single Detection or std::vector<Detection>. The output can
// be either a single Rect or NormalizedRect, or std::vector<Rect> or
// std::vector<NormalizedRect>. If Rect is used, the LocationData format is
// expected to be BOUNDING_BOX, and if NormalizedRect is used it is expected to
// be RELATIVE_BOUNDING_BOX.
//
// When the input is std::vector<Detection> and the output is a Rect or
// NormalizedRect, only the first detection is converted. When the input is a
// single Detection and the output is a std::vector<Rect> or
// std::vector<NormalizedRect>, the output is a vector of size 1.
//
// Inputs:
//
// One of the following:
// DETECTION: A Detection proto.
// DETECTIONS: An std::vector<Detection>.
//
// IMAGE_SIZE (optional): A std::pair<int, int> represention image width and
//   height. This is required only when rotation needs to be computed (see
//   calculator options).
//
// Output:
// One of the following:
// RECT: A Rect proto.
// NORM_RECT: A NormalizedRect proto.
// RECTS: An std::vector<Rect>.
// NORM_RECTS: An std::vector<NormalizedRect>.
//
// Example config:
// node {
//   calculator: "DetectionsToRectsCalculator"
//   input_stream: "DETECTIONS:detections"
//   input_stream: "IMAGE_SIZE:image_size"
//   output_stream: "NORM_RECT:rect"
//   options: {
//     [mediapipe.DetectionsToRectCalculatorOptions.ext] {
//       rotation_vector_start_keypoint_index: 0
//       rotation_vector_end_keypoint_index: 2
//       rotation_vector_target_angle_degrees: 90
//       output_zero_rect_for_empty_detections: true
//     }
//   }
// }
class DetectionsToRectsCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 protected:
  virtual absl::Status DetectionToRect(const ::mediapipe::Detection& detection,
                                       const DetectionSpec& detection_spec,
                                       ::mediapipe::Rect* rect);
  virtual absl::Status DetectionToNormalizedRect(
      const ::mediapipe::Detection& detection,
      const DetectionSpec& detection_spec, ::mediapipe::NormalizedRect* rect);
  virtual absl::Status ComputeRotation(const ::mediapipe::Detection& detection,
                                       const DetectionSpec& detection_spec,
                                       float* rotation);
  virtual DetectionSpec GetDetectionSpec(const CalculatorContext* cc);

  static inline float NormalizeRadians(float angle) {
    return angle - 2 * M_PI * std::floor((angle - (-M_PI)) / (2 * M_PI));
  }

  ::mediapipe::DetectionsToRectsCalculatorOptions options_;
  int start_keypoint_index_;
  int end_keypoint_index_;
  float target_angle_ = 0.0f;  // In radians.
  bool rotate_;
  bool output_zero_rect_for_empty_detections_;
};

}  // namespace mediapipe
#endif  // MEDIAPIPE_CALCULATORS_UTIL_DETECTIONS_TO_RECTS_CALCULATOR_H_
