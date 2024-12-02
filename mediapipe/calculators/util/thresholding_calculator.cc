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

#include "mediapipe/calculators/util/thresholding_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"

namespace mediapipe {

constexpr char kThresholdTag[] = "THRESHOLD";
constexpr char kRejectTag[] = "REJECT";
constexpr char kAcceptTag[] = "ACCEPT";
constexpr char kFlagTag[] = "FLAG";
constexpr char kFloatTag[] = "FLOAT";

// Applies a threshold on a stream of numeric values and outputs a flag and/or
// accept/reject stream. The threshold can be specified by one of the following:
//   1) Input stream.
//   2) Input side packet.
//   3) Calculator option.
//
// Input:
//  FLOAT: A float, which will be cast to double to be compared with a
//         threshold of double type.
//  THRESHOLD(optional): A double specifying the threshold at current timestamp.
//
// Output:
//   FLAG(optional): A boolean indicating if the input value is larger than the
//                   threshold.
//   ACCEPT(optional): A packet will be sent if the value is larger than the
//                     threshold.
//   REJECT(optional): A packet will be sent if the value is no larger than the
//                     threshold.
//
// Usage example:
// node {
//   calculator: "ThresholdingCalculator"
//   input_stream: "FLOAT:score"
//   output_stream: "ACCEPT:accept"
//   output_stream: "REJECT:reject"
//   options: {
//     [mediapipe.ThresholdingCalculatorOptions.ext] {
//       threshold: 0.1
//     }
//   }
// }
class ThresholdingCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) override;

  absl::Status Process(CalculatorContext* cc) override;

 private:
  double threshold_{};
};
REGISTER_CALCULATOR(ThresholdingCalculator);

absl::Status ThresholdingCalculator::GetContract(CalculatorContract* cc) {
  RET_CHECK(cc->Inputs().HasTag(kFloatTag));
  cc->Inputs().Tag(kFloatTag).Set<float>();

  if (cc->Outputs().HasTag(kFlagTag)) {
    cc->Outputs().Tag(kFlagTag).Set<bool>();
  }
  if (cc->Outputs().HasTag(kAcceptTag)) {
    cc->Outputs().Tag(kAcceptTag).Set<bool>();
  }
  if (cc->Outputs().HasTag(kRejectTag)) {
    cc->Outputs().Tag(kRejectTag).Set<bool>();
  }
  if (cc->Inputs().HasTag(kThresholdTag)) {
    cc->Inputs().Tag(kThresholdTag).Set<double>();
  }
  if (cc->InputSidePackets().HasTag(kThresholdTag)) {
    cc->InputSidePackets().Tag(kThresholdTag).Set<double>();
    RET_CHECK(!cc->Inputs().HasTag(kThresholdTag))
        << "Using both the threshold input side packet and input stream is not "
           "supported.";
  }

  return absl::OkStatus();
}

absl::Status ThresholdingCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(TimestampDiff(0));

  const auto& options =
      cc->Options<::mediapipe::ThresholdingCalculatorOptions>();
  if (options.has_threshold()) {
    RET_CHECK(!cc->Inputs().HasTag(kThresholdTag))
        << "Using both the threshold option and input stream is not supported.";
    RET_CHECK(!cc->InputSidePackets().HasTag(kThresholdTag))
        << "Using both the threshold option and input side packet is not "
           "supported.";
    threshold_ = options.threshold();
  }

  if (cc->InputSidePackets().HasTag(kThresholdTag)) {
    threshold_ = cc->InputSidePackets().Tag(kThresholdTag).Get<double>();
  }
  return absl::OkStatus();
}

absl::Status ThresholdingCalculator::Process(CalculatorContext* cc) {
  if (cc->Inputs().HasTag(kThresholdTag) &&
      !cc->Inputs().Tag(kThresholdTag).IsEmpty()) {
    threshold_ = cc->Inputs().Tag(kThresholdTag).Get<double>();
  }

  bool accept = false;
  RET_CHECK(!cc->Inputs().Tag(kFloatTag).IsEmpty());
  accept = static_cast<double>(cc->Inputs().Tag(kFloatTag).Get<float>()) >
           threshold_;

  if (cc->Outputs().HasTag(kFlagTag)) {
    cc->Outputs().Tag(kFlagTag).AddPacket(
        MakePacket<bool>(accept).At(cc->InputTimestamp()));
  }

  if (accept && cc->Outputs().HasTag(kAcceptTag)) {
    cc->Outputs()
        .Tag(kAcceptTag)
        .AddPacket(MakePacket<bool>(true).At(cc->InputTimestamp()));
  }
  if (!accept && cc->Outputs().HasTag(kRejectTag)) {
    cc->Outputs()
        .Tag(kRejectTag)
        .AddPacket(MakePacket<bool>(false).At(cc->InputTimestamp()));
  }

  return absl::OkStatus();
}
}  // namespace mediapipe
