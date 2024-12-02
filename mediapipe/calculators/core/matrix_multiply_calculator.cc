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

#include "Eigen/Core"
#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {
namespace api2 {
// Perform a (left) matrix multiply.  Meaning (output = A * input)
// where A is the matrix which is provided as an input side packet.
//
// Example config:
// node {
//   calculator: "MatrixMultiplyCalculator"
//   input_stream: "samples"
//   output_stream: "multiplied_samples"
//   input_side_packet: "multiplication_matrix"
// }
class MatrixMultiplyCalculator : public Node {
 public:
  static constexpr Input<Matrix> kIn{""};
  static constexpr Output<Matrix> kOut{""};
  static constexpr SideInput<Matrix> kSide{""};

  MEDIAPIPE_NODE_CONTRACT(kIn, kOut, kSide);

  absl::Status Process(CalculatorContext* cc) override;
};
MEDIAPIPE_REGISTER_NODE(MatrixMultiplyCalculator);

absl::Status MatrixMultiplyCalculator::Process(CalculatorContext* cc) {
  kOut(cc).Send(*kSide(cc) * *kIn(cc));
  return absl::OkStatus();
}

}  // namespace api2
}  // namespace mediapipe
