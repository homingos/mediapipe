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

#include "absl/strings/string_view.h"
#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/canonical_errors.h"
#include "mediapipe/framework/port/ret_check.h"

namespace mediapipe {

constexpr char kIncrementTag[] = "INCREMENT";
constexpr char kInitialValueTag[] = "INITIAL_VALUE";
constexpr char kBatchSizeTag[] = "BATCH_SIZE";
constexpr char kErrorCountTag[] = "ERROR_COUNT";
constexpr char kMaxCountTag[] = "MAX_COUNT";
constexpr char kErrorOnOpenTag[] = "ERROR_ON_OPEN";

// Source calculator that produces MAX_COUNT*BATCH_SIZE int packets of
// sequential numbers from INITIAL_VALUE (default 0) with a common
// difference of INCREMENT (default 1) between successive numbers (with
// timestamps corresponding to the sequence numbers).  The packets are
// produced in BATCH_SIZE sized batches with each call to Process().  An
// error will be returned after ERROR_COUNT batches.  An error will be
// produced in Open() if ERROR_ON_OPEN is true.  Either MAX_COUNT or
// ERROR_COUNT must be provided and non-negative.  If BATCH_SIZE is not
// provided, then batches are of size 1.
class CountingSourceCalculator : public CalculatorBase {
 public:
  static absl::Status GetContract(CalculatorContract* cc) {
    cc->Outputs().Index(0).Set<int>();

    if (cc->InputSidePackets().HasTag(kErrorOnOpenTag)) {
      cc->InputSidePackets().Tag(kErrorOnOpenTag).Set<bool>();
    }

    RET_CHECK(cc->InputSidePackets().HasTag(kMaxCountTag) ||
              cc->InputSidePackets().HasTag(kErrorCountTag));
    if (cc->InputSidePackets().HasTag(kMaxCountTag)) {
      cc->InputSidePackets().Tag(kMaxCountTag).Set<int>();
    }
    if (cc->InputSidePackets().HasTag(kErrorCountTag)) {
      cc->InputSidePackets().Tag(kErrorCountTag).Set<int>();
    }

    if (cc->InputSidePackets().HasTag(kBatchSizeTag)) {
      cc->InputSidePackets().Tag(kBatchSizeTag).Set<int>();
    }
    if (cc->InputSidePackets().HasTag(kInitialValueTag)) {
      cc->InputSidePackets().Tag(kInitialValueTag).Set<int>();
    }
    if (cc->InputSidePackets().HasTag(kIncrementTag)) {
      cc->InputSidePackets().Tag(kIncrementTag).Set<int>();
    }
    return absl::OkStatus();
  }

  absl::Status Open(CalculatorContext* cc) override {
    if (cc->InputSidePackets().HasTag(kErrorOnOpenTag) &&
        cc->InputSidePackets().Tag(kErrorOnOpenTag).Get<bool>()) {
      return absl::NotFoundError("expected error");
    }
    if (cc->InputSidePackets().HasTag(kErrorCountTag)) {
      error_count_ = cc->InputSidePackets().Tag(kErrorCountTag).Get<int>();
      RET_CHECK_LE(0, error_count_);
    }
    if (cc->InputSidePackets().HasTag(kMaxCountTag)) {
      max_count_ = cc->InputSidePackets().Tag(kMaxCountTag).Get<int>();
      RET_CHECK_LE(0, max_count_);
    }
    if (cc->InputSidePackets().HasTag(kBatchSizeTag)) {
      batch_size_ = cc->InputSidePackets().Tag(kBatchSizeTag).Get<int>();
      RET_CHECK_LT(0, batch_size_);
    }
    if (cc->InputSidePackets().HasTag(kInitialValueTag)) {
      counter_ = cc->InputSidePackets().Tag(kInitialValueTag).Get<int>();
    }
    if (cc->InputSidePackets().HasTag(kIncrementTag)) {
      increment_ = cc->InputSidePackets().Tag(kIncrementTag).Get<int>();
      RET_CHECK_LT(0, increment_);
    }
    RET_CHECK(error_count_ >= 0 || max_count_ >= 0);
    return absl::OkStatus();
  }

  absl::Status Process(CalculatorContext* cc) override {
    if (error_count_ >= 0 && batch_counter_ >= error_count_) {
      return absl::InternalError("expected error");
    }
    if (max_count_ >= 0 && batch_counter_ >= max_count_) {
      return tool::StatusStop();
    }
    for (int i = 0; i < batch_size_; ++i) {
      cc->Outputs().Index(0).Add(new int(counter_), Timestamp(counter_));
      counter_ += increment_;
    }
    ++batch_counter_;
    return absl::OkStatus();
  }

 private:
  int max_count_ = -1;
  int error_count_ = -1;
  int batch_size_ = 1;
  int batch_counter_ = 0;
  int counter_ = 0;
  int increment_ = 1;
};
REGISTER_CALCULATOR(CountingSourceCalculator);

}  // namespace mediapipe
