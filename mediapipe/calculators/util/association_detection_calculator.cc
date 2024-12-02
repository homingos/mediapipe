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

#include "mediapipe/calculators/util/association_calculator.h"
#include "mediapipe/framework/calculator_context.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/location.h"
#include "mediapipe/framework/port/rectangle.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// A subclass of AssociationCalculator<T> for Detection. Example:
// node {
//   calculator: "AssociationDetectionCalculator"
//   input_stream: "PREV:input_vec_0"
//   input_stream: "input_vec_1"
//   input_stream: "input_vec_2"
//   output_stream: "output_vec"
//   options {
//     [mediapipe.AssociationCalculatorOptions.ext] {
//       min_similarity_threshold: 0.1
//     }
// }
class AssociationDetectionCalculator
    : public AssociationCalculator<::mediapipe::Detection> {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    return AssociationCalculator<::mediapipe::Detection>::GetContract(cc);
  }

  absl::Status Open(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::Detection>::Open(cc);
  }

  absl::Status Process(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::Detection>::Process(cc);
  }

  absl::Status Close(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::Detection>::Close(cc);
  }

 protected:
  absl::StatusOr<Rectangle_f> GetRectangle(
      const ::mediapipe::Detection& input) override {
    if (!input.has_location_data()) {
      return absl::InternalError("Missing location_data in Detection");
    }
    const Location location(input.location_data());
    return location.GetRelativeBBox();
  }

  std::pair<bool, int> GetId(const ::mediapipe::Detection& input) override {
    return {input.has_detection_id(), input.detection_id()};
  }

  void SetId(::mediapipe::Detection* input, int id) override {
    input->set_detection_id(id);
  }
};

REGISTER_CALCULATOR(AssociationDetectionCalculator);

}  // namespace mediapipe
