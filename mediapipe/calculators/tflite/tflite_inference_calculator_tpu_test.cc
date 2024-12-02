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

#include <cstdint>

#include "absl/strings/str_replace.h"
#include "mediapipe/calculators/tflite/tflite_inference_calculator_test_common.h"

namespace mediapipe {

// Tests a simple add model that adds an input tensor to itself.
TEST(TfLiteInferenceCalculatorTpuTest, SmokeTest) {
  std::string graph_proto = R"(
    input_stream: "tensor_in"
    node {
      calculator: "TfLiteInferenceCalculator"
      input_stream: "TENSORS:tensor_in"
      output_stream: "TENSORS:tensor_out"
      options {
        [mediapipe.TfLiteInferenceCalculatorOptions.ext] {
          model_path: "mediapipe/calculators/tflite/testdata/add_quantized.bin"
          $delegate
        }
      }
    }
  )";
  DoSmokeTest<uint8_t>(
      /*graph_proto=*/absl::StrReplaceAll(graph_proto, {{"$delegate", ""}}));
  DoSmokeTest<uint8_t>(/*graph_proto=*/absl::StrReplaceAll(
      graph_proto, {{"$delegate", "delegate { tflite {} }"}}));
}

}  // namespace mediapipe
