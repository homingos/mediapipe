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

#include <string>
#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/calculator_runner.h"
#include "mediapipe/framework/port/gmock.h"
#include "mediapipe/framework/port/gtest.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/status_matchers.h"  // NOLINT

namespace mediapipe {

constexpr char kFloatVectorTag[] = "FLOAT_VECTOR";
constexpr char kEncodedTag[] = "ENCODED";

TEST(QuantizeFloatVectorCalculatorTest, WrongConfig) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "DequantizeByteArrayCalculator"
        input_stream: "ENCODED:encoded"
        output_stream: "FLOAT_VECTOR:float_vector"
        options {
          [mediapipe.DequantizeByteArrayCalculatorOptions.ext]: {
            max_quantized_value: 2
          }
        }
      )pb");
  CalculatorRunner runner(node_config);
  std::string empty_string;
  runner.MutableInputs()
      ->Tag(kEncodedTag)
      .packets.push_back(
          MakePacket<std::string>(empty_string).At(Timestamp(0)));
  auto status = runner.Run();
  EXPECT_FALSE(status.ok());
  EXPECT_THAT(
      status.message(),
      testing::HasSubstr(
          "Both max_quantized_value and min_quantized_value must be provided"));
}

TEST(QuantizeFloatVectorCalculatorTest, WrongConfig2) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "DequantizeByteArrayCalculator"
        input_stream: "ENCODED:encoded"
        output_stream: "FLOAT_VECTOR:float_vector"
        options {
          [mediapipe.DequantizeByteArrayCalculatorOptions.ext]: {
            max_quantized_value: -2
            min_quantized_value: 2
          }
        }
      )pb");
  CalculatorRunner runner(node_config);
  std::string empty_string;
  runner.MutableInputs()
      ->Tag(kEncodedTag)
      .packets.push_back(
          MakePacket<std::string>(empty_string).At(Timestamp(0)));
  auto status = runner.Run();
  EXPECT_FALSE(status.ok());
  EXPECT_THAT(
      status.message(),
      testing::HasSubstr(
          "max_quantized_value must be greater than min_quantized_value"));
}

TEST(QuantizeFloatVectorCalculatorTest, WrongConfig3) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "DequantizeByteArrayCalculator"
        input_stream: "ENCODED:encoded"
        output_stream: "FLOAT_VECTOR:float_vector"
        options {
          [mediapipe.DequantizeByteArrayCalculatorOptions.ext]: {
            max_quantized_value: 1
            min_quantized_value: 1
          }
        }
      )pb");
  CalculatorRunner runner(node_config);
  std::string empty_string;
  runner.MutableInputs()
      ->Tag(kEncodedTag)
      .packets.push_back(
          MakePacket<std::string>(empty_string).At(Timestamp(0)));
  auto status = runner.Run();
  EXPECT_FALSE(status.ok());
  EXPECT_THAT(
      status.message(),
      testing::HasSubstr(
          "max_quantized_value must be greater than min_quantized_value"));
}

TEST(DequantizeByteArrayCalculatorTest, TestDequantization) {
  CalculatorGraphConfig::Node node_config =
      ParseTextProtoOrDie<CalculatorGraphConfig::Node>(R"pb(
        calculator: "DequantizeByteArrayCalculator"
        input_stream: "ENCODED:encoded"
        output_stream: "FLOAT_VECTOR:float_vector"
        options {
          [mediapipe.DequantizeByteArrayCalculatorOptions.ext]: {
            max_quantized_value: 2
            min_quantized_value: -2
          }
        }
      )pb");
  CalculatorRunner runner(node_config);
  unsigned char input[4] = {0x7F, 0xFF, 0x00, 0x01};
  runner.MutableInputs()
      ->Tag(kEncodedTag)
      .packets.push_back(
          MakePacket<std::string>(
              std::string(reinterpret_cast<char const*>(input), 4))
              .At(Timestamp(0)));
  auto status = runner.Run();
  MP_ASSERT_OK(runner.Run());
  const std::vector<Packet>& outputs =
      runner.Outputs().Tag(kFloatVectorTag).packets;
  EXPECT_EQ(1, outputs.size());
  const std::vector<float>& result = outputs[0].Get<std::vector<float>>();
  ASSERT_FALSE(result.empty());
  EXPECT_EQ(4, result.size());
  EXPECT_NEAR(0, result[0], 0.01);
  EXPECT_NEAR(2, result[1], 0.01);
  EXPECT_NEAR(-2, result[2], 0.01);
  EXPECT_NEAR(-1.976, result[3], 0.01);

  EXPECT_EQ(Timestamp(0), outputs[0].Timestamp());
}

}  // namespace mediapipe
