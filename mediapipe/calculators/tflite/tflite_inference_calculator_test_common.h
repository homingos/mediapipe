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

#ifndef MEDIAPIPE_CALCULATORS_TFLITE_TFLITE_INFERENCE_CALCULATOR_TEST_H_
#define MEDIAPIPE_CALCULATORS_TFLITE_TFLITE_INFERENCE_CALCULATOR_TEST_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "absl/strings/str_replace.h"
#include "absl/strings/string_view.h"
#include "mediapipe/calculators/tflite/tflite_inference_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/deps/file_path.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status_matchers.h"  // NOLINT
#include "mediapipe/framework/tool/validate_type.h"
#include "tensorflow/lite/error_reporter.h"
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/portable_type_to_tflitetype.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif  // defined(__APPLE__)

namespace mediapipe {

using ::tflite::Interpreter;

template <typename T>
void DoSmokeTest(const std::string& graph_proto) {
  const int width = 8;
  const int height = 8;
  const int channels = 3;

  static_assert(std::is_same_v<T, float> || std::is_same_v<T, uint8_t>,
                "Only float & uint8 currently supported.");

  // Prepare interpreter and input tensor.
  std::unique_ptr<Interpreter> interpreter(new Interpreter);
  ASSERT_NE(interpreter, nullptr);

  interpreter->AddTensors(1);
  interpreter->SetInputs({0});
  interpreter->SetOutputs({0});
  TfLiteQuantization quant;
  if (std::is_integral_v<T>) {
    auto* affine_quant = static_cast<TfLiteAffineQuantization*>(
        malloc(sizeof(TfLiteAffineQuantization)));
    affine_quant->scale = TfLiteFloatArrayCreate(1);
    affine_quant->zero_point = TfLiteIntArrayCreate(1);
    affine_quant->scale->data[0] = 1.0;
    affine_quant->zero_point->data[0] = 0;
    quant.type = kTfLiteAffineQuantization;
    quant.params = affine_quant;
  } else {
    quant.type = kTfLiteNoQuantization;
    quant.params = nullptr;
  }
  interpreter->SetTensorParametersReadWrite(0, tflite::typeToTfLiteType<T>(),
                                            "", {3}, quant);

  int t = interpreter->inputs()[0];
  TfLiteTensor* input_tensor = interpreter->tensor(t);
  interpreter->ResizeInputTensor(t, {width, height, channels});
  interpreter->AllocateTensors();

  T* input_tensor_buffer = tflite::GetTensorData<T>(input_tensor);
  ASSERT_NE(input_tensor_buffer, nullptr);
  for (int i = 0; i < width * height * channels - 1; i++) {
    input_tensor_buffer[i] = 1;
  }

  auto input_vec = absl::make_unique<std::vector<TfLiteTensor>>();
  input_vec->emplace_back(*input_tensor);

  // Prepare single calculator graph to and wait for packets.
  CalculatorGraphConfig graph_config =
      ParseTextProtoOrDie<CalculatorGraphConfig>(graph_proto);
  std::vector<Packet> output_packets;
  tool::AddVectorSink("tensor_out", &graph_config, &output_packets);
  CalculatorGraph graph(graph_config);
  MP_ASSERT_OK(graph.StartRun({}));

  // Push the tensor into the graph.
  MP_ASSERT_OK(graph.AddPacketToInputStream(
      "tensor_in", Adopt(input_vec.release()).At(Timestamp(0))));
  // Wait until the calculator done processing.
  MP_ASSERT_OK(graph.WaitUntilIdle());
  ASSERT_EQ(1, output_packets.size());

  // Get and process results.
  const std::vector<TfLiteTensor>& result_vec =
      output_packets[0].Get<std::vector<TfLiteTensor>>();
  ASSERT_EQ(1, result_vec.size());

  const TfLiteTensor* result = &result_vec[0];
  const T* result_buffer = tflite::GetTensorData<T>(result);
  ASSERT_NE(result_buffer, nullptr);
  for (int i = 0; i < width * height * channels - 1; i++) {
    ASSERT_EQ(3, result_buffer[i]);
  }

  // Fully close graph at end, otherwise calculator+tensors are destroyed
  // after calling WaitUntilDone().
  MP_ASSERT_OK(graph.CloseInputStream("tensor_in"));
  MP_ASSERT_OK(graph.WaitUntilDone());
}

}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_TFLITE_TFLITE_INFERENCE_CALCULATOR_TEST_H_
