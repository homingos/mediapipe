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

#include <sys/types.h>

#include <cstdint>
#include <memory>
#include <string>

#include "absl/strings/numbers.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// Calculator that converts a string into an integer type, or fails if the
// conversion is not possible.
//
// Example config:
// node {
//   calculator: "StringToIntCalculator"
//   input_side_packet: "string"
//   output_side_packet: "index"
// }
template <typename IntType>
class StringToIntCalculatorTemplate : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->InputSidePackets().Index(0).Set<std::string>();
    cc->OutputSidePackets().Index(0).Set<IntType>();
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    IntType number;
    if (!absl::SimpleAtoi(cc->InputSidePackets().Index(0).Get<std::string>(),
                          &number)) {
      return absl::InvalidArgumentError(
          "The string could not be parsed as an integer.");
    }
    cc->OutputSidePackets().Index(0).Set(MakePacket<IntType>(number));
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    return absl::OkStatus();
  }
};

using StringToIntCalculator = StringToIntCalculatorTemplate<int>;
REGISTER_CALCULATOR(StringToIntCalculator);

using StringToUintCalculator = StringToIntCalculatorTemplate<unsigned int>;
REGISTER_CALCULATOR(StringToUintCalculator);

using StringToInt32Calculator = StringToIntCalculatorTemplate<int32_t>;
REGISTER_CALCULATOR(StringToInt32Calculator);

using StringToUint32Calculator = StringToIntCalculatorTemplate<uint32_t>;
REGISTER_CALCULATOR(StringToUint32Calculator);

using StringToInt64Calculator = StringToIntCalculatorTemplate<int64_t>;
REGISTER_CALCULATOR(StringToInt64Calculator);

using StringToUint64Calculator = StringToIntCalculatorTemplate<uint64_t>;
REGISTER_CALCULATOR(StringToUint64Calculator);

}  // namespace mediapipe
