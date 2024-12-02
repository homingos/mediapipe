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

#include "absl/log/absl_log.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

class SimpleCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->Outputs().Index(0).Set<int>();
    if (cc->InputSidePackets().HasTag("MAX_COUNT")) {
      cc->InputSidePackets().Tag("MAX_COUNT").Set<int>();
    }
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) final {
    ABSL_LOG(WARNING) << "Simple Calculator Process called, count_: " << count_;
    int max_count = 1;
    if (cc->InputSidePackets().HasTag("MAX_COUNT")) {
      max_count = cc->InputSidePackets().Tag("MAX_COUNT").Get<int>();
    }
    if (count_ >= max_count) {
      return tool::StatusStop();
    }
    cc->Outputs().Index(0).Add(new int(count_), Timestamp(count_));
    ++count_;
    return absl::OkStatus();
  }

 private:
  int count_ = 0;
};
REGISTER_CALCULATOR(SimpleCalculator);

}  // namespace mediapipe
