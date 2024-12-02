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
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

namespace {

constexpr char kOptionalValueTag[] = "OPTIONAL_VALUE";
constexpr char kDefaultValueTag[] = "DEFAULT_VALUE";
constexpr char kValueTag[] = "VALUE";

}  // namespace

// Outputs side packet default value if optional value is not provided.
//
// This calculator utilizes the fact that MediaPipe automatically removes
// optional side packets of the calculator configuration (i.e. OPTIONAL_VALUE).
// And if it happens - returns default value, otherwise - returns optional
// value.
//
// Input:
//   OPTIONAL_VALUE (optional) - AnyType (but same type as DEFAULT_VALUE)
//     Optional side packet value that is outputted by the calculator as is if
//     provided.
//
//   DEFAULT_VALUE - AnyType
//     Default side pack value that is outputted by the calculator if
//     OPTIONAL_VALUE is not provided.
//
// Output:
//   VALUE - AnyType (but same type as DEFAULT_VALUE)
//     Either OPTIONAL_VALUE (if provided) or DEFAULT_VALUE (otherwise).
//
// Usage example:
//   node {
//     calculator: "DefaultSidePacketCalculator"
//     input_side_packet: "OPTIONAL_VALUE:segmentation_mask_enabled_optional"
//     input_side_packet: "DEFAULT_VALUE:segmentation_mask_enabled_default"
//     output_side_packet: "VALUE:segmentation_mask_enabled"
//   }
class DefaultSidePacketCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;
};
REGISTER_CALCULATOR(DefaultSidePacketCalculator);

absl::Status DefaultSidePacketCalculator::GetContract(CalculatorContract* cc) {
  RET_CHECK(cc->InputSidePackets().HasTag(kDefaultValueTag))
      << "Default value must be provided";
  cc->InputSidePackets().Tag(kDefaultValueTag).SetAny();

  // Optional input side packet can be unspecified. In this case MediaPipe will
  // remove it from the calculator config.
  if (cc->InputSidePackets().HasTag(kOptionalValueTag)) {
    cc->InputSidePackets()
        .Tag(kOptionalValueTag)
        .SetSameAs(&cc->InputSidePackets().Tag(kDefaultValueTag))
        .Optional();
  }

  RET_CHECK(cc->OutputSidePackets().HasTag(kValueTag));
  cc->OutputSidePackets().Tag(kValueTag).SetSameAs(
      &cc->InputSidePackets().Tag(kDefaultValueTag));

  return absl::OkStatus();
}

absl::Status DefaultSidePacketCalculator::Open(CalculatorContext* cc) {
  // If optional value is provided it is returned as the calculator output.
  if (cc->InputSidePackets().HasTag(kOptionalValueTag)) {
    auto& packet = cc->InputSidePackets().Tag(kOptionalValueTag);
    cc->OutputSidePackets().Tag(kValueTag).Set(packet);
    return absl::OkStatus();
  }

  // If no optional value
  auto& packet = cc->InputSidePackets().Tag(kDefaultValueTag);
  cc->OutputSidePackets().Tag(kValueTag).Set(packet);

  return absl::OkStatus();
}

absl::Status DefaultSidePacketCalculator::Process(CalculatorContext* cc) {
  return absl::OkStatus();
}

}  // namespace mediapipe
