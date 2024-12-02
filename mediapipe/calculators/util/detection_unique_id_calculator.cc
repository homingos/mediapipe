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

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

namespace {

constexpr char kDetectionsTag[] = "DETECTIONS";
constexpr char kDetectionListTag[] = "DETECTION_LIST";

// Each detection processed by DetectionUniqueIDCalculator will be assigned an
// unique id that starts from 1. If a detection already has an ID other than 0,
// the ID will be overwritten.
static int64_t detection_id = 0;

inline int GetNextDetectionId() { return ++detection_id; }

}  // namespace

// Assign a unique id to detections.
// Note that the calculator will consume the input vector of Detection or
// DetectionList. So the input stream can not be connected to other calculators.
//
// Example config:
// node {
//   calculator: "DetectionUniqueIdCalculator"
//   input_stream: "DETECTIONS:detections"
//   output_stream: "DETECTIONS:output_detections"
// }
class DetectionUniqueIdCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag(kDetectionListTag) ||
              cc->Inputs().HasTag(kDetectionsTag))
        << "None of the input streams are provided.";

    if (cc->Inputs().HasTag(kDetectionListTag)) {
      RET_CHECK(cc->Outputs().HasTag(kDetectionListTag));
      cc->Inputs().Tag(kDetectionListTag).Set<DetectionList>();
      cc->Outputs().Tag(kDetectionListTag).Set<DetectionList>();
    }
    if (cc->Inputs().HasTag(kDetectionsTag)) {
      RET_CHECK(cc->Outputs().HasTag(kDetectionsTag));
      cc->Inputs().Tag(kDetectionsTag).Set<std::vector<Detection>>();
      cc->Outputs().Tag(kDetectionsTag).Set<std::vector<Detection>>();
    }

    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(mediapipe::TimestampDiff(0));
    return absl::OkStatus();
  }
  absl::Status Process(CalculatorContext* cc) override;
};
REGISTER_CALCULATOR(DetectionUniqueIdCalculator);

absl::Status DetectionUniqueIdCalculator::Process(CalculatorContext* cc) {
  if (cc->Inputs().HasTag(kDetectionListTag) &&
      !cc->Inputs().Tag(kDetectionListTag).IsEmpty()) {
    auto result =
        cc->Inputs().Tag(kDetectionListTag).Value().Consume<DetectionList>();
    if (result.ok()) {
      auto detection_list = std::move(result).value();
      for (Detection& detection : *detection_list->mutable_detection()) {
        detection.set_detection_id(GetNextDetectionId());
      }
      cc->Outputs()
          .Tag(kDetectionListTag)
          .Add(detection_list.release(), cc->InputTimestamp());
    }
  }

  if (cc->Inputs().HasTag(kDetectionsTag) &&
      !cc->Inputs().Tag(kDetectionsTag).IsEmpty()) {
    auto result = cc->Inputs()
                      .Tag(kDetectionsTag)
                      .Value()
                      .Consume<std::vector<Detection>>();
    if (result.ok()) {
      auto detections = std::move(result).value();
      for (Detection& detection : *detections) {
        detection.set_detection_id(GetNextDetectionId());
      }
      cc->Outputs()
          .Tag(kDetectionsTag)
          .Add(detections.release(), cc->InputTimestamp());
    }
  }
  return absl::OkStatus();
}

}  // namespace mediapipe
