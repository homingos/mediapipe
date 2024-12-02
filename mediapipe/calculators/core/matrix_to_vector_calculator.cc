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
//
// Defines MatrixToVectorCalculator.
#include <math.h>

#include <deque>
#include <memory>
#include <string>

#include "Eigen/Core"
#include "absl/memory/memory.h"
#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/matrix.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/tool/status_util.h"
#include "mediapipe/util/time_series_util.h"

namespace mediapipe {
namespace api2 {

// A calculator that converts a Matrix M to a vector containing all the
// entries of M in column-major order.
//
// Example config:
// node {
//   calculator: "MatrixToVectorCalculator"
//   input_stream: "input_matrix"
//   output_stream: "column_major_vector"
// }
class MatrixToVectorCalculator : public Node {
 public:
  static constexpr Input<Matrix> kIn{""};
  static constexpr Output<std::vector<float>> kOut{""};

  MEDIAPIPE_NODE_CONTRACT(kIn, kOut);

  absl::Status Open(CalculatorContext* cc) override;

  // Outputs a packet containing a vector for each input packet.
  absl::Status Process(CalculatorContext* cc) override;
};
MEDIAPIPE_REGISTER_NODE(MatrixToVectorCalculator);

absl::Status MatrixToVectorCalculator::Open(CalculatorContext* cc) {
  cc->SetOffset(0);
  return mediapipe::OkStatus();
}

absl::Status MatrixToVectorCalculator::Process(CalculatorContext* cc) {
  const Matrix& input = *kIn(cc);
  auto output = absl::make_unique<std::vector<float>>();

  // The following lines work to convert the Matrix to a vector because Matrix
  // is an Eigen::MatrixXf and Eigen uses column-major layout by default.
  output->resize(input.rows() * input.cols());
  auto output_as_matrix =
      Eigen::Map<Matrix>(output->data(), input.rows(), input.cols());
  output_as_matrix = input;

  kOut(cc).Send(std::move(output));
  return absl::OkStatus();
}

}  // namespace api2
}  // namespace mediapipe
