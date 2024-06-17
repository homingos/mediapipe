#ifndef MEDIAPIPE_CALCULATORS_UTIL_FEATURE_CALCULATOR_H_
#define MEDIAPIPE_CALCULATORS_UTIL_FEATURE_CALCULATOR_H_

#include <cstdint>
#include <vector>

#include "absl/status/status.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/port/opencv_core_inc.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

constexpr char kImageTag[] = "IMAGE";
constexpr char kFeaturesTag[] = "FEATURES";

class FeatureDetection : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;

  absl::Status Process(CalculatorContext* cc) override;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_UTIL_FEATURE_CALCULATOR_H_