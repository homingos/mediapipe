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

#ifndef MEDIAPIPE_CALCULATORS_CORE_END_LOOP_CALCULATOR_H_
#define MEDIAPIPE_CALCULATORS_CORE_END_LOOP_CALCULATOR_H_

#include <type_traits>

#include "absl/status/status.h"
#include "mediapipe/framework/calculator_context.h"
#include "mediapipe/framework/calculator_contract.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {

// Calculator for completing the processing of loops on iterable collections
// inside a MediaPipe graph. The EndLoopCalculator collects all input packets
// from ITEM input_stream into a collection and upon receiving the flush signal
// from the "BATCH_END" tagged input stream, it emits the aggregated results
// at the original timestamp contained in the "BATCH_END" input stream.
//
// See BeginLoopCalculator for a usage example.
template <typename IterableT>
class EndLoopCalculator : public CalculatorBase {
  using ItemT = typename IterableT::value_type;

 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    RET_CHECK(cc->Inputs().HasTag("BATCH_END"))
        << "Missing BATCH_END tagged input_stream.";
    cc->Inputs().Tag("BATCH_END").Set<Timestamp>();

    RET_CHECK(cc->Inputs().HasTag("ITEM"));
    cc->Inputs().Tag("ITEM").Set<ItemT>();

    RET_CHECK(cc->Outputs().HasTag("ITERABLE"));
    cc->Outputs().Tag("ITERABLE").Set<IterableT>();
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    if (!cc->Inputs().Tag("ITEM").IsEmpty()) {
      if (!input_stream_collection_) {
        input_stream_collection_.reset(new IterableT);
      }

      if constexpr (std::is_copy_constructible_v<ItemT>) {
        input_stream_collection_->push_back(
            cc->Inputs().Tag("ITEM").Get<ItemT>());
      } else {
        // Try to consume the item and move it into the collection. Return an
        // error if the items are not consumable.
        auto item_ptr_or = cc->Inputs().Tag("ITEM").Value().Consume<ItemT>();
        if (item_ptr_or.ok()) {
          input_stream_collection_->push_back(std::move(*item_ptr_or.value()));
        } else {
          return absl::InternalError(
              "The item type is not copiable. Consider making the "
              "EndLoopCalculator the sole owner of the input packets so that "
              "it can be moved instead of copying.");
        }
      }
    }

    if (!cc->Inputs().Tag("BATCH_END").Value().IsEmpty()) {  // flush signal
      Timestamp loop_control_ts =
          cc->Inputs().Tag("BATCH_END").template Get<Timestamp>();
      if (input_stream_collection_) {
        cc->Outputs()
            .Tag("ITERABLE")
            .Add(input_stream_collection_.release(), loop_control_ts);
      } else {
        // Since there is no collection, inform downstream calculators to not
        // expect any packet by updating the timestamp bounds.
        cc->Outputs()
            .Tag("ITERABLE")
            .SetNextTimestampBound(Timestamp(loop_control_ts.Value() + 1));
      }
    }
    return absl::OkStatus();
  }

 private:
  std::unique_ptr<IterableT> input_stream_collection_;
};

}  // namespace mediapipe

#endif  // MEDIAPIPE_CALCULATORS_CORE_END_LOOP_CALCULATOR_H_
