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
// Calculator converts from one-dimensional Tensor of DT_FLOAT to vector<float>
// OR from (batched) two-dimensional Tensor of DT_FLOAT to vector<vector<float>.

#include <memory>
#include <vector>

#include "mediapipe/calculators/tensorflow/tensor_to_vector_float_calculator_options.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/platform/bfloat16.h"

namespace mediapipe {

namespace tf = ::tensorflow;

class TensorToVectorFloatCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc);

  absl::Status Open(CalculatorContext* cc) override;
  absl::Status Process(CalculatorContext* cc) override;

 private:
  TensorToVectorFloatCalculatorOptions options_;
};
REGISTER_CALCULATOR(TensorToVectorFloatCalculator);

absl::Status TensorToVectorFloatCalculator::GetContract(
    CalculatorContract* cc) {
  // Start with only one input packet.
  RET_CHECK_EQ(cc->Inputs().NumEntries(), 1)
      << "Only one input stream is supported.";
  cc->Inputs().Index(0).Set<tf::Tensor>(
      // Input Tensor
  );
  RET_CHECK_EQ(cc->Outputs().NumEntries(), 1)
      << "Only one output stream is supported.";
  const auto& options = cc->Options<TensorToVectorFloatCalculatorOptions>();
  if (options.tensor_is_2d()) {
    RET_CHECK(!options.flatten_nd());
    cc->Outputs().Index(0).Set<std::vector<std::vector<float>>>(
        /* "Output vector<vector<float>>." */);
  } else {
    cc->Outputs().Index(0).Set<std::vector<float>>(
        // Output vector<float>.
    );
  }
  return absl::OkStatus();
}

absl::Status TensorToVectorFloatCalculator::Open(CalculatorContext* cc) {
  options_ = cc->Options<TensorToVectorFloatCalculatorOptions>();

  // Inform mediapipe that this calculator produces an output at time t for
  // each input received at time t (i.e. this calculator does not buffer
  // inputs). This enables mediapipe to propagate time of arrival estimates in
  // mediapipe graphs through this calculator.
  cc->SetOffset(/*offset=*/0);

  return absl::OkStatus();
}

absl::Status TensorToVectorFloatCalculator::Process(CalculatorContext* cc) {
  const tf::Tensor& input_tensor =
      cc->Inputs().Index(0).Value().Get<tf::Tensor>();
  RET_CHECK(tf::DT_FLOAT == input_tensor.dtype() ||
            tf::DT_BFLOAT16 == input_tensor.dtype())
      << "expected DT_FLOAT or DT_BFLOAT_16 input but got "
      << tensorflow::DataTypeString(input_tensor.dtype());

  if (options_.tensor_is_2d()) {
    RET_CHECK(2 == input_tensor.dims())
        << "Expected 2-dimensional Tensor, but the tensor shape is: "
        << input_tensor.shape().DebugString();
    auto output = std::make_unique<std::vector<std::vector<float>>>(
        input_tensor.dim_size(0), std::vector<float>(input_tensor.dim_size(1)));
    for (int i = 0; i < input_tensor.dim_size(0); ++i) {
      auto& instance_output = output->at(i);
      if (tf::DT_BFLOAT16 == input_tensor.dtype()) {
        const auto& slice =
            input_tensor.Slice(i, i + 1).unaligned_flat<tf::bfloat16>();
        for (int j = 0; j < input_tensor.dim_size(1); ++j) {
          instance_output.at(j) = static_cast<float>(slice(j));
        }
      } else {
        const auto& slice =
            input_tensor.Slice(i, i + 1).unaligned_flat<float>();
        for (int j = 0; j < input_tensor.dim_size(1); ++j) {
          instance_output.at(j) = slice(j);
        }
      }
    }
    cc->Outputs().Index(0).Add(output.release(), cc->InputTimestamp());
  } else {
    if (!options_.flatten_nd()) {
      RET_CHECK(1 == input_tensor.dims())
          << "`flatten_nd` is not set. Expected 1-dimensional Tensor, but the "
          << "tensor shape is: " << input_tensor.shape().DebugString();
    }
    auto output =
        std::make_unique<std::vector<float>>(input_tensor.NumElements());
    if (tf::DT_BFLOAT16 == input_tensor.dtype()) {
      const auto& tensor_values = input_tensor.unaligned_flat<tf::bfloat16>();
      for (int i = 0; i < input_tensor.NumElements(); ++i) {
        output->at(i) = static_cast<float>(tensor_values(i));
      }
    } else {
      const auto& tensor_values = input_tensor.unaligned_flat<float>();
      for (int i = 0; i < input_tensor.NumElements(); ++i) {
        output->at(i) = tensor_values(i);
      }
    }
    cc->Outputs().Index(0).Add(output.release(), cc->InputTimestamp());
  }

  return absl::OkStatus();
}

}  // namespace mediapipe
