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
#include <cmath>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/rect.pb.h"

namespace mediapipe {

namespace {

constexpr char kNormRectTag[] = "NORM_RECT";
constexpr char kNormReferenceRectTag[] = "NORM_REFERENCE_RECT";

using ::mediapipe::NormalizedRect;

}  // namespace

// Projects rectangle from reference coordinate system (defined by reference
// rectangle) to original coordinate system (in which this reference rectangle
// is defined).
//
// Inputs:
//   NORM_RECT - A NormalizedRect to be projected.
//   NORM_REFERENCE_RECT - A NormalizedRect that represents reference coordinate
//     system for NORM_RECT and is defined in original coordinates.
//
// Outputs:
//   NORM_RECT: A NormalizedRect projected to the original coordinates.
//
// Example config:
//   node {
//     calculator: "RectProjectionCalculator"
//     input_stream: "NORM_RECT:face_rect"
//     input_stream: "NORM_REFERENCE_RECT:face_reference_rect"
//     output_stream: "NORM_RECT:projected_face_rect"
//   }
//
class RectProjectionCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;
};
REGISTER_CALCULATOR(RectProjectionCalculator);

absl::Status RectProjectionCalculator::GetContract(CalculatorContract* cc) {
  cc->Inputs().Tag(kNormRectTag).Set<NormalizedRect>();
  cc->Inputs().Tag(kNormReferenceRectTag).Set<NormalizedRect>();
  cc->Outputs().Tag(kNormRectTag).Set<NormalizedRect>();
  return absl::OkStatus();
}

absl::Status RectProjectionCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(TimestampDiff(0));

  return absl::OkStatus();
}

absl::Status RectProjectionCalculator::Process(CalculatorContext* cc) {
  if (cc->Inputs().Tag(kNormRectTag).IsEmpty()) {
    return absl::OkStatus();
  }

  const auto& rect = cc->Inputs().Tag(kNormRectTag).Get<NormalizedRect>();
  const auto& reference_rect =
      cc->Inputs().Tag(kNormReferenceRectTag).Get<NormalizedRect>();

  // Project center.
  const float x = rect.x_center() - 0.5f;
  const float y = rect.y_center() - 0.5f;
  const float angle = reference_rect.rotation();
  float new_x = std::cos(angle) * x - std::sin(angle) * y;
  float new_y = std::sin(angle) * x + std::cos(angle) * y;
  new_x = new_x * reference_rect.width() + reference_rect.x_center();
  new_y = new_y * reference_rect.height() + reference_rect.y_center();

  // Project size.
  const float new_width = rect.width() * reference_rect.width();
  const float new_height = rect.height() * reference_rect.height();

  // Project rotation.
  const float new_rotation = rect.rotation() + reference_rect.rotation();

  auto new_rect = absl::make_unique<NormalizedRect>();
  new_rect->set_x_center(new_x);
  new_rect->set_y_center(new_y);
  new_rect->set_width(new_width);
  new_rect->set_height(new_height);
  new_rect->set_rotation(new_rotation);

  cc->Outputs().Tag(kNormRectTag).Add(new_rect.release(), cc->InputTimestamp());

  return absl::OkStatus();
}

}  // namespace mediapipe
