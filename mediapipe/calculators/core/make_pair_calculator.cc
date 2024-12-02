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

#include <utility>
#include <vector>

#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace api2 {

// Given two input streams (A, B), output a single stream containing a pair<A,
// B>.
//
// Example config:
// node {
//   calculator: "MakePairCalculator"
//   input_stream: "packet_a"
//   input_stream: "packet_b"
//   output_stream: "output_pair_a_b"
// }
class MakePairCalculator : public Node {
 public:
  static constexpr Input<AnyType>::Multiple kIn{""};
  // Note that currently api2::Packet is a different type from mediapipe::Packet
  static constexpr Output<std::pair<mediapipe::Packet, mediapipe::Packet>>
      kPair{""};

  MEDIAPIPE_NODE_CONTRACT(kIn, kPair);

  static absl::Status UpdateContract(CalculatorContract* cc) {
    RET_CHECK_EQ(kIn(cc).Count(), 2);
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    kPair(cc).Send({kIn(cc)[0].packet(), kIn(cc)[1].packet()});
    return absl::OkStatus();
  }
};

MEDIAPIPE_REGISTER_NODE(MakePairCalculator);

}  // namespace api2
}  // namespace mediapipe
