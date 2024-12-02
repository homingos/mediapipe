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

#ifndef MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_VISUAL_SCORER_H_
#define MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_VISUAL_SCORER_H_

#include "mediapipe/examples/desktop/autoflip/autoflip_messages.pb.h"
#include "mediapipe/examples/desktop/autoflip/quality/visual_scorer.pb.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace autoflip {

// This class scores a SalientRegion within an image based on weighted averages
// of various signals computed on the patch.
class VisualScorer {
 public:
  explicit VisualScorer(const VisualScorerOptions& options);

  // Computes a score on a salientregion and returns a value [0...1].
  absl::Status CalculateScore(const cv::Mat& image, const SalientRegion& region,
                              float* score) const;

 private:
  absl::Status CalculateColorfulness(const cv::Mat& image,
                                     float* colorfulness) const;

  VisualScorerOptions options_;
};

}  // namespace autoflip
}  // namespace mediapipe

#endif  // MEDIAPIPE_EXAMPLES_DESKTOP_AUTOFLIP_QUALITY_VISUAL_SCORER_H_
