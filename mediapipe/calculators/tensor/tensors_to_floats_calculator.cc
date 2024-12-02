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

#include "mediapipe/calculators/tensor/tensors_to_floats_calculator.pb.h"
#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/tensor.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {

namespace {

inline float Sigmoid(float value) { return 1.0f / (1.0f + std::exp(-value)); }

}  // namespace

// A calculator for converting Tensors to to a float or a float vector.
//
// Input:
//  TENSORS - Vector of Tensors of type kFloat32. Only the first
//            tensor will be used.
// Output:
//  FLOAT(optional) - Converted single float number.
//  FLOATS(optional) - Converted float vector.
//
// Notes: To output FLOAT stream, the input tensor must have size 1, e.g.
//        only 1 float number in the tensor.
//
// Usage example:
// node {
//   calculator: "TensorsToFloatsCalculator"
//   input_stream: "TENSORS:tensors"
//   output_stream: "FLOATS:floats"
// }
namespace api2 {
class TensorsToFloatsCalculator : public Node {
 public:
  static constexpr Input<std::vector<Tensor>> kInTensors{"TENSORS"};
  static constexpr Output<float>::Optional kOutFloat{"FLOAT"};
  static constexpr Output<std::vector<float>>::Optional kOutFloats{"FLOATS"};
  MEDIAPIPE_NODE_INTERFACE(TensorsToFloatsCalculator, kInTensors, kOutFloat,
                           kOutFloats);

  static absl::Status UpdateContract(CalculatorContract* cc);
  absl::Status Open(CalculatorContext* cc) final;
  absl::Status Process(CalculatorContext* cc) final;

 private:
  ::mediapipe::TensorsToFloatsCalculatorOptions options_;
};
MEDIAPIPE_REGISTER_NODE(TensorsToFloatsCalculator);

absl::Status TensorsToFloatsCalculator::UpdateContract(CalculatorContract* cc) {
  // Only exactly a single output allowed.
  RET_CHECK(kOutFloat(cc).IsConnected() ^ kOutFloats(cc).IsConnected());
  return absl::OkStatus();
}

absl::Status TensorsToFloatsCalculator::Open(CalculatorContext* cc) {
  options_ = cc->Options<::mediapipe::TensorsToFloatsCalculatorOptions>();
  return absl::OkStatus();
}

absl::Status TensorsToFloatsCalculator::Process(CalculatorContext* cc) {
  const auto& input_tensors = *kInTensors(cc);
  RET_CHECK(!input_tensors.empty());
  RET_CHECK(input_tensors[0].element_type() == Tensor::ElementType::kFloat32);
  // TODO: Add option to specify which tensor to take from.
  auto view = input_tensors[0].GetCpuReadView();
  auto raw_floats = view.buffer<float>();
  int num_values = input_tensors[0].shape().num_elements();
  auto output_floats = absl::make_unique<std::vector<float>>(
      raw_floats, raw_floats + num_values);

  switch (options_.activation()) {
    case TensorsToFloatsCalculatorOptions::SIGMOID:
      std::transform(output_floats->begin(), output_floats->end(),
                     output_floats->begin(), Sigmoid);
      break;
    case TensorsToFloatsCalculatorOptions::NONE:
      break;
  }

  if (kOutFloat(cc).IsConnected()) {
    // TODO: Could add an index in the option to specifiy returning
    // one value of a float array.
    RET_CHECK_EQ(num_values, 1);
    kOutFloat(cc).Send(output_floats->at(0));
  } else {
    kOutFloats(cc).Send(std::move(output_floats));
  }
  return absl::OkStatus();
}
}  // namespace api2
}  // namespace mediapipe
