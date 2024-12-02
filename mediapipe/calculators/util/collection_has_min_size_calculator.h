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

#ifndef MEDIAPIPE_CALCULATORS_UTIL_COLLECTION_HAS_MIN_SIZE_CALCULATOR_H_
#define MEDIAPIPE_CALCULATORS_UTIL_COLLECTION_HAS_MIN_SIZE_CALCULATOR_H_

#include <vector>

#include "mediapipe/calculators/util/collection_has_min_size_calculator.pb.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/ret_check.h"
#include "mediapipe/framework/port/status.h"

namespace mediapipe {

// Deterimines if an input iterable collection has a minimum size, specified
// in CollectionHasMinSizeCalculatorOptions. Example usage:
// node {
//   calculator: "IntVectorHasMinSizeCalculator"
//   input_stream: "ITERABLE:input_int_vector"
//   output_stream: "has_min_ints"
//  options {
//    [mediapipe.CollectionHasMinSizeCalculatorOptions.ext] {
//      min_size: 2
//    }
//   }
// }
// Optionally, uses a side packet to override `min_size` specified in the
// calculator options.
template <typename IterableT>
class CollectionHasMinSizeCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag("ITERABLE"));
    RET_CHECK_EQ(1, cc->Inputs().NumEntries());

    RET_CHECK_EQ(1, cc->Outputs().NumEntries());

    RET_CHECK_GE(
        cc->Options<::mediapipe::CollectionHasMinSizeCalculatorOptions>()
            .min_size(),
        0);

    cc->Inputs().Tag("ITERABLE").Set<IterableT>();
    cc->Outputs().Index(0).Set<bool>();

    // Optional input side packet that determines `min_size_`.
    if (cc->InputSidePackets().NumEntries() > 0) {
      cc->InputSidePackets().Index(0).Set<int>();
    }
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    cc->SetOffset(TimestampDiff(0));
    min_size_ =
        cc->Options<::mediapipe::CollectionHasMinSizeCalculatorOptions>()
            .min_size();
    // Override `min_size` if passed as side packet.
    if (cc->InputSidePackets().NumEntries() > 0 &&
        !cc->InputSidePackets().Index(0).IsEmpty()) {
      min_size_ = cc->InputSidePackets().Index(0).Get<int>();
    }
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    const IterableT& input = cc->Inputs().Tag("ITERABLE").Get<IterableT>();
    bool has_min_size = input.size() >= min_size_;

    cc->Outputs().Index(0).AddPacket(
        MakePacket<bool>(has_min_size).At(cc->InputTimestamp()));

    return absl::OkStatus();
  }

 private:
  int min_size_ = 0;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_UTIL_COLLECTION_HAS_MIN_SIZE_CALCULATOR_H_
