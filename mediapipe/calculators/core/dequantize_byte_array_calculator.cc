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

#include <cfloat>

#include "mediapipe/calculators/core/dequantize_byte_array_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/status.h"

// Dequantizes a byte array to a vector of floats.
//
// Example config:
//   node {
//     calculator: "DequantizeByteArrayCalculator"
//     input_stream: "ENCODED:encoded"
//     output_stream: "FLOAT_VECTOR:float_vector"
//     options {
//       [mediapipe.DequantizeByteArrayCalculatorOptions.ext]: {
//         max_quantized_value: 2
//         min_quantized_value: -2
//       }
//     }
//   }
namespace mediapipe {

constexpr char kFloatVectorTag[] = "FLOAT_VECTOR";
constexpr char kEncodedTag[] = "ENCODED";

class DequantizeByteArrayCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->Inputs().Tag(kEncodedTag).Set<std::string>();
    cc->Outputs().Tag(kFloatVectorTag).Set<std::vector<float>>();
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) final {
    const auto options =
        cc->Options<::mediapipe::DequantizeByteArrayCalculatorOptions>();
    if (!options.has_max_quantized_value() ||
        !options.has_min_quantized_value()) {
      return absl::InvalidArgumentError(
          "Both max_quantized_value and min_quantized_value must be provided "
          "in DequantizeByteArrayCalculatorOptions.");
    }
    float max_quantized_value = options.max_quantized_value();
    float min_quantized_value = options.min_quantized_value();
    if (max_quantized_value < min_quantized_value + FLT_EPSILON) {
      return absl::InvalidArgumentError(
          "max_quantized_value must be greater than min_quantized_value.");
    }
    float range = max_quantized_value - min_quantized_value;
    scalar_ = range / 255.0;
    bias_ = (range / 512.0) + min_quantized_value;
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) final {
    const std::string& encoded =
        cc->Inputs().Tag(kEncodedTag).Value().Get<std::string>();
    std::vector<float> float_vector;
    float_vector.reserve(encoded.length());
    for (int i = 0; i < encoded.length(); ++i) {
      float_vector.push_back(
          static_cast<unsigned char>(encoded.at(i)) * scalar_ + bias_);
    }
    cc->Outputs()
        .Tag(kFloatVectorTag)
        .AddPacket(MakePacket<std::vector<float>>(float_vector)
                       .At(cc->InputTimestamp()));
    return absl::OkStatus();
  }

 private:
  float scalar_;
  float bias_;
};

REGISTER_CALCULATOR(DequantizeByteArrayCalculator);

}  // namespace mediapipe
