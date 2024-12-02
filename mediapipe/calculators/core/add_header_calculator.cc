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

#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/logging.h"

namespace mediapipe {
namespace api2 {

// Attach the header from a stream or side input to another stream.
//
// The header stream (tag HEADER) must not have any packets in it.
//
// Before using this calculator, please think about changing your
// calculator to not need a header or to accept a separate stream with
// a header, that would be more future proof.
//
// Example usage 1:
// node {
//   calculator: "AddHeaderCalculator"
//   input_stream: "DATA:audio"
//   input_stream: "HEADER:audio_header"
//   output_stream: "audio_with_header"
// }
//
// Example usage 2:
// node {
//   calculator: "AddHeaderCalculator"
//   input_stream: "DATA:audio"
//   input_side_packet: "HEADER:audio_header"
//   output_stream: "audio_with_header"
// }
//
class AddHeaderCalculator : public Node {
 public:
  static constexpr Input<NoneType>::Optional kHeader{"HEADER"};
  static constexpr SideInput<AnyType>::Optional kHeaderSide{"HEADER"};
  static constexpr Input<AnyType> kData{"DATA"};
  static constexpr Output<SameType<kData>> kOut{""};

  MEDIAPIPE_NODE_CONTRACT(kHeader, kHeaderSide, kData, kOut);

  static absl::Status UpdateContract(CalculatorContract* cc) {
    if (kHeader(cc).IsConnected() == kHeaderSide(cc).IsConnected()) {
      return absl::InvalidArgumentError(
          "Header must be provided via exactly one of side input and input "
          "stream");
    }
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    const PacketBase& header =
        kHeader(cc).IsConnected() ? kHeader(cc).Header() : kHeaderSide(cc);
    if (!header.IsEmpty()) {
      kOut(cc).SetHeader(header);
    }
    cc->SetOffset(0);
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    kOut(cc).Send(kData(cc).packet());
    return absl::OkStatus();
  }
};

MEDIAPIPE_REGISTER_NODE(AddHeaderCalculator);

}  // namespace api2
}  // namespace mediapipe
