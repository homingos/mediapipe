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
#include "mediapipe/framework/formats/rect.pb.h"
#include "mediapipe/framework/port/rectangle.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// A subclass of AssociationCalculator<T> for NormalizedRect. Example use case:
// node {
//   calculator: "AssociationNormRectCalculator"
//   input_stream: "input_vec_0"
//   input_stream: "input_vec_1"
//   input_stream: "input_vec_2"
//   output_stream: "output_vec"
//   options {
//     [mediapipe.AssociationCalculatorOptions.ext] {
//       min_similarity_threshold: 0.1
//     }
// }
class AssociationNormRectCalculator
    : public AssociationCalculator<::mediapipe::NormalizedRect> {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    return AssociationCalculator<::mediapipe::NormalizedRect>::GetContract(cc);
  }

  absl::Status Open(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::NormalizedRect>::Open(cc);
  }

  absl::Status Process(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::NormalizedRect>::Process(cc);
  }

  absl::Status Close(CalculatorContext* cc) override {
    return AssociationCalculator<::mediapipe::NormalizedRect>::Close(cc);
  }

 protected:
  absl::StatusOr<Rectangle_f> GetRectangle(
      const ::mediapipe::NormalizedRect& input) override {
    if (!input.has_x_center() || !input.has_y_center() || !input.has_width() ||
        !input.has_height()) {
      return absl::InternalError("Missing dimensions in NormalizedRect.");
    }
    const float xmin = input.x_center() - input.width() / 2.0;
    const float ymin = input.y_center() - input.height() / 2.0;
    // TODO: Support rotation for rectangle.
    return Rectangle_f(xmin, ymin, input.width(), input.height());
  }
};

REGISTER_CALCULATOR(AssociationNormRectCalculator);

}  // namespace mediapipe
