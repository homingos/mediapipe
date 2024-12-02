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

#include "absl/strings/str_replace.h"
#include "mediapipe/calculators/tflite/tflite_inference_calculator_test_common.h"

namespace mediapipe {

// Tests a simple add model that adds an input tensor to itself.
TEST(TfLiteInferenceCalculatorTest, SmokeTest) {
  std::string graph_proto = R"(
    input_stream: "tensor_in"
    node {
      calculator: "TfLiteInferenceCalculator"
      input_stream: "TENSORS:tensor_in"
      output_stream: "TENSORS:tensor_out"
      options {
        [mediapipe.TfLiteInferenceCalculatorOptions.ext] {
          model_path: "mediapipe/calculators/tflite/testdata/add.bin"
          try_mmap_model: $mmap
          $delegate
        }
      }
    }
  )";
  // Test CPU inference only.
  DoSmokeTest<float>(/*graph_proto=*/absl::StrReplaceAll(
      graph_proto,
      {{"$delegate", "delegate { tflite {} }"}, {"$mmap", "false"}}));
  DoSmokeTest<float>(/*graph_proto=*/absl::StrReplaceAll(
      graph_proto,
      {{"$delegate", "delegate { tflite {} }"}, {"$mmap", "true"}}));
  DoSmokeTest<float>(absl::StrReplaceAll(
      graph_proto,
      {{"$delegate", "delegate { xnnpack {} }"}, {"$mmap", "false"}}));
  DoSmokeTest<float>(absl::StrReplaceAll(
      graph_proto, {{"$delegate", "delegate { xnnpack { num_threads: 10 } }"},
                    {"$mmap", "false"}}));
}

TEST(TfLiteInferenceCalculatorTest, SmokeTest_ModelAsInputSidePacket) {
  std::string graph_proto = R"(
    input_stream: "tensor_in"

    node {
      calculator: "ConstantSidePacketCalculator"
      output_side_packet: "PACKET:model_path"
      options: {
        [mediapipe.ConstantSidePacketCalculatorOptions.ext]: {
          packet { string_value: "mediapipe/calculators/tflite/testdata/add.bin" }
        }
      }
    }

    node {
      calculator: "LocalFileContentsCalculator"
      input_side_packet: "FILE_PATH:model_path"
      output_side_packet: "CONTENTS:model_blob"
    }

    node {
      calculator: "TfLiteModelCalculator"
      input_side_packet: "MODEL_BLOB:model_blob"
      output_side_packet: "MODEL:model"
    }

    node {
      calculator: "TfLiteInferenceCalculator"
      input_stream: "TENSORS:tensor_in"
      output_stream: "TENSORS:tensor_out"
      input_side_packet: "MODEL:model"
      options {
        [mediapipe.TfLiteInferenceCalculatorOptions.ext] {
          use_gpu: false
          delegate { tflite {} }
        }
      }
    }
  )";
  DoSmokeTest<float>(graph_proto);
}

}  // namespace mediapipe
