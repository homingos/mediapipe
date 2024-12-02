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
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/timestamp.h"

namespace mediapipe {
namespace api2 {

// A calculator that takes a packet of an input stream and converts it to an
// output side packet. This calculator only works under the assumption that the
// input stream only has a single packet passing through.
//
// Example config:
// node {
//   calculator: "StreamToSidePacketCalculator"
//   input_stream: "stream"
//   output_side_packet: "side_packet"
// }
class StreamToSidePacketCalculator : public Node {
 public:
  static constexpr Input<AnyType>::Optional kIn{""};
  static constexpr SideOutput<SameType<kIn>> kOut{""};

  MEDIAPIPE_NODE_CONTRACT(kIn, kOut);

  absl::Status Process(mediapipe::CalculatorContext* cc) override {
    kOut(cc).Set(
        kIn(cc).packet().As<AnyType>().At(mediapipe::Timestamp::Unset()));
    return absl::OkStatus();
  }
};

MEDIAPIPE_REGISTER_NODE(StreamToSidePacketCalculator);

}  // namespace api2
}  // namespace mediapipe
