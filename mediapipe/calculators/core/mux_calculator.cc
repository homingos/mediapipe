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

#include "mediapipe/framework/api2/node.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {
namespace api2 {

// A Calculator that selects an input stream from "INPUT:0", "INPUT:1", ...,
// using the integer value (0, 1, ...) in the packet on the "SELECT" input
// stream, and passes the packet on the selected input stream to the "OUTPUT"
// output stream.
//
// Note that this calculator defaults to use MuxInputStreamHandler, which is
// required for this calculator. However, it can be overridden to work with
// other InputStreamHandlers. Check out the unit tests on for an example usage
// with DefaultInputStreamHandler.
// TODO: why would you need to use DefaultISH? Perhaps b/167596925?
class MuxCalculator : public Node {
 public:
  static constexpr Input<int>::SideFallback kSelect{"SELECT"};
  // TODO: this currently sets them all to Any independently, instead
  // of the first being Any and the others being SameAs.
  static constexpr Input<AnyType>::Multiple kIn{"INPUT"};
  static constexpr Output<SameType<kIn>> kOut{"OUTPUT"};

  MEDIAPIPE_NODE_CONTRACT(kSelect, kIn, kOut,
                          StreamHandler("MuxInputStreamHandler"));

  absl::Status Process(CalculatorContext* cc) final {
    if (kSelect(cc).IsStream() && kSelect(cc).IsEmpty()) {
      return absl::OkStatus();
    }

    int select = *kSelect(cc);
    RET_CHECK(0 <= select && select < kIn(cc).Count());
    if (!kIn(cc)[select].IsEmpty()) {
      kOut(cc).Send(kIn(cc)[select].packet());
    }
    return absl::OkStatus();
  }
};

MEDIAPIPE_REGISTER_NODE(MuxCalculator);

}  // namespace api2
}  // namespace mediapipe
