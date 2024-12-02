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

#include <memory>

#include "mediapipe/calculators/tflite/tflite_custom_op_resolver_calculator.pb.h"
#include "mediapipe/framework/api2/packet.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/util/tflite/cpu_op_resolver.h"
#include "mediapipe/util/tflite/op_resolver.h"
#include "tensorflow/lite/core/api/op_resolver.h"

namespace mediapipe {

namespace {
constexpr char kOpResolverTag[] = "OP_RESOLVER";
}  // namespace

// This calculator creates a custom op resolver as a side packet that can be
// used in TfLiteInferenceCalculator. Current custom op resolver supports the
// following custom op on CPU and GPU:
//   Convolution2DTransposeBias
//   MaxPoolArgmax
//   MaxUnpooling
//
// Usage examples:
//
// For using with TfliteInferenceCalculator:
// node {
//   calculator: "TfLiteCustomOpResolverCalculator"
//   output_side_packet: "op_resolver"
//   node_options: {
//     [type.googleapis.com/mediapipe.TfLiteCustomOpResolverCalculatorOptions] {
//       use_gpu: true
//     }
//   }
// }
//
// For using with InferenceCalculator:
// node {
//   calculator: "TfLiteCustomOpResolverCalculator"
//   output_side_packet: "OP_RESOLVER:op_resolver"
//   node_options: {
//     [type.googleapis.com/mediapipe.TfLiteCustomOpResolverCalculatorOptions] {
//       use_gpu: true
//     }
//   }
// }
class TfLiteCustomOpResolverCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    if (cc->OutputSidePackets().HasTag(kOpResolverTag)) {
      cc->OutputSidePackets().Tag(kOpResolverTag).Set<tflite::OpResolver>();
    } else {
      cc->OutputSidePackets()
          .Index(0)
          .Set<tflite::ops::builtin::BuiltinOpResolver>();
    }
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(TimestampDiff(0));

    const TfLiteCustomOpResolverCalculatorOptions& options =
        cc->Options<TfLiteCustomOpResolverCalculatorOptions>();

    std::unique_ptr<tflite::ops::builtin::BuiltinOpResolver> op_resolver;
    if (options.use_gpu()) {
      op_resolver = absl::make_unique<mediapipe::OpResolver>();
    } else {
      op_resolver = absl::make_unique<mediapipe::CpuOpResolver>();
    }

    if (cc->OutputSidePackets().HasTag(kOpResolverTag)) {
      cc->OutputSidePackets()
          .Tag(kOpResolverTag)
          .Set(mediapipe::api2::PacketAdopting<tflite::OpResolver>(
              std::move(op_resolver)));
    } else {
      cc->OutputSidePackets().Index(0).Set(Adopt(op_resolver.release()));
    }
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    return absl::OkStatus();
  }
};
REGISTER_CALCULATOR(TfLiteCustomOpResolverCalculator);

}  // namespace mediapipe
